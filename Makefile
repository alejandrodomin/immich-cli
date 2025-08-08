SRC_DIR := src
OBJ_DIR := build
BIN := immich-cli

CXX := gcc
LIB_INCLUDE_DIRS := $(shell find lib -type d)
CXXFLAGS := -std=c2x -Wall $(addprefix -I,$(LIB_INCLUDE_DIRS)) 
LDFLAGS := -lcurl

SRCS := $(shell find $(SRC_DIR) $(LIB_DIR) -type f -name '*.c')
OBJS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))


all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN)

install-lib:
	sudo apt install libcurl4-openssl-dev

install-dev: install-lib
	sudo apt install libcli11-doc valgrind
	brew install bear

clangd:
	bear -- make

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN) upload

.PHONY: all clean install-lib clangd valgrind
