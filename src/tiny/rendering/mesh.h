#ifndef __TINY_CORE_MESH_H__
#define __TINY_CORE_MESH_H__

#include <GL/glew.h>
#include <tiny/core/types.h>
#include <vector>
#include <string>
#include <tiny/core/transformable.h>
#include <tiny/contrib/tiny_obj_loader.h>
#include "gl/material.h"
#include "gl/vertexarray.h"
#include "gl/vertexbuffer.h"
#include "gl/indexbuffer.h"

namespace tiny { namespace rendering {

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

		const gl::Material &material() const { return mMaterial; }

	private:	
		gl::Material mMaterial;
		gl::VertexArray mVertexArray;
		gl::VertexBuffer mPositions;
		gl::VertexBuffer mTexcoords;
		gl::VertexBuffer mNormals;
		gl::IndexBuffer mIndices;
	};

}}

#endif