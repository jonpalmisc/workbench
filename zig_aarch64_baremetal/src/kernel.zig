const builtin = @import("std").builtin;

const log = @import("log.zig");

pub export fn main() void {
    log.write("Reached kernel entry point" ++ "\n");
    log.write("Spinning indefinitely..." ++ "\n");

    while (true) {}
}

/// Custom panic handler.
pub fn panic(message: []const u8, trace: ?*builtin.StackTrace) noreturn {
    _ = trace;

    log.write("\n" ++ "*" ** 70 ++ "\n\n");
    log.print("Panic at 0x{x}: {s}\n\n", .{ @returnAddress(), message });

    while (true) {}
}
