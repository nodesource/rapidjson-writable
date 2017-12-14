RAPIDJSON_DIR      = $(DEPS)/rapidjson
RAPIDJSON_INCLUDES = $(RAPIDJSON_DIR)/include/

$(RAPIDJSON_DIR):
	git clone git@github.com:miloyip/rapidjson.git $@
