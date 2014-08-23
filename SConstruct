env = Environment()

CacheDir("/tmp/sconscache/")

env["CXX"] = "clang++ -m32 -std=c++11 -fcolor-diagnostics"
env["CXXCOMSTR"]    = "CXX $TARGET"
env["LINKCOMSTR"]   = "LN  $TARGET"
env["ARCOMSTR"]     = "AR  $TARGET"
env["RANLIBCOMSTR"] = "RL  $TARGET"

env.MergeFlags("-Wall -pedantic")
env.MergeFlags("-Isrc/")
env.MergeFlags("-I/usr/local/include/")
env.MergeFlags("-O3")


env.Library("tiny", Split("""
	src/tiny/core/transformable.cpp
	src/tiny/rendering/window.cpp

"""))

test = env.Clone()
test.MergeFlags("-Wno-unused")
test.MergeFlags("-ggdb -O0 -Itest")

math_tests = Glob("test/tiny/math/*_tests.cpp")
gl_tests   = Glob("test/tiny/gl/*_tests.cpp")
gtest      = Split("test/gmock_main.cc test/gmock-gtest-all.cc")

window_test = test.Clone()
window_test.MergeFlags("-framework SDL2 -framework OpenGL -framework GLEW -L. -ltiny");
window_test.Program("test/all", gtest + math_tests + gl_tests)
window_test.Program("test/window", Split("test/window_main.cpp"))