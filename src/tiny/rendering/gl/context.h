#ifndef __TINY_RENDERING_GL_CONTEXT_H__
#define __TINY_RENDERING_GL_CONTEXT_H__

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <boost/noncopyable.hpp>
#include <stdexcept>

#include <tiny/core/window.h>

namespace tiny { namespace rendering { namespace gl {

	class Context : public boost::noncopyable
	{
	public:

		Context() : mHandle(NULL) {}

		Context(Context &&other) : mHandle(other.mHandle)
		{
			other.release();
		}

		Context(core::Window &window) : mHandle(NULL)
		{
			mHandle = SDL_GL_CreateContext(window.handle());
			if (mHandle == NULL)
				throw std::runtime_error("could not create GL context");

			glewExperimental = GL_TRUE; 
			if (GLEW_OK != glewInit())
				throw std::runtime_error("could not initiazlie GLEW");

			glClearColor(0,0,0,1);
		}

		Context &operator=(Context &&other)
		{
			destroy();
			mHandle = other.mHandle;
			other.release();
			return (*this);
		}

		void destroy()
		{
			if (mHandle)
				SDL_GL_DeleteContext(mHandle);
		}

		void release()
		{
			mHandle = NULL;
		}


		void clear()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void vsync(bool onoff)
		{
			SDL_GL_SetSwapInterval(onoff ? 1 : 0);
		}

		std::string version() const
		{
			return (char *)glGetString(GL_VERSION);
		}

		void enableBlending()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			glDepthMask(false);
			glDepthFunc(GL_EQUAL);
		}

		void disableBlending()
		{
			glDepthFunc(GL_LESS);
			glDepthMask(true);
			glDisable(GL_BLEND);
		}

		void enableBackfaceCulling()
		{
			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
		}

		void disableBackfaceCulling()
		{
			glDisable(GL_CULL_FACE);
		}

		void enableDepthTest()
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
		}

		void disableDepthTest()
		{
			glDisable(GL_DEPTH_TEST);
		}	

	private:	

		void *mHandle;
	};

}}}


#endif