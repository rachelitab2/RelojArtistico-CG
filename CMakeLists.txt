cmake_minimum_required(VERSION 3.20)

project(RelojArtistico VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/main.c
    src/display.c
    src/clock.c
    src/segments.c
    src/utils.c
    src/artwork_catalog.c
    src/app_config.c
    src/ui.c
    src/audio.c
    src/texture.c
    src/segment_hokusai.c
    src/segment_vangogh.c
    src/segment_kandinsky.c
    src/segment_mondrian.c
    src/segment_monet.c
    src/segment_klimt.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    opengl32
    glu32
    freeglut
    winmm
)
