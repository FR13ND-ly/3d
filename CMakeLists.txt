cmake_minimum_required(VERSION 3.28)
project(3d LANGUAGES CXX)

# Set runtime output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Option to build shared libraries
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Fetch SFML from the repository
include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

include_directories(${CMAKE_SOURCE_DIR}/modules/nlohmann_json/include)
include_directories(${CMAKE_SOURCE_DIR}/modules/cpptoml/include)

set(SOURCE_FILES
        src/main.cpp
        src/core/feature/App.cpp
        src/core/feature/Camera.cpp
        src/core/feature/Renderer.cpp
        src/core/feature/Scene.cpp
        src/core/objects/Cube.cpp
        src/core/objects/CustomShape.cpp
        src/core/objects/GridPlane.cpp
        src/core/objects/Object3d.cpp
        src/core/objects/ObjectsFactory.cpp
        src/core/objects/Sphere.cpp
        src/core/ui/Button.cpp
        src/core/ui/Checkbox.cpp
        src/core/ui/Component.cpp
        src/core/ui/Expandable.cpp
        src/core/ui/Input.cpp
        src/core/ui/NumberInput.cpp
        src/core/ui/Select.cpp
        src/core/ui/Slider.cpp
        src/core/ui/ScrollView.cpp
        src/core/ui/Surface.cpp
        src/core/ui/Text.cpp
        src/utils/FontManager.cpp
        src/utils/LanguageManager.cpp
        src/utils/WindowManager.cpp
        src/utils/files/Config.cpp
        src/utils/files/FileManager.cpp
        src/utils/files/FileType.cpp
        src/utils/files/JsonFile.cpp
        src/utils/files/TomlFile.cpp
        src/utils/math/Matrix4.cpp
        src/utils/math/Vector3.cpp
        src/utils/math/Vector4.cpp
        src/views/EditorView.cpp
        src/views/HomeView.cpp
        src/views/ProjectsView.cpp
        src/views/SettingsView.cpp
        src/views/View.cpp
        src/views/ViewsManager.cpp
        src/views/editor-menu/AddObjectsMenu.cpp
        src/views/editor-menu/MenuManager.cpp
        src/views/editor-menu/Menu.cpp
        src/views/editor-menu/ObjectsMenu.cpp
        src/views/editor-menu/VerticesMenu.cpp
)

# Create the executable with all the source files
add_executable(3d ${SOURCE_FILES})

# Link SFML and other necessary libraries
target_link_libraries(3d PRIVATE sfml-graphics)

# Set C++ standard to 17
target_compile_features(3d PRIVATE cxx_std_17)

# Copy OpenAL DLL on Windows
if(WIN32)
    add_custom_command(
            TARGET 3d
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:3d>
            VERBATIM)
endif()
