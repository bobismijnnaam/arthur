include_directories(.)

add_executable(testgui
    main.cpp
    )

target_link_libraries(testgui imgui gl3w GL dl ${SDL2_LIBRARY})
