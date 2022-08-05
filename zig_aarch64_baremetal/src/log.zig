const uart = @import("uart.zig").writer;

/// Log a message to the default logging interface.
pub fn write(msg: []const u8) void {
    _ = uart.write(msg) catch unreachable;
}

/// Log a formatted message to the default logging interface.
pub fn print(comptime fmt: []const u8, args: anytype) void {
    uart.print(fmt, args) catch unreachable;
}
