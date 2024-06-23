
add_library(
        opmath STATIC
        OPCurve.hpp
        OPMath.hpp
        OPCurve.cpp
        OPMath.cpp
        OPRect.hpp
        OPRect.cpp
)
target_include_directories(
        opmath
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(
        opmath
        cocos2d
)