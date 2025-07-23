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
	sudo rm -rf $(OBJ_DIR)/postgres $(OBJ_DIR)/library
	rm -rf $(OBJ_DIR) $(BIN)

install-lib:
	sudo apt install libcli11-dev libcurl4-openssl-dev
	brew install nlohmann-json

install-dev: install-lib
	sudo apt install libcli11-doc valgrind
	brew install bear

clangd:
	bear -- make

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN) upload

docker: all
	bash test/immich-app-setup.sh
	bash test/immich-fresh-token.sh
	sudo docker build -f test/Dockerfile -t cli .
	sudo docker run --network host --rm cli:latest
	sudo docker compose --project-directory build/ down

.PHONY: all clean install-lib clangd valgrind docker
