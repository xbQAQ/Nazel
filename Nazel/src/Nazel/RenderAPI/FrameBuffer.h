#pragma once

#include "Nazel/Core/Core.h"

namespace Nazel {
struct FramebufferSpecification
{
	uint32_t Width, Height;
	uint32_t Samples = 1;
	bool SwapChainTarget = false;
};

class FrameBuffer
{
public:
	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual uint32_t GetColorAttachmentRendererID() const = 0;

	virtual const FramebufferSpecification& GetSpecification() const = 0;

	static Ref<FrameBuffer> Create(const FramebufferSpecification& spec);
};
}

