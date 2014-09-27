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

		FrameBuffer(size_t height, size_t width, std::vector<GLenum> attachments) : mWidth(width), mHeight(height), mDepthBuffer(width, height)
		{
			create(mHandle);
			bind();

			for(auto attachment: attachments)
			{
				mAttachments.emplace(attachment, Texture(NULL, height, width, GL_NEAREST));
				glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, mAttachments[attachment].handle(), 0);
			}
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer.handle());
			glDrawBuffers(attachments.size(), &attachments[0]);

			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				throw std::runtime_error("error during the setup of framebuffer attachments");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		virtual ~FrameBuffer() = default;

		void bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
			glViewport(0, 0, mWidth, mHeight);
		}

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
		std::map<GLenum, Texture> mAttachments;
	};

}}}

#endif