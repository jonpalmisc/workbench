#!/usr/bin/env python3


UNICODE_ARROW = "\u25B2"
UNICODE_VLINE = "\u2502"
UNICODE_HLINE = "\u2500"
UNICODE_JOINT = "\u2518"


class Field:
    """Singular field inside of a bitfield."""

    name: str
    start: int
    end: int

    def __init__(self, name: str, end: int, start: int = -1):
        self.name = name
        self.end = end
        self.start = start if start >= 0 else end

    def __lt__(self, other):
        return self.end < other.end

    def extract(self, value: int) -> int:
        """Extract this field's value from the given input value."""

        return (value >> self.start) & (2 ** (self.end + 1 - self.start) - 1)

    def bits(self, value: int) -> str:
        """Like 'extract', but returns a binary-representation string."""

        return "{n:0{w}b}".format(n=self.extract(value), w=self.width)

    @property
    def width(self) -> int:
        """Field width (in bits)."""

        return self.end + 1 - self.start

    @property
    def range(self) -> str:
        """Field range formatted as an '[end:start]' string."""

        if self.end == self.start:
            return f"[{self.start}]"
        else:
            return f"[{self.end}:{self.start}]"


class Bitfield:
    """Numerous, optionally-contiguous fields."""

    fields: list[Field]

    def __init__(self, fields):
        self.fields = fields

    def dump(self, value: int):
        """Dump bitfield contents."""

        for s in self.fields:
            bits = "{n:0{w}b}".format(n=s.extract(value), w=s.width)
            print(f"{s.name:<16}{s.range:<12}{bits}")

    def diagram(self, value: int):
        # Must sort so that fields are listed in left-to-right order.
        fields = sorted(self.fields, reverse=True)

        max_name_width = max([len(s.name) for s in fields]) + 2

        # Print the bits in each field.
        print(f"{'':>{max_name_width}}   ", end="")
        for t in fields:
            print(t.bits(value) + " ", end="")
        print("")

        # Print the arrow below each field.
        print(f"{'':>{max_name_width}}   ", end="")
        for t in fields:
            print(" " * (t.width - 1) + UNICODE_ARROW + " ", end="")
        print("")

        # Print the lines connecting each label to each field.
        for i, r in enumerate(fields):
            print(f"{r.name:>{max_name_width}} " + UNICODE_HLINE * 2, end="")

            for j, s in enumerate(fields):
                leading = UNICODE_HLINE * (s.width - 1)

                if j == i:
                    print(f"{leading}{UNICODE_JOINT} ", end="")
                elif j < i:
                    print(f"{leading}{UNICODE_HLINE * 2}", end="")
                else:
                    print(f"{'':{s.width-1}}{UNICODE_VLINE} ", end="")

            print("")

        print("")


upper = Bitfield(
    [
        Field("GP", 50),
        Field("DBM", 51),
        Field("Contiguous", 52),
        Field("PXN", 53),
        Field("(U)XN", 54),
        Field("Reserved", 58, 55),
        Field("PBHA", 62, 59),
        Field("Ignored", 63),
    ]
)
lower = Bitfield(
    [
        Field("AttrIndex[2:0]", 4, 2),
        Field("NS", 5),
        Field("AP[2:1]", 7, 6),
        Field("SH[1:0]", 9, 8),
        Field("AF", 10),
        Field("nG", 11),
        Field("OA", 15, 12),
        Field("nT", 16),
    ]
)

upper.diagram(0x60000180000625)
lower.diagram(0x60000180000625)
