import Testing

@testable import Day03

@Test func day03Part1() async throws {
  #expect(bestJoltage("987654321111111", length: 2) == 98)
  #expect(bestJoltage("811111111111119", length: 2) == 89)
  #expect(bestJoltage("234234234234278", length: 2) == 78)
  #expect(bestJoltage("818181911112111", length: 2) == 92)
}

@Test func day03Part2() async throws {
  #expect(bestJoltage("987654321111111", length: 12) == 987_654_321_111)
  #expect(bestJoltage("811111111111119", length: 12) == 811_111_111_119)
  #expect(bestJoltage("234234234234278", length: 12) == 434_234_234_278)
  #expect(bestJoltage("818181911112111", length: 12) == 888_911_112_111)
}
