#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Chess.h"

class Engine : public Chess
{
private:
	unsigned int VAO;
	unsigned int boardVBO;
	unsigned int chessManVBO;


	unsigned int texture1;
	unsigned int texture2;


public:
	unsigned int chessManShader;
	unsigned int boardShader;

	glm::vec3 boardColor1 = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 boardColor2 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 chessManPosition[MAXR][MAXC];

	glm::vec3 position;

public:
	Engine();
	~Engine();

	void ChessManTexture(const char* path1, const char* path2);

	void Bind();
	void Unbind();

	void DrawChessMan();
	void DrawBoard();

	void AdjustBoard();

	void SetUniformVec2(unsigned int shader, const std::string& name, glm::vec2& value);
	void SetUniformVec2(unsigned int shader, const std::string& name, float x, float y);
	void SetUniformVec3(unsigned int shader, const std::string& name, glm::vec3& value);
	void SetUniformVec3(unsigned int shader, const std::string& name, float x, float y, float z);
	void SetUniformMat4(unsigned int shader, const std::string& name, const glm::mat4& matrix);

private:

	void CreateVAO();
	void CreateChessManVBO();
	void CreateBoardVBO();
	unsigned int CreateShader(const char* vertexPath, const char* fragmentPath);

	void CheckCompileErrors(unsigned int shader, std::string type);

};

