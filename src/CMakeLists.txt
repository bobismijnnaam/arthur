include_directories(.)

include_directories(imgui)
include_directories(imgui/examples)
include_directories(imgui/examples/libs/gl3w)
add_subdirectory(imgui)

add_subdirectory(testgui)
add_subdirectory(daemon)

