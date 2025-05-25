cmake_minimum_required(VERSION 3.1)

project(ServerModule)

################################################################################
# Source files
################################################################################
file(GLOB COMMON_FILES
       ${PROJECT_SOURCE_DIR}/*.cpp
       ${PROJECT_SOURCE_DIR}/*.h)
	   
file(GLOB_RECURSE MODULE_FILES
       ${PROJECT_SOURCE_DIR}/Private/*.cpp
       ${PROJECT_SOURCE_DIR}/Public/*.h
	   )

file(GLOB_RECURSE THIRDPARTY_FILES
       ${PROJECT_SOURCE_DIR}/ThirdParty/*.h
       ${PROJECT_SOURCE_DIR}/ThirdParty/*.cpp
	   )

set(ALL_FILES
    ${COMMON_FILES}
    ${MODULE_FILES}
    ${THIRDPARTY_FILES}
	)
	
foreach(FILE ${ALL_FILES})
    # make rel path
	# FILE의 상대 경로 계산 후 REL_PATH에 저장
	file(RELATIVE_PATH REL_PATH "${PROJECT_SOURCE_DIR}" "${FILE}")
	set(ALL_FILES_REL ${ALL_FILES_REL} ${REL_PATH})
endforeach()	

set_source_files_properties(${ALL_FILES_REL} PROPERTIES HEADER_FILE_ONLY FALSE)

# 가상 폴더 지정
foreach(FILE ${ALL_FILES_REL})
    # get the base path
    get_filename_component(BASE_PATH "${FILE}" PATH)

    # changes /'s to \\'s
    string(REPLACE "/" "\\" GROUP "${BASE_PATH}")

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

################################################################################
# Precompiled Headers
################################################################################
set(ARG_PCH_HEADER pch.h)
set(ARG_PCH_SOURCE pch.cpp)
foreach (FILE ${ALL_FILES_REL})
  if (FILE MATCHES ".*\\.(c|cpp)$")
	if (FILE STREQUAL ${ARG_PCH_SOURCE})
	  set_source_files_properties(${FILE} PROPERTIES COMPILE_FLAGS "/Yc\"${ARG_PCH_HEADER}\"")
	else()
	  set_source_files_properties(${FILE} PROPERTIES COMPILE_FLAGS "/Yu\"${ARG_PCH_HEADER}\"")
	endif()
  endif()
endforeach(FILE)

add_executable(${PROJECT_NAME} WIN32 ${ALL_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "/SUBSYSTEM:CONSOLE"
)
