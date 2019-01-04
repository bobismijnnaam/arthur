include_directories(.)

add_executable(testgui
    main.cpp
    GuiUtils.cpp
    TriangleRenderer.cpp
    SpectrangleTexture.cpp
    TileTexture.cpp
    SpectrangleGameStateWindow.cpp
    )

target_link_libraries(testgui utils spectrangle ${IMGUI_LIBRARIES})
