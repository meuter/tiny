#ifndef __TINY_RENDERING_GL_SCENE_H__
#define __TINY_RENDERING_GL_SCENE_H__

#include <boost/noncopyable.hpp>

#include "mesh.h"

namespace tiny { namespace rendering { namespace gl {
	
	class Scene : boost::noncopyable
	{
	public:	
		
		Mesh &add(Mesh &&mesh)
		{
			mMeshes.push_back(std::move(mesh));
			return mMeshes.back();
		}

		inline const std::vector<Mesh> &meshes() const { return mMeshes; }

	private:
		std::vector<Mesh> mMeshes;
	};

}}}

#endif