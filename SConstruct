env = Environment()

env["CXX"] = "clang++ -std=c++11 -fcolor-diagnostics"
env.MergeFlags("-Iinclude/")


test = env.Clone()
test.MergeFlags("-ggdb -O0 -Itest")

test.Program("test/all", Split("test/all.cpp test/gmock_main.cc test/gmock-gtest-all.cc"))
