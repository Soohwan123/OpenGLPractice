#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray() 
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        //항상 모든 Attribute 는 Eable -> 활성화 시켜줘야한다 (안하면 검은화면만 뜸)
        //첫번째 Attribute 니 0
        GLCall(glEnableVertexAttribArray(i));
        // 이 예시같은 경우는 Attribute 가 한가지밖에없어서 굉장히 단순한 예시다. - Positions (고로 한번만 이 함수를 부르면 된다.)
        // 0 -> 처음이자 마지막으로 부르는 Attribute 니 0,
        // 2d 니 2,
        // type 은 float
        // 색이아니니 normalize 필요없으니 FALSE
        // 각각의 vertex 가 2개의 float 로 이루어진 꼭짓점이니 float*2 (하나의 vertex 크기) 
        GLCall(glVertexAttribPointer(i, element.count, element.type, 
            element.normalized, layout.GetStride(), (const void*)offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}