#pragma once
#include <glew\glew.h>
//#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <vector>
#include "Structs.h"

class MeshEBuf
{
public:
	Transform transform;
	MeshEBuf(std::vector<glm::vec3>* verts, std::vector<GLuint>* ebuf, glm::vec3 position, glm::vec3 velocity, float scale, GLuint program);
	MeshEBuf(std::vector<glm::vec3>* verts, std::vector<GLuint>* ebuf, std::vector<std::vector<GLfloat>>* trussAng,
			 std::vector<std::vector<GLuint>>* trussEdges, glm::vec3 position, glm::vec3 velocity, float scale, GLuint program);
	MeshEBuf();
	~MeshEBuf();

	void update(float dt);
	void render(glm::vec3 color, GLenum drawType);

private:
	unsigned int numVerts;
	unsigned int numElements;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint program;
	//Keeping this, so that I can mess with them on the CPU side
	//Get this to work and then try it on a compute shader
	std::vector<glm::vec3>* verts;
	std::vector<std::vector<GLfloat>>* trussAng;
	std::vector<std::vector<GLuint>>* trussEdges;
	//int numVerts;
	// This is a reference to your uniform MVP matrix in your vertex shader
	GLuint mvpLoc;
	// This is a reference to your uniform color in your vertex shader
	GLuint colorLoc;
};

