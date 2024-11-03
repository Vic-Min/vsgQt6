set(SOURCES
    main.cpp
)

set(HEADERS
)

add_executable(vsgqtmultiviewer ${MODE} ${SOURCES} ${HEADERS} ${FORMS})

target_link_libraries(vsgqtmultiviewer vsgQt6)

if (vsgXchange_FOUND)
    target_compile_definitions(vsgqtmultiviewer PRIVATE vsgXchange_FOUND)
    target_link_libraries(vsgqtmultiviewer vsgXchange::vsgXchange)
endif()


install(TARGETS vsgqtmultiviewer DESTINATION bin)

