#ifndef __TINY_RENDERING_SHADER_PROGRAM_H__
#define __TINY_RENDERING_SHADER_PROGRAM_H__

#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>
#include "../core/types.h"
#include "shader.h"

namespace tiny { namespace rendering {

	class Texture;

	class ShaderProgram
	{
	public:
		static ShaderProgram fromFiles(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename);

		ShaderProgram();
		ShaderProgram(const ShaderProgram &other) = delete;
		ShaderProgram(ShaderProgram &&other);
		virtual ~ShaderProgram();

		ShaderProgram &operator=(const ShaderProgram &other) = delete;
		ShaderProgram &operator=(ShaderProgram &&other);

		void addShader(Shader &&shader);
		void link();

		void detectUniform(const std::string &uniform);
		void setUniform(const std::string &uniform, float value);
		void setUniform(const std::string &uniform, const core::vec2 &value);
		void setUniform(const std::string &uniform, const core::vec3 &value);
		void setUniform(const std::string &uniform, const core::vec4 &value);
		void setUniform(const std::string &uniform, const core::mat4 &value);

		GLint getAttributeLocation(const std::string &attribute) const;

		void use();		

	protected:
		void bindAttributeLocations();
		void checkProgramError(GLenum linkingStage);
		void destroy();

	private:
		GLuint mProgramHandle;
		std::vector<Shader> mShaders;
		std::map<std::string, GLint> mUniformLocations;
	};

}}

#endif