cmake_minimum_required(VERSION 3.10)
project(chain C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO REQUIRED
    allegro-5
    allegro_image-5
    allegro_font-5
    allegro_primitives-5
    allegro_ttf-5
    allegro_main-5
    )


add_executable(chain 
    chain.c 
    sprites.c
    hero.c
    config.c
    attack.c
    gaal.c
    enemy.c
    bar.c
    )

target_include_directories(chain PRIVATE ${ALLEGRO_INCLUDE_DIRS})
target_link_libraries(chain PRIVATE ${ALLEGRO_LIBRARIES})
target_link_directories(chain PRIVATE ${ALLEGRO_LIBRARY_DIRS})
target_compile_options(chain PRIVATE ${ALLEGRO_CFLAGS_OTHER})