include_directories(.)

add_executable(server
    main.cpp
    )

target_link_libraries(server utils spectrangle)
