CXXFLAGS	+= -std=c++17
CXXFLAGS	+= -Wall -Wextra

LDFLAGS		+= -lcrypto -lssl

.PHONY: all
all: pem_write

pem_write: main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f pem_write
