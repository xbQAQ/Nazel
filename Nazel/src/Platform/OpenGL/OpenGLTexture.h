#pragma once

#include "Nazel/RenderAPI/Texture.h"
#include <glad/glad.h>

namespace Nazel {

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(uint32_t width, uint32_t height);
	OpenGLTexture2D(const std::string& path);
	virtual ~OpenGLTexture2D();

	virtual uint32_t GetWidth() const override { return m_Width; }
	virtual uint32_t GetHeight() const override { return m_Height; }
	virtual uint32_t GetRendererID() const override { return m_RendererID; }

	void SetData(void* data, uint32_t size) override;

	virtual void Bind(uint32_t slot = 0) const override;
	bool operator==(const Texture& other) override;
private:
	std::string m_Path;
	uint32_t m_Width, m_Height;
	uint32_t m_RendererID;
	GLenum m_InternalFormat, m_DataFormat;
};
}
