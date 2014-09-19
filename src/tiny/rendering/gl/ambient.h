#ifndef __TINY_RENDERING_GL_AMBIENT_H__
#define __TINY_RENDERING_GL_AMBIENT_H__

#include <tiny/core/camera.h>

#include "shaderprogram.h"
#include "mesh.h"

namespace tiny { namespace rendering { namespace gl {

	class AmbientShader : public ShaderProgram
	{
	public:
		using ShaderProgram::setUniform;
		using ShaderProgram::use;

		AmbientShader() : ShaderProgram(ShaderProgram::fromFiles("res/shaders/ambient.vs", "res/shaders/ambient.fs")) {}

		void shade(const core::Camera &camera, const Mesh &mesh)
		{
			use();
			setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			setUniform("material", mesh.material());

			mesh.draw();
		}
	};

}}}

#endif