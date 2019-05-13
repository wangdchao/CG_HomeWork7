#pragma once
#include "allheader.h"
extern  GLfloat vertices[] = {
	// Back face
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // Bottom-left
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // top-right
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // bottom-right         
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  // top-right
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  // bottom-left
	-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,// top-left
	// Front face
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // bottom-left
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // bottom-right
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // top-right
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // top-right
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // top-left
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // bottom-left
	// Left face
	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, // top-right
	-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // top-left
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  // bottom-left
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // bottom-left
	-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  // bottom-right
	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, // top-right
	// Right face
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // top-left
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // bottom-right
	0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // top-right         
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  // bottom-right
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  // top-left
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // bottom-left     
	// Bottom face
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // top-right
	0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,// top-left
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,// bottom-left
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, // bottom-left
	-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, // bottom-right
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  // top-right
	// Top face
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,// top-left
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,// bottom-right
	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // top-right     
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // bottom-right
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,// top-left
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // bottom-left        
};

extern  GLfloat planeVertices[] = {
	25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 
	-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f,
	-25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f,

	25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f,
	25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f,
	-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f,
};