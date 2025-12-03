import Foundation

func digitsToInt(_ digits: [Int]) -> Int {
  return Int(digits.map(String.init).joined())!
}

func bestJoltage(_ bank: String, length: Int) -> Int {
  let digits = bank.compactMap { Int(String($0)) }

  var best: [Int] = []

  var windowStart = 0
  var windowEnd = digits.count - length + 1

  for _ in 0..<length {
    var biggest = 0
    var biggestIdx = windowStart

    for i in windowStart..<windowEnd {
      if digits[i] > biggest {
        biggest = digits[i]
        biggestIdx = i
      }
    }

    best.append(biggest)

    windowStart = biggestIdx + 1
    windowEnd += 1
  }

  return digitsToInt(best)
}

let input = try String(contentsOfFile: "Inputs/Day03.txt", encoding: .utf8)
let lines = input.components(separatedBy: .newlines).filter { !$0.isEmpty }

var sum = 0
var sum2 = 0
for line in lines {
  sum += bestJoltage(line, length: 2)
  sum2 += bestJoltage(line, length: 12)
}

print(sum)
print(sum2)
