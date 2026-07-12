# Configure using the modern Ninja generator (installed in Step 1)
# cmake -S . -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/msys64/ucrt64"

# Compile the executable
# cmake --build build

cmake_minimum_required(VERSION 3.11)

project(
    CPPSWEEPER
    VERSION 1.0
    LANGUAGES CXX
)

# Declare the executable target
add_executable(minesweeper
    src/main.cpp
)

# Match MSYS2's library configuration (Fixes the undefined reference!)
target_compile_definitions(minesweeper PRIVATE 
    PDC_STATIC_BUILD
    PDC_WIDE
    PDC_FORCE_UTF8
)

# Set C++ standard
set_target_properties(minesweeper PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)

# Explicitly define the UCRT64 pdcurses include path
set(PDCURSES_INCLUDE_DIR "C:/msys64/ucrt64/include/pdcurses")

# Declare paths to include in the compiler
target_include_directories(minesweeper PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/headers
    ${PDCURSES_INCLUDE_DIR}
)

# Link directly to the UCRT system static archive
find_library(PDCURSES_LIB NAMES libpdcurses.a REQUIRED PATHS "C:/msys64/ucrt64/lib")

target_link_libraries(minesweeper PRIVATE
    ${PDCURSES_LIB}
    winmm
)

# Force static linking so the target exe is completely independent
target_link_options(minesweeper PRIVATE -static)