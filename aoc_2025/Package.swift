// swift-tools-version: 6.0

import Foundation
import PackageDescription

var dayTargets = (1...3).map { Target.executableTarget(name: String(format: "Day%02d", $0)) }

let package = Package(
  name: "AOC",
  targets: dayTargets + [
    .testTarget(name: "DayTests", dependencies: dayTargets.map { .byName(name: $0.name) })
  ]
)
