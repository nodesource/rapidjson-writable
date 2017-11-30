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


SRCS := $(wildcard $(SRC_DIR)/*.cc)
OBJS = $(SRCS:.cc=.o)
INCS =-I$(UV_INCLUDES) -I$(RAPIDJSON_INCLUDES)

TEST_DUMP = $(BIN_DIR)/test_dump
TST_DUMP_SRCS=$(TST_DIR)/test_dump.cc
TST_DUMP_OBJS=$(TST_DUMP_SRCS:.cc=.o)

TEST_MINIFY = $(BIN_DIR)/test_minify
TST_MINIFY_SRCS=$(TST_DIR)/test_minify.cc
TST_MINIFY_OBJS=$(TST_MINIFY_SRCS:.cc=.o)

$(TEST_DUMP): $(UV_LIB) $(OBJS) $(TST_DUMP_OBJS) $(RAPIDJSON_DIR)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LIBS) $(OBJS) $(TST_DUMP_OBJS) $(UV_LIB) -o $@

$(TEST_MINIFY): $(UV_LIB) $(OBJS) $(TST_MINIFY_OBJS) $(RAPIDJSON_DIR)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LIBS) $(OBJS) $(TST_MINIFY_OBJS) $(UV_LIB) -o $@

test_dump: $(TEST_DUMP)
test_minify: $(TEST_MINIFY)

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
	@rm -f $(OBJS) \
		$(TEST_DUMP) $(TST_DUMP_OBJS) \
		$(TEST_MINIFY) $(TST_MINFY_OBJS) 

xcode:
	@mkdir -p build
	(cd build && rm -rf CMakeCache.txt CMakeFiles && \
		CC=`xcrun -find cc` CXX=`xcrun -find c++` LDFLAGS='$(UV_LIB)' cmake -G Xcode ..)

.PHONY: test_dump test_minify xcode
