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
        OPENGL_COMPAT_PROFILE은 VAO 하나를 기본으로 생성한 채로 시작한다.
        OPENGL_CORE_PROFILE은 VAO 없이 시작한다.
        때문에 OPENGL_CORE_PROFILE을 선택했다면glGenVertexArrays함수를 이용해
        VAO 하나를 만들어주어야 한다.
    */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core 에선 vao 를 만들어야한다.

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Soohwan's OpenGL Practice", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //화면 프레임률
    glfwSwapInterval(1);

    // Glew 항상 OpenGL Rending context 다음에 선언해야한다. (아니면 오류)
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    //OpenGL 버전 확인
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
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // 소스 알파에서 -1를 빼서 붙이겠다는 뜻인데 나중에..

        //VAO 바인딩
        VertexArray va;
        va.Bind();
        
        //VBO 생성 및 VAO 에 바인딩
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        //텍스쳐 레이아웃
        layout.Push<float>(2);

        va.AddBuffer(vb, layout);

        //IB 생성
        IndexBuffer ib(indices, 6);

        //쉐이더, 유니폼 바인딩
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        //텍스쳐
        Texture texture("res/textures/mipmaps.png");
        texture.Bind();      // 디폴트 슬롯이 0 
        shader.SetUniform1i("u_Texture", 0); // 여기도 0 (슬롯을 바꾸면 여기도 바꿔주기)

        // unbind 하기 (공부를 위해)
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