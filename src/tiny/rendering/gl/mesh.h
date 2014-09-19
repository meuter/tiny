#ifndef __TINY_RENDERING_GL_MESH_H__
#define __TINY_RENDERING_GL_MESH_H__

#include <GL/glew.h>
#include <vector>
#include <string>

#include <tiny/core/types.h>
#include <tiny/core/transformable.h>
#include <tiny/contrib/tiny_obj_loader.h>

#include "material.h"
#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

namespace tiny { namespace rendering { namespace gl {

	class Mesh : public core::Transformable
	{
	public:

		static Mesh fromFile(const std::string &objFilname);
		static Mesh fromFiles(const std::string &objFilname, const std::string &mtlFilename);

		enum AttributeLocation : GLuint
		{
			POSITION,
			TEXCOORD,
			NORMAL,
		};

		Mesh() = default;
		Mesh(const Mesh &other) = delete;
		Mesh(Mesh &&mesh) = default;
		
		virtual ~Mesh() = default;

		Mesh &operator=(const Mesh &mesh) = delete;
		Mesh &operator=(Mesh &&other) = default;

		void load(const tinyobj::mesh_t &mesh);
		void unload();
		void draw() const;

		const Material &material() const { return mMaterial; }

	private:	
		Material mMaterial;
		VertexArray mVertexArray;
		VertexBuffer mPositions;
		VertexBuffer mTexcoords;
		VertexBuffer mNormals;
		IndexBuffer mIndices;
	};

}}}

#endif