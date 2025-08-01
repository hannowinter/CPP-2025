enable_testing()

file(GLOB ModelSources "${CMAKE_CURRENT_SOURCE_DIR}/model/*.cpp")
file(GLOB ControlSources "${CMAKE_CURRENT_SOURCE_DIR}/control/*.cpp")
file(GLOB ViewSources "${CMAKE_CURRENT_SOURCE_DIR}/view/*.cpp")

set(TestSources
    ${CMAKE_CURRENT_SOURCE_DIR}/GameTest.cpp
    )

add_executable(Test
    ${ModelSources}
    ${ControlSources}
    ${ViewSources}
    ${TestSources}
    )
