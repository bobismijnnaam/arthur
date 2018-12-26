catkin_add_gtest(UnitTests 
    TestMain.cpp
    trianglegrid_test.cpp
    spectrangle_test.cpp
    )

target_link_libraries(UnitTests
    utils
    spectrangle
    )
