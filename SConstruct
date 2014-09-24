env = Environment()

CacheDir("/tmp/sconscache/")

debug   = ARGUMENTS.get("debug", 0)
verbose = ARGUMENTS.get("verbose", 0)

env["CXX"] = "clang++ -std=c++11 -stdlib=libc++ -fcolor-diagnostics"
if not verbose:
	env["CXXCOMSTR"]    = " CXX  $TARGET"
	env["LINKCOMSTR"]   = " LN   $TARGET"
	env["ARCOMSTR"]     = " AR   $TARGET"
	env["RANLIBCOMSTR"] = " RL   $TARGET"

env.MergeFlags("-Wall -pedantic -Wno-unused")
env.MergeFlags("-Isrc/")
env.MergeFlags("-I/usr/local/include/")
env.MergeFlags("-ggdb -O0" if debug else "-O3")
env.Library("tiny", Split("""

	src/tiny/core/transformable.cpp
	src/tiny/core/inputs.cpp
	src/tiny/core/game.cpp
	src/tiny/core/window.cpp
	src/tiny/core/camera.cpp

	src/tiny/utils/strutils.cpp

	src/tiny/rendering/lightsource.cpp
	src/tiny/rendering/gl/mesh.cpp
	src/tiny/rendering/gl/shaderprogram.cpp
	src/tiny/rendering/gl/shader.cpp
	src/tiny/rendering/gl/texture.cpp

	src/tiny/contrib/tiny_obj_loader.cc
"""))

example = env.Clone()
example.MergeFlags("-L. -ltiny")
example.MergeFlags("-L/usr/local/lib -lGLEW -lSDL2")
example.MergeFlags("-framework OpenGL")
example.Program("testgl", Split("""
	test/tiny/example/testgl.cpp
"""));

example.Program("testsw", Split("""
	test/tiny/example/testsw.cpp
"""));

test = env.Clone()
test.MergeFlags("-Itest/gtest")
test.MergeFlags("-L. -ltiny")
test.MergeFlags("-L/usr/local/lib -lGLEW -lSDL2")
test.MergeFlags("-framework OpenGL")
test.Program("test/tiny/all", Split("""
	test/tiny/core/transformable_tests.cpp
	test/tiny/core/camera_tests.cpp
	test/tiny/math/matrix_tests.cpp
	test/tiny/math/quaternion_tests.cpp
	test/tiny/math/trigo_tests.cpp
	test/tiny/utils/strutils_tests.cpp
	test/tiny/utils/include_tests.cpp
	test/gtest/gmock-gtest-all.cc
	test/gtest/gmock_main.cc
"""))

