set(Main_SRCS
    main.cpp
)

add_executable(myApp
    ${Main_SRCS}
)

#find_package(Threads)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)


# Specify here the libraries this program depends on
target_link_libraries(myApp
    libthreads
	libhttpserver
    ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS myApp DESTINATION bin)
