SRC_DIR := src
OBJ_DIR := build
DEB_DIR := $(wildcard immich-cli_*/)
BIN := immich-cli
DEB := $(wildcard immich-cli_*.deb)

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
	rm -rf $(OBJ_DIR) $(BIN) $(DEB_DIR)

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

deb-pack: all
	bash release/deb-util.sh
	@DEB_DIR=$(DEB_DIR); \
	if [ -z "$$DEB_DIR" ]; then \
	    DEB_DIR=$$(find . -maxdepth 1 -type d -name 'immich-cli_*' | head -n 1); \
	fi; \
	dpkg-deb --build "$$DEB_DIR"

deb-install: deb-pack
	@DEB=$(DEB); \
	if [ -z "$$DEB" ]; then \
	    DEB=$$(find . -maxdepth 1 -type d -name 'immich-cli_*.deb' | head -n 1); \
	fi; \
	sudo dpkg -i "$$DEB" 

deb-clean: clean
	sudo dpkg -r immich-cli

.PHONY: all clean install-lib clangd valgrind docker deb-pack deb-install deb-clean
