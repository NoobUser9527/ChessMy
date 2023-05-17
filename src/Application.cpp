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

    // 创建窗口上下文
    GLFWwindow* window = glfwCreateWindow(750, 750, "Let's Chess!", NULL, NULL);
    if (window == nullptr)
    {
        printf("创建 GLFW 窗口失败");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);// 开启垂直同步

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("初始化GLAD失败");
        return -1;
    }
    // ImGui上下文
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    //const char* glsl_version = "#version 330 core";
    ImGui_ImplOpenGL3_Init("#version 330 core");
    io.IniFilename = NULL;
    // 设置ImGui可以输入中文
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
        // 开始ImGui窗口
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui
        // 初始化棋盘
        bool play = false;
        if (play != true)
        {
            
            ImGui::Begin(u8"设置");
            ImGui::Text(u8"设置棋盘");
            ImGui::ColorEdit3(u8"棋盘颜色1", (float*)&engine.boardColor1);
            ImGui::ColorEdit3(u8"棋盘颜色2", (float*)&engine.boardColor2);

            // 界定row范围为[1 ,20]
            engine.row = engine.row < 1 ? engine.row : engine.row > 20 ? 20 : engine.row;
            ImGui::InputInt(u8"棋盘行数(<=20)", &engine.row);
            // 界定row范围为[1 ,20]
            engine.col = engine.col < 1 ? engine.col : engine.col > 20 ? 20 : engine.col;
            ImGui::InputInt(u8"棋盘列数(<=20)", &engine.col);

            ImGui::InputInt(u8"棋盘风格", &engine.boardStyle);
            ImGui::Checkbox(u8"棋盘上有初始棋子？", &engine.haveInitChessman);


            if (engine.haveInitChessman)
            {
                ImGui::Text(u8"输入棋盘");
                ImGui::InputInt(u8"横坐标", &x);
                ImGui::InputInt(u8"纵坐标", &y);
                
                ImGui::InputInt(u8"棋子", &engine.board[x+1][y+1]);

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
            ImGui::Text(u8"一些棋盘数据");
            
            
            ImGui::End();
        }
        
        // Engine
        


        // 画棋盘

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
