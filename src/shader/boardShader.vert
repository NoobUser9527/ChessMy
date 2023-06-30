#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;



out vec2 vTexCoord;

uniform mat4 model;


void main()
{
	
	vTexCoord = aTexCoord;
	gl_Position =  model * vec4(aPos, 0.0, 1.0);
	
};