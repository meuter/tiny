#ifndef __TINY_RENDERING_GL_SHADER_PROGRAM_H__
#define __TINY_RENDERING_GL_SHADER_PROGRAM_H__

#include <GL/glew.h>
#include <string>
#include <vector>
#include <tiny/core/types.h>
#include <tiny/core/lightsource.h>

#include "shader.h"
#include "handle.h"

namespace tiny { namespace rendering { namespace gl {

	class Material;

	class ShaderProgram 
	{
		using LightSource = core::LightSource;
		using vec2 = core::vec2;
		using vec3 = core::vec3;
		using vec4 = core::vec4;
		using mat4 = core::mat4;
	public:
		using Constants = std::map<std::string, std::string>;

		static ShaderProgram fromFiles(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename, Constants constants=Constants());

		ShaderProgram();
		ShaderProgram(ShaderProgram &&other) = default;
		virtual ~ShaderProgram() = default;

		ShaderProgram &operator=(ShaderProgram &&other) = default;

		void add(Shader &&shader);
		void link();
		void use();		

		void setUniform(const std::string &uniform, float value);
		void setUniform(const std::string &uniform, const vec2 &value);
		void setUniform(const std::string &uniform, const vec3 &value);
		void setUniform(const std::string &uniform, const vec4 &value);
		void setUniform(const std::string &uniform, const mat4 &value);
		void setUniform(const std::string &uniform, const Material &material);
		void setUniform(const std::string &uniform, const LightSource &ambientLight);

		GLint getUniformLocation(const std::string &uniform) const;
		GLint getAttributeLocation(const std::string &attribute) const;

	protected:
		void bindAttributeLocations();
		void check(GLenum linkingStage);

		static void destroy(GLuint handle);

	private:
		Handle<destroy> mHandle;
	};

}}}

#endif