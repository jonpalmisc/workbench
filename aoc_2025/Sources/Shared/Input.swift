import Foundation

/// Get the puzzle input for day N.
public func puzzleInput(day: Int) -> String {
  let path = String(format: "Inputs/Day%02d.txt", day)
  guard let input = try? String(contentsOfFile: path, encoding: .utf8) else {
    fatalError()
  }

  return input
}

/// Get the puzzle input for day N as an array of lines.
public func puzzleInputAsLines(day: Int, includeEmpty: Bool) -> [String] {
  return puzzleInput(day: day).components(separatedBy: .newlines).filter {
    includeEmpty || !$0.isEmpty
  }
}
