SRC_DIR := src
OBJ_DIR := build
BIN := immich-cli

CXX := g++
CXXFLAGS := -std=c++11 -Wall -Ilib
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
	sudo apt install libcli11-dev libcli11-doc libcurl4-openssl-dev

.PHONY: all clean install-lib
