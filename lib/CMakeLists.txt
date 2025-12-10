add_library(libasync SHARED async.cpp handler.cpp state.cpp worker.cpp)

target_include_directories(libasync PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(FILES async.h DESTINATION include)
install(TARGETS libasync DESTINATION lib)