// swift-tools-version: 6.0

import Foundation
import PackageDescription

var dayTargets = (1...4).map {
  Target.executableTarget(
    name: String(format: "Day%02d", $0), dependencies: [.byName(name: "Shared")])
}

let package = Package(
  name: "AOC",
  targets: [.target(name: "Shared")] + dayTargets + [
    .testTarget(name: "DayTests", dependencies: dayTargets.map { .byName(name: $0.name) })
  ]
)
