/*
* Title: TrussCollisionTrial
* File Name: Main.cpp
* Copyright © 2015
* Original authors: Sean Connolly
* Written under the supervision of David I. Schwartz, Ph.D., and
* supported by a professional development seed grant from the B. Thomas
* Golisano College of Computing & Information Sciences
* (https://www.rit.edu/gccis) at the Rochester Institute of Technology.
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or (at
* your option) any later version.
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* Notes:
*	Use Base and PhysicsTimestep as the base code for this project.
*	Changed Base so that a lot of the OpenGL code is inside the Mesh class.
*	This is so that it isn't in the way to understanding the point of this tutorial.
* Description:
*	This is a trial of deformable bodies by treating the game objects as Trusses in a statically determinate method of joints problem.
*
*	A simple program with a static triangle that falls on a Warren Bridge Truss. (Might also try a Pratt or Howe Truss)
*	This will be using a simple Planar Truss. Only do work on the joints.
*	Next step is to treat 3D objects as a combination of Space Frame Trusses.
*		Each 3D object will be composed of 3D tetrahedrons with the triangles being the base and the position being the tip.
*			Might not even be necessary, since it deals with working on the joints and treating the problem as statically determinate.
*/
#include "Mesh.h"

Mesh::Mesh(std::vector<glm::vec3>* verts, glm::vec3 position, glm::vec3 velocity, float scale, GLuint program)
{
	this->numVerts = verts->size();
	this->program = program;
	this->verts = verts;

	transform.position = position;
	transform.velocity = velocity;
	transform.acceleration = glm::vec3(0, -0.25f, 0);
	transform.scale = glm::vec3(scale, scale, 1);

	colorLoc = glGetUniformLocation(program, "color");
	mvpLoc = glGetUniformLocation(program, "MVP");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * numVerts * 3, &(*verts)[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

Mesh::Mesh()
{
	numVerts = 0;
	program = 0;
	vao = 0;
	vbo = 0;
}

Mesh::~Mesh()
{
	// After the object is done, cleanup your data!
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Mesh::update(float dt)
{
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

void Mesh::render(glm::vec3 color, GLenum drawType)
{
	// Tell OpenGL to use the shader program you've created.
	glUseProgram(program);

	//We aren't creating a camera, so I only need the model matrix

	glm::mat4 modelMatrix = glm::translate(transform.position) * glm::scale(transform.scale) * glm::rotate(0.0f, glm::vec3(0, 0, 1));
	glProgramUniformMatrix4fv(program, mvpLoc, 1, GL_FALSE, &modelMatrix[0][0]);
	glProgramUniform4f(program, colorLoc, color.r, color.g, color.b, 1.0f);

	glBindVertexArray(vao);
	glDrawArrays(drawType, 0, verts->size()); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glBindVertexArray(0);
}