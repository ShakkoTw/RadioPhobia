idf_component_register(SRCS "radiophobia.c"
                    INCLUDE_DIRS ".")

target_link_libraries(${COMPONENT_LIB} "-z muldefs")
