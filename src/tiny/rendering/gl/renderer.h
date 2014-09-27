#ifndef __TINY_RENDERING_GL_RENDERER_H__
#define __TINY_RENDERING_GL_RENDERER_H__

#include <tiny/core/types.h>
#include <tiny/core/camera.h>
#include <tiny/core/lightsource.h>

#include "shaderprogram.h"
#include "material.h"
#include "mesh.h"
#include "context.h"
#include "scene.h"

#include <tiny/core/game.h>

namespace tiny { namespace rendering { namespace gl {

	class Renderer 
	{
		using vec3 = core::vec3;
		using vec2 = core::vec2;
		using sec = core::sec;
		using Game = core::Game;
		using LightSource = core::LightSource;
	public:	

		Renderer(Context &context, Scene &scene);
		Renderer(const Renderer &other) = delete;
		Renderer(Renderer &&other) = default;

		virtual ~Renderer() = default;

		Renderer &operator=(const Renderer &other) = delete;
		Renderer &operator=(Renderer &&other) = default;

		void init();
		void update(Game &game, sec t, sec dt);
		void render();
		void renderToActiveFrameBuffer();

	protected:	
		ShaderProgram &getShader(int lightSourceCount);

	private:
		std::map<int,ShaderProgram> mLightShaders;
		Context &mContext;
		const Scene &mScene;
	};

}}}

#endif