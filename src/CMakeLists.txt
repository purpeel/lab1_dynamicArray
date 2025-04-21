add_library(lab1_library
    collection.c
    doubleTypeinfo.c
    stringTypeinfo.c
    input.c
    menu.c
    ui.c
    sort.c
    util.c
)

target_link_libraries(lab1_library m)

add_executable(program
    main.c
)

target_link_libraries(program
    lab1_library
)