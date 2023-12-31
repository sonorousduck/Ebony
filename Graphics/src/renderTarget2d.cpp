#include "renderTarget2d.hpp"
#include <iostream>

namespace Ebony
{
	RenderTarget2D RenderTarget2D::Create(std::uint16_t width, std::uint16_t height, GLint minFilter, GLint magFilter, bool depthStencil, bool usesCamera)
	{
		
		unsigned int framebuffer = 0;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		unsigned int textureColorBuffer = 0;
		glGenTextures(1, &textureColorBuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		
		glBindTexture(GL_TEXTURE_2D, 0);

		// Attach it to the framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

		unsigned int rbo = 0;
		if (depthStencil)
		{
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return RenderTarget2D(width, height, framebuffer, textureColorBuffer, rbo, depthStencil, usesCamera);
	}

	// https://stackoverflow.com/questions/44763449/updating-width-and-height-of-render-target-on-the-fly
	void RenderTarget2D::Resize(std::uint16_t newWidth, std::uint16_t newHeight)
	{
		// Resize Color Attachment
		glBindTexture(GL_TEXTURE_2D, m_TextureColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newWidth, newHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Resize Depth Attachment
		if (depthStencil)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, m_Rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newWidth, newHeight);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}

		// Update internal dimensions
		this->m_Width = newWidth;
		this->m_Height = newHeight;

	}
}