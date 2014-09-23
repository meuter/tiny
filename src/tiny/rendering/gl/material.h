#ifndef __TINY_RENDERING_GL_MATERIAL_H__
#define __TINY_RENDERING_GL_MATERIAL_H__

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
		inline const core::vec3 &diffuse()  const { return mDiffuse; }
		inline const core::vec3 &specular() const { return mSpecular; }
		inline const float shininess()      const { return mShininess; }

		void load(const tinyobj::material_t &material)
		{
			mDiffuse   = core::vec3(material.diffuse);
			mSpecular  = core::vec3(material.specular);
			mShininess = material.shininess;

			if (!material.diffuse_texname.empty())
				mTexture = gl::Texture::fromFile(material.diffuse_texname);
			else
			{
				unsigned char white[1024];
				memset(white, 0xFF, sizeof(white));
				mTexture = Texture(white, 16,16);
			}

		}


	private:
		core::vec3 mDiffuse, mSpecular;
		float mShininess;
		gl::Texture mTexture;

	};

}}}

#endif