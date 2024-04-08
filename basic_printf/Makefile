CXXFLAGS	+= -std=c++17
CXXFLAGS	+= -g -O1 -fno-exceptions -fno-rtti
CXXFLAGS	+= -Wall -Wextra
CXXFLAGS	+= -nostartfiles


.PHONY: all
all: basic_printf

basic_printf: printf.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -fr basic_printf
