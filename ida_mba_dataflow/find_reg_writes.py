from typing import List, Tuple, Optional

import ida_hexrays
import ida_funcs
import ida_idp
import ida_xref
import ida_name
import ida_kernwin

DEBUG = True


def dbg(*args):
    if not DEBUG:
        return

    print("\t", *args)


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


def find_x0_backwards(start_ea: int) -> Optional[Tuple[int, int]]:
    func = ida_funcs.get_func(start_ea)

    dbg(
        f"Finding writes to X0 starting from {start_ea:#x} in `{ida_name.get_name(func.start_ea)}`:"
    )
    all_writes = find_reg_writes("X0", func)

    last_rel_write = None
    for addr, value in all_writes:
        dbg(f"  at {addr:#x}: X0 <- {value:#x} {'(after)' if addr > start_ea else ''}")
        if addr < start_ea:
            last_rel_write = (addr, value)
    else:
        dbg("  (none)")

    if last_rel_write:
        return last_rel_write

    dbg("No relevant writes, searching backwards...")
    blk = ida_xref.xrefblk_t()
    refs = list(blk.fcrefs_to(func.start_ea))
    if len(refs) != 1:
        dbg("  Found non-linear call chain!")
        return None

    return find_x0_backwards(refs[0])


def find_info(start_ea: int):
    print(f"Searching from {start_ea:#x}...")
    result = find_x0_backwards(start_ea)
    if result:
        addr, value = result
        print(f"  {addr:#x}: X0 <- {value:#x}")
    else:
        print("  (unknown)")


OSMC_CTOR_EA = 0xFFFFFFF0083A2664

i = 0
ida_kernwin.show_wait_box(f"Searching for X0...")
for call_ea in ida_xref.xrefblk_t().fcrefs_to(OSMC_CTOR_EA):
    ida_kernwin.replace_wait_box(f"Searching for X0 from {call_ea:#x}... ({i})")
    if ida_kernwin.user_cancelled():
        break

    find_info(call_ea)
    i += 1

ida_kernwin.hide_wait_box()
