include_directories("${PROJECT_SOURCE_DIR}/../deps/rapidjson/include")
file(GLOB rapidjson_headers ${PROJECT_SOURCE_DIR}/../deps/rapidjson/include/rapidjson/*)
source_group(deps\\rapidjson FILES ${rapidjson_headers})
