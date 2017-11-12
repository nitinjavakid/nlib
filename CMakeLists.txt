cmake_minimum_required (VERSION 3.0.2)
project (nlib)

set(LINKER_LANGUAGE CXX)

set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP  avr-objdump)
set(AVRSIZE  avr-size)
set(AVRDUDE  avrdude)

set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_ASM_COMPILER ${AVRC})

set(COPT "-Os ${COPT}")
set(CMCU "-mmcu=${MCU}")

set(CFLAGS   "${CMCU} ${COPT}")
set(CXXFLAGS "${CMCU} ${COPT} -std=c++11")

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS "${CFLAGS}")

include_directories("include")
install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN *.h)

add_library(adc.${MCU} "src/adc.c")
install(TARGETS adc.${MCU} ARCHIVE DESTINATION lib)

add_library(usart.${MCU} "src/usart.cpp")
install(TARGETS usart.${MCU} ARCHIVE DESTINATION lib)

add_library(delay.${MCU} "src/delay.c")
install(TARGETS delay.${MCU} ARCHIVE DESTINATION lib)

add_library(io.${MCU} "src/io.cpp")
install(TARGETS io.${MCU} ARCHIVE DESTINATION lib)

add_library(wifi.${MCU} "src/wifi.cpp")
install(TARGETS wifi.${MCU} ARCHIVE DESTINATION lib)

add_library(memory.${MCU} "src/memory.cpp")
install(TARGETS memory.${MCU} ARCHIVE DESTINATION lib)

add_library(twi.${MCU} "src/twi.cpp")
install(TARGETS twi.${MCU} ARCHIVE DESTINATION lib)

add_library(debug.${MCU} "src/debug.cpp")
install(TARGETS debug.${MCU} ARCHIVE DESTINATION lib)

add_library(http.${MCU} "src/http.cpp")
install(TARGETS http.${MCU} ARCHIVE DESTINATION lib)

#wifi drivers
add_library(esp8266.${MCU} "src/esp8266.cpp")
install(TARGETS esp8266.${MCU} ARCHIVE DESTINATION lib)
