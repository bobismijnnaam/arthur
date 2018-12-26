catkin_add_gtest(StochasticTests 
    TestMain.cpp
    infinite_random_test.cpp
    pick_random_move_test.cpp
    )

target_link_libraries(StochasticTests
    utils
    spectrangle
    )
