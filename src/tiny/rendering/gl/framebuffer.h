#ifndef __TINY_RENDERING_GL_FRAMEBUFFER_H__
#define __TINY_RENDERING_GL_FRAMEBUFFER_H__

#include <GL/glew.h>
#include <map>
#include "texture.h"

namespace tiny { namespace rendering { namespace gl {

	class DepthBuffer
	{
	public:
		DepthBuffer(size_t width, size_t height)
		{
			create(mHandle);
			bind();
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		}

		virtual ~DepthBuffer() = default;

		void bind()
		{
			glBindRenderbuffer(GL_RENDERBUFFER, mHandle);
		}

		inline GLuint handle() const { return mHandle; }

	protected:

		static void create(GLuint &handle)
		{
			glGenRenderbuffers(1, &handle);
			if (handle == 0)
				throw std::runtime_error("could not create render buffer");
		}

		static void destroy(GLuint handle)
		{
			glDeleteRenderbuffers(1, &handle);
		}

	private:
		Handle<destroy> mHandle;
	};

	class FrameBuffer
	{
	public:	

		FrameBuffer(size_t width, size_t height, std::vector<GLenum> attachments) : mWidth(width), mHeight(height), mDepthBuffer(width, height)
		{
			create(mHandle);
			bind();

			for(auto attachment: attachments)
			{
				mTextures.emplace(attachment, Texture(NULL, width, height, GL_NEAREST));
				glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, mTextures[attachment].handle(), 0);
			}
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer.handle());
			glDrawBuffers(attachments.size(), &attachments[0]);

			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				throw std::runtime_error("error during the setup of framebuffer attachments");

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}

		virtual ~FrameBuffer() = default;

		void bind(GLenum forWhat = GL_DRAW_FRAMEBUFFER)
		{
			glBindFramebuffer(forWhat, mHandle);
			glViewport(0, 0, mWidth, mHeight);
		}

		const Texture& texture(GLenum attachment)
		{
			auto hit = mTextures.find(attachment);
			if (hit == mTextures.end())
				throw std::runtime_error("could not find texture for provided attachment in frame buffer");

			return hit->second;
		}

		inline GLuint handle() { return mHandle; }

	protected:

		static void create(GLuint &handle)
		{
			glGenFramebuffers(1, &handle);
			if (handle == 0)
				throw std::runtime_error("could not create frame buffer");
		}

		static void destroy(GLuint handle)
		{
			glDeleteFramebuffers(1, &handle);
		}

	private:
		Handle<destroy> mHandle;	
		size_t mWidth, mHeight;
		DepthBuffer mDepthBuffer;
		std::map<GLenum, Texture> mTextures;
	};

}}}

#endif