cmake_minimum_required (VERSION 3.0.2)
project (nlib)

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

set(COPT "-Os")
set(CMCU "-mmcu=${MCU}")

set(CFLAGS   "${CMCU} ${COPT}")
set(CXXFLAGS "${CMCU} ${COPT}")

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS "${CFLAGS}")

include_directories("include")
add_library(adc.${MCU} "src/adc.c")
target_include_directories(adc.${MCU} PUBLIC INTERFACE include)

install(TARGETS adc.${MCU} ARCHIVE DESTINATION lib)
install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN *.h)
