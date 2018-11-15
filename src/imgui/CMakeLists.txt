add_library(imgui SHARED
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_widgets.cpp

    examples/imgui_impl_opengl3.cpp
    examples/imgui_impl_sdl.cpp

    examples/libs/gl3w/GL/gl3w.c
)

set(IMGUI_LIBRARIES imgui GL dl ${SDL2_LIBRARY} PARENT_SCOPE)
