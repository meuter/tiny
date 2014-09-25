#ifndef __TINY_RENDERING_GL_SCENE_H__
#define __TINY_RENDERING_GL_SCENE_H__

#include <functional>
#include <string>
#include <map>

#include <tiny/core/camera.h>
#include <tiny/core/lightsource.h>

#include "mesh.h"

namespace tiny { namespace rendering { namespace gl {

	class Scene
	{
		using vec3 = core::vec3;
		using Camera = core::Camera;
		using LightSource = core::LightSource;
	public:

		Scene() = default;
		Scene(Scene &&other) = default;
		Scene(const Scene &other) = delete;

		virtual ~Scene() = default;

		Scene &operator=(Scene &&other) = default;
		Scene &operator=(const Scene &other) = delete;

		LightSource &addLight(const std::string &name)	{ return mLightSources[name]; }
		Mesh        &addMesh(const std::string &name)	{ return mMeshes[name];	}

		Mesh &getMesh(const std::string &name)
		{
			auto hit = mMeshes.find(name);

			if (hit == mMeshes.end())
				throw std::runtime_error("mesh not found");

			return hit->second;
		}

		vec3 &setAmbient(const vec3 &color)
		{
			mAmbient = color;
			return mAmbient;
		}

		void forAll(std::function<void(const Mesh &)> processMesh) const 
		{
			for (const auto &pair: mMeshes)
				processMesh(pair.second);
		}

		void forAll(std::function<void(const LightSource &)> processLightSource) const 
		{
			for (const auto &pair: mLightSources)
				processLightSource(pair.second);
		}

		inline size_t lightSourceCount() const { return mLightSources.size(); }
		inline size_t meshCount() const { return mMeshes.size(); }
		inline vec3   ambientLight() const { return mAmbient; }

		inline Camera &camera() { return mCamera; }
		inline const Camera &camera() const { return mCamera; }

	private:	
		std::map<std::string, LightSource> mLightSources;
		std::map<std::string, Mesh> mMeshes;
		vec3 mAmbient;
		Camera mCamera;
	};

}}}

#endif