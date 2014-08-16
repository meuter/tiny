env = Environment()

env["CXX"] = "clang++ -std=c++11 -fcolor-diagnostics"
env.MergeFlags("-Iinclude/")
env.MergeFlags("-I/usr/local/include/")


test = env.Clone()
test.MergeFlags("-ggdb -O0 -Itest -Wall -Wno-unused -pedantic")

math_tests = Glob("test/math/*_tests.cpp")
gtest     = Split("test/gmock_main.cc test/gmock-gtest-all.cc")

test.Program("test/all", gtest + math_tests)
