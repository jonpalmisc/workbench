import Foundation

// https://www.hackingwithswift.com/example-code/language/how-to-split-an-array-into-chunks
extension Array {
  func chunked(into size: Int) -> [[Element]] {
    return stride(from: 0, to: count, by: size).map {
      Array(self[$0..<Swift.min($0 + size, count)])
    }
  }
}

enum Rules {
  case part1
  case part2
}

func isInvalid(id: Int, rules: Rules) -> Bool {
  let chars = String(id).map(\.asciiValue!)

  // Shortcut: If using Part 1's rules, the ID has to be an even number of
  // digits to even potentially be invalid.
  if rules == .part1 && chars.count % 2 != 0 {
    return false
  }

  // We want to know if the string is composed of a repeated pattern of N
  // digits. N can be no greater than the length of the string divided by 2,
  // since the pattern needs to repeat at least once.
  let maxN = chars.count / 2

  // Using the rules for Part 1, we only need to test the max N. For part 2, we
  // need to check all possible values of N.
  let chunkSizes = (rules == .part2 ? 1 : maxN)..<(maxN + 1)

  for i in chunkSizes {
    let chunks = chars.chunked(into: i)

    if chunks.allSatisfy({ $0 == chunks.first }) {
      return true
    }
  }

  return false
}

func invalidInRange(_ range: ClosedRange<Int>, rules: Rules) -> [Int] {
  range.filter { isInvalid(id: $0, rules: rules) }
}

func parseIntRange(_ rangeString: String) -> ClosedRange<Int> {
  let parts = rangeString.components(separatedBy: "-")
  return Int(parts[0])!...Int(parts[1])!
}

assert(invalidInRange(11...22, rules: .part1) == [11, 22])
assert(invalidInRange(11...22, rules: .part2) == [11, 22])
assert(invalidInRange(99...115, rules: .part1) == [99])
assert(invalidInRange(99...115, rules: .part2) == [99, 111])
assert(invalidInRange(998...1012, rules: .part1) == [1010])
assert(invalidInRange(998...1012, rules: .part2) == [999, 1010])
assert(invalidInRange(1_188_511_880...1_188_511_890, rules: .part1) == [1_188_511_885])
assert(invalidInRange(1_188_511_880...1_188_511_890, rules: .part2) == [1_188_511_885])
assert(invalidInRange(38_593_856...38_593_862, rules: .part1) == [38_593_859])
assert(invalidInRange(38_593_856...38_593_862, rules: .part2) == [38_593_859])
assert(invalidInRange(565653...565659, rules: .part1) == [])
assert(invalidInRange(565653...565659, rules: .part2) == [565656])
assert(invalidInRange(824_824_821...824_824_827, rules: .part2) == [824_824_824])

let input = try String(contentsOfFile: "input.txt", encoding: .utf8)
let rangeStrings = input.trimmingCharacters(in: .newlines).components(separatedBy: ",")

var sumPart1 = 0
var sumPart2 = 0

for rangeString in rangeStrings {
  let range = parseIntRange(rangeString)

  sumPart1 += invalidInRange(range, rules: .part1).reduce(0, +)
  sumPart2 += invalidInRange(range, rules: .part2).reduce(0, +)
}

print(sumPart1)
print(sumPart2)
