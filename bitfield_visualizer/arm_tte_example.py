#!/usr/bin/env python3

from bitviz import *

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

print("")
upper.diagram(0x60000180000625)
lower.diagram(0x60000180000625)
