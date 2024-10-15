import dataclasses
from dataclasses import dataclass, field
import inspect


@dataclass(frozen=True)
class Coordinate:
    lat: float
    lon: float

    def __str__(self):
        ns = "N" if self.lat >= 0 else "S"
        we = "E" if self.lon >= 0 else "W"
        return f"{abs(self.lat):.1f}°{ns}, {abs(self.lon):.1f}°{we}"


point = Coordinate(1, 2)
print(point)
print(issubclass(Coordinate, object))
print(Coordinate.__annotations__)
print(inspect.get_annotations(Coordinate))


fields = dataclasses.asdict(point)
print(fields)


@dataclass
class ClubMember:
    name: str
    guests: list = field(default_factory=list)
    athlete: bool = field(default=False, repr=False)


member = ClubMember('abc')
print(member)
