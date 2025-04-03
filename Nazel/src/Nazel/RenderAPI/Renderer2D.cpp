#include "pch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

// Batch的基本思路
// CPU和GPU都开辟同样大小的一大块内存（为了存储顶点信息）
//
// 索引在程序运行时生成对应规则后绑定到索引缓冲中
// 
// 动态生成顶点信息（现在改成Drawquad只是确定图形顶点的位置）
// 
// 然后在Endscene，将CPU的动态生成的顶点数据上传给GPU，然后再绘制出来。

// Tetxure Batch 思路
// 先提前为此次的shader上传一个默认的大小为32的采样数组
// u_Textures[i] = j，其中i = j，u_Textures[1] = 1表示片段着色器采样纹理槽1号上的纹理
// 而纹理槽1号上的纹理缓冲区ID是不是等于1可以不用关心。
// 加载一个纹理得到纹理对象，用数组保存这个纹理对象
// 在绘制带有纹理的quad图形时，判断数组中是否有这个纹理对象
// 有就取出 i 下标
// 没有就加在数组已有纹理的末尾，并且记录下标 i
// 设置当前顶点采样的纹理单元是 i，后续会将这个纹理槽号 i 从顶点阶段传入到fragment片段着色器阶段
// 在Drawcall前，TextureSlots数组上存储已经加载的纹理，按照顺序依次绑定到对应的纹理槽上
// Drawcall时，在片段着色器上，读取采样对应纹理槽号i上的纹理
namespace Nazel {
struct QuadVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};
struct Renderer2DData
{
	const uint32_t MaxQuads = 10000;
	const uint32_t MaxVertices = MaxQuads * 4;
	const uint32_t MaxIndices = MaxQuads * 6;
	// OpenGL限制了一次drawcall能使用多少个纹理槽，通常是32个
	static const uint32_t MaxTexureSlots = 32;	// 以后可以做纹理集Texture altas

	Ref<VertexArray> QuadVertexArray;
	Ref<VertexBuffer> QuadVertexBuffer;
	//Ref<Shader> FlatColorShader;
	Ref<Shader> TextureShader;
	Ref<Texture2D> WhiteTexture;

	uint32_t QuadIndexCount = 0;
	QuadVertex* QuadVertexBufferBase = nullptr;
	QuadVertex* QuadVertexBufferPtr = nullptr;

	std::array<Ref<Texture2D>, MaxTexureSlots> TextureSlots;
	uint32_t TextureSlotIndex = 1; // 0号是白色纹理
};

static Renderer2DData s_Data;

void Renderer2D::Init() {
	PROFILE_FUNCTION();

	s_Data.QuadVertexArray = VertexArray::Create();
	s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
	s_Data.QuadVertexBuffer->SetLayout({
		{ShaderDataType::Float3, "a_Position" },
		{ShaderDataType::Float4, "a_Color" },
		{ShaderDataType::Float2, "a_TexCoord" },
		{ShaderDataType::Float , "a_TexIndex" },
		{ShaderDataType::Float , "a_TilingFactor" }
									   });
	s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

	s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxIndices];
	uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

	uint32_t offset = 0;
	for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;
		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;
		offset += 4;
	}

	Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
	s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
	delete quadIndices;

	int32_t samplers[s_Data.MaxTexureSlots];
	for (auto i = 0; i < s_Data.MaxTexureSlots; i++) {
		samplers[i] = i;
	}

	s_Data.WhiteTexture = Texture2D::Create(1, 1);
	uint32_t whiteTextureData = 0xffffffff;
	s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

	s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
	s_Data.TextureShader->Bind();
	// 设置采样器
	s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTexureSlots);

	s_Data.TextureSlots[0] = s_Data.WhiteTexture;
}
void Renderer2D::Shutdown() {
}
void Renderer2D::BeginScene(OrthographicCamera& camera) {
	PROFILE_FUNCTION();
	s_Data.TextureShader->Bind();
	s_Data.TextureShader->SetMat4("u_ProjectionView", camera.GetProjectionViewMatrix());
	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

	s_Data.TextureSlotIndex = 1;
}
void Renderer2D::EndScene() {
	PROFILE_FUNCTION();
	uint32_t size = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
	s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, size);
	Flush();
}
void Renderer2D::Flush() {
	for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
		s_Data.TextureSlots[i]->Bind(i);
	}
	RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
}
void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
	DrawQuad({ position.x, position.y, 0.0f }, size, color);
}
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
	PROFILE_FUNCTION();

	const float texIndex = 0;
	const float tilingFactor = 1.0f;

	s_Data.QuadVertexBufferPtr->Position = position;
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadIndexCount += 6;
	/*s_Data->TextureShader->SetFloat4("u_Color", color);
	s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
	s_Data->WhiteTexture->Bind();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	s_Data->TextureShader->SetMat4("u_ModelTransform", transform);

	s_Data->QuadVertexArray->Bind();
	RenderCommand::DrawIndexed(s_Data->QuadVertexArray);*/
}
void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) {
	DrawQuad(glm::vec3(position, 0.0f), size, texture, tilingFactor, tintColor);
}
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) {
	PROFILE_FUNCTION();

	constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	float textureIndex = 0.0f;
	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
		if (*s_Data.TextureSlots[i].get() == *texture.get()) {
			textureIndex = i;
			break;
		}
	}
	if (textureIndex == 0.0f) {
		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[s_Data.TextureSlotIndex++] = texture;
	}

	s_Data.QuadVertexBufferPtr->Position = position;
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadIndexCount += 6;

#if OLD_PATH
	s_Data.TextureShader->SetFloat4("u_Color", tintColor);
	s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
	texture->Bind();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	s_Data.TextureShader->SetMat4("u_ModelTransform", transform);

	s_Data.QuadVertexArray->Bind();
	RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
#endif
}
void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
	DrawRotatedQuad(position, size, rotation, color);
}
void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
	PROFILE_FUNCTION();

	s_Data.TextureShader->SetFloat4("u_Color", color);
	s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
	s_Data.WhiteTexture->Bind();

	// 绕z轴旋转
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	s_Data.TextureShader->SetMat4("u_ModelTransform", transform);

	s_Data.QuadVertexArray->Bind();
	RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
}
void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) {
	DrawRotatedQuad(position, size, rotation, texture, tilingFactor, tintColor);
}
void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) {
	PROFILE_FUNCTION();

	s_Data.TextureShader->SetFloat4("u_Color", tintColor);
	s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
	texture->Bind();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	s_Data.TextureShader->SetMat4("u_ModelTransform", transform);

	s_Data.QuadVertexArray->Bind();
	RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
}
}