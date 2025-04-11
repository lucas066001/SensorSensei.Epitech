# This is the CMake file for the dust_sensor component

idf_component_register(SRCS "dust_sensor_service.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES arduino)
