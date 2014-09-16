#ifndef __TINY_CORE_MESH_H__
#define __TINY_CORE_MESH_H__

#include <GL/glew.h>
#include <tiny/core/types.h>
#include <vector>
#include <string>
#include "tiny_obj_loader.h"

namespace tiny { namespace rendering {

	class Mesh
	{
	public:

		static Mesh fromFile(const std::string &filname);

		enum AttributeLocation : GLuint
		{
			POSITION,
			TEXCOORD,
			NORMAL,
		};

		Mesh();
		Mesh(const Mesh &other) = delete;
		Mesh(Mesh &&mesh);
		virtual ~Mesh();

		Mesh &operator=(const Mesh &mesh) = delete;
		Mesh &operator=(Mesh &&other);

		void load(const tinyobj::mesh_t &mesh);
		void unload();
		void draw() const;

	private:	
		size_t mSize;
		GLuint mVertexArrayHandle;
		GLuint mPositionsBufferHandle;
		GLuint mTexcoordsBufferHandle;		
		GLuint mNormalsBufferHandle;
		GLuint mIndexBufferHandle;
		bool mLoaded;
	};

}}

#endif