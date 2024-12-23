# Can't use CPM due to: https://github.com/vsg-dev/VulkanSceneGraph/issues/1271
# TODO: Switch to using ExternalProject_Add instead.

include(GNUInstallDirs)

set(GLSLANG_INSTALL_DIR "${CMAKE_FIND_PACKAGE_REDIRECTS_DIR}/glslang")

# Set paths
if(APPLE)
    set(SPIRV-Tools_DIR ${GLSLANG_INSTALL_DIR}/lib/cmake/SPIRV-Tools CACHE PATH "" FORCE)
    set(SPIRV-Tools-opt_DIR ${GLSLANG_INSTALL_DIR}/lib/cmake/SPIRV-Tools-opt CACHE PATH "" FORCE)
else()
    set(SPIRV-Tools_DIR ${GLSLANG_INSTALL_DIR}/SPIRV-Tools/cmake CACHE PATH "" FORCE)
    set(SPIRV-Tools-opt_DIR ${GLSLANG_INSTALL_DIR}/SPIRV-Tools-opt/cmake CACHE PATH "" FORCE)
endif()

set(GLSLANG_REQUIRED_VERSION 14.3.0)

# Skip if it's already installed
find_package(glslang ${GLSLANG_REQUIRED_VERSION})

if(NOT ${glslang_FOUND})
    message("[glslang-wrapper message]: glslang was not found - fetching and building.")

    include(FetchContent)

    set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE NEVER)

    # Download the source
    FetchContent_Declare(glslang
        GIT_REPOSITORY "https://github.com/KhronosGroup/glslang.git"
        GIT_TAG "4a9f08891540263e19c2468dd602ffdd8c446390" # Use this commit (version 14.3) until VSG updates against glslang 15
        GIT_PROGRESS TRUE
    )
    FetchContent_GetProperties(glslang)
    if (NOT glslang_POPULATED)
        FetchContent_Populate(glslang)
    endif()
    message("glslang_SOURCE_DIR=${glslang_SOURCE_DIR}")
    message("glslang_BINARY_DIR=${glslang_BINARY_DIR}")

    message("[glslang-wrapper message]: Running update_glslang_sources.py...")
    execute_process(
        COMMAND python3 update_glslang_sources.py
        WORKING_DIRECTORY ${glslang_SOURCE_DIR}
    )

    message("[glslang-wrapper message]: Configuring glslang...")
    # Configure it
    execute_process(
        COMMAND ${CMAKE_COMMAND} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_SHARED_LIBS=OFF -DGLSLANG_ENABLE_INSTALL=ON -DGLSLANG_TESTS=OFF ${glslang_SOURCE_DIR}
        WORKING_DIRECTORY ${glslang_BINARY_DIR}
    )
    message("[glslang-wrapper message]: Building glslang...")
    # Build it
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build . -j16 --config ${CMAKE_BUILD_TYPE}
        WORKING_DIRECTORY ${glslang_BINARY_DIR}
    )

    message("[glslang-wrapper message]: Installing glslang to ${GLSLANG_INSTALL_DIR}")
    # Install it
    execute_process(
        COMMAND ${CMAKE_COMMAND} --install . --prefix ${GLSLANG_INSTALL_DIR} --config ${CMAKE_BUILD_TYPE}
        WORKING_DIRECTORY ${glslang_BINARY_DIR}
    )

    find_package(glslang ${GLSLANG_REQUIRED_VERSION})
else()
    message("[glslang-wrapper message]: glslang found. If you have reason to think that it might be out-of-date, delete ${GLSLANG_INSTALL_DIR} and re-run cmake.")
endif()

message("* glslang_DIR=${glslang_DIR}")
