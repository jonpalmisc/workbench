import Foundation

enum Direction {
  case left
  case right
}

extension Direction {
  init?(_ string: String) {
    switch string {
    case "L":
      self = .left
    case "R":
      self = .right
    default:
      return nil
    }
  }

  /// Multiplication coefficient, left is negative.
  var coef: Int {
    self == .left ? -1 : 1
  }
}

class Dial {
  let size = 100
  private(set) var pos = 50

  private(set) var landedOnZero = 0
  private(set) var crossedZero = 0

  func turn(_ dir: Direction, amount: Int) {
    let posBefore = pos

    // If the amount we are turning is greater than the dial size in either
    // direction, then we can count these trivial zero-crossings now.
    crossedZero += abs(amount) / size

    // We can disregard cycles and just add the effective delta.
    pos += dir.coef * (amount % size)

    if pos <= 0 || pos >= size {
      // Position has gone negative or exceeded the dial size as the result of
      // our turn. We should count this as a zero-crossing if the dial was not
      // already on zero before.
      if posBefore != 0 {
        crossedZero += 1
      }
    }

    // Fix up dial position, wrap around if it has gone negative.
    pos %= size
    if pos < 0 {
      pos += size
    }

    if pos == 0 {
      landedOnZero += 1
    }
  }
}

let input = try String(contentsOfFile: "Inputs/Day01.txt", encoding: .utf8)
let lines = input.components(separatedBy: .newlines).filter { !$0.isEmpty }

let dial = Dial()
for line in lines {
  let index = line.index(line.startIndex, offsetBy: 1)

  let dir = Direction(String(line[..<index]))!
  let mag = Int(line[index...])!

  dial.turn(dir, amount: mag)
}

print(dial.landedOnZero)
print(dial.crossedZero)
