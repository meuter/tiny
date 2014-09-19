#ifndef __TINY_RENDERING_MATERIAL_H__
#define __TINY_RENDERING_MATERIAL_H__

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <tiny/core/types.h>
#include <tiny/utils/strutils.h>
#include <tiny/contrib/tiny_obj_loader.h>

#include "texture.h"

#include <iostream>

namespace tiny { namespace rendering { namespace gl {

	class Material {
	public:	

		static Material fromFile(const std::string &filename)
		{
			Material result;
			std::ifstream file;
			std::map<std::string, int> materialMap;
			std::vector<tinyobj::material_t> materials;
			std::string error;

			if (utils::toupper(utils::fileExtension(filename)) != "MTL")
				throw std::runtime_error("unexpected file type '"+filename+"'");

			file.open(filename);



			if (!file)
				throw std::runtime_error("could not open '" + filename + "'");

			error = tinyobj::LoadMtl(materialMap, materials, file);
			if (!error.empty())
				throw std::runtime_error("could not load '"+filename+"':" + error);

			if (materials.size() != 1)
				throw std::runtime_error("more than one mesh found in '"+filename+"'");

			result.load(materials[0]);

			return result;
		}


		Material() = default;
		Material(Material &&other) = default;

		virtual ~Material() = default;

		Material &operator=(Material &&other) = default;

		inline const gl::Texture &texture() const { return mTexture; }
		inline const core::vec3 &ambient()  const { return mAmbient; }
		inline const core::vec3 &diffuse()  const { return mDiffuse; }
		inline const core::vec3 &specular() const { return mSpecular; }
		inline const float shininess()      const { return mShininess; }

		void load(const tinyobj::material_t &material)
		{
			mAmbient   = core::vec3(material.ambient);
			mDiffuse   = core::vec3(material.diffuse);
			mSpecular  = core::vec3(material.specular);
			mEmission  = core::vec3(material.emission);
			mShininess = material.shininess;

			if (!material.ambient_texname.empty())
				mAmbientMap = gl::Texture::fromFile(material.ambient_texname);

			if (!material.specular_texname.empty())
				mSpecularMap = gl::Texture::fromFile(material.specular_texname);

			if (!material.normal_texname.empty())
				mNormalMap = gl::Texture::fromFile(material.normal_texname);
	
			if (!material.diffuse_texname.empty())
				mTexture = gl::Texture::fromFile(material.diffuse_texname);
		}


	private:
		core::vec3 mAmbient, mDiffuse, mSpecular, mEmission;
		float mShininess;
		gl::Texture mAmbientMap, mSpecularMap, mNormalMap, mTexture;

	};

}}}

#endif