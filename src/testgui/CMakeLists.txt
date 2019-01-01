include_directories(.)

add_executable(testgui
    main.cpp
    )

target_link_libraries(testgui utils spectrangle ${IMGUI_LIBRARIES})
