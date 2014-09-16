#ifndef __TINY_CORE_MESH_H__
#define __TINY_CORE_MESH_H__

#include <GL/glew.h>
#include <tiny/core/types.h>
#include <vector>
#include <string>
#include "../core/transformable.h"
#include "tiny_obj_loader.h"
#include "material.h"
#include "gl/vertexbuffer.h"
#include "gl/vertexarray.h"

namespace tiny { namespace rendering {

	class Mesh : public core::Transformable
	{
	public:

		static Mesh fromFile(const std::string &objFilname);

		enum AttributeLocation : GLuint
		{
			POSITION,
			TEXCOORD,
			NORMAL,
		};

		Mesh();
		Mesh(const Mesh &other) = delete;
		Mesh(Mesh &&mesh) = default;
		
		virtual ~Mesh();

		Mesh &operator=(const Mesh &mesh) = delete;
		Mesh &operator=(Mesh &&other) = default;

		void load(const tinyobj::mesh_t &mesh);
		void unload();
		void draw() const;

		const Material &material() const { return mMaterial; }

		Material mMaterial;
		
	private:	
		gl::VertexArray mVertexArray;
		gl::VertexBuffer mPositions;
		gl::VertexBuffer mTexcoords;
		gl::VertexBuffer mNormals;
		gl::VertexBuffer mIndices;
	};

}}

#endif