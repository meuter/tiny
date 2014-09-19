#ifndef __TINY_RENDERING_SHADER_PROGRAM_H__
#define __TINY_RENDERING_SHADER_PROGRAM_H__

#include <GL/glew.h>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <tiny/core/types.h>

#include "shader.h"

namespace tiny { namespace rendering { namespace gl {

	class Material;

	class ShaderProgram : boost::noncopyable
	{
	public:
		static ShaderProgram fromFiles(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename);

		ShaderProgram();
		ShaderProgram(ShaderProgram &&other);
		virtual ~ShaderProgram();

		ShaderProgram &operator=(ShaderProgram &&other);

		void addShader(Shader &&shader);
		void link();

		void setUniform(const std::string &uniform, float value);
		void setUniform(const std::string &uniform, const core::vec2 &value);
		void setUniform(const std::string &uniform, const core::vec3 &value);
		void setUniform(const std::string &uniform, const core::vec4 &value);
		void setUniform(const std::string &uniform, const core::mat4 &value);

		GLint getUniformLocation(const std::string &uniform) const;
		GLint getAttributeLocation(const std::string &attribute) const;

		void use();		

	protected:
		void bindAttributeLocations();
		void checkProgramError(GLenum linkingStage);
		void release();
		void destroy();

	private:
		GLuint mHandle;
		std::vector<Shader> mShaders;
	};

}}}

#endif