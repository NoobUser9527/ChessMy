#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Chess.h"

const int SPEED = 12;

class Engine
{
private:
	unsigned int VAO;
	unsigned int boardVBO;



	unsigned int texChessman1;
	unsigned int texChessman2;

	unsigned int line;
	unsigned int slash;

	unsigned int ring;
	
	int scaleNum;
public:

	Chess chess;

	unsigned int boardShader;

	glm::vec3 bgColor =glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 boardColor1 = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 boardColor2 = glm::vec3(0.0f, 0.0f, 0.0f);




public:
	Engine();
	~Engine();

	void Clear();


	void InitBoard(int row, int col);
	void SetRule(int turn, int playMod, int howToChangeTurn, int placeMod, int placeOkMod, int ifEndMod, int series = 0, bool seriesMod = false, int endNum = 0);
	void DrawBoard(glm::vec2 sign);

private:
	void DrawThing(unsigned int texture, glm::mat4 model, float angle = 0.0f, glm::vec3 scale = glm::vec3(1.0f));

public:
	void SetUniform1i(unsigned int shader, const std::string& name, int value);
	void SetUniform2v(unsigned int shader, const std::string& name, glm::vec2& value);
	void SetUniform2v(unsigned int shader, const std::string& name, float x, float y);
	void SetUniform3v(unsigned int shader, const std::string& name, glm::vec3& value);
	void SetUniform3v(unsigned int shader, const std::string& name, float x, float y, float z);
	void SetUniformMat4(unsigned int shader, const std::string& name, const glm::mat4& matrix);

private:
	void CreateVAO();
	void CreateBoardVBO();

	void Texture(const char* path, unsigned int &texture);
	void BindTexture(unsigned int texture, unsigned int slot);


	unsigned int CreateShader(const char* vertexPath, const char* fragmentPath);

	void CheckCompileErrors(unsigned int shader, std::string type);

};

