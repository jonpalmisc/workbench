import Foundation

class Map {
  private var data: [[Bool]]

  init(text: String) {
    let lines = text.components(separatedBy: .newlines).filter { !$0.isEmpty }
    data = lines.map { $0.map { $0 == "@" } }
  }

  func adjPositions(row: Int, col: Int) -> [(Int, Int)] {
    let deltas = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]

    return deltas.compactMap { (rowDelta, colDelta) in
      let newRow = row + rowDelta
      let newCol = col + colDelta

      guard newRow >= 0, newRow < data.count,
        newCol >= 0, newCol < data[newRow].count
      else { return nil }

      return (newRow, newCol)
    }
  }

  func reachablePoints(withRemoval: Bool) -> Int {
    var accessible = 0

    while true {
      var didRemove = false

      round: for row in 0..<data.count {
        for col in 0..<data[row].count {
          if !data[row][col] {
            continue
          }

          let numAdj = adjPositions(row: row, col: col).filter { data[$0.0][$0.1] }.count
          if numAdj >= 4 {
            continue
          }

          accessible += 1

          if withRemoval {
            data[row][col] = false
            didRemove = true
            break round
          }
        }
      }

      if !didRemove {
        break
      }
    }

    return accessible
  }
}

let input = try String(contentsOfFile: "Inputs/Day04.txt", encoding: .utf8)
let map = Map(text: input)

print(map.reachablePoints(withRemoval: false))
print(map.reachablePoints(withRemoval: true))
