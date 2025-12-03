import Testing

@testable import Day02

@Test func examples() async throws {
  #expect(invalidInRange(11...22, rules: .part1) == [11, 22])
  #expect(invalidInRange(11...22, rules: .part2) == [11, 22])
  #expect(invalidInRange(99...115, rules: .part1) == [99])
  #expect(invalidInRange(99...115, rules: .part2) == [99, 111])
  #expect(invalidInRange(998...1012, rules: .part1) == [1010])
  #expect(invalidInRange(998...1012, rules: .part2) == [999, 1010])
  #expect(invalidInRange(1_188_511_880...1_188_511_890, rules: .part1) == [1_188_511_885])
  #expect(invalidInRange(1_188_511_880...1_188_511_890, rules: .part2) == [1_188_511_885])
  #expect(invalidInRange(38_593_856...38_593_862, rules: .part1) == [38_593_859])
  #expect(invalidInRange(38_593_856...38_593_862, rules: .part2) == [38_593_859])
  #expect(invalidInRange(565653...565659, rules: .part1) == [])
  #expect(invalidInRange(565653...565659, rules: .part2) == [565656])
  #expect(invalidInRange(824_824_821...824_824_827, rules: .part2) == [824_824_824])
}
