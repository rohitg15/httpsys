set(libthreads_SRCS
   threadpool.h
   threadsafequeue.h
   threadpool.cpp
)

# Declare the library
add_library(libthreads STATIC
    ${libthreads_SRCS}
)

# Specify here the include directories exported
# by this library
target_include_directories(libthreads PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
