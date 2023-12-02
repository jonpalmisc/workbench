#include <fstream>
#include <iostream>

int read_calibration_value(std::string const &line) {
  int first = -1, last = -1;

  for (char c : line) {
    if (!std::isdigit(c))
      continue;

    if (first == -1)
      first = c - '0';

    last = c - '0';
  }

  return first * 10 + last;
}

int main(int argc, char const **argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input>\n";
    return 1;
  }

  std::ifstream input(argv[1]);

  int sum = 0;
  std::string line;
  while (std::getline(input, line))
    sum += read_calibration_value(line);

  std::cout << sum << "\n";
}
