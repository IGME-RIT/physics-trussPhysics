#include "MeshEBuf.h"

MeshEBuf::MeshEBuf(std::vector<glm::vec3>* verts, std::vector<GLuint>* ebuf, glm::vec3 position, glm::vec3 velocity, float scale, GLuint program)
{
	//this->numVerts = verts->size();
	this->numElements = ebuf->size();
	this->program = program;
	this->verts = verts;

	transform.mass = 2.0f;
	transform.position = position;
	transform.velocity = velocity;
	//transform.acceleration = glm::vec3(0, 0, 0);
	//transform.acceleration = glm::vec3(0, -0.25f, 0);
	transform.scale = glm::vec3(scale, scale, 1);

	colorLoc = glGetUniformLocation(program, "color");
	mvpLoc = glGetUniformLocation(program, "MVP");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * verts->size() * 3, &(*verts)[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebuf->size() * sizeof(GLuint), &(*ebuf)[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

MeshEBuf::MeshEBuf(std::vector<glm::vec3>* verts, std::vector<GLuint>* ebuf, std::vector<std::vector<GLfloat>>* trussAng,
				   std::vector<std::vector<GLuint>>* trussEdges, glm::vec3 position, glm::vec3 velocity, float scale, GLuint program)
{
	//this->numVerts = verts->size();
	this->numElements = ebuf->size();
	this->program = program;
	this->verts = verts;
	this->trussAng = trussAng;
	this->trussEdges = trussEdges;

	transform.mass = 2.0f;
	transform.position = position;
	transform.velocity = velocity;
	//transform.acceleration = glm::vec3(0, 0, 0);
	//transform.acceleration = glm::vec3(0, -0.25f, 0);
	transform.scale = glm::vec3(scale, scale, 1);

	colorLoc = glGetUniformLocation(program, "color");
	mvpLoc = glGetUniformLocation(program, "MVP");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * verts->size() * 3, &(*verts)[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebuf->size() * sizeof(GLuint), &(*ebuf)[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

MeshEBuf::MeshEBuf()
{
	memset(&transform, 0, sizeof(Transform));
	numVerts = 0;
	program = 0;
	vao = 0;
	vbo = 0;
	ebo = 0;
	trussAng = nullptr;
	trussEdges = nullptr;
}

MeshEBuf::~MeshEBuf()
{
	// After the object is done, cleanup your data!
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void MeshEBuf::update(float dt)
{
	if (trussAng != nullptr)
	{
		//s
	}

	transform.acceleration = glm::vec3(0, -1, 0)/transform.mass;
	transform.velocity += transform.acceleration * dt;
	transform.position += transform.velocity * dt;

	if (transform.position.x < -1)
		transform.position.x = 1;
	if (transform.position.x > 1)
		transform.position.x = -1;
	if (transform.position.y < -1)
		transform.position.y = -1;
	/*if (transform.position.y > 1)
	transform.position.y = 1;*/
}

void MeshEBuf::render(glm::vec3 color, GLenum drawType)
{
	// Tell OpenGL to use the shader program you've created.
	glUseProgram(program);

	//We aren't creating a camera, so I only need the model matrix
	glm::mat4 modelMatrix = glm::translate(transform.position) * glm::scale(transform.scale) * glm::rotate(3.0f, glm::vec3(0, 1, 0));
	glProgramUniformMatrix4fv(program, mvpLoc, 1, GL_FALSE, &modelMatrix[0][0]);
	glProgramUniform4f(program, colorLoc, color.r, color.g, color.b, 1.0f);

	glBindVertexArray(vao);
	glDrawElements(drawType, numElements, GL_UNSIGNED_INT, (void*)0);
	//glDrawArrays(drawType, 0, verts->size()); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glBindVertexArray(0);
}