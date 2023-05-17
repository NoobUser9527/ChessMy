#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "Engine.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int Max(int x, int y)
{
    return x > y ? x : y;
}

int main()
{
    glfwSetErrorCallback(glfw_error_callback);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ��������������
    GLFWwindow* window = glfwCreateWindow(750, 750, "Let's Chess!", NULL, NULL);
    if (window == nullptr)
    {
        printf("���� GLFW ����ʧ��");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);// ������ֱͬ��

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("��ʼ��GLADʧ��");
        return -1;
    }
    // ImGui������
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    //const char* glsl_version = "#version 330 core";
    ImGui_ImplOpenGL3_Init("#version 330 core");
    io.IniFilename = NULL;
    // ����ImGui������������
    io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 13.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());



    
    
    Engine engine;

    glm::mat4 projection = glm::mat4(1.0f);
    engine.SetUniformMat4(engine.boardShader, "projection", projection);

    glm::mat4 view = glm::mat4(1.0f);
    engine.SetUniformMat4(engine.boardShader, "view", view);
    


    int x = 0, y = 0;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // ��ʼImGui����
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui
        // ��ʼ������
        bool play = false;
        if (play != true)
        {
            
            ImGui::Begin(u8"����");
            ImGui::Text(u8"��������");
            ImGui::ColorEdit3(u8"������ɫ1", (float*)&engine.boardColor1);
            ImGui::ColorEdit3(u8"������ɫ2", (float*)&engine.boardColor2);

            // �綨row��ΧΪ[1 ,20]
            engine.row = engine.row < 1 ? engine.row : engine.row > 20 ? 20 : engine.row;
            ImGui::InputInt(u8"��������(<=20)", &engine.row);
            // �綨row��ΧΪ[1 ,20]
            engine.col = engine.col < 1 ? engine.col : engine.col > 20 ? 20 : engine.col;
            ImGui::InputInt(u8"��������(<=20)", &engine.col);

            ImGui::InputInt(u8"���̷��", &engine.boardStyle);
            ImGui::Checkbox(u8"�������г�ʼ���ӣ�", &engine.haveInitChessman);


            if (engine.haveInitChessman)
            {
                ImGui::Text(u8"��������");
                ImGui::InputInt(u8"������", &x);
                ImGui::InputInt(u8"������", &y);
                
                ImGui::InputInt(u8"����", &engine.board[x+1][y+1]);

            }
            
           
            

            if (ImGui::Button("OK"))
            {
                std::cout << "OK" << std::endl;
                play = true;
            }
            
            engine.AdjustBoard();
           
            ImGui::End();

        }
        else if(play == true)
        {
            ImGui::Begin("Chess!");
            ImGui::Text(u8"һЩ��������");
            
            
            ImGui::End();
        }
        
        // Engine
        


        // ������

        engine.DrawBoard();

     




        

        
        
        //ImGui end
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(window);
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
