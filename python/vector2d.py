from array import array
import math


class Vector2d:
    __match_args__ = ("x", "y")

    typecode = "d"

    def __init__(self, x, y):
        self.__x = float(x)
        self.__y = float(y)

    @property
    def x(self):
        return self.__x

    @property
    def y(self):
        return self.__y

    def __iter__(self):
        return (i for i in (self.x, self.y))

    def __repr__(self) -> str:
        class_name = type(self).__name__
        return "{}({!r}, {!r})".format(class_name, *self)

    def __str__(self):
        return str(tuple(self))

    def __bytes__(self):
        return bytes([ord(self.typecode)]) + bytes(array(self.typecode, self))

    def __eq__(self, other):
        return tuple(self) == tuple(other)

    def __hash__(self):
        return hash((self.x, self.y))

    def __abs__(self):
        return math.hypot(self.x, self.y)

    def __bool__(self):
        return bool(abs(self))

    def angle(self):
        return math.atan2(self.y, self.x)

    @classmethod
    def frombytes(cls, octets):
        typecode = chr(octets[0])
        memv = memoryview(octets[1:]).cast(typecode)
        return cls(*memv)


v = Vector2d(3, 4)
print(hash(v))


def positional_pattern_demo(v: Vector2d) -> None:
    match v:
        case Vector2d(0, 0):
            print(f"{v!r} is null")
        case Vector2d(0):
            print(f"{v!r} is vertical")
        case Vector2d(_, 0):
            print(f"{v!r} is horizontal")
        case Vector2d(x, y) if x == y:
            print(f"{v!r} is diagonal")
        case _:
            print(f"{v!r} is awesome")


i = Vector2d(1, 0)
j = Vector2d(0, 1)
u = Vector2d(1, 1)
positional_pattern_demo(i)
positional_pattern_demo(j)
positional_pattern_demo(u)

# Name mangling to prevent accidental access but we can still
# modify the attribute if we want
i._Vector2d__x = 2
print(i._Vector2d__x)


# Python allows us to dynamically add a new attribute
# attributes are stored internally using __dict__
# We can change this behavior by using __slots__
i.d = 10
print(i.d)
