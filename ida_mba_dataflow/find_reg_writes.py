from typing import List, Tuple, Optional, TYPE_CHECKING

import ida_hexrays
import ida_funcs
import ida_name
import ida_idp


def mop_to_int(mop: ida_hexrays.mop_t) -> Optional[int]:
    # Can peek into these "address of" operand types and grab the thing being
    # wrapped before trying to handle it below.
    if mop.t == ida_hexrays.mop_a:
        mop = mop.a

    if mop.t == ida_hexrays.mop_v and mop.g:
        return mop.g  # Address of global
    if mop.t == ida_hexrays.mop_n:
        return mop.nnn.value  # Immediate number

    return None


class reg_write_finder_t(ida_hexrays.minsn_visitor_t):
    results: List[Tuple[int, int]]
    mreg: int

    def __init__(self, reg_name, results):
        super().__init__()
        self.mreg = ida_hexrays.reg2mreg(ida_idp.str2reg(reg_name))
        self.results = results

    def visit_minsn(self, *args) -> int:
        minsn = self.curins

        if minsn.d.r != self.mreg:
            return 0
        if (value := mop_to_int(minsn.l)) is not None:
            self.results.append((minsn.ea, value))
        else:
            print(f"Warning: Found unhandled write at {minsn.ea:#x} {minsn.l.t}!")

        return 0


def get_func_mba(func: ida_funcs.func_t, mmat: int) -> ida_hexrays.mba_t:
    ida_hexrays.mark_cfunc_dirty(func.start_ea)

    ranges = ida_hexrays.mba_ranges_t(func)
    failure = ida_hexrays.hexrays_failure_t()
    mlist = ida_hexrays.mlist_t()
    mba = ida_hexrays.gen_microcode(
        ranges,
        failure,
        mlist,  # TODO: Can this just be `None`?
        ida_hexrays.DECOMP_NO_WAIT | ida_hexrays.DECOMP_ALL_BLKS,
        mmat,
    )
    mba.set_mba_flags(ida_hexrays.MBA_SHORT)

    return mba


def find_reg_writes(reg_name: str, func: ida_funcs.func_t) -> List[Tuple[int, int]]:
    writes = []
    mba = get_func_mba(func, ida_hexrays.MMAT_LOCOPT)
    mba.for_all_insns(reg_write_finder_t(reg_name, writes))

    return writes


if TYPE_CHECKING:

    def here() -> int:
        return 0


func = ida_funcs.get_func(here())
reg = "X0"

print(f"Finding writes to {reg} in `{ida_name.get_name(func.start_ea)}`:")
for addr, value in find_reg_writes(reg, func):
    print(f"  at {addr:#x}: {reg} <- {value:#x}")
