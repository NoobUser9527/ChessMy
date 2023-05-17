#version 330 core
layout (location = 0) in vec2 aPos;


out vec2 FragPos; 
out vec3 mColor;

uniform mat4 model;
uniform vec3 color;

void main()
{
	mColor = color;

	gl_Position =  model * vec4(aPos, 0.0, 1.0);
	
};