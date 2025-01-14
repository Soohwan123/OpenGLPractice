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
        //�׻� ��� Attribute �� Eable -> Ȱ��ȭ ��������Ѵ� (���ϸ� ����ȭ�鸸 ��)
        //ù��° Attribute �� 0
        GLCall(glEnableVertexAttribArray(i));
        // �� ���ð��� ���� Attribute �� �Ѱ����ۿ���� ������ �ܼ��� ���ô�. - Positions (��� �ѹ��� �� �Լ��� �θ��� �ȴ�.)
        // 0 -> ó������ ���������� �θ��� Attribute �� 0,
        // 2d �� 2,
        // type �� float
        // ���̾ƴϴ� normalize �ʿ������ FALSE
        // ������ vertex �� 2���� float �� �̷���� �������̴� float*2 (�ϳ��� vertex ũ��) 
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