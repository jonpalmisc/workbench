REGS_PER_ROW = 3

regs = []
for i in range(0, 29):
    regs.append(f"x{i}")
regs += ["fp", "lr", "sp", "pc", "cpsr", "far", "esr"]

for i in range(0, len(regs)):
    reg = regs[i]
    # print(f"{reg:>4}=0x0000000000000000  ", end="")

    if i == 0 or (i) % REGS_PER_ROW == 0:
        print("printf(\"", end="")
    
    print(f"{reg:>4}=0x%16lx  ", end=""),
    if i > 0 and (i + 1) % REGS_PER_ROW == 0:
        print(f"\\n\", state.x[{i-2}], state.x[{i-1}], state.x[{i}]);")
