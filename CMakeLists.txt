cmake_minimum_required(VERSION 3.6.2)
project(zilu_gb28181)

#头文件
include_directories(include)
include_directories(src)


#设置编译参数
set(CMAKE_CXX_FLAGS "-ffunction-sections -fdata-sections -Wall -std=c++11 -DPOSIX -DDEBUG_")

#导入库
add_library(Infra STATIC IMPORTED)
set_target_properties(Infra PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/m64x86/libInfra.a)

add_library(json STATIC IMPORTED)
set_target_properties(json PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/m64x86/libjson.a)

add_library(eXosip2 STATIC IMPORTED)
set_target_properties(eXosip2 PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/m64x86/libeXosip2.a)

add_library(osip2 STATIC IMPORTED)
set_target_properties(osip2 PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/m64x86/libosip2.a)

add_library(osipparser2 STATIC IMPORTED)
set_target_properties(osipparser2 PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/m64x86/libosipparser2.a)

add_library(yaml STATIC IMPORTED)
set_target_properties(yaml PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/m64x86/libyaml.a)

add_library(StreamResolver STATIC IMPORTED)
set_target_properties(StreamResolver PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/m64x86/libStreamResolver.a)

# 设置可执行文件的输出路径(EXCUTABLE_OUTPUT_PATH是全局变量)
set(EXECUTABLE_OUTPUT_PATH ./)

#源文件
add_executable(zilu_gb28181
        src/main.cpp
        src/utils/tinyxml2.cpp
        src/utils/common.cpp
        src/utils/ptz/ptz_parser.cpp
        src/config_manager.cpp
        src/gb_service/sip_server.cpp
        src/gb_service/gb28181_svr_manager.cpp
        src/gb_service/event_handler/handler_manager.cpp
        src/gb_service/event_handler/register_handler.cpp
        src/gb_service/event_handler/message_handler.cpp
        src/gb_service/event_handler/subscribe_handler.cpp
        src/gb_service/event_handler/call_handler.cpp
        src/gb_service/event_handler/base_handler.cpp
        src/gb_service/manscdp/request_sender.cpp
        src/gb_service/manscdp/xml_parser.cpp
        src/gb_service/manscdp/xml_assembly.cpp
        src/gb_service/request_manager/request_manager.cpp
        src/gb_service/request_manager/base_request.cpp
        src/gb_service/request_manager/message_request.cpp
        src/gb_service/event_handler/sdp/sdp.cpp
        src/media_transpond/media_transpond.cpp
        src/media_transpond/session_manager.cpp
        )

#设置链接库
link_directories(${PROJECT_SOURCE_DIR}/lib/m64x86/)
target_link_libraries(zilu_gb28181 Infra pthread json eXosip2 osip2 osipparser2 uuid ssl crypto resolv yaml StreamResolver)
#target_link_libraries(drs_gb28181 Infra pthread json eXosip2 uuid ssl crypto resolv yaml jvmpsvrmediadevice)
