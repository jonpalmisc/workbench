CXXFLAGS	+= -std=c++20
CXXFLAGS	+= -fno-exceptions
CXXFLAGS	+= -Wall -Wextra -Wpedantic

DAY_S	:= $(wildcard *.cpp)
DAY_T	:= $(DAY_S:.cpp=)

.PHONY: all
all: $(DAY_T)

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -fr $(DAY_T)
