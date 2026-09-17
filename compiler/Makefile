CXX ?= g++
CXXFLAGS ?= -std=gnu++14 -O2 -Isrc
BISON ?= bison
FLEX ?= flex

BIN_DIR = bin
TARGET = $(BIN_DIR)/sql2ra

SRCS = src/parser.tab.cpp \
       src/lex.yy.cpp \
       src/ast.cpp \
       src/schema.cpp \
       src/ra.cpp \
       src/optimizer.cpp \
       src/main.cpp

.PHONY: all clean

all: $(TARGET)

src/parser.tab.cpp src/parser.tab.hpp: src/parser.y
	$(BISON) -d -v -o src/parser.tab.cpp src/parser.y

src/lex.yy.cpp: src/lexer.l src/parser.tab.hpp
	$(FLEX) -o src/lex.yy.cpp src/lexer.l

$(TARGET): src/parser.tab.cpp src/lex.yy.cpp $(SRCS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -rf $(BIN_DIR) src/parser.tab.cpp src/parser.tab.hpp src/parser.output src/lex.yy.cpp
