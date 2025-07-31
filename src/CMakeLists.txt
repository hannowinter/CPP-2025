# Models, Views and Controllers
file(GLOB ModelSources "${CMAKE_CURRENT_SOURCE_DIR}/model/*.cpp")
file(GLOB ControlSources "${CMAKE_CURRENT_SOURCE_DIR}/control/*.cpp")
file(GLOB ViewSources "${CMAKE_CURRENT_SOURCE_DIR}/view/*.cpp")

# add src code to target
add_executable(SpaceInvaders
        ${ModelSources}
        ${ControlSources}
        ${ViewSources}
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )