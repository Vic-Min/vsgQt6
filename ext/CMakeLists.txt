# glslang
include(glslang.cmake)

# VulkanSceneGraph
CPMAddPackage(
    NAME VulkanSceneGraph
    GITHUB_REPOSITORY vsg-dev/VulkanSceneGraph
    GIT_TAG "master"
    OPTIONS
        "BUILD_SHARED_LIBS ON"
)

# vsgXchange
CPMAddPackage(
    NAME vsgXchange
    GITHUB_REPOSITORY vsg-dev/vsgXchange
    GIT_TAG "master"
    OPTIONS
        "BUILD_SHARED_LIBS ON"
)
