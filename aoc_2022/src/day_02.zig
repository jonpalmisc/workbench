const std = @import("std");

/// Score a single round of play.
pub fn scoreRound(opponent: u8, own: u8) i32 {
    const adj_opponent = @intCast(i32, opponent - 'A');
    const adj_own = @intCast(i32, own - 'X');

    var bonus = @mod((1 + adj_own - adj_opponent), 3);
    return 1 + adj_own + (bonus * 3);
}

pub fn main() !void {
    const input = try std.fs.cwd().openFile("input/day_02.txt", .{});
    const reader = input.reader();

    var score: i32 = 0;
    var line_buf: [64]u8 = undefined;
    while (try reader.readUntilDelimiterOrEof(&line_buf, '\n')) |line| {
        var parts = std.mem.split(u8, line, " ");
        var opponent = parts.next().?;
        var own = parts.next().?;

        var round_score = scoreRound(opponent[0], own[0]);

        score += round_score;
    }

    std.debug.print("Part 1:\t{}\n", .{score});
}

test "scoreRound" {
    try std.testing.expect(scoreRound('A', 'X') == 3 + 1);
    try std.testing.expect(scoreRound('A', 'Y') == 6 + 2);
    try std.testing.expect(scoreRound('A', 'Z') == 0 + 3);
    try std.testing.expect(scoreRound('B', 'X') == 0 + 1);
    try std.testing.expect(scoreRound('B', 'Y') == 3 + 2);
    try std.testing.expect(scoreRound('B', 'Z') == 6 + 3);
    try std.testing.expect(scoreRound('C', 'X') == 6 + 1);
    try std.testing.expect(scoreRound('C', 'Y') == 0 + 2);
    try std.testing.expect(scoreRound('C', 'Z') == 3 + 3);
}
