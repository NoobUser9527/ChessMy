#include "Engine.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Engine::Engine()
{
	// 创建Shader
	boardShader = CreateShader("src\\shader\\boardShader.vert", "src\\shader\\boardShader.frag");

	CreateVAO();
	CreateBoardVBO();

	Texture("res\\texture\\line.png", line);
	Texture("res\\texture\\slash.png", slash);
	Texture("res\\texture\\chessman1.png", texChessman1);
	Texture("res\\texture\\chessman2.png", texChessman2);
	Texture("res\\texture\\ring.png", ring);


	BindTexture(line, 1);
	BindTexture(slash, 2);
	BindTexture(texChessman1, 3);
	BindTexture(texChessman2, 4);
	BindTexture(ring, 5);
}

Engine::~Engine()
{
	// delete texture
	glDeleteTextures(1, &line);
	glDeleteTextures(1, &slash);
	glDeleteTextures(1, &texChessman1);
	glDeleteTextures(1, &texChessman2);
	glDeleteTextures(1, &ring);

	// delete shader
	glDeleteProgram(boardShader);
	
	//delete VBO
	glDeleteBuffers(1, &boardVBO);

	//delete VAO
	glDeleteVertexArrays(1, &VAO);
}


void Engine::Clear()
{
	glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Engine::InitBoard(int row, int col)
{
	if (row < 1 || row > chess.MAXR || col < 1 || col > chess.MAXC)
	{
		std::cout << "错误的行列数" << std::endl;
		return;
	}
	chess.row = row;
	chess.col = col;

	// 设置缩放参数
	scaleNum = row > col ? row : col;

	// 初始化棋盘显示位置
	for (int i = 0; i < chess.MAXR + 2; i++)
		for (int j = 0; j < chess.MAXC + 2; j++)
		{
			chess.board[i][j].position = glm::vec3((float)i, (float)j, 0.0f);

			chess.board[i][j].position.x = chess.board[i][j].position.x * 2 / (scaleNum +1) - 1.0f;
			chess.board[i][j].position.y = chess.board[i][j].position.y * 2 / (scaleNum +1) - 1.0f;
		}

	// 初始化棋盘内容
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
		{
			chess.board[i][j].type = 0;
			chess.board[i][j].isMi = false;
		}
}


void Engine::DrawBoard(glm::vec2 sign)
{
	
	//根据棋盘行列数设置缩放参数
	glm::vec3 scale = glm::vec3(1.0f / (scaleNum +1), 1.0f / (scaleNum+1), 0.0f);

	glUseProgram(boardShader);
	glBindVertexArray(VAO);
	// 画棋盘
	glm::mat4 model = glm::mat4(1.0f);
	for (int i = 1; i <= chess.col; i++)
	{
		for (int j = 1; j <= chess.row; j++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, chess.board[i][j].position);
			model = glm::scale(model, scale);
			// 不是无效点
			if (chess.board[i][j].type != -1)
			{
				//画直线
				// 判断右方、上方是否是有效格点
				if (chess.board[i + 1][j].type != -1)
					DrawThing(line, model, 0.0f, glm::vec3(2.0f, 1.0f, 0.0f));
				if (chess.board[i][j + 1].type != -1)
					DrawThing(line, model, 90.0f, glm::vec3(1.0f, 2.0f, 0.0f));

				//画斜线
				if (chess.board[i][j].isMi)
				{
					// 判断右上、左上、左下、右下方是否为有效格点
					if (chess.board[i + 1][j + 1].type >= 0)
						DrawThing(slash, model, 0.0f, glm::vec3(2.0f, 2.0f, 0.0f));
					if (chess.board[i - 1][j + 1].type >= 0)
						DrawThing(slash, model, 90.0f, glm::vec3(2.0f, 2.0f, 0.0f));
					if (chess.board[i - 1][j - 1].type >= 0)
						DrawThing(slash, model, 180.0f, glm::vec3(2.0f, 2.0f, 0.0f));
					if (chess.board[i + 1][j - 1].type >= 0)
						DrawThing(slash, model, 270.0f, glm::vec3(2.0f, 2.0f, 0.0f));
				}
			}
		}
	}
	// 画棋子

	for (int i = 1; i <= chess.col; i++)
	{
		for (int j = 1; j <= chess.row; j++)
		{
			// 一号
			if (chess.board[i][j].type == 1)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, chess.board[i][j].position);
				model = glm::scale(model, scale);
				DrawThing(texChessman1, model);
			}
			//二号
			else if (chess.board[i][j].type == 2)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, chess.board[i][j].position);
				model = glm::scale(model, scale);
				DrawThing(texChessman2, model);
			}

			// 标记
			if (i == (int)sign.x / SPEED && j == (int)sign.y / SPEED)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, chess.board[i][j].position);
				model = glm::scale(model, scale);
				DrawThing(ring, model);
			}
		}
	}


}

void Engine::SetRule(int turn, int playMod, int howToChangeTurn, int mod, int okMod, int ifEndMod, int series, bool seriesMod, int endNum)
{
	chess.turn = turn;
	chess.playMod = playMod;
	chess.howToChangeTurn = howToChangeTurn;
	chess.placeMod = mod;
	chess.placeOkMod = okMod;
	chess.ifEndMod = ifEndMod;

	chess.series = series;
	chess.seriesMod = seriesMod;
	chess.endNum = endNum;
}

void Engine::DrawThing(unsigned int texture, glm::mat4 model, float angle, glm::vec3 scale )
{
	SetUniform1i(boardShader, "tex", texture);
	model = glm::scale(model, scale);
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	SetUniformMat4(boardShader, "model", model);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}



void Engine::SetUniform1i(unsigned int shader, const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(shader, name.c_str()), value);
}

void Engine::SetUniform2v(unsigned int shader, const std::string& name, glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
}

void Engine::SetUniform2v(unsigned int shader, const std::string& name, float x, float y)
{
	glUniform2f(glGetUniformLocation(shader, name.c_str()), x, y);
}

void Engine::SetUniform3v(unsigned int shader, const std::string& name, glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
}

void Engine::SetUniform3v(unsigned int shader, const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(shader, name.c_str()), x, y, z);
}

void Engine::SetUniformMat4(unsigned int shader, const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Engine::CreateVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

}


void Engine::CreateBoardVBO()
{
	//0坐标点在中心
	float vertices[] =
	{
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};// --- pos ---- --texcoord--

	//0坐标点在左下
	//float vertices[] =
	//{
	//	 0.0f, 0.0f, 0.0f, 0.0f,
	//	 0.0f, 2.0f, 0.0f, 1.0f,
	//	 2.0f, 2.0f, 1.0f, 1.0f,
	//	 2.0f, 2.0f, 1.0f, 1.0f,
	//	 2.0f, 0.0f, 1.0f, 0.0f,
	//	 0.0f, 0.0f, 0.0f, 0.0f
	//};// --- pos ---- --texCoord--

	glGenBuffers(1, &boardVBO);

	glBindBuffer(GL_ARRAY_BUFFER, boardVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)8);
	glEnableVertexAttribArray(1);
	

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Engine::Texture(const char* path, unsigned int& texture)
{
	//上下翻转图片
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = nullptr;
	int width, height, nrChannels;
	pixels = stbi_load(path, &width, &height, &nrChannels, 4);
	// req_comp
	// 1 灰度图
	// 2 灰度图加透明度
	// 3 红绿蓝 RGB 三色图
	// 4 红绿蓝加透明度 RGBA 图

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (pixels)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "加载纹理:\"" << path << "\"失败！" << std::endl;
	}
	stbi_image_free(pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Engine::BindTexture(unsigned int texture, unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}

unsigned int Engine::CreateShader(const char* vertexPath, const char* fragmentPath)
{
	unsigned int shader;

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	// 创建vertexShader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");
	// 创建fragmentShader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");
	// 创建Shader
	shader = glCreateProgram();
	glAttachShader(shader, vertex);
	glAttachShader(shader, fragment);
	glLinkProgram(shader);
	CheckCompileErrors(shader, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return shader;
}

void Engine::CheckCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << std::endl;
			std::cout << infoLog << std::endl;
			std::cout << "-- --------------------------------------------------- --" << std::endl;
		}
	}
	else if (type == "PROGRAM")
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << std::endl;
			std::cout << infoLog << std::endl;
			std::cout << "-- --------------------------------------------------- --" << std::endl;
		}
	}
	else
	{
		std::cout << "着色器种类错误" << std::endl;
	}
}