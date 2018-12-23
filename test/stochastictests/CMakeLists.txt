catkin_add_gtest(Tests 
    TestMain.cpp
    infinite_random_test.cpp
    pick_random_move_test.cpp
    )

target_link_libraries(Tests
    utils
    spectrangle
    )
