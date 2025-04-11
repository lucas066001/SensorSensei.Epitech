# This is the CMake file for the audio_sensor component

idf_component_register(SRCS "audio_sensor_service.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES arduino)
