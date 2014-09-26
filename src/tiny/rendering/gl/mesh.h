#ifndef __TINY_RENDERING_GL_MESH_H__
#define __TINY_RENDERING_GL_MESH_H__

#include <GL/glew.h>
#include <vector>
#include <string>

#include <tiny/core/types.h>
#include <tiny/core/transformable.h>


#include "material.h"
#include "bufferobject.h"

namespace tiny { namespace rendering { namespace gl {

	class Mesh : public core::Transformable
	{
	public:

		enum AttributeLocation : GLuint
		{
			POSITION,
			TEXCOORD,
			NORMAL,
			TANGENT,
			N_ATTRIBUTES,
		};

		Mesh();
		Mesh(Mesh &&mesh) = default;
		
		virtual ~Mesh() = default;

		Mesh &operator=(Mesh &&other) = default;

		void draw() const;

		const Material &material() const { return mMaterial; }
		Material &material() { return mMaterial; }

		Mesh &fromFile(const std::string &objFilename, int shape=0);
		Mesh &fromFiles(const std::string &objFilname, const std::string &mtlFilename);

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