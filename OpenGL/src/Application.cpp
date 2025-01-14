#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /*
        OpenGL 3.3,
        OPENGL_COMPAT_PROFILE�� VAO �ϳ��� �⺻���� ������ ä�� �����Ѵ�.
        OPENGL_CORE_PROFILE�� VAO ���� �����Ѵ�.
        ������ OPENGL_CORE_PROFILE�� �����ߴٸ�glGenVertexArrays�Լ��� �̿���
        VAO �ϳ��� ������־�� �Ѵ�.
    */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core ���� vao �� �������Ѵ�.

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Soohwan's OpenGL Practice", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //ȭ�� �����ӷ�
    glfwSwapInterval(1);

    // Glew �׻� OpenGL Rending context ������ �����ؾ��Ѵ�. (�ƴϸ� ����)
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    //OpenGL ���� Ȯ��
    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float positions[] = {
        -0.5f, -0.5f,  0.0f, 0.0f, //0
         0.5f, -0.5f,  1.0f, 0.0f,//1
         0.5f,  0.5f,  1.0f, 1.0f,//2
        -0.5f,  0.5f,  0.0f, 1.0f //3
        };

        //Index Buffer
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // �ҽ� ���Ŀ��� -1�� ���� ���̰ڴٴ� ���ε� ���߿�..

        //VAO ���ε�
        VertexArray va;
        va.Bind();
        
        //VBO ���� �� VAO �� ���ε�
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        //�ؽ��� ���̾ƿ�
        layout.Push<float>(2);

        va.AddBuffer(vb, layout);

        //IB ����
        IndexBuffer ib(indices, 6);

        //���̴�, ������ ���ε�
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        //�ؽ���
        Texture texture("res/textures/mipmaps.png");
        texture.Bind();      // ����Ʈ ������ 0 
        shader.SetUniform1i("u_Texture", 0); // ���⵵ 0 (������ �ٲٸ� ���⵵ �ٲ��ֱ�)

        // unbind �ϱ� (���θ� ����)
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
                increment = -0.009f;
            else if (r < 0.0f)
                increment = 0.009f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
     
}