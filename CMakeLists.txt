cmake_minimum_required(VERSION 3.22)
project(LexonEngine LANGUAGES CXX)

# ==================== Configuración C++ ====================
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS       ON)

# ==================== Opciones de Vulkan ====================
option(USE_VALIDATION_LAYERS "Enable Vulkan Validation Layers" OFF)

# ==================== Buscar Vulkan ====================
find_package(Vulkan REQUIRED)

if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan SDK no encontrado!")
endif()




# ==================== GLFW ====================
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE
)

# Configuración de GLFW (solo la librería)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# ==================== STB_IMAGE ====================
FetchContent_Declare(
    stb_image
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(stb_image)

# ==================== Dear ImGui (docking branch) ====================
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        docking
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(imgui)

set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    # Backends
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC 
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw Vulkan::Vulkan)
# ==================== Cargar shaders ====================

function (add_slang_shader_target TARGET)
  cmake_parse_arguments ("SHADER" "" "" "SOURCES" ${ARGN})
  set (SHADERS_DIR ${CMAKE_CURRENT_LIST_DIR}/shaders)
  set (ENTRY_POINTS -entry vertMain -entry fragMain)
  add_custom_command (
          OUTPUT ${SHADERS_DIR}
          COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DIR}
  )
  add_custom_command (
          OUTPUT  ${SHADERS_DIR}/slang.spv
          COMMAND ${SLANGC_EXECUTABLE} ${SHADER_SOURCES} -target spirv -profile spirv_1_4 -emit-spirv-directly -fvk-use-entrypoint-name ${ENTRY_POINTS} -o slang.spv
          WORKING_DIRECTORY ${SHADERS_DIR}
          DEPENDS ${SHADERS_DIR} ${SHADER_SOURCES}
          COMMENT "Compiling Slang Shaders"
          VERBATIM
  )
  add_custom_target (${TARGET} DEPENDS ${SHADERS_DIR}/slang.spv)
endfunction()


# ==================== Archivos fuente ====================
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.h"

)

add_executable(${PROJECT_NAME} ${SOURCES})

# ==================== Include y Link ====================
target_include_directories(${PROJECT_NAME} PRIVATE
    ${Vulkan_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
    ${stb_image_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/third_party/tinyobjloader
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    glfw
    imgui
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE dwmapi)
endif()

# ==================== Definiciones ====================
if(USE_VALIDATION_LAYERS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_VALIDATION_LAYERS)
endif()


add_custom_target(clear
    COMMAND powershell -NoProfile -ExecutionPolicy Bypass 
            -Command "Remove-Item -Path '${CMAKE_SOURCE_DIR}/build/*' -Recurse -Force -ErrorAction SilentlyContinue"
    COMMENT "Limpiando carpeta build con PowerShell..."
    VERBATIM
)

# ==================== Opciones de compilación ====================
if(MSVC)
    add_slang_shader_target( foo SOURCES ${SHADER_SLANG_SOURCES})
    add_dependencies(bar foo)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /MP)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()





# ==================== Copiar assets ====================
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    ${CMAKE_SOURCE_DIR}/assets
#    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
#    COMMENT "Copiando carpeta de assets..."
#)