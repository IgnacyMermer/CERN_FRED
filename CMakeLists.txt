cmake_minimum_required(VERSION 3.1)
#set(CMAKE_CXX_STANDARD 17)
project(FREDServer)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include/FREDServer include include/occi)
include_directories(${CMAKE_SOURCE_DIR}/dim/dim)

if (APPLE)
    link_directories(/usr/local/lib /usr/local/lib/occi)
else ()
    link_directories(lib)
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ./lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ./lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

set(ALFRED_SOURCES
		"src/Alfred/alfred.cpp"
		"src/Alfred/client.cpp"
		"src/Alfred/command.cpp"
		"src/Alfred/info.cpp"
		"src/Alfred/print.cpp"
		"src/Alfred/rpc.cpp"
		"src/Alfred/rpcinfo.cpp"
		"src/Alfred/service.cpp"
        "src/Alfred/dimutilities.cpp"
        "src/Alfred/queuelock.cpp"
	)

add_library(Alfred SHARED ${ALFRED_SOURCES})

if (MAPI)
    message(STATUS "Including MAPI")
    include_directories(../mapi/include)
    add_compile_definitions(USE_MAPI)
    file(GLOB_RECURSE MAPI_SOURCES
        "../mapi/src/mapifactory.cpp"
        "../mapi/src/*.cpp"
    )
endif ()

if (APPLE)
    target_link_libraries(Alfred libdim.a)
else ()
    target_link_libraries(Alfred libdim.a libpthread.so)
endif ()

file(GLOB SOURCES "src/main.cpp"
		"src/Parser/utility.cpp"
        "src/Parser/parser.cpp"
        "src/Parser/processmessage.cpp"
        "src/Fred/alfclients.cpp"
        "src/Fred/alfrpcinfo.cpp"
        "src/Fred/fred.cpp"
        "src/Fred/fredtopics.cpp"
        "src/Fred/mappedcommand.cpp"
        "src/Fred/groupcommand.cpp"
        "src/Fred/queue.cpp"
        "src/Fred/queueexecutor.cpp"
		"src/Fred/section.cpp"
        "src/Fred/selectcommand.cpp"
        "src/Fred/alfinfo.cpp"
        "src/Fred/crualfrpcinfo.cpp"
        "src/Fred/cruregistercommand.cpp"
        "src/Fred/Config/groups.cpp"
        "src/Fred/Config/instructions.cpp"
        "src/Fred/Config/mapping.cpp"
        "src/Fred/Config/crumapping.cpp"
        "src/Fred/Config/llamapping.cpp"
        "src/Fred/llalock.cpp"
        "src/Fred/llaalfrpcinfo.cpp"
        "src/Fred/Mapi/mapi.cpp"
        "src/Fred/Mapi/iterativemapi.cpp"
        "src/Fred/Mapi/mapigroup.cpp"
        "src/Fred/Mapi/indefinitemapi.cpp"
        "src/Fred/Protocols/SCA.cpp"
        "src/Fred/Protocols/SWT.cpp"
        "src/Fred/Protocols/IC.cpp"
        "src/Fred/Protocols/CRORC.cpp"
        "src/Fred/Protocols/SCAMFT.cpp"
        "src/Fred/Protocols/CRU.cpp"
		"src/Fred/Database/databaseinterface.cpp"
		"src/Fred/Database/multitype.cpp"
	)

if (MAPI)
    add_executable(FREDServer ${SOURCES} ${MAPI_SOURCES})
else ()
    add_executable(FREDServer ${SOURCES})
endif ()    

if (APPLE)
    target_link_libraries(FREDServer Alfred clntsh occi clntshcore nnz19 ${CMAKE_SOURCE_DIR}/dim/linux/libdim.a)
else ()
    target_link_libraries(FREDServer Alfred clntsh occi clntshcore nnz19 ociicus ${CMAKE_SOURCE_DIR}/dim/linux/libdim.a)
endif ()

target_link_libraries(FREDServer ${CMAKE_SOURCE_DIR}/dim/linux/libdim.a pthread)