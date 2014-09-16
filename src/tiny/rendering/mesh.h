#ifndef __TINY_CORE_MESH_H__
#define __TINY_CORE_MESH_H__

#include <GL/glew.h>
#include <tiny/core/types.h>
#include <vector>
#include <string>
#include "../core/transformable.h"
#include "tiny_obj_loader.h"
#include "Material.h"
#include "gl/buffer.h"

namespace tiny { namespace rendering {

	class VertexArray
	{
	public:	
		VertexArray()
		{
			glGenVertexArraysAPPLE(1, &mHandle);
			if (mHandle == 0)
				throw std::runtime_error("could not create vertex array");
		}

		VertexArray(const VertexArray &other) = delete;

		VertexArray(VertexArray &&other) : mHandle(other.mHandle)
		{
			other.release();
		}

		VertexArray &operator=(const VertexArray &other) = delete;

		VertexArray &operator=(VertexArray &&other) 
		{
			free();
			mHandle = other.mHandle;
			other.release();
			return (*this);
		}

		virtual ~VertexArray()
		{
			free();
		}

		void release()
		{
			mHandle = 0;
		}

		void free()
		{
			glDeleteVertexArrays(1, &mHandle);
			mHandle = 0;
		}

		void bind() const
		{
			glBindVertexArrayAPPLE(mHandle);
		}

		void unbind() const
		{
			glBindVertexArrayAPPLE(0);	
		}

	private:
		GLuint mHandle;
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
		size_t mSize;
		VertexArray mVertexArray;
		gl::Buffer mPositions;
		gl::Buffer mTexcoords;
		gl::Buffer mNormals;
		gl::Buffer mIndices;
	};

}}

#endif