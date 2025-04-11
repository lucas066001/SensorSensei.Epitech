# This is the CMake file for the lora_sender component

idf_component_register(SRCS "lora_sender.c"
                    INCLUDE_DIRS "."
                    REQUIRES arduino)

