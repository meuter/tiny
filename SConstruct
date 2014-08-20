env = Environment()

CacheDir("/tmp/sconscache/")

env["CXX"] = "clang++ -std=c++11 -fcolor-diagnostics -ferror-limit=2"
env["CXXCOMSTR"]  = "CXX $TARGET"
env["LINKCOMSTR"] = "LN  $TARGET"

env.MergeFlags("-Wall -pedantic")
env.MergeFlags("-Iinclude/")
env.MergeFlags("-I/usr/local/include/")

test = env.Clone()
test.MergeFlags("-Wno-unused")
test.MergeFlags("-ggdb -O0 -Itest")

math_tests = Glob("test/math/*_tests.cpp")
gl_tests   = Glob("test/gl/*_tests.cpp")
gtest      = Split("test/gmock_main.cc test/gmock-gtest-all.cc")

test.Program("test/all", gtest + math_tests + gl_tests)
