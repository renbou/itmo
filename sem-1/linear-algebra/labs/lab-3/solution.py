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
    
    # cross product only for 3d vectors
    def __imatmul__(self, other):
        if len(self._coordinates) == 3:
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
        return 'Vector('+', '.join(map(str, self._coordinates))+')'
    
    # hacky shit
    
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
        if len(self._coordinates) > 1:
            return self._coordinates[1]
        raise AttributeError("Vectors of length less than 2 don't contain 'y'")
    
    @y.setter
    def y(self, value):
        if len(self._coordinates) < 2:
            raise AttributeError("Vectors of length less than 2 don't contain 'y'")
        if not isinstance(value, Number):
            raise ValueError("Cannot set y to non-number value")
        self._coordinates = (self._coordinates[0], value) + self._coordinates[2:]
        
        
    @property
    def z(self):
        if len(self._coordinates) > 2:
            return self._coordinates[2]
        raise AttributeError("Vectors of length less than 3 don't contain 'z'")
    
    @z.setter
    def z(self, value):
        if len(self._coordinates) < 3:
            raise AttributeError("Vectors of length less than 3 don't contain 'z'")
        if not isinstance(value, Number):
            raise ValueError("Cannot set z to non-number value")
        self._coordinates = self._coordinates[:2] + (value,) + self._coordinates[3:]

class Plane():
    def __init__(self, plane_type, p1=0, p2=0, p3=0, norm=None, d=0):
        if plane_type != "cube" and plane_type != "glass":
            raise ValueError("Invalid plane type")
        self._plane_type = plane_type

        if norm != None:
            self._norm = norm.unit
            self._d = d
        else:
            v, u = p2 - p1, p3 - p1
            self._norm = (v @ u).unit
            self._d = -(self._norm * p1) # this should be equal to norm*p2 and norm*p3
        
    def __str__(self):
        return "%s(%s, %s)" % (self._plane_type, self._norm, self._d)
    
    def __repr__(self):
        return "Plane(%r, norm=%r, d=%r)" % (self._plane_type, self._norm, self._d)
    
    @property
    def type(self):
        return self._plane_type

    @property
    def norm(self):
        return dcopy(self._norm)
    
    @property
    def d(self):
        return self._d
    
    def _is_type(self, type_name):
        return self._plane_type == type_name
    
    is_cube = fn.partialmethod(_is_type, "cube")
    is_glass = fn.partialmethod(_is_type, "glass")

# def find_intersection_t_1(Origin, Ray, Plane):
#     x_0, y_0, z_0 = Plane.p1.x, Plane.p1.y, Plane.p1.z
#     x_1, y_1, z_1 = Plane.p2.x, Plane.p2.y, Plane.p2.z
#     x_2, y_2, z_2 = Plane.p3.x, Plane.p3.y, Plane.p3.z
#     Origin_x, Origin_y, Origin_z = Origin.x, Origin.y, Origin.z
#     Ray_x, Ray_y, Ray_z = Ray.x, Ray.y, Ray.z
    
#     equation_right_side = \
#         (Origin_z*y_0*x_1 - Origin_y*z_0*x_1 - Origin_z*x_0*y_1 + \
#          Origin_x*z_0*y_1 + Origin_y*x_0*z_1 - Origin_x*y_0*z_1 - \
#          Origin_z*y_0*x_2 + Origin_y*z_0*x_2 + Origin_z*y_1*x_2 - \
#          z_0*y_1*x_2 - Origin_y*z_1*x_2 + y_0*z_1*x_2 + \
#          Origin_z*x_0*y_2 - Origin_x*z_0*y_2 - Origin_z*x_1*y_2 + \
#          z_0*x_1*y_2 + Origin_x*z_1*y_2 - x_0*z_1*y_2 - \
#          Origin_y*x_0*z_2 + Origin_x*y_0*z_2 + Origin_y*x_1*z_2 - \
#          y_0*x_1*z_2 - Origin_x*y_1*z_2 + x_0*y_1*z_2)
#     equation_left_side = (-Ray_z*y_0*x_1 + Ray_y*z_0*x_1 + Ray_z*x_0*y_1 - \
#                           Ray_x*z_0*y_1 - Ray_y*x_0*z_1 + Ray_x*y_0*z_1 + \
#                           Ray_z*y_0*x_2 - Ray_y*z_0*x_2 - Ray_z*y_1*x_2 + \
#                           Ray_y*z_1*x_2 - Ray_z*x_0*y_2 + Ray_x*z_0*y_2 + \
#                           Ray_z*x_1*y_2 - Ray_x*z_1*y_2 + Ray_y*x_0*z_2 - \
#                           Ray_x*y_0*z_2 - Ray_y*x_1*z_2 + Ray_x*y_1*z_2)
#     if equation_left_side == 0:
#         return False, 0
#     print(equation_right_side, equation_left_side)
#     t = equation_right_side / equation_left_side
#     return True, t

def get_cube_planes(a, b, c, c1):
    plane_abc = Plane("cube", p1=a, p2=b, p3=c)
    plane_bcc1 = Plane("cube", p1=b, p2=c, p3=c1)

    plane_abb1_norm = (plane_abc.norm @ plane_bcc1.norm).unit
    plane_abb1_d = -(plane_abb1_norm * a) # should be equal to -(plane_abb1_norm * b)
    assert plane_abb1_d == -(plane_abb1_norm * b), "something wrong mate #1"
    plane_abb1 = Plane("cube", norm=plane_abb1_norm, d=plane_abb1_d)

    plane_a1b1c1_norm = plane_abc.norm
    plane_a1b1c1_d = -(plane_a1b1c1_norm * c1)
    plane_a1b1c1 = Plane("cube", norm=plane_a1b1c1_norm, d=plane_a1b1c1_d)

    plane_ada1_norm = plane_bcc1.norm
    plane_ada1_d = -(plane_ada1_norm * a)
    plane_ada1 = Plane("cube", norm=plane_ada1_norm, d=plane_ada1_d)

    plane_dcc1_norm = plane_abb1_norm
    plane_dcc1_d = -(plane_dcc1_norm * c) # should be equal to -(plane_dcc1_norm * c1)
    assert plane_dcc1_d == -(plane_dcc1_norm * c1), "something wrong mate #2"
    plane_dcc1 = Plane("cube", norm=plane_dcc1_norm, d=plane_dcc1_d)

    return [plane_abc, plane_bcc1, plane_abb1, plane_a1b1c1, plane_ada1, plane_dcc1]

def find_intersection_t(Origin, Ray, Plane):
    right_side = Plane.norm * Ray
    if right_side == 0:
        return False, 0
    t = (-Plane.d - Plane.norm * Origin) / right_side
    return True, t

def find_closest_intersection(Origin, Ray, Planes):
    t = 1e16
    closest_plane = None
    for Plane in Planes:
        intersects, current_t = find_intersection_t(Origin, Ray, Plane)
        #print(f"Intersection of ({Origin}, {Ray}) with plane {Plane} - {intersects}, {current_t} -> {Origin + Ray * current_t}")
        # if current_t == 0 then we are intersecting with the plane from which we started
        # if current_t < 0 then we are intersecting with a plane that's behind us e.g. when
        #                  we reflect from the mirror and a cube's plane is behind the mirror
        if intersects and current_t < t and current_t > 0: 
            t = current_t
            closest_plane = Plane
    return t, closest_plane

def find_reflection(Ray, Plane):
    return Ray - Plane.norm * 2 * (Plane.norm * Ray)

def scan_3d_vector(scanner):
    x, y, z = scanner.next_float(), scanner.next_float(), scanner.next_float()
    return Vector(x, y, z)

def lab3():
    with Scanner(file=open('input.txt', 'r'), close_file=True) as scanner, open('output.txt', 'w') as output:
        a, b, c, d = scan_3d_vector(scanner), scan_3d_vector(scanner), scan_3d_vector(scanner), scan_3d_vector(scanner)
        cube_planes = get_cube_planes(a, b, c, d)
        ray, origin = scan_3d_vector(scanner), scan_3d_vector(scanner)
        energy = scanner.next_int()
        num_glasses = scanner.next_int()
        glass_planes = []
        for i in range(num_glasses):
            p1, p2, p3 = scan_3d_vector(scanner), scan_3d_vector(scanner), scan_3d_vector(scanner)
            glass = Plane("glass", p1=p1, p2=p2, p3=p3)
            glass_planes.append(glass)
        planes = cube_planes + glass_planes
        
        while True:
            t, closest_plane = find_closest_intersection(origin, ray, planes)
            new_origin = origin + ray * t
            if closest_plane.type == "glass":
                energy -= 1
                if energy == 0:
                    output.write("0\n%f %f %f\n" % (new_origin.x, new_origin.y, new_origin.z))
                    return
                new_ray = find_reflection(ray, closest_plane)

                origin, ray = new_origin, new_ray
            else:
                output.write("1\n%d\n%f %f %f\n%f %f %f\n" % (energy, \
                                        new_origin.x, new_origin.y, new_origin.z,\
                                        ray.x, ray.y, ray.z))
                return
        

if __name__ == '__main__':
    lab3()