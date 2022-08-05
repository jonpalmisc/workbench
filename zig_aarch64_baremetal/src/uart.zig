const std = @import("std");

/// Handle to a UART port.
const Uart = struct {
    const Self = @This();
    const Writer = std.io.Writer(Self, error{}, write);

    port: [*c]volatile u8,

    /// Create a UART handle with the given port.
    fn init(port: [*c]volatile u8) Self {
        return Self{ .port = port };
    }

    /// Write bytes to the UART interface. Cannot fail but must have an error
    /// return type to satisfy std.io.Writer's requirements.
    fn write(self: Self, bytes: []const u8) error{}!usize {
        self.safeWrite(bytes);
        return bytes.len;
    }

    /// Write bytes to the UART interface; cannot fail.
    pub fn safeWrite(self: Self, bytes: []const u8) void {
        for (bytes) |b| {
            self.put(b);
        }
    }

    /// Write a single byte to the UART interface.
    pub fn put(self: Self, byte: u8) void {
        self.port.* = byte;
    }
};

/// Shared UART instance at default port.
pub const default = Uart.init(0x9000000);

/// Shared writer around the default UART instance.
pub const writer: Uart.Writer = .{ .context = default };
