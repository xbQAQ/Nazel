#pragma once

#include "Nazel/RenderAPI/FrameBuffer.h"

namespace Nazel {
class OpenGLFrameBuffer : public FrameBuffer
{
public:
	OpenGLFrameBuffer(const FramebufferSpecification& spec);
	virtual ~OpenGLFrameBuffer();

	virtual void Bind() override;
	virtual void Unbind() override;

	void Reset();

	virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

	virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

private:
	uint32_t m_RendererID = 0;
	uint32_t m_ColorAttachment, m_DepthAttachment;
	FramebufferSpecification m_Specification;
};
}
