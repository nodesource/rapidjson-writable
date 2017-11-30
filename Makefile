ROOT        = $(shell pwd)
DEPS        = $(ROOT)/deps

UV_PATH     = $(DEPS)/uv
UV_LIB      = $(UV_PATH)/out/Debug/libuv.a
UV_INCLUDES = $(UV_PATH)/include/
UV_SRCS     = $(wildcard $(UV_PATH)/src/*.c) $(wildcard $(UV_PATH)/src/unix/*.c)
UV_FLAGS    = -pthread

RAPIDJSON_DIR      = $(DEPS)/rapidjson
RAPIDJSON_INCLUDES = $(RAPIDJSON_DIR)/include/

CCFLAGS = $(UV_FLAGS) -std=c++11 -g

uname_S=$(shell uname -s)
LIBS=
ifeq (Linux, $(uname_S))
LIBS=-lrt -ldl -lm -pthread
endif

SRC_DIR = $(ROOT)/src
TST_DIR = $(ROOT)/test
BIN_DIR = $(ROOT)/bin


SRCS := $(wildcard $(SRC_DIR)/*.cc) $(wildcard $(TST_DIR)/*.cc)
OBJS = $(SRCS:.cc=.o)
INCS =-I$(UV_INCLUDES) -I$(RAPIDJSON_INCLUDES)

TEST_DUMP = $(BIN_DIR)/test_dump

$(TEST_DUMP): $(UV_LIB) $(OBJS) $(RAPIDJSON_DIR)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LIBS) $(OBJS) $(UV_LIB) -o $@

test_dump: $(TEST_DUMP)

$(UV_PATH):
	git clone git@github.com:libuv/libuv.git $@ && \
	cd $(UV_PATH) &&                               \
	git checkout tags/v1.16.1

$(UV_LIB): $(UV_PATH)
	cd $(UV_PATH) &&                                                                                            \
    (test -d ./build/gyp ||                                                                                     \
    	(mkdir -p ./build && git clone https://chromium.googlesource.com/external/gyp $(UV_PATH)/build/gyp)) && \
	./gyp_uv.py -f make &&                                                                                      \
	make -C out -j8

$(RAPIDJSON_DIR):
	git clone git@github.com:miloyip/rapidjson.git $@

.SUFFIXES: .cc .o
.cc.o:
	$(CXX) $< $(CCFLAGS) $(INCS) -c -o $@

clean:
	@rm -f $(OBJS) $(MAIN)

xcode:
	@mkdir -p build
	(cd build && rm -rf CMakeCache.txt CMakeFiles && \
		CC=`xcrun -find cc` CXX=`xcrun -find c++` LDFLAGS='$(LDFLAGS)' cmake -G Xcode ..)

.PHONY: main xcode
