CXXFLAGS	+= -std=c++17
CXXFLAGS	+= -g -O2
CXXFLAGS	+= -Wall -Wextra

.PHONY: all
all: kv_store

kv_store: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f kv_store *.o
