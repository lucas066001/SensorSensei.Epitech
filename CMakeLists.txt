cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Add extra components to the project (Arduino is one of the components)
set(EXTRA_COMPONENT_DIRS 
    components/sensors/audio_sensor
    components/sensors/dust_sensor
    components/sensors/lora_sender
    ${IDF_PATH}/components/arduino)

project(SensorSenseisender)
