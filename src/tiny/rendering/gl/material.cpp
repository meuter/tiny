#include "material.h"
#include <fstream>
#include <array>

namespace tiny { namespace rendering { namespace gl {

	Material &Material::fromFile(const std::string &mtlFilename)
	{
		std::ifstream file;
		std::map<std::string, int> materialMap;
		std::vector<tinyobj::material_t> materials;
		std::string error;

		if (utils::toupper(utils::fileExtension(mtlFilename)) != "MTL")
			throw std::runtime_error("unexpected file type '"+mtlFilename+"'");

		file.open(mtlFilename);

		if (!file)
			throw std::runtime_error("could not open '" + mtlFilename + "'");

		error = tinyobj::LoadMtl(materialMap, materials, file);
		if (!error.empty())
			throw std::runtime_error("could not load '"+mtlFilename+"':" + error);

		if (materials.size() != 1)
			throw std::runtime_error("more than one mesh found in '"+mtlFilename+"'");

		load(materials[0]);
		return (*this);
	}

	void Material::load(const tinyobj::material_t &material)
	{
		mDiffuse   = core::vec3(material.diffuse);
		mSpecular  = core::vec3(material.specular);
		mShininess = material.shininess;

		if (!material.diffuse_texname.empty())
			mTexture.fromFile(material.diffuse_texname);
		else
			mTexture.fromColor(core::vec3(1,1,1));

		if (!material.normal_texname.empty())
			mNormalMap.fromFile(material.diffuse_texname);
		else
			mNormalMap.fromColor(core::vec3(0.5,0.5,1.0));
	}

}}}
