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

	// ���û��(��ͼƬ����͸��)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
		// ��ʼImGui����
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// ImGui
		// ��ʼ������
		switch (page)
		{
		case 0:// ��ʼ����
			ImGui::Begin(u8"Let's Chess!");
			ImGui::Text(u8"��ӭ����������");

			if (ImGui::Button(u8"����������Ϸ"))
				page--;

			if (ImGui::Button(u8"����������Ϸ"))
				page++;
			ImGui::End();
			break;
		case 1:// ���̳�ʼ�趨����
			ImGui::Begin(u8"���̳�ʼ�趨");
			ImGui::InputInt(u8"��������(<=20)", &row);
			ImGui::InputInt(u8"��������(<=20)", &col);
			ImGui::Text(u8"ÿ�θ����������������\"ȷ������\"");
			if (ImGui::Button(u8"ȷ������"))
				engine.InitBoard(row, col);

			ImGui::Text(u8"���̾������");
			// ����x�ķ�Χ
			x = x < 1 ? 1 : x >= engine.chess.row ? engine.chess.row : x;
			ImGui::InputInt(u8"������", &x);
			// ����y�ķ�Χ
			y = y < 1 ? 1 : y >= engine.chess.col ? engine.chess.col : y;
			ImGui::InputInt(u8"������", &y);

			ImGui::RadioButton(u8"��Ч��", &engine.chess.board[x][y].type, -1);
			ImGui::SameLine();
			ImGui::RadioButton(u8"������", &engine.chess.board[x][y].type, 0);
			ImGui::SameLine();
			ImGui::RadioButton(u8"���һ", &engine.chess.board[x][y].type, 1);
			ImGui::SameLine();
			ImGui::RadioButton(u8"��Ҷ�", &engine.chess.board[x][y].type, 2);

			ImGui::Checkbox(u8"���õ�����Ϊ���ָ�", &engine.chess.board[x][y].isMi);

			if (ImGui::Button(u8"��һ��"))
				page--;
			if (ImGui::Button(u8"��һ��"))
				page++;

			ImGui::End();
			break;
		case 2:// ���̹����趨����
			ImGui::Begin(u8"���̹����趨");

			ImGui::Text(u8"˭����");
			ImGui::RadioButton(u8"���һ", &engine.chess.turn, 1);
			ImGui::SameLine();
			ImGui::RadioButton(u8"��Ҷ�", &engine.chess.turn, 2);

			ImGui::Text(u8"����л����巽");
			ImGui::RadioButton(u8"�����ֻ�", &engine.chess.howToChangeTurn, 0);
			ImGui::SameLine();
			ImGui::RadioButton(u8"�������", &engine.chess.howToChangeTurn, 1);
			ImGui::SameLine();
			ImGui::RadioButton(u8"���ݶ�������ֻ�", &engine.chess.howToChangeTurn, 2);
			if (engine.chess.howToChangeTurn == 2)
			{
				ImGui::InputInt(u8"���һһ�ο����ƶ���������", &engine.chess.turnOptNum[1]);
				ImGui::InputInt(u8"��Ҷ�һ�ο����ƶ���������", &engine.chess.turnOptNum[2]);
			}
			ImGui::Text(u8"���巽ʽ");
			ImGui::RadioButton(u8"˫������", &engine.chess.playMod, 0);
			ImGui::RadioButton(u8"����", &engine.chess.playMod, 1);
			if (engine.chess.playMod == 0)
			{
				ImGui::Text(u8"���ӹ���");
				ImGui::RadioButton(u8"������", &engine.chess.placeMod, 0);
				ImGui::RadioButton(u8"�г�", &engine.chess.placeMod, 1);

				ImGui::Text(u8"���Ӿ������");
				ImGui::RadioButton(u8"�пո񼴿�", &engine.chess.placeOkMod, 0);
				ImGui::RadioButton(u8"�пո���һ�Ҫ���Ӹı���������", &engine.chess.placeOkMod, 1);
			}
			else if (engine.chess.playMod = 1)
			{
				ImGui::Text(u8"���ӹ���");
				ImGui::RadioButton(u8"ƽ��", &engine.chess.moveMod, 0);
				ImGui::RadioButton(u8"���ӻ�����", &engine.chess.moveMod, 1);
				ImGui::RadioButton(u8"���һ����, ��Ҷ����ӻ�����", &engine.chess.moveMod, 2);
				ImGui::RadioButton(u8"����Ҷ������Χֻ��һ�����һ���ӣ���Ե�", &engine.chess.moveMod, 3);

				ImGui::Text(u8"���Ӿ������");
				ImGui::RadioButton(u8"�Ƶ����ڿ�λ", &engine.chess.moveOkMod, 0);
				ImGui::RadioButton(u8"���ӻ�����", &engine.chess.moveOkMod, 1);
				ImGui::RadioButton(u8"���һ���ӣ���Ҷ����ӻ�����", &engine.chess.moveOkMod, 2);
				ImGui::RadioButton(u8"�����ƶ�������յ�", &engine.chess.moveOkMod, 3);
				ImGui::RadioButton(u8"ֻ������", &engine.chess.moveOkMod, 4);
			}
			ImGui::Text(u8"ʤ������");
			ImGui::RadioButton(u8"һ�������¾ͻ���һ��,˫���������¾ͽ���", &engine.chess.ifEndMod, 1);

			ImGui::RadioButton(u8"��������", &engine.chess.ifEndMod, 2);
			if (engine.chess.ifEndMod == 2)
			{
				ImGui::InputInt(u8"��������", &engine.chess.series);
				ImGui::Checkbox(u8"ֻ����б��", &engine.chess.seriesMod);
			}
			ImGui::RadioButton(u8"�ϻ���3", &engine.chess.ifEndMod, 3);
			if (engine.chess.ifEndMod == 3)
				ImGui::InputInt(u8"����\"��\"������", &engine.chess.endNum);

			ImGui::RadioButton(u8"������ʯ", &engine.chess.ifEndMod, 4);

			if (ImGui::Button(u8"ȷ��"))
				page++;
			if (ImGui::Button(u8"����"))
				page = 0;

			ImGui::End();
			break;
		case 3:// �������
			ImGui::Begin(u8"�������");
			if (engine.chess.isEnd == false)
			{
				if (engine.chess.playMod == 0)//����
				{
					ImGui::Text(u8"�ֵ����%d", engine.chess.turn);
					ImGui::Text(u8"��ǰ����:(%d, %d)", (int)sign.x / SPEED, (int)sign.y / SPEED);

					ImGui::Text(u8"һ����ҹ�%d����", engine.chess.GetChessNum(1));
					ImGui::Text(u8"������ҹ�%d����", engine.chess.GetChessNum(2));

					if (ImGui::Button(u8"ȷ��"))
					{
						x = (int)sign.x / SPEED;
						y = (int)sign.y / SPEED;
						engine.chess.PlayPlace(x, y);
					}
				}
				else if (engine.chess.playMod == 1)//����
				{
					ImGui::Text(u8"�ֵ����%d", engine.chess.turn);
					ImGui::Text(u8"��ǰ����:(%d, %d)", (int)sign.x / SPEED, (int)sign.y / SPEED);

					if (ImGui::Button(u8"ѡ��"))
					{
						x = (int)sign.x / SPEED;
						y = (int)sign.y / SPEED;
					}
					ImGui::Text(u8"ѡ�к�����%d", x);
					ImGui::Text(u8"ѡ��������%d", y);
					ImGui::Text(u8"�ƶ���%d", x2);
					ImGui::Text(u8"�ƶ���%d", y2);

					if (ImGui::Button(u8"�ƶ���"))
					{
						x2 = (int)sign.x/ SPEED;
						y2 = (int)sign.y/ SPEED;
						engine.chess.PlayMove(x, y, x2, y2);
					}
				}
				if (ImGui::Button(u8"��ʾ��ʷ"))
					engine.chess.manual.Show(engine.chess.playMod);
			}
			else
			{
				ImGui::Begin(u8"��Ϸ����");

				if(engine.chess.winner != 0)
					ImGui::Text(u8"��ϲ���%dʤ��!", engine.chess.winner);

				ImGui::End();
			}
			ImGui::End();
			break;
		case -1:// Ԥ����Ϸ

			ImGui::Begin(u8"��ѡ��");

			ImGui::Text(u8"ѡ����Ҫ�����Ϸ");
			if (ImGui::Button(u8"������"))
			{
				row = 15;
				col = 15;
				engine.InitBoard(row, col);

				engine.SetRule(1, 0, 0, 0, 0, 2, 5);
				page = 3;
			}
			if (ImGui::Button(u8"�ڰ���"))
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
			if (ImGui::Button(u8"Ȧ����"))
			{
				row = 3;
				col = 3;
				engine.InitBoard(row, col);

				engine.SetRule(1, 0, 0, 0, 0, 2, 3);

				page = 3;
			}
			if (ImGui::Button(u8"������"))
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
			if (ImGui::Button(u8"������ʯ��"))
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
			if (ImGui::Button(u8"�ϻ���"))
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

			if (ImGui::Button(u8"����"))
				page++;

			ImGui::End();
			break;
		}

		// Engine
		// ������
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
