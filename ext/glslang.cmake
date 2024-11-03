# Can't use CPM due to: https://github.com/vsg-dev/VulkanSceneGraph/issues/1271
# TODO: Switch to using ExternalProject_Add instead.

include(GNUInstallDirs)

# Set paths
set(SPIRV-Tools_DIR ${CMAKE_CURRENT_BINARY_DIR}/glslang-install/SPIRV-Tools/cmake CACHE PATH "" FORCE)
set(SPIRV-Tools-opt_DIR ${CMAKE_CURRENT_BINARY_DIR}/glslang-install/SPIRV-Tools-opt/cmake CACHE PATH "" FORCE)
set(glslang_DIR ${CMAKE_CURRENT_BINARY_DIR}/glslang-install/${CMAKE_INSTALL_LIBDIR}/cmake/glslang CACHE PATH "" FORCE)

set(GLSLANG_REQUIRED_VERSION 14.3.0)

# Skip if it's already installed
find_package(glslang ${GLSLANG_REQUIRED_VERSION})

if((NOT ${glslang_FOUND}) OR ())
    message("[glslang-wrapper message]: glslang was not found - fetching and building.")

    include(FetchContent)

    set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE NEVER)

    # Download the source
    FetchContent_Declare(glslang
        GIT_REPOSITORY "https://github.com/KhronosGroup/glslang.git"
        GIT_TAG "4a9f08891540263e19c2468dd602ffdd8c446390" # Use this commit (version 14.3) until VSG updates against glslang 15
        GIT_PROGRESS TRUE
        FIND_PACKAGE_ARGS
    )
    FetchContent_Populate(glslang)
    message("glslang_SOURCE_DIR=${glslang_SOURCE_DIR}")
    message("glslang_BINARY_DIR=${glslang_BINARY_DIR}")

    message("[glslang-wrapper message]: Running update_glslang_sources.py...")
    execute_process(
        COMMAND python update_glslang_sources.py
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
    message("[glslang-wrapper message]: Installing glslang to ${CMAKE_CURRENT_BINARY_DIR}/glslang-install")
    # Install it
    execute_process(
        COMMAND ${CMAKE_COMMAND} --install . --prefix ${CMAKE_CURRENT_BINARY_DIR}/glslang-install --config ${CMAKE_BUILD_TYPE}
        WORKING_DIRECTORY ${glslang_BINARY_DIR}
    )

    set(glslang_DIR ${CMAKE_CURRENT_BINARY_DIR}/glslang-install/${CMAKE_INSTALL_LIBDIR}/cmake/glslang CACHE PATH "" FORCE)
    find_package(glslang ${GLSLANG_REQUIRED_VERSION})
else()
    message("[glslang-wrapper message]: glslang found. If you have reason to think that it might be out-of-date, delete ${CMAKE_CURRENT_BINARY_DIR}/glslang-install and re-run cmake.")
endif()

message("* glslang_DIR=${glslang_DIR}")
