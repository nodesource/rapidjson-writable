RAPIDJSON_DIR      = $(DEPS)/rapidjson
RAPIDJSON_INCLUDES = $(RAPIDJSON_DIR)/include/

$(RAPIDJSON_DIR):
	git clone https://github.com/miloyip/rapidjson.git $@
