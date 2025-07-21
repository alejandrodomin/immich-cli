SRC_DIR := src
OBJ_DIR := build
BIN := immich-cli

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Ilib -I$(shell brew --prefix nlohmann-json)/include
LDFLAGS := -lcurl

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN)

install-lib:
	sudo apt install libcli11-dev libcli11-doc libcurl4-openssl-dev valgrind
	brew install nlohmann-json bear

clangd:
	bear -- make

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN) upload

.PHONY: all clean install-lib clangd valgrind
