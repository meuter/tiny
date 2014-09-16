#ifndef __TINY_RENDERING_MATERIAL_H__
#define __TINY_RENDERING_MATERIAL_H__

#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <tiny/core/types.h>
#include <tiny/utils/strutils.h>

#include "gl/texture.h"
#include "tiny_obj_loader.h"

namespace tiny { namespace rendering {

	class Material {
	public:	

		static Material fromFile(const std::string &filename)
		{
			Material result;
			std::ifstream file;
			std::map<std::string, int> materialMap;
			std::vector<tinyobj::material_t> materials;
			std::string error;

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


		Material() {}
		virtual ~Material() {}
		Material(const Material &other) = delete;
		Material(Material &&other) = default;

		Material &operator=(const Material &other) = delete;
		Material &operator=(Material &&other) = default;

		inline const gl::Texture &texture() const { return mTexture; }

		inline const core::vec3 &ambient() const { return mAmbient; }
		inline const core::vec3 &diffuse() const { return mDiffuse; }

		void load(const tinyobj::material_t &material)
		{
			mAmbient  = core::vec3(material.ambient);
			mDiffuse  = core::vec3(material.diffuse);
			mSpecular = core::vec3(material.specular);

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
		core::vec3 mAmbient, mDiffuse, mSpecular;
		gl::Texture mAmbientMap, mSpecularMap, mNormalMap, mTexture;

	};
}}

#endif