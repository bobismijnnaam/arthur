include_directories(.)

add_subdirectory(GL)

include_directories(imgui)
include_directories(imgui/examples)
add_subdirectory(imgui)

add_subdirectory(testgui)
add_subdirectory(daemon)

