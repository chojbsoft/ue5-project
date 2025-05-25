# CMake Setting for Server 

cmake_minimum_required(VERSION 3.1)

# c++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CMakeDependentOption)

# 프로젝트명 정의
set(MAIN_PROJECT_NAME Server)
project(${MAIN_PROJECT_NAME})

set(RUNTIME_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source/Runtime)

# 윈도우 MSVC 환경만 허용
if(NOT WIN32)
	message(FATAL_ERROR "This project only supports windows environment currently.")
endif()
if(NOT MSVC)
	message(FATAL_ERROR "This project only supports MSVC project currently.")
endif()

# 확장성을 위한 플랫폼별 변수 설정
set(PLATFORM_DEFINITION PLATFORM_GENERIC)
set(PLATFORM_FOLDER Generic)

option(TARGET_WINDOWS "Windows Option" ON)

if(TARGET_WINDOWS)
	set(PLATFORM_DEFINITION PLATFORM_WINDOWS)
	set(PLATFORM_FOLDER Windows)
endif()

# 서브 디렉터리 추가
add_subdirectory(Source/Runtime/Server)

# 자동 CMake 재생성 막기
set(CMAKE_SUPPRESS_REGENERATION true)

# 처음 실행될 프로젝트 지정
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ServerModule)
