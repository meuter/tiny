env = Environment()

env["CXX"] = "clang++ -std=c++11 -fcolor-diagnostics"
env.MergeFlags("-Iinclude/")


test = env.Clone()
test.MergeFlags("-ggdb -O0 -Itest -Wall -Wno-unused -pedantic")

all_tests = Split("test/matrix_tests.cpp")

test.Program("test/all", Split("test/gmock_main.cc test/gmock-gtest-all.cc") + all_tests)
