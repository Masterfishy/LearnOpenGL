cmake_minimum_required(VERSION 3.22)
project(LearnOpenGL)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Emscripten specific configuration
set(EMSCRIPTEN_LINK_FLAGS
    "-s WASM=1"
    "-s USE_WEBGL2=1"
    "-s FULL_ES3=1"
    "-s MIN_WEBGL_VERSION=2"
    "-s MAX_WEBGL_VERSION=2"
    "-s ALLOW_MEMORY_GROWTH=1"
    "-s NO_EXIT_RUNTIME=1"
    # "-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
    "--bind"
    "-s USE_GLFW=3"
    # Flags for asset loading
    # "-s FETCH=1"
    # "-s ASYNCIFY=1"
    # "--preload-file assets"
    # Flags for audio support
    # "-s USE_SDL=2"
    # "-s USE_SDL_MIXER=2"
)
string(JOIN " " EMSCRIPTEN_LINK_FLAGS_STR ${EMSCRIPTEN_LINK_FLAGS})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EMSCRIPTEN_LINK_FLAGS_STR}")

# Set output file extensions
set(CMAKE_EXECUTABLE_SUFFIX ".js")

# Create the executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME} glfw # From Emscripten
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Copy resource files to build
add_custom_target(copy_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/web ${CMAKE_CURRENT_BINARY_DIR}
)
add_dependencies(${PROJECT_NAME} copy_resources)

# Output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
