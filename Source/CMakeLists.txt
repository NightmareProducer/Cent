add_library(Source Token.cpp Expressions.cpp Scanner.cpp Parser.cpp Error.cpp Value.cpp)

add_executable(Main Cent.cpp)
target_link_libraries(Main Source)