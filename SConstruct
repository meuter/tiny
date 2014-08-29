env = Environment()

CacheDir("/tmp/sconscache/")

debug   = ARGUMENTS.get("debug", 0)
verbose = ARGUMENTS.get("verbose", 0)

env["CXX"] = "clang++ -std=c++11 -fcolor-diagnostics"
if not verbose:
	env["CXXCOMSTR"]    = " CXX  $TARGET"
	env["LINKCOMSTR"]   = " LN   $TARGET"
	env["ARCOMSTR"]     = " AR   $TARGET"
	env["RANLIBCOMSTR"] = " RL   $TARGET"

env.MergeFlags("-Wall -pedantic")
env.MergeFlags("-Isrc/")
env.MergeFlags("-I/usr/local/include/")
env.MergeFlags("-ggdb -O0" if debug else "-O3")
env.Library("tiny", Split("""
	src/tiny/core/transformable.cpp
	src/tiny/core/inputs.cpp
	src/tiny/rendering/window.cpp
	src/tiny/rendering/shader.cpp
	src/tiny/rendering/shaderprogram.cpp
"""))

example = env.Clone()
example.MergeFlags("-L. -ltiny")
example.MergeFlags("-L/usr/local/lib -lGLEW -lSDL2")
example.MergeFlags("-framework OpenGL")
example.Program("window_example", Split("""
	test/tiny/example/window_example.cpp
"""));

test = env.Clone()
test.MergeFlags("-Itest/gtest")
test.MergeFlags("-L. -ltiny")
test.MergeFlags("-L/usr/local/lib -lGLEW -lSDL2")
test.Program("test/tiny/all", Split("""
	test/tiny/core/transformable_tests.cpp
	test/tiny/math/matrix_tests.cpp
	test/tiny/math/quaternion_tests.cpp
	test/tiny/math/trigo_tests.cpp
	test/tiny/rendering/shader_tests.cpp
	test/gtest/gmock-gtest-all.cc
	test/gtest/gmock_main.cc
"""))

