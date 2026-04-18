cmake_minimum_required(VERSION 3.28)

project(out-project)
add_executable(out)
set_target_properties(out PROPERTIES
    C_STANDARD              99
    C_STANDARD_REQUIRED     ON
    C_EXTENSIONS            OFF
    EXPORT_COMPILE_COMMANDS ON
)

target_compile_definitions(out PRIVATE
    $<IF:$<CONFIG:Debug>,DEBUG,NDEBUG>
)

target_compile_options(out PRIVATE
    $<$<CONFIG:Debug>:-g3>
    $<$<CONFIG:Release>:-O2>
)
target_link_options   (out PRIVATE
    $<$<CONFIG:Release>:-s>
)
target_compile_options(out PRIVATE
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wdeprecated
    -Wimplicit-fallthrough
)


target_compile_options(out PRIVATE
    -Wextra-semi
    -Wsuggest-override
    -Wnon-virtual-dtor
    -Wshadow
    -Wformat=2 
    #-Wfloat-equal
    -Wzero-as-null-pointer-constant
    -Wdouble-promotion
    -Wcast-qual
    -Wcast-align
    -Wold-style-cast
    -Wno-unknown-warning-option
)

target_compile_options(out PRIVATE
    -Wno-unused-parameter
)

set(san_enable 1)# $<CONFIG:Debug>)
set(san_args
    -fsanitize=undefined,implicit-conversion,float-divide-by-zero,nullability
    $<$<CONFIG:Debug>:-O1 -fsanitize=address>
    $<$<CONFIG:Release>:-fsanitize-minimal-runtime>
)
target_compile_options(out PRIVATE $<${san_enable}:${san_args}>)
target_link_options   (out PRIVATE $<${san_enable}:${san_args}>)
target_compile_options(out PRIVATE $<${san_enable}:
    $<$<CONFIG:Debug>:
        -fno-omit-frame-pointer
        -fno-optimize-sibling-calls
        -fno-sanitize-recover
    >
#    -fsanitize-recover=unsigned-integer-overflow
>)

target_include_directories(out PRIVATE include)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

target_sources(out PRIVATE ${sources})
