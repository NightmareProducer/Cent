add_library(Source Token.cpp Expressions.cpp Scanner.cpp Parser.cpp Error.cpp)

add_executable(Main Cent.cpp)
target_link_libraries(Main Source)