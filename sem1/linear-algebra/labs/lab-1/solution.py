from copy import deepcopy as copy
from typing import AnyStr, TextIO, Union
from contextlib import AbstractContextManager
from sys import setdlopenflags, stdin, stdout
import io, re

class TokenMismatchError(Exception):
    pass

class Scanner(AbstractContextManager):
    def __init__(self, file: TextIO = None, source: str = None,
            delim: str = None, use_regex: bool = False, close_file: bool = False):

        ## Type checking ##
        if file and not isinstance(file, io.IOBase):
            raise ValueError("Can't create scanner with a non-file infile (type %s)" %
                file.__class__.__name__)

        if source and not isinstance(source, str):
                raise ValueError("Can't create scanner with a non-string source (types %s)" %
                    source.__class__.__name__)
        
        if delim and not isinstance(delim, str):
            raise ValueError("Can't create scanner with a non-string delimiter (type %s)" %
                delim.__class__.__name__)

        if file and source:
            raise ValueError("Can't create a scanner from a file and string source simultaneously")

        ## Choose the input file ##
        if file:
            self._inputFile = file
        elif source:
            self._inputFile = io.StringIO(source)
        else:
            self._inputFile = stdin

        if use_regex:
            if not delim:
                raise ValueError("When creating a regex scanner, the delimiter *must* be specified")
            else:
                try:
                    self._regex = re.compile(delim)
                except Exception as e:
                    raise ValueError("Passed invalid delimiter for regex scanner: %s" % repr(e))
        else:
            self._regex = None

        self._delim = delim
        self._close_file = close_file
        self._token: str = None
        self._tokens: AnyStr = None

        self._int_regex = re.compile("^[-+]?[0-9]+$")
        self._float_regex = re.compile("^\d+?\.\d+?$")

    def __enter__(self):
        return self
    
    def __exit__(self, exc_type, exc_value, traceback):
        if self._inputFile != None and self._close_file:
            self._inputFile.close()

    def _peek(self) -> str:
        if self._token:
            return self._token
        
        if not self._tokens:
            line = self._inputFile.readline()
            if not line:
                return None

            if self._regex != None:
                splits = self._regex.split(line)
            elif self._delim != None:
                splits = line.split(self._delim)
            else:
                splits = line.split()

            self._tokens = iter(splits)

        try:
            self._token = next(self._tokens)
        except StopIteration:
            self._tokens = None
        
        return self._peek()

    def _next_token(self) -> str:
        next_token = self._peek()
        self._token = None
        return next_token
    
    def _next_type(self, cast_type: type):
        try:
            return cast_type(self._next_token())
        except ValueError as e:
            raise TokenMismatchError("Asked for %s from scanner, but got a different type of token"
                % cast_type.__name__)

    
    def has_next(self) -> bool:
        return self._peek() is not None
    
    def has_next_int(self) -> bool:
        next_token = self._peek()
        return next_token is not None and \
            self._int_regex.match(next_token) is not None
    
    def has_next_float(self) -> bool:
        next_token = self._peek()
        return next_token is not None and \
            (self._float_regex.match(next_token) is not None or next_token.isdigit())
    
    def next(self) -> str:
        return self._next_token()
    
    def next_line(self) -> str:
        current = self._next_token()
        if not self._delim:
            rest = ' '.join(self._tokens)
        else:
            rest = self._delim.join(self._tokens)
        return current + rest
    
    def next_int(self):
        return self._next_type(int)

    def next_float(self):
        return self._next_type(float)
    

class Matrix:
    def __init__(self, matrix: list[list]):
        self._rows = len(matrix)
        self._columns = len(matrix[0])

        for i in range(self._rows):
            if len(matrix[i]) != self._columns:
                raise ValueError("Unable to create Matrix object from a two-dimensional array with different row sizes")
                    
        self._matrix = copy(matrix)
    
    @property
    def columns(self):
        return self._columns
    
    @property
    def rows(self):
        return self._rows
    
    def __repr__(self):
        return '\n'.join([' '.join(list(map(str, self._matrix[i]))) for i in range(self._rows)])+'\n'
    
    def __str__(self):
        return self.__repr__()

    def __imul__(self, other: Union['Matrix', int, float]) -> 'Matrix':
        if isinstance(other, int) or isinstance(other, float):
            for i in range(self._rows):
                for j in range(self._columns):
                    self._matrix[i][j] *= other
        elif isinstance(other, Matrix):
            if self._columns != other._rows:
                raise ValueError("Can't multiply two matrices where the first matrix' columns don't match the second one's rows")
            new_matrix = [[0.0 for j in range(other._columns)] for i in range(self._rows)]
            for i in range(self._rows):
                for j in range(other._columns):
                    for k in range(other._rows):
                        new_matrix[i][j] += self._matrix[i][k] * other._matrix[k][j]
            self._columns = other._columns
            self._matrix = new_matrix
        else:
            return NotImplemented

        return self
    
    def __mul__(self, other: Union['Matrix', int, float]) -> 'Matrix':
        result = copy(self)
        return result.__imul__(other)
    
    def __rmul__(self, other: Union['Matrix', int, float]) -> 'Matrix':
        if isinstance(other, Matrix):
            result = copy(other)
            return result.__imul__(self)
        return self.__mul__(other)
        

    def __iadd__(self, other: 'Matrix') -> 'Matrix':
        if isinstance(other, Matrix):
            if self._rows != other._rows or self._columns != other._columns:
                raise ValueError("Can't add two matrices of different sizes")
            for i in range(self._rows):
                for j in range(self._columns):
                    self._matrix[i][j] += other._matrix[i][j]
        else:
            return NotImplemented
        
        return self

    def __add__(self, other: 'Matrix') -> 'Matrix':
        result = copy(self)
        return result.__iadd__(other)
    
    def __radd__(self, other: 'Matrix') -> 'Matrix':
        return self.__add__(other)

    def __isub__(self, other: 'Matrix') -> 'Matrix':
        if isinstance(other, Matrix):
            if self._rows != other._rows or self._columns != other._columns:
                raise ValueError("Can't add two matrices of different sizes")
            for i in range(self._rows):
                for j in range(self._columns):
                    self._matrix[i][j] -= other._matrix[i][j]
        else:
            return NotImplemented
        
        return self

    def __sub__(self, other: 'Matrix') -> 'Matrix':
        result = copy(self)
        return result.__isub__(other)
    
    def __rsub__(self, other: 'Matrix') -> 'Matrix':
        if isinstance(other, Matrix):
            return other.__sub__(self)
        return NotImplemented
    
    def itranspose(self) -> 'Matrix':
        self._matrix = list(map(list, zip(*self._matrix)))
        self._rows, self._columns = self._columns, self._rows
        return self
    
    @property
    def iT(self) -> 'Matrix':
        return self.itranspose()
    
    def transpose(self) -> 'Matrix':
        result = copy(self)
        return result.itranspose()
    
    @property
    def T(self) -> 'Matrix':
        return self.transpose()

def read_matrix(scanner: Scanner) -> Matrix:
    n, m = scanner.next_int(), scanner.next_int()

    matrix = []
    for i in range(n):
        matrix.append([])
        for j in range(m):
            matrix[-1].append(scanner.next_float())
    
    return Matrix(matrix)

def lab_expression(alpha: Union[int, float], beta: Union[int, float],
        A: Matrix, B: Matrix, C: Matrix, D: Matrix, F: Matrix):
    return C * (alpha * A + beta * B.T).T * D - F

def lab1():
    scanner = Scanner()
    alpha, beta = scanner.next_float(), scanner.next_float()
    A = read_matrix(scanner)
    B = read_matrix(scanner)
    C = read_matrix(scanner)
    D = read_matrix(scanner)
    F = read_matrix(scanner)
    try:
        X = lab_expression(alpha, beta, A, B, C, D, F)
        print(1)
        print(X.rows, X.columns)
        print(X)
    except ValueError:
        print(0)
        print(0, 0)


if __name__ == '__main__':
    lab1()