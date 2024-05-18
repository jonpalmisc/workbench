CXXFLAGS	+= -std=c++17
CXXFLAGS	+= -fno-exceptions -fno-rtti
CXXFLAGS	+= -Wall -Wextra

.PHONY: all
all: values.O1 values.O2

values.O1: CXXFLAGS += -O1
values.O1: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

values.O2: CXXFLAGS += -O2
values.O2: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -fr values*
