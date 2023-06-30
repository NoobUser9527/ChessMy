#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Engine.h"


static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void processInput(GLFWwindow* window, int row, int col, glm::vec2* position)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		if (position->y < row * SPEED)
			position->y++;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		if (position->y > SPEED)
			position->y--;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		if (position->x < col * SPEED)
			position->x++;
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		if (position->x > SPEED)
			position->x--;
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

	// 启用混合(让图片可以透明)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	glm::vec2 sign = glm::vec2((float)SPEED);

	int row = 0, col = 0;

	int x = 0, y = 0;
	int x2 = 0, y2 = 0;
	bool choose = true;

	int page = 0;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window, row, col, &sign);
		engine.Clear();
		// 开始ImGui窗口
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// ImGui
		// 初始化棋盘
		switch (page)
		{
		case 0:// 初始界面
			ImGui::Begin(u8"Let's Chess!");
			ImGui::Text(u8"欢迎来到本程序");

			if (ImGui::Button(u8"游玩已有游戏"))
				page--;

			if (ImGui::Button(u8"创建棋类游戏"))
				page++;
			ImGui::End();
			break;
		case 1:// 棋盘初始设定界面
			ImGui::Begin(u8"棋盘初始设定");
			ImGui::InputInt(u8"棋盘行数(<=20)", &row);
			ImGui::InputInt(u8"棋盘列数(<=20)", &col);
			ImGui::Text(u8"每次更改行列数后必须点击\"确认行列\"");
			if (ImGui::Button(u8"确定行列"))
				engine.InitBoard(row, col);

			ImGui::Text(u8"棋盘具体设计");
			// 设置x的范围
			x = x < 1 ? 1 : x >= engine.chess.row ? engine.chess.row : x;
			ImGui::InputInt(u8"横坐标", &x);
			// 设置y的范围
			y = y < 1 ? 1 : y >= engine.chess.col ? engine.chess.col : y;
			ImGui::InputInt(u8"纵坐标", &y);

			ImGui::RadioButton(u8"无效点", &engine.chess.board[x][y].type, -1);
			ImGui::SameLine();
			ImGui::RadioButton(u8"空棋盘", &engine.chess.board[x][y].type, 0);
			ImGui::SameLine();
			ImGui::RadioButton(u8"玩家一", &engine.chess.board[x][y].type, 1);
			ImGui::SameLine();
			ImGui::RadioButton(u8"玩家二", &engine.chess.board[x][y].type, 2);

			ImGui::Checkbox(u8"将该点设置为米字格？", &engine.chess.board[x][y].isMi);

			if (ImGui::Button(u8"上一步"))
				page--;
			if (ImGui::Button(u8"下一步"))
				page++;

			ImGui::End();
			break;
		case 2:// 棋盘规则设定界面
			ImGui::Begin(u8"棋盘规则设定");

			ImGui::Text(u8"谁先下");
			ImGui::RadioButton(u8"玩家一", &engine.chess.turn, 1);
			ImGui::SameLine();
			ImGui::RadioButton(u8"玩家二", &engine.chess.turn, 2);

			ImGui::Text(u8"如何切换下棋方");
			ImGui::RadioButton(u8"正常轮换", &engine.chess.howToChangeTurn, 0);
			ImGui::SameLine();
			ImGui::RadioButton(u8"单人棋局", &engine.chess.howToChangeTurn, 1);
			ImGui::SameLine();
			ImGui::RadioButton(u8"操纵多个棋子轮换", &engine.chess.howToChangeTurn, 2);
			if (engine.chess.howToChangeTurn == 2)
			{
				ImGui::InputInt(u8"玩家一一次可以移动几个棋子", &engine.chess.turnOptNum[1]);
				ImGui::InputInt(u8"玩家二一次可以移动几个棋子", &engine.chess.turnOptNum[2]);
			}
			ImGui::Text(u8"下棋方式");
			ImGui::RadioButton(u8"双方落子", &engine.chess.playMod, 0);
			ImGui::RadioButton(u8"移子", &engine.chess.playMod, 1);
			if (engine.chess.playMod == 0)
			{
				ImGui::Text(u8"落子规则");
				ImGui::RadioButton(u8"仅落子", &engine.chess.placeMod, 0);
				ImGui::RadioButton(u8"夹吃", &engine.chess.placeMod, 1);

				ImGui::Text(u8"落子具体规则");
				ImGui::RadioButton(u8"有空格即可", &engine.chess.placeOkMod, 0);
				ImGui::RadioButton(u8"有空格而且还要落子改变其他棋子", &engine.chess.placeOkMod, 1);
			}
			else if (engine.chess.playMod = 1)
			{
				ImGui::Text(u8"移子规则");
				ImGui::RadioButton(u8"平移", &engine.chess.moveMod, 0);
				ImGui::RadioButton(u8"移子或跳吃", &engine.chess.moveMod, 1);
				ImGui::RadioButton(u8"玩家一移子, 玩家二移子或跳吃", &engine.chess.moveMod, 2);
				ImGui::RadioButton(u8"若玩家二落点周围只有一个玩家一棋子，则吃掉", &engine.chess.moveMod, 3);

				ImGui::Text(u8"移子具体规则");
				ImGui::RadioButton(u8"移到相邻空位", &engine.chess.moveOkMod, 0);
				ImGui::RadioButton(u8"移子或跳子", &engine.chess.moveOkMod, 1);
				ImGui::RadioButton(u8"玩家一移子，玩家二移子或跳子", &engine.chess.moveOkMod, 2);
				ImGui::RadioButton(u8"可以移动到任意空地", &engine.chess.moveOkMod, 3);
				ImGui::RadioButton(u8"只能跳子", &engine.chess.moveOkMod, 4);
			}
			ImGui::Text(u8"胜利条件");
			ImGui::RadioButton(u8"一方不可下就换另一方,双方都不能下就结束", &engine.chess.ifEndMod, 1);

			ImGui::RadioButton(u8"数子连珠", &engine.chess.ifEndMod, 2);
			if (engine.chess.ifEndMod == 2)
			{
				ImGui::InputInt(u8"连子数量", &engine.chess.series);
				ImGui::Checkbox(u8"只允许斜向", &engine.chess.seriesMod);
			}
			ImGui::RadioButton(u8"老虎棋3", &engine.chess.ifEndMod, 3);
			if (engine.chess.ifEndMod == 3)
				ImGui::InputInt(u8"最少\"人\"的数量", &engine.chess.endNum);

			ImGui::RadioButton(u8"独立钻石", &engine.chess.ifEndMod, 4);

			if (ImGui::Button(u8"确定"))
				page++;
			if (ImGui::Button(u8"返回"))
				page = 0;

			ImGui::End();
			break;
		case 3:// 游玩界面
			ImGui::Begin(u8"游玩界面");
			if (engine.chess.isEnd == false)
			{
				if (engine.chess.playMod == 0)//落子
				{
					ImGui::Text(u8"轮到玩家%d", engine.chess.turn);
					ImGui::Text(u8"当前坐标:(%d, %d)", (int)sign.x / SPEED, (int)sign.y / SPEED);

					ImGui::Text(u8"一号玩家共%d颗子", engine.chess.GetChessNum(1));
					ImGui::Text(u8"二号玩家共%d颗子", engine.chess.GetChessNum(2));

					if (ImGui::Button(u8"确认"))
					{
						x = (int)sign.x / SPEED;
						y = (int)sign.y / SPEED;
						engine.chess.PlayPlace(x, y);
					}
				}
				else if (engine.chess.playMod == 1)//移子
				{
					ImGui::Text(u8"轮到玩家%d", engine.chess.turn);
					ImGui::Text(u8"当前坐标:(%d, %d)", (int)sign.x / SPEED, (int)sign.y / SPEED);

					if (ImGui::Button(u8"选中"))
					{
						x = (int)sign.x / SPEED;
						y = (int)sign.y / SPEED;
					}
					ImGui::Text(u8"选中横坐标%d", x);
					ImGui::Text(u8"选中纵坐标%d", y);
					ImGui::Text(u8"移动到%d", x2);
					ImGui::Text(u8"移动到%d", y2);

					if (ImGui::Button(u8"移动到"))
					{
						x2 = (int)sign.x/ SPEED;
						y2 = (int)sign.y/ SPEED;
						engine.chess.PlayMove(x, y, x2, y2);
					}
				}
				if (ImGui::Button(u8"显示历史"))
					engine.chess.manual.Show(engine.chess.playMod);
			}
			else
			{
				ImGui::Begin(u8"游戏结束");

				if(engine.chess.winner != 0)
					ImGui::Text(u8"恭喜玩家%d胜利!", engine.chess.winner);

				ImGui::End();
			}
			ImGui::End();
			break;
		case -1:// 预设游戏

			ImGui::Begin(u8"请选择");

			ImGui::Text(u8"选择你要玩的游戏");
			if (ImGui::Button(u8"五子棋"))
			{
				row = 15;
				col = 15;
				engine.InitBoard(row, col);

				engine.SetRule(1, 0, 0, 0, 0, 2, 5);
				page = 3;
			}
			if (ImGui::Button(u8"黑白棋"))
			{
				row = 8;
				col = 8;
				engine.InitBoard(row, col);

				engine.chess.board[4][4].type = 1;
				engine.chess.board[5][5].type = 1;
				engine.chess.board[4][5].type = 2;
				engine.chess.board[5][4].type = 2;

				engine.SetRule(1, 0, 0, 1, 1, 1);
				page = 3;
			}
			if (ImGui::Button(u8"圈叉棋"))
			{
				row = 3;
				col = 3;
				engine.InitBoard(row, col);

				engine.SetRule(1, 0, 0, 0, 0, 2, 3);

				page = 3;
			}
			if (ImGui::Button(u8"米字棋"))
			{
				row = 3;
				col = 3;
				engine.InitBoard(row, col);

				engine.chess.board[2][2].isMi = true;

				engine.chess.board[1][1].type = 1;
				engine.chess.board[2][1].type = 1;
				engine.chess.board[3][1].type = 1;
				engine.chess.board[1][3].type = 2;
				engine.chess.board[2][3].type = 2;
				engine.chess.board[3][3].type = 2;

				engine.SetRule(1, 1, 0, 0, 0, 2, 3, true);

				page = 3;
			}
			if (ImGui::Button(u8"独立钻石棋"))
			{
				row = 7;
				col = 7;
				engine.InitBoard(row, col);

				for (int i = 1; i <= row; i++)
					for (int j = 1; j <= col; j++)
						if ((i + j) % 2 == 0)
							engine.chess.board[i][j].type = 1;
						else
							engine.chess.board[i][j].type = 2;

				engine.chess.board[4][4].type = 0;

				engine.chess.board[1][1].type = -1;
				engine.chess.board[1][2].type = -1;
				engine.chess.board[1][6].type = -1;
				engine.chess.board[1][7].type = -1;
				engine.chess.board[2][1].type = -1;
				engine.chess.board[2][2].type = -1;
				engine.chess.board[2][6].type = -1;
				engine.chess.board[2][7].type = -1;
				engine.chess.board[6][1].type = -1;
				engine.chess.board[6][2].type = -1;
				engine.chess.board[6][6].type = -1;
				engine.chess.board[6][7].type = -1;
				engine.chess.board[7][1].type = -1;
				engine.chess.board[7][2].type = -1;
				engine.chess.board[7][6].type = -1;
				engine.chess.board[7][7].type = -1;

				engine.SetRule(1, 1, 1, 1, 4, 4);

				page = 3;
			}
			if (ImGui::Button(u8"老虎棋"))
			{
				row = 5;
				col = 5;
				engine.InitBoard(row, col);

				engine.chess.board[1][1].type = 1;
				engine.chess.board[1][2].type = 1;
				engine.chess.board[2][1].type = 1;
				engine.chess.board[2][2].type = 1;
				engine.chess.board[3][1].type = 1;
				engine.chess.board[3][2].type = 1;
				engine.chess.board[4][1].type = 1;
				engine.chess.board[4][2].type = 1;
				engine.chess.board[5][1].type = 1;
				engine.chess.board[5][2].type = 1;

				engine.chess.board[3][5].type = 2;

				engine.SetRule(1, 1, 0, 3, 0, 3, 0, 0, 4);

				page = 3;
			}

			if (ImGui::Button(u8"返回"))
				page++;

			ImGui::End();
			break;
		}

		// Engine
		// 画棋盘
		engine.DrawBoard(sign);


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
