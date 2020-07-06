# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required( VERSION 3.10.3 )
project( sequencer LANGUAGES CXX )

set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )


set( GCC_FLAGS "-fmax-errors=1 -Wall -Wextra -Werror" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_FLAGS}" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 ${GCC_FLAGS}" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 ${GCC_FLAGS}" )
set( CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_RELEASE} -Os ${GCC_FLAGS}" )

set( CMAKE_EXE_LINKER_FLAGS  ${CMAKE_EXE_LINKER_FLAGS} -lpthread ) 

find_package( Qt5 COMPONENTS Widgets Core Quick Multimedia REQUIRED )
find_package( SDL2 REQUIRED )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set( MIDIFILE_INCLUDE_DIRS lib/midifile )

# include( lib/midifile/CMakeLists.txt )
include_directories(
    ${CMAKE_SOURCE_DIR}

    ${SDL2_INCLUDE_DIRS}
    ${MIDIFILE_INCLUDE_DIRS}
    ${MIDIFILE_INCLUDE_DIRS}/include
)

add_library( midifile STATIC
    lib/midifile/include/Binasc.h
    lib/midifile/include/MidiEvent.h
    lib/midifile/include/MidiEventList.h
    lib/midifile/include/MidiFile.h
    lib/midifile/include/MidiMessage.h
    lib/midifile/include/Options.h

    lib/midifile/src/Options.cpp
    lib/midifile/src/Binasc.cpp
    lib/midifile/src/MidiEvent.cpp
    lib/midifile/src/MidiEventList.cpp
    lib/midifile/src/MidiFile.cpp
    lib/midifile/src/MidiMessage.cpp
)

set(
    MAIN_FILES

    app/Clock.h
    app/EngineModel.h
    app/PadModel.h
    app/PadModel.cpp
    app/Piano.h
    app/main.h
    app/main.cpp

    res/qml.qrc
)

add_executable( ${PROJECT_NAME}  ${MAIN_FILES} )

target_link_libraries( ${PROJECT_NAME}
    midifile
    SDL2
    SDL2_mixer
    Qt5::Widgets
    Qt5::Core
    Qt5::Quick
    Qt5::Multimedia )

add_custom_command(
    TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res/wav $<TARGET_FILE_DIR:sequencer>/wav)
