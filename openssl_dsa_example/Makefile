CXXFLAGS	+= -std=c++17
CXXFLAGS	+= -Wall -Wextra

LDFLAGS		+= -lcrypto -lssl

.PHONY: all
all: dsa_sign

dsa_sign: main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f dsa_sign
