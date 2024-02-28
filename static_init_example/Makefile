CXXFLAGS	+= -std=c++17
CXXFLAGS	+= -O1 -fno-exceptions -fno-rtti
CXXFLAGS	+= -Wall -Wextra


.PHONY: all
all: static_init

static_init: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -fr static_init
