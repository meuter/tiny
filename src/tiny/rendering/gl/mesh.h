#ifndef __TINY_RENDERING_GL_MESH_H__
#define __TINY_RENDERING_GL_MESH_H__

#include <GL/glew.h>
#include <vector>
#include <string>

#include <tiny/core/types.h>
#include <tiny/core/transformable.h>
#include <tiny/contrib/tiny_obj_loader.h>

#include "material.h"
#include "bufferobject.h"

namespace tiny { namespace rendering { namespace gl {

	class Mesh : public core::Transformable
	{
	public:

		static Mesh fromFile(const std::string &objFilname, int shape = 0);
		static Mesh fromFiles(const std::string &objFilname, const std::string &mtlFilename);

		enum AttributeLocation : GLuint
		{
			POSITION,
			TEXCOORD,
			NORMAL,
			N_ATTRIBUTES,
		};

		Mesh() = default;
		Mesh(const tinyobj::mesh_t &meshData);
		Mesh(Mesh &&mesh) = default;
		
		virtual ~Mesh() = default;

		Mesh &operator=(Mesh &&other) = default;

		void draw() const;

		const Material &material() const { return mMaterial; }
		void setMaterial(Material &&material) { mMaterial = std::move(material); }

	protected:
		static void destroy(GLuint handle);

	private:
		Material mMaterial;
		BufferObject mAttributes[N_ATTRIBUTES];
		BufferObject mIndices;
		Handle<GLuint, destroy> mHandle;

	};

}}}

#endif