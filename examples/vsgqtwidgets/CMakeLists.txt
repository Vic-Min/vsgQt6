set(SOURCES
    main.cpp
)

set(HEADERS
)

add_executable(vsgqtwidgets ${MODE} ${SOURCES} ${HEADERS} ${FORMS})

target_link_libraries(vsgqtwidgets vsgQt6)

if (vsgXchange_FOUND)
    target_compile_definitions(vsgqtwidgets PRIVATE vsgXchange_FOUND)
    target_link_libraries(vsgqtwidgets vsgXchange::vsgXchange)
endif()


install(TARGETS vsgqtwidgets DESTINATION bin)

