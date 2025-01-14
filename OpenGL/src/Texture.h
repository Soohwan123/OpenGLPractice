#pragma once
#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer; // local storage for texture
	int m_Width, m_Height, m_BPP; //bits per pixel

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const; // 옵션 파라미터. modern gpu 는 32 슬롯을 넣는다. 모바일은 주로 8
	void Unbind();

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};