const std = @import("std");

const input = @embedFile("input_01.txt");

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();

    const alloc = arena.allocator();

    var left = try std.ArrayList(i64).initCapacity(alloc, 1000);
    var right = try std.ArrayList(i64).initCapacity(alloc, 1000);
    defer left.deinit();
    defer right.deinit();

    var is_right: bool = false;
    var tokens = std.mem.tokenizeAny(u8, input, " \n");
    while (tokens.next()) |token| {
        const num = try std.fmt.parseInt(i64, token, 10);
        if (is_right) {
            try right.append(num);
        } else {
            try left.append(num);
        }

        is_right = !is_right;
    }

    std.debug.assert(left.items.len == right.items.len);

    std.mem.sort(i64, left.items, {}, std.sort.asc(i64));
    std.mem.sort(i64, right.items, {}, std.sort.asc(i64));

    var total: u64 = 0;
    for (left.items, right.items) |l, r| {
        total += @abs(l - r);
    }

    std.debug.print("{}\n", .{total});
}
