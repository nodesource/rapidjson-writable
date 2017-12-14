ROOT        = $(shell pwd)
DEPS        = $(ROOT)/deps

-include rapidjson.mk
-include libuv.mk

CCFLAGS = $(UV_FLAGS) -std=c++11 -g

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

$(TEST_DUMP): $(UV_LIB) $(OBJS) $(TST_DUMP_OBJS) $(RAPIDJSON)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LIBS) $(OBJS) $(TST_DUMP_OBJS) $(UV_LIB) -o $@

$(TEST_MINIFY): $(UV_LIB) $(OBJS) $(TST_MINIFY_OBJS) $(RAPIDJSON) 
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LIBS) $(OBJS) $(TST_MINIFY_OBJS) $(UV_LIB) -o $@

test_dump: $(TEST_DUMP)
test_minify: $(TEST_MINIFY)

.SUFFIXES: .cc .o
.cc.o:
	@([ -d $(RAPIDJSON_DIR) ] || $(MAKE) $(RAPIDJSON_DIR)) && \
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
