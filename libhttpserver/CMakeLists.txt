set(libhttpserver_SRCS
   Utils.h
   HttpDispatchTable.h
   HttpDispatchTable.cpp
   HttpServer.h
   HttpServer.cpp
   ThreadContext.h
   ThreadContext.cpp
   Callbacks.h
   Callbacks.cpp
)

# Declare the library
add_library(libhttpserver STATIC
    ${libhttpserver_SRCS}
)

# Specify here the include directories exported
# by this library
target_include_directories(libhttpserver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(libhttpserver
    libthreads
)
