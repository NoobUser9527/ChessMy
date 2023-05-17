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

	chessManShader = CreateShader("src\\shader\\chessManShader.vert", "src\\shader\\chessManShader.frag");
	boardShader = CreateShader("src\\shader\\boardShader.vert", "src\\shader\\boardShader.frag");

	CreateVAO();
	CreateBoardVBO();

	
}

Engine::~Engine()
{
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);

	glDeleteProgram(boardShader);
	glDeleteProgram(chessManShader);

	glDeleteBuffers(1, &boardVBO);
	glDeleteBuffers(1, &chessManVBO);

	glDeleteVertexArrays(1, &VAO);
}

void Engine::ChessManTexture(const char* path1, const char* path2)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = nullptr;
	int width = 0, height = 0, nrChannels = 0;

	pixels = stbi_load(path1, &width, &height, &nrChannels, 3);
	// req_comp
	// 1 灰度图
	// 2 灰度图加透明度
	// 3 红绿蓝 RGB 三色图
	// 4 红绿蓝加透明度 RGBA 图

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (pixels)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "加载纹理:\"" << path1 << "\"失败！" << std::endl;
	}
	stbi_image_free(pixels);


	pixels = stbi_load(path2, &width, &height, &nrChannels, 3);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (pixels)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "加载纹理:\"" << path2 << "\"失败！" << std::endl;
	}
	stbi_image_free(pixels);


	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texture2);

}

void Engine::Bind()
{
	glUseProgram(chessManShader);
	glBindVertexArray(VAO);
}

void Engine::Unbind()
{
}

void Engine::DrawChessMan()
{
	glUseProgram(chessManShader);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Engine::DrawBoard()
{
	glm::vec3 trans = glm::vec3(1.0f / 22.0f, 1.0f / 22.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f / (row + 2.0f), 1.0f / (col + 2.0f), 0.0f);
	//scale = trans;
	for (int i = 0; i < row+2; i++)
	{
		for (int j = 0; j < col+2; j++)
		{
			if((i == 0 && i == row + 1) || (j == 0 && j == col + 1))
				SetUniformVec3(boardShader, "color", 1.0f, 0.0f, 0.0f);

			if ((i + j) % 2 == 0)
				SetUniformVec3(boardShader, "color", boardColor1);
			else
				SetUniformVec3(boardShader, "color", boardColor2);





			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, chessManPosition[i][j]);
			model = glm::scale(model, scale);




			SetUniformMat4(boardShader, "model", model);





			glUseProgram(boardShader);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);



		}
	}
}

void Engine::AdjustBoard()
{
	
	for (int i = 0; i < MAXR; i++)
	{
		for (int j = 0; j < MAXC; j++)
		{
			chessManPosition[i][j] = glm::vec3(i, j, 0);
		}
	}

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			//                         --------- 将棋盘坐标调整为屏幕坐标  -------------------------  
			chessManPosition[i][j].x = chessManPosition[i][j].x * 2.0f / (row + 2.0f) - 1.0f + 1.0f / (row + 2.0f);
			chessManPosition[i][j].y = chessManPosition[i][j].y * 2.0f / (col + 2.0f) - 1.0f + 1.0f / (col + 2.0f);
		}
	}
}

void Engine::SetUniformVec2(unsigned int shader, const std::string& name, glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
}

void Engine::SetUniformVec2(unsigned int shader, const std::string& name, float x, float y)
{
	glUniform2f(glGetUniformLocation(shader, name.c_str()), x, y);
}

void Engine::SetUniformVec3(unsigned int shader, const std::string& name, glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
}

void Engine::SetUniformVec3(unsigned int shader, const std::string& name, float x, float y, float z)
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

void Engine::CreateChessManVBO()
{
	float vertices[] =
	{
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};// --- pos ---- --texCoord--



	glGenBuffers(1, &chessManVBO);

	glBindBuffer(GL_ARRAY_BUFFER, chessManVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)8);
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Engine::CreateBoardVBO()
{
	//坐标点在中心
	float vertices[] =
	{
		 1.0f,  1.0f,
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f
	};// --- pos ---- --texCoord--

	//坐标点在左下
	//float vertices[] =
	//{
	//	 0.0f, 0.0f,
	//	 0.0f, 2.0f,
	//	 2.0f, 2.0f,
	//	 2.0f, 2.0f,
	//	 2.0f, 0.0f,
	//	 0.0f, 0.0f
	//};// --- pos ---- --texCoord--



	glGenBuffers(1, &boardVBO);

	glBindBuffer(GL_ARRAY_BUFFER, boardVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

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