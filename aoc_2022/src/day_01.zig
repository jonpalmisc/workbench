const std = @import("std");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const alloc = gpa.allocator();

    const input = try std.fs.cwd().openFile("input/day_01.txt", .{});
    const reader = input.reader();

    var elves = std.ArrayList(u32).init(alloc);
    defer elves.deinit();

    var current_cals: u32 = 0;
    var line_buf: [64]u8 = undefined;
    while (try reader.readUntilDelimiterOrEof(&line_buf, '\n')) |line| {
        // Elves are separated by a blank line; if we encounter one, finish
        // summing calories for the current elf.
        if (line.len == 0) {
            try elves.append(current_cals);
            current_cals = 0;

            continue;
        }

        current_cals += try std.fmt.parseUnsigned(u32, line, 10);
    }

    // Sort elves in place.
    std.sort.sort(u32, elves.items, {}, comptime std.sort.desc(u32));

    std.debug.print("Part 1:\t{}\n", .{elves.items[0]});
    std.debug.print("Part 2:\t{}, {}, {} ({})\n", .{
        elves.items[0],
        elves.items[1],
        elves.items[2],
        elves.items[0] + elves.items[1] + elves.items[2],
    });
}
