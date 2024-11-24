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

# assimp (optional)
# ASSIMP_BUILD_ZLIB is enabled because the static library on Linux (at-least Ubuntu) is not compiled with fPIC
if(VSGQT6_BUILD_EXAMPLES)
    CPMAddPackage(
        NAME              "assimp"
        GITHUB_REPOSITORY "assimp/assimp"
        VERSION           "5.4.2"
        OPTIONS           "ASSIMP_BUILD_TESTS OFF" "ASSIMP_INSTALL OFF" "BUILD_SHARED_LIBS OFF" "ASSIMP_INSTALL_PDB OFF" "ASSIMP_BUILD_ZLIB ON"
        EXCLUDE_FROM_ALL  YES
    )
    if (assimp_ADDED)
        set(assimp_VERSION "5.4.0" CACHE INTERNAL "assimp_VERSION")
        get_target_property(assimp_INCLUDE_DIRS assimp::assimp INCLUDE_DIRECTORIES)
        set(assimp_INCLUDE_DIRS ${assimp_INCLUDE_DIRS} CACHE INTERNAL "assimp_INCLUDE_DIRS")
    endif()

    # vsgXchange
    CPMAddPackage(
        NAME vsgXchange
        GITHUB_REPOSITORY vsg-dev/vsgXchange
        GIT_TAG "master"
        OPTIONS
            "BUILD_SHARED_LIBS ON"
    )
endif()
