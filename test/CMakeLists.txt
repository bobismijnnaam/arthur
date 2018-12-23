catkin_add_gtest(Tests 
    TestMain.cpp
    infinite_random_test.cpp
    trianglegrid_test.cpp
    spectrangle_test.cpp
    )

target_link_libraries(Tests
    utils
    spectrangle
    )
