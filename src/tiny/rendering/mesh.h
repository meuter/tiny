#ifndef __TINY_CORE_MESH_H__
#define __TINY_CORE_MESH_H__

#include <GL/glew.h>
#include <tiny/core/types.h>
#include <vector>
#include <string>
#include "../core/transformable.h"
#include "tiny_obj_loader.h"
#include "Material.h"

namespace tiny { namespace rendering {


	class VertexAttributeBuffer
	{
	public:	
		VertexAttributeBuffer() : mHandle(0), mLocation(0) 
		{

		}
		
		VertexAttributeBuffer(GLuint location) : mHandle(0), mLocation(location)
		{
			glGenBuffers(1, &mHandle);
			if (mHandle == 0)
				throw std::runtime_error("could not create vertex buffer object");
		}

		VertexAttributeBuffer(VertexAttributeBuffer &&other) : mHandle(other.mHandle), mLocation(other.mHandle)
		{
			other.release();
		}

		VertexAttributeBuffer(const VertexAttributeBuffer &other) = delete;

		virtual ~VertexAttributeBuffer()
		{
			if (mHandle)
				unload();
		}

		VertexAttributeBuffer &operator=(const VertexAttributeBuffer &other) = delete;

		VertexAttributeBuffer &operator=(VertexAttributeBuffer &&other) 
		{
			unload();
			mHandle = other.mHandle;
			mLocation = other.mLocation;
			other.release();

			return (*this);
		}

		void bind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, mHandle);
		}

		void load(std::vector<float> &data, size_t nPerVertex) 
		{	
			glBindBuffer(GL_ARRAY_BUFFER, mHandle);
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), &data[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(mLocation);
			glVertexAttribPointer(mLocation, nPerVertex, GL_FLOAT, GL_FALSE, nPerVertex * sizeof(data[0]), (GLvoid *)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void release()
		{
			mHandle = 0;
		}

		void unload()
		{
			glDeleteBuffers(1, &mHandle);
		}


	private:
		GLuint mHandle;
		GLuint mLocation;
	};

	class Mesh : public core::Transformable
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

		const Material &material() const { return mMaterial; }


		Material mMaterial;

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