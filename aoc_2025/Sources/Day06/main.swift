import Shared

let SPACE = " ".first!.asciiValue!

enum Operation {
  case add
  case multiply
}

extension Operation {
  static func fromAscii(_ char: UInt8) -> Operation? {
    char == 0x2a ? .multiply : (char == 0x2b ? .add : nil)
  }

  static func fromString(_ string: String) -> Operation? {
    fromAscii(string.first!.asciiValue!)
  }

  func reduceGroup(_ terms: [Int]) -> Int {
    self == .multiply ? terms.reduce(1, *) : terms.reduce(0, +)
  }
}

func intFromChars(_ chars: [UInt8]) -> Int? {
  guard var string = String(bytes: chars, encoding: .utf8) else { return nil }
  string = string.trimmingCharacters(in: .whitespaces)

  return Int(string)
}

let lines = puzzleInputAsLines(day: 6, includeEmpty: false)

func solvePart1() {
  let rows = lines.map { $0.components(separatedBy: .whitespaces).filter { !$0.isEmpty } }

  var problems: [[Int]] = []
  for row in rows.dropLast() {
    for (problemIndex, digitChar) in row.enumerated() {
      if problemIndex >= problems.count {
        // Insert empty problem if this is the furthest column we've seen so far.
        problems.append([])
      }

      problems[problemIndex].append(Int(digitChar)!)
    }
  }

  let total = rows.last!.enumerated()
    .map({ Operation.fromString($1)!.reduceGroup(problems[$0]) })
    .reduce(0, +)

  print(total)
}

func solvePart2() {
  // We are going to turn the entire input into a 2D array and iterate one column
  // at a time.
  let grid = lines.map { $0.map(\.asciiValue!) }
  let dataRows = grid.dropLast()
  let opRow = grid.last!

  var op: Operation? = nil
  var terms: [Int] = []
  var total = 0

  // The operators row will be shorter than some of the data rows, need to
  // iterate up to the longest row to avoid missing terms.
  let maxCol = dataRows.map { $0.count }.max()!

  for i in 0..<maxCol {
    let opChar = i < opRow.count ? opRow[i] : SPACE

    if opChar != SPACE {
      // Saw new operator character, need to reduce last group and reset
      // the group context.
      if op != nil {
        total += op!.reduceGroup(terms)
      }

      op = Operation.fromAscii(opChar)!
      terms = []
    }

    // Get all the digits (or spaces) in this column.
    let termChars = dataRows.map { $0[i] }
    if termChars.allSatisfy({ $0 == SPACE }) {
      // This is the gap column, nothing to do.
      continue
    }

    terms.append(intFromChars(termChars)!)
  }

  // Loop above will exit without seeing another operator which will leave the
  // last group not included in the total.
  total += op!.reduceGroup(terms)

  print(total)
}

solvePart1()
solvePart2()
