#ifndef __TINY_CORE_MESH_H__
#define __TINY_CORE_MESH_H__

#include <GL/glew.h>
#include <tiny/core/types.h>
#include <vector>

namespace tiny { namespace rendering {

	class Mesh
	{
	public:

		struct vertex
		{
			core::vec3 position;
		};

		Mesh();
		Mesh(const std::vector<vertex> &vertices);
		Mesh(const Mesh &other) = delete;
		Mesh(Mesh &&mesh);
		virtual ~Mesh();

		Mesh &operator=(const Mesh &mesh) = delete;
		Mesh &operator=(Mesh &&other);

		void loadVertices(const std::vector<vertex> &vertices);
		void unload();
		void draw();

	private:	
		size_t mNumberOfVertices;
		GLuint mVertexArrayHandle;
		GLuint mPositionBufferHandle;
		bool mLoaded;
	};

}}

#endif