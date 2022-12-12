const std = @import("std");

pub fn addDayTarget(
    b: *std.build.Builder,
    comptime name: []const u8,
    target: anytype,
    mode: anytype,
) void {
    const exe = b.addExecutable(name, "src/" ++ name ++ ".zig");
    exe.setTarget(target);
    exe.setBuildMode(mode);
    exe.install();
}

pub fn build(b: *std.build.Builder) void {
    const target = b.standardTargetOptions(.{});
    const mode = b.standardReleaseOptions();

    addDayTarget(b, "day_01", target, mode);
    addDayTarget(b, "day_02", target, mode);
}
