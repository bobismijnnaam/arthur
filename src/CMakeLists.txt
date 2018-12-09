include_directories(.)

include_directories(utils)
add_subdirectory(utils)

include_directories(imgui)
include_directories(imgui/examples)
include_directories(imgui/examples/libs/gl3w)
add_subdirectory(imgui)

add_subdirectory(testgui)
add_subdirectory(daemon)

include_directories(spectrangle)
add_subdirectory(spectrangle)

add_subdirectory(infinite_random_test)

