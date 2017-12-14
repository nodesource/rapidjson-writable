ROOT        = $(shell pwd)
DEPS        = $(ROOT)/deps

-include rapidjson.mk
-include uv.mk

CCFLAGS = $(UV_FLAGS) -std=c++11 -g

INC_DIR = $(ROOT)/include
TST_DIR = $(ROOT)/test
BIN_DIR = $(ROOT)/bin

INCS =-I$(UV_INCLUDES) -I$(RAPIDJSON_INCLUDES) -I$(INC_DIR)

TEST_DUMP = $(BIN_DIR)/test_dump
TST_DUMP_SRCS=$(TST_DIR)/test_dump.cc
TST_DUMP_OBJS=$(TST_DUMP_SRCS:.cc=.o)

TEST_MINIFY = $(BIN_DIR)/test_minify
TST_MINIFY_SRCS=$(TST_DIR)/test_minify.cc
TST_MINIFY_OBJS=$(TST_MINIFY_SRCS:.cc=.o)

$(TEST_DUMP): $(UV_LIB) $(TST_DUMP_OBJS) $(RAPIDJSON)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LIBS) $(TST_DUMP_OBJS) $(UV_LIB) -o $@

$(TEST_MINIFY): $(UV_LIB) $(TST_MINIFY_OBJS) $(RAPIDJSON)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LIBS) $(TST_MINIFY_OBJS) $(UV_LIB) -o $@

test_dump: $(TEST_DUMP)
test_minify: $(TEST_MINIFY)

.SUFFIXES: .cc .o
.cc.o:
	@([ -d $(RAPIDJSON_DIR) ] || $(MAKE) $(RAPIDJSON_DIR)) && \
	$(CXX) $< $(CCFLAGS) $(INCS) -c -o $@

clean:
	@rm -f $(TEST_DUMP) $(TST_DUMP_OBJS) \
		$(TEST_MINIFY) $(TST_MINFY_OBJS)
	@rm -rf $(ROOT)/build/CMakeCache.txt $(ROOT)/build/CMakeFiles

xcode:
	@mkdir -p build
	(cd build && \
		CC=`xcrun -find cc` CXX=`xcrun -find c++` LDFLAGS='$(UV_LIB)' cmake -G Xcode ..)

ninja:
	@mkdir -p build
	(cd build && \
		CC=`xcrun -find cc` CXX=`xcrun -find c++` LDFLAGS='$(UV_LIB)' cmake -G Ninja ..) && \
	ninja -C build

.PHONY: test_dump test_minify xcode
