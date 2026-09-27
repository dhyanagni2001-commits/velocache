CC = g++
CFLAGS = -I./include -O3 -Wall # '-O3' tells the compiler to optimize for MAX SPEED
# 'Wall'adds useful warnings for keeping the code clean

# defining the headers to make the Makefile track changes
DEPS = include/cache.hpp include/common.hpp include/utils.hpp

BUILD_DIR = build

all: $(BUILD_DIR) v_server v_bench
# 'all' target ensures '/build` exists before compiling

# create '/build' if it doesn't exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

v_server: src/cache.cpp src/storage.cpp apps/server.cpp utils/input_validation.cpp $(DEPS)
	$(CC) $(CFLAGS) src/cache.cpp src/storage.cpp apps/server.cpp utils/input_validation.cpp -o $(BUILD_DIR)/v_server

v_bench: src/cache.cpp utils/metrics.cpp tests/benchmark.cpp $(DEPS)
	$(CC) $(CFLAGS) src/cache.cpp utils/metrics.cpp tests/benchmark.cpp -o $(BUILD_DIR)/v_bench

clean:
	rm -rf $(BUILD_DIR)/*
	@echo "Cleaned $(BUILD_DIR) directory."

# 'cmake-build' is a separate directory from '$(BUILD_DIR)' because it holds
# CMake's own cache plus the fetched GoogleTest sources (see CMakeLists.txt),
# which the Makefile-driven build doesn't need or know about.
TEST_BUILD_DIR = cmake-build

test:
	cmake -S . -B $(TEST_BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(TEST_BUILD_DIR)
	ctest --test-dir $(TEST_BUILD_DIR) --output-on-failure

.PHONY: all clean test # used to treat 'all', 'clean' and 'test' as commands and not files