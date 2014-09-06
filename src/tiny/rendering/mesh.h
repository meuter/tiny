#ifndef __TINY_CORE_MESH_H__
#define __TINY_CORE_MESH_H__

#include <GL/glew.h>
#include <tiny/core/types.h>
#include <vector>
#include <string>

namespace tiny { namespace rendering {

	class ShaderProgram;

	class Mesh
	{
	public:

		static Mesh fromFile(const std::string &filname);

		enum AttributeLocation : GLuint
		{
			POSITION,
			TEXCOORD,
		};

		struct vertex
		{
			core::vec3 position;
			core::vec2 texcoord;
		};

		Mesh();
		Mesh(const std::vector<vertex> &vertices, const std::vector<unsigned int> &indices);
		Mesh(const Mesh &other) = delete;
		Mesh(Mesh &&mesh);
		virtual ~Mesh();

		Mesh &operator=(const Mesh &mesh) = delete;
		Mesh &operator=(Mesh &&other);

		void load(const std::vector<vertex> &vertices, const std::vector<unsigned int> &indices);
		void unload();
		void draw();

	private:	
		size_t mSize;
		GLuint mVertexArrayHandle;
		GLuint mVertexBufferHandle;
		GLuint mIndexBufferHandle;
		bool mLoaded;
	};

}}

#endif