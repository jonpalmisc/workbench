from binaryninja import BasicBlock, Function


def function_to_dot(function: Function) -> str:
    NODE_ATTRIBUTES = "shape=box fontname=Courier fontsize=10"

    def node_name(block: BasicBlock) -> str:
        if block.is_low_level_il:
            kind = "llil"
        elif block.is_medium_level_il:
            kind = "mlil"
        elif block.is_high_level_il:
            kind = "hlil"
        else:
            kind = "asm"

        return f"{kind}_{block.index}"

    def label_escape(line: str) -> str:
        return line.replace("\\n", "\\\\n").replace('"', '\\"')

    nodes = []
    edges = []

    for block in function.basic_blocks:
        lines = [f"{line.address:08x}: {line}" for line in block.get_disassembly_text()]
        label = label_escape("\\l".join(lines)) + "\\l"
        nodes.append(f'  {node_name(block)}[{NODE_ATTRIBUTES} label="{label}"];')

    for block in function.basic_blocks:
        for edge in block.outgoing_edges:
            edges.append(f"  {node_name(block)} -> {node_name(edge.target)};")

    return "\n".join(["digraph {"] + edges + nodes + ["}"])


print(function_to_dot(current_function))
