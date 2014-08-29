#ifndef __TINY_RENDERING_SHADER_H__
#define __TINY_RENDERING_SHADER_H__

#include <GL/glew.h>
#include <string>

namespace tiny { namespace rendering {

	class Shader
	{
	public:
		Shader(GLenum shaderType);
		Shader(const Shader &other) = delete;
		Shader(Shader &&other);
		virtual ~Shader();

		Shader &operator=(Shader &&other);
		Shader &operator=(const Shader &other) = delete;

		void loadSource(const std::string &shaderSource);
		void loadFile(const std::string &filename);
		void compile();

		inline GLuint getHandle() const { return mShaderHandle; }

	private:
		GLuint mShaderHandle;
	};

}}

#endif