from binaryninja import *


XOR = Transform["XOR"]


def decrypt_str(bv, address):
    log_info(f"Decryption requested for data at {hex(address)}")

    refs = []
    for r in bv.get_code_refs(address):
        refs.append(r.address)

    log_info(f"  Found code references at {refs}")

    # TODO: Make this better, remove hardcoding.
    df = bv.get_functions_containing(refs[2])[0]
    for i in df.get_low_level_il_at(refs[2]).il_basic_block:
        if i.operation != LowLevelILOperation.LLIL_SET_REG:
            continue

        if i.operands[1].operation == LowLevelILOperation.LLIL_XOR:
            raw_key = i.operands[1].operands[1].value.value & 0xFF
            log_info(f"  XOR key detected as {hex(raw_key)}")

            break

    # Needs to be done so transform works correctly
    key_bytes = bytearray(b"")
    key_bytes.append(raw_key)
    encoded_key = bytes(key_bytes)

    # Find the data variable and do the transform
    data_var = bv.get_data_var_at(address)
    result = XOR.encode(data_var.value[:-1], {"key": encoded_key})

    # Print the result & set a comment on the data variable
    log_info(f"  Decrypted value is \"{result.decode('utf-8')}\"")

    return result.decode("utf-8")


def annotate_str(bv, address):
    plain_str = decrypt_str(bv, address)
    bv.set_comment_at(address, plain_str)


def flatten_retrieval(bv, address):
    try:
        t = next(bv.get_code_refs(address))
        s = next(bv.get_code_refs(t.function.start))
    except StopIteration:
        log_info(f"Data variable at {hex(address)} doesn't have cross references")
        return

    plain_str = decrypt_str(bv, address)
    bv.set_comment_at(address, plain_str)

    # Rename the top-level accessor
    sanitized = plain_str.replace(" ", "_")
    s.function.name = f"__tsd_{sanitized}"

    d = bv.get_data_var_at(address)
    d.name = f"__tse_{sanitized}"


def fixup_all(bv, address):
    sec = bv.get_sections_at(address)[0]

    for v in bv.data_vars:
        if v > sec.start and v < sec.end:
            try:
                flatten_retrieval(bv, v)
            except Exception:
                continue


def decrypt_symbols(bv):
    def decode(b):
        data = bytearray()
        for a in b:
            data.append(a ^ 128)
        return data.decode("utf-8")

    for n, s in bv.symbols.items():
        if n[0:2] == b"_\xdf":
            for sym in s:
                bv.define_user_symbol(
                    Symbol(sym.type, sym.address, decode(sym.raw_bytes[2:]))
                )


PluginCommand.register_for_address(
    "TNT Decrypt\\Annotate Encrypted String", "", annotate_str
)

PluginCommand.register_for_address(
    "TNT Decrypt\\Flatten String Retrieval", "", flatten_retrieval
)

PluginCommand.register_for_address("TNT Decrypt\\Apply All Fixups", "", fixup_all)

PluginCommand.register("TNT Decrypt\\Decrypt Symbol Names", "", decrypt_symbols)
