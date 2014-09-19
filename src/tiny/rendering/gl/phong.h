#ifndef __TINY_RENDERING_GL_PHONG_H__
#define __TINY_RENDERING_GL_PHONG_H__

#include <tiny/core/types.h>
#include <tiny/core/camera.h>

#include "shaderprogram.h"
#include "material.h"
#include "mesh.h"
#include "context.h"
#include "scene.h"
#include "ambient.h"
#include "directional.h"
#include "point.h"

namespace tiny { namespace rendering { namespace gl {

	class PhongRenderer
	{
	public:	

		PhongRenderer(Context &context) : mContext(context) {}

		void init()
		{
			mContext.enableBackfaceCulling();
			mContext.enableDepthTest();
		}

		void addDirectionalLight(const DirectionalLight &directionalLight)
		{
			mDirectionalLights.push_back(directionalLight);
		}

		void addPointLight(const PointLight &pointLight)
		{
			mPointLights.push_back(pointLight);
		}

		void render(const core::Camera &camera, const Mesh &mesh)
		{
			mAmbientShader.shade(camera, mesh);			
			mContext.enableBlending();
			{
				for (const auto &directionalLight : mDirectionalLights)
					mDirectionalShader.shade(camera, mesh, directionalLight);

				for (const auto &pointLight : mPointLights)
					mPointShader.shade(camera, mesh, pointLight);
			}
			mContext.disableBlending();
		}

		void render(const core::Camera &camera, const Scene &scene)
		{
			for (const auto &mesh: scene.meshes())
				render(camera, mesh);
		}

	private:
		AmbientShader mAmbientShader;
		DirectionalLightShader mDirectionalShader;
		PointLightShader mPointShader;
		Context &mContext;
		std::vector<DirectionalLight> mDirectionalLights;
		std::vector<PointLight> mPointLights;
	};

}}}

#endif