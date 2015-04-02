set(DB_SRC
    ../../../animation/Animation.cpp 
    ../../../animation/AnimationState.cpp 
    ../../../animation/TimelineState.cpp 
    ../../../animation/WorldClock.cpp 
    ../../../core/Armature.cpp 
    ../../../core/Bone.cpp 
    ../../../core/Object.cpp 
    ../../../core/Slot.cpp 
    ../../../events/EventData.cpp 
    ../../../factories/BaseFactory.cpp 
    ../../../parsers/BaseDataParser.cpp 
    ../../../parsers/ConstValues.cpp 
    ../../../parsers/XMLDataParser.cpp 
    ../../../parsers/dbtinyxml2.cpp 
    ../DBCCArmature.cpp 
    ../DBCCArmatureNode.cpp 
    ../DBCCFactory.cpp 
    ../DBCCSlot.cpp
	../DBCCUtils.cpp
)

include_directories(
  .. ../../..
)

add_library(dragonbones STATIC
  ${DB_SRC}
)

target_link_libraries(dragonbones
)

set_target_properties(dragonbones
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

