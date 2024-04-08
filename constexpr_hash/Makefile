CXXFLAGS	+= -std=c++17
CXXFLAGS	+= -O1 -fno-exceptions -fno-rtti
CXXFLAGS	+= -Wall -Wextra


.PHONY: all
all: constexpr_hash

constexpr_hash: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -fr constexpr_hash
