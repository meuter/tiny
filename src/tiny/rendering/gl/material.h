#ifndef __TINY_RENDERING_GL_MATERIAL_H__
#define __TINY_RENDERING_GL_MATERIAL_H__

#include <string>
#include <vector>
#include <map>
#include <tiny/core/types.h>
#include <tiny/utils/strutils.h>
#include <tiny/contrib/tiny_obj_loader.h>

#include "texture.h"

#include <iostream>

namespace tiny { namespace rendering { namespace gl {

	class Material 
	{
		using byte = core::byte;
	public:	
		Material() = default;
		Material(Material &&other) = default;

		virtual ~Material() = default;

		Material &operator=(Material &&other) = default;

		inline const gl::Texture &texture() const   { return mTexture; }
		inline const gl::Texture &normalMap() const { return mNormalMap; }

		inline gl::Texture &texture()   { return mTexture; }
		inline gl::Texture &normalMap() { return mNormalMap; }

		inline core::vec3 &diffuse()  { return mDiffuse; }
		inline core::vec3 &specular() { return mSpecular; }
		inline float &shininess()     { return mShininess; }



		inline const core::vec3 &diffuse()  const { return mDiffuse; }
		inline const core::vec3 &specular() const { return mSpecular; }
		inline const float &shininess()     const { return mShininess; }

		Material &fromFile(const std::string &mtlFilename);
		void load(const tinyobj::material_t &material);

	private:
		core::vec3 mDiffuse, mSpecular;
		float mShininess;
		Texture mTexture, mNormalMap;

	};

}}}

#endif