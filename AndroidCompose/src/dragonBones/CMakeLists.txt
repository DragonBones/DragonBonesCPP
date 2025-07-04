cmake_minimum_required(VERSION 3.22.1)

project("dragonbones_native")

# Define path to the root of the DragonBonesCPP repository
set(DB_ROOT_PATH ../../..)

# Define path to the core DragonBones library
set(DB_CORE_SRC_DIR ${DB_ROOT_PATH}/DragonBones/src)

# Gather all C++ source files from the core dragonbones library
file(GLOB_RECURSE DRAGONBONES_SOURCES
    "${DB_CORE_SRC_DIR}/dragonBones/animation/*.cpp"
    "${DB_CORE_SRC_DIR}/dragonBones/armature/*.cpp"
    "${DB_CORE_SRC_DIR}/dragonBones/core/*.cpp"
    "${DB_CORE_SRC_DIR}/dragonBones/event/*.cpp"
    "${DB_CORE_SRC_DIR}/dragonBones/factory/*.cpp"
    "${DB_CORE_SRC_DIR}/dragonBones/geom/*.cpp"
    "${DB_CORE_SRC_DIR}/dragonBones/model/*.cpp"
    "${DB_CORE_SRC_DIR}/dragonBones/parser/*.cpp"
)

# Add platform-specific implementation and JNI bridge
add_library(
    dragonbones_native
    SHARED
    "cpp/JniBridge.cpp"
    "cpp/opengl/OpenGLSlot.cpp"
    "cpp/opengl/OpenGLFactory.cpp"
    ${DRAGONBONES_SOURCES}
)

# Specify include directories for headers
target_include_directories(dragonbones_native PRIVATE
    "cpp" # For JniBridge.h, opengl headers
    "${DB_CORE_SRC_DIR}" # For all DragonBones core headers
    "${DB_ROOT_PATH}/3rdParty" # For rapidjson
)

# Link against required Android libraries
find_library(log-lib log)
find_library(GLESv2-lib GLESv2)
find_library(android-lib android)

target_link_libraries(
    dragonbones_native
    ${log-lib}
    ${GLESv2-lib}
    ${android-lib}
)