find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
target_link_libraries(main.cpp PRIVATE CURL::libcurl nlohmann_json nlohmann_json::nlohmann_json)
