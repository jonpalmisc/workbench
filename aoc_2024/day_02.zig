const std = @import("std");

const ReportError = error{
    BadParse,
    DeltaTooLarge,
    NotMonotonic,
};

fn reportIsValid(line: []const u8) !bool {
    const Trend = enum { inc, dec };
    var trend: ?Trend = null;
    var maybe_prev: ?i32 = null;

    var tokens = std.mem.tokenizeScalar(u8, line, ' ');
    while (tokens.next()) |token| {
        const num = std.fmt.parseInt(i32, token, 10) catch {
            return ReportError.BadParse;
        };

        if (maybe_prev == null) {
            maybe_prev = num;
            continue;
        }

        const prev = maybe_prev.?;
        if (trend == null) {
            if (num > prev) {
                trend = .inc;
            } else {
                trend = .dec;
            }
        }

        if ((trend == .inc and num <= prev) or (trend == .dec and num >= prev)) {
            return ReportError.NotMonotonic;
        }

        if (@abs(num - prev) > 3) {
            return ReportError.DeltaTooLarge;
        }

        maybe_prev = num;
    }

    return true;
}

pub fn main() !void {
    const input = @embedFile("input_02.txt");

    var num_safe: u32 = 0;
    var lines = std.mem.tokenizeScalar(u8, input, '\n');
    while (lines.next()) |line| {
        if (reportIsValid(line) catch false) {
            num_safe += 1;
        }
    }

    std.debug.print("{}\n", .{num_safe});
}

test "valid increasing trend" {
    try std.testing.expect(try reportIsValid("1 2 3 4 5"));
    try std.testing.expect(try reportIsValid("1 3 6 7 9"));
}

test "valid decreasing trend" {
    try std.testing.expect(try reportIsValid("5 4 3 2 1"));
    try std.testing.expect(try reportIsValid("7 6 4 2 1"));
}

test "bad delta" {
    try std.testing.expectError(ReportError.DeltaTooLarge, reportIsValid("1 2 7 8 9"));
    try std.testing.expectError(ReportError.DeltaTooLarge, reportIsValid("9 7 6 2 1"));
}

test "bad trend" {
    try std.testing.expectError(ReportError.NotMonotonic, reportIsValid("1 3 2 4 5"));
    try std.testing.expectError(ReportError.NotMonotonic, reportIsValid("8 6 4 4 1"));
}
