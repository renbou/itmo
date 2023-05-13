from copy import deepcopy as dcopy
from numbers import Number
from sys import stdin
import functools as fn
import operator as op
import io, re
import math

class TokenMismatchError(Exception):
    pass

class Scanner():
    def __init__(self, file = None, source = None,
            delim = None, use_regex = False, close_file = False):

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
        self._token = None
        self._tokens = None

        self._int_regex = re.compile("^[-+]?[0-9]+$")
        self._float_regex = re.compile("^\d+?\.\d+?$")
    
    def __enter__(self):
        return self
    
    def __exit__(self, exc_type, exc_value, traceback):
        if self._inputFile != None and self._close_file:
            self._inputFile.close()

    def _peek(self):
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

    def _next_token(self):
        next_token = self._peek()
        self._token = None
        return next_token
    
    def _next_type(self, cast_type):
        try:
            return cast_type(self._next_token())
        except ValueError as e:
            raise TokenMismatchError("Asked for %s from scanner, but got a different type of token"
                % cast_type.__name__)

    
    def has_next(self):
        return self._peek() is not None
    
    def has_next_int(self):
        next_token = self._peek()
        return next_token is not None and \
            self._int_regex.match(next_token) is not None
    
    def has_next_float(self):
        next_token = self._peek()
        return next_token is not None and \
            (self._float_regex.match(next_token) is not None or next_token.isdigit())
    
    def next(self):
        return self._next_token()
    
    def next_line(self):
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
    
class Vector():
    def __init__(self, *coordinates):
        if not all(map(lambda x: isinstance(x, Number), coordinates)):
            raise ValueError("Cannot create vector from non-number coordinates")
        self._coordinates = tuple(coordinates)
        
    @staticmethod
    def _validate_vector(other):
        if isinstance(other, Vector):
            return other
        if isinstance(other, (tuple, list)):
            return Vector(*other)
        return None
    
    def _check_binary_op_compatibility(self, other):
        if len(self._coordinates) != len(other._coordinates):
            raise ValueError("Cannot operate on vector of length %s and vector of length %s" %
                                 (len(self._coordinates), len(other._coordinates)))

    def __iadd__(self, other):
        other = Vector._validate_vector(other)
        if other != None:
            self._check_binary_op_compatibility(other)
            self._coordinates = tuple(map(op.add, self._coordinates, other._coordinates))
            return self
        return NotImplemented

    def __add__(self, other):
        new = dcopy(self)
        return new.__iadd__(other)
    
    def __isub__(self, other):
        other = Vector._validate_vector(other)
        if other != None:
            self._check_binary_op_compatibility(other)
            self._coordinates = tuple(map(op.sub, self._coordinates, other._coordinates))
            return self
        return NotImplemented
    
    def __sub__(self, other):
        new = dcopy(self)
        return new.__isub__(other)
    
    def __imul__(self, other):
        # multiplying by scalar
        if isinstance(other, Number):
            self._coordinates = tuple(map(fn.partial(op.mul, other), self._coordinates))
            return self
        # dot product
        other = Vector._validate_vector(other)
        if other != None:
            self._check_binary_op_compatibility(other)
            return fn.reduce(op.add, (map(op.mul, self._coordinates, other._coordinates)))
        return NotImplemented
    
    def __mul__(self, other):
        new = dcopy(self)
        return new.__imul__(other)
    
    def __itruediv__(self, other):
        if isinstance(other, Number):
            self._coordinates = tuple(map(lambda x: x / other, self._coordinates))
            return self
        return NotImplemented
    
    def __truediv__(self, other):
        new = dcopy(self)
        return new.__itruediv__(other)
    
    def __ifloordiv__(self, other):
        if isinstance(other, Number):
            self._coordinates = tuple(map(lambda x: x // other, self._coordinates))
            return self
        return NotImplemented
    
    def __floordiv__(self, other):
        new = dcopy(self)
        return new.__ifloordiv__(other)
    
    def __eq__(self, other):
        other = Vector._validate_vector(other)
        if other != None:
            self._check_binary_op_compatibility(other)
            return all(map(op.eq, self._coordinates, other._coordinates))
        return NotImplemented
    
    # unary methods
    def __neg__(self):
        new = dcopy(self)
        new._coordinates = tuple(map(op.neg, new._coordinates))
        return new
    
    def __pos__(self):
        return dcopy(self)
    
    # string representations
    def __str__(self):
        return '('+' '.join(map(str, self._coordinates))+')'
    
    def __repr__(self):
        return '('+', '.join(map(str, self._coordinates))+')'
    
    # magnitude of the vector
    @property
    def magnitude(self):
        return math.sqrt(fn.reduce(op.add, map(lambda x: x*x, self._coordinates)))
    
    @property
    def mag(self):
        return self.magnitude
    
    # the vector's unit vector
    @property
    def unit(self):
        return dcopy(self) / self.magnitude
    
    @property
    def coordinates(self):
        return self._coordinates

    @coordinates.setter
    def coordinates(self, new_coordinates):
        if not isinstance(new_coordinates, (list, tuple)):
            raise ValueError("Cannot set vector's coordinates to non-list")
        if not all(map(lambda x: isinstance(x, Number), new_coordinates)):
            raise ValueError("Cannot set vector's coordinates to non-number coordinates")
        if len(new_coordinates) != len(self._coordinates):
            raise ValueError("Cannot set vector's coordinates to a different length")
        self._coordinates = tuple(dcopy(new_coordinates))

# 2-dimensional vector, with added xy
class Vector2(Vector):
    def __init__(self, *coordinates):
        if len(coordinates) != 2:
            raise ValueError("Cannot initialize 2D vector with more or less than 2 coordinates")
        super().__init__(*coordinates)
    
    # x, y, z
    @property
    def x(self):
        return self._coordinates[0]
    
    @x.setter
    def x(self, value):
        if not isinstance(value, Number):
            raise ValueError("Cannot set x to non-number value")
        self._coordinates = (value,) + self._coordinates[1:]
    
    @property
    def y(self):
        return self._coordinates[1]
    
    @y.setter
    def y(self, value):
        if not isinstance(value, Number):
            raise ValueError("Cannot set y to non-number value")
        self._coordinates = (self._coordinates[0], value, self._coordinates[2])

# 3-dimensional vector, with an added cross product and xyz
class Vector3(Vector2):
    def __init__(self, *coordinates):
        if len(coordinates) != 3:
            raise ValueError("Cannot initialize 3D vector with more or less than 3 coordinates")
        Vector.__init__(self, *coordinates)
    
    def __imatmul__(self, other):
        other = Vector._validate_vector(other)
        if other != None:
            self._check_binary_op_compatibility(other)
            sc, oc = self._coordinates, other._coordinates
            self._coordinates = [sc[1]*oc[2]-sc[2]*oc[1], -(sc[0]*oc[2]-sc[2]*oc[0]), sc[0]*oc[1]-sc[1]*oc[0]]
            return self
        return NotImplemented
    
    def __matmul__(self, other):
        new = dcopy(self)
        return new.__imatmul__(other)

    @property
    def z(self):
        return self._coordinates[2]
    
    @z.setter
    def z(self, value):
        if not isinstance(value, Number):
            raise ValueError("Cannot set z to non-number value")
        self._coordinates = self._coordinates[:2] + (value,)

# signum function
sign = fn.partial(math.copysign, 1.0)

# result is in [0, 180]
def vectors_angle(a, b):
    dot, cross = a * b, (a @ b).mag
    if dot == 0.0:
        return 90.0
    if cross == 0.0:
        return 0.0 if dot > 0 else 180.0
    return math.degrees(math.atan2(cross, dot))

# only works for 2d vectors, calculates with respect to side, so result is in [0, 360]
def vectors_angle_2d(a, b):
    dot, cross = a * b, a.x*b.y - b.x*a.y
    if dot == 0.0:
        return 90.0 * sign(cross)
    if cross == 0.0:
        return 0.0 if dot > 0 else 180.0
    return math.degrees(math.atan2(cross, dot))

def ans_for_pirates(ship_start_rv, ship_keel_v, pirates_rv):
    pirates_v = pirates_rv - ship_start_rv
    true_angle_from_keel = vectors_angle_2d(ship_keel_v, pirates_v)
    angle_from_keel = abs(true_angle_from_keel)
    if angle_from_keel < 30 or angle_from_keel > 150:
        return 0, 0.0
    side = 1 if true_angle_from_keel > 0 else -1
    angle = 90.0 - angle_from_keel
    return side, angle

# calculates angle between mast and the vertical, since we know that mast must be perpendicular to keel
def ans_for_mast(mast_v, keel_v):
    vert = Vector3(0, 0, 1)
    if vert == mast_v:
        return 0.0
    
    # since mast_v isn't (0, 0, 1), then (mast_v.x, mast_v.y, 0) / mast_v.mag and (0, 0, 1) will be the basis
    new_mast_v = Vector2(Vector2(mast_v.x, mast_v.y).mag, 1)
    new_vert_v = Vector2(0, 1)
    angle = vectors_angle_2d(new_vert_v, new_mast_v) * \
                sign(vectors_angle_2d(Vector2(mast_v.x, mast_v.y), keel_v))
    return angle

def scan_2d_vector(scanner):
    x, y = scanner.next_float(), scanner.next_float()
    return Vector2(x, y)

def lab1():
    with Scanner(file=open('input.txt', 'r'), close_file=True) as scanner, open('output.txt', 'w') as output:
        ship_rv, keel_v, mast_2d_v, pirates_rv = scan_2d_vector(scanner), scan_2d_vector(scanner), scan_2d_vector(scanner), scan_2d_vector(scanner)
        mast_v = Vector3(mast_2d_v.x, mast_2d_v.y, 1)
        output.write('%d\n%f\n' % ans_for_pirates(ship_rv, keel_v, pirates_rv))
        output.write('%f\ndedinside\n' % ans_for_mast(mast_v, keel_v))
        

if __name__ == '__main__':
    lab1()