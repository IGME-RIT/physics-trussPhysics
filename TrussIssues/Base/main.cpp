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
* References:
*	PhysicsTimestep by Brockton Roth
*	Base by Srinivasan Thiagarajan
*/
#include <string>
#include <cmath>
#include <glew\glew.h>
#include "glfw\glfw3.h"
#include "ShaderLoader.h"
#include "Mesh.h"
#include "MeshEBuf.h"

#define FAIL do{ getchar(); exit(EXIT_FAILURE); }while(false)

// Global data members
#pragma region Base_data
// This is your reference to your shader program.
// This will be assigned with glCreateProgram().
// This program will run on your GPU.
GLuint program;

// These are your references to your actual compiled shaders
GLuint vertex_shader;
GLuint fragment_shader;

/*std::vector<glm::vec3> vertsTri = {
	glm::vec3(-0.25f, -0.25f, 0.0f),
	glm::vec3(0.25f, -0.25f, 0.0f),
	glm::vec3(0.0f, 0.25f, 0.0f)
};*/

std::vector<glm::vec3> vertsSq = {
	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(-0.5f, 0.5f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f),
	glm::vec3(0.5f, 0.5f, 0.0f)
};

std::vector<GLuint> ebufSq = { 0, 1, 2, 3 };

std::vector<glm::vec3> vertsTruss = {
	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(-0.5f, 0.5f, 0.0f),
	glm::vec3(-0.25f, 0.5f, 0.0f),
	glm::vec3(0.0f, -0.5f, 0.0f),
	glm::vec3(0.25f, 0.5f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f),
	glm::vec3(0.5f, 0.5f, 0.0f),
};

std::vector<GLuint> ebufTruss = { 1, 2, 0,
								  3, 2, 4,
								  3, 5, 6,
								  4 };

std::vector<glm::vec3> vertsTruss2 = {
	glm::vec3(-0.5f, -0.25f, 0.0f),	//0 - 0
	glm::vec3(-0.25f, 0.25f, 0.0f),	//2 - 1
	glm::vec3(0.0f, -0.25f, 0.0f),	//3 - 2
	glm::vec3(0.25f, 0.25f, 0.0f),	//4 - 3
	glm::vec3(0.5f, -0.25f, 0.0f),	//5 - 4
};

std::vector<glm::vec3> vertsTrussDeformed = {
	glm::vec3(-0.5f-0.032f+0.036f, -0.25f+0.063f, 0.0f),	//0 - 0
	glm::vec3(-0.25f+0.032f-0.036f, 0.25f, 0.0f),	//2 - 1
	glm::vec3(0.0f, -0.25f-0.063f, 0.0f),	//3 - 2
	glm::vec3(0.25f-0.032f+0.036f, 0.25f, 0.0f),	//4 - 3
	glm::vec3(0.5f+0.032f-0.036f, -0.25f+0.063f, 0.0f),	//5 - 4
};

std::vector<GLuint> ebufTruss2 = { 0, 1, 2,
								   1, 2, 3,
								   2, 3, 4};

/*std::vector<glm::vec3> vertsSquare = {
	glm::vec3(-0.500000, -0.500000, 0.500000),
	glm::vec3(0.500000, -0.500000, 0.500000),
	glm::vec3(-0.500000, 0.500000, 0.500000),
	glm::vec3(0.500000, 0.500000, 0.500000),
	glm::vec3(-0.500000, 0.500000, -0.500000),
	glm::vec3(0.500000, 0.500000, -0.500000),
	glm::vec3(-0.500000, -0.500000, -0.500000),
	glm::vec3(0.500000, -0.500000, -0.500000),
};

std::vector<GLuint> ebuf = { 1, 2, 3, 3, 2, 4,
							 3, 4, 5, 5, 4, 6,
							 5, 6, 7, 7, 6, 8,
							 7, 8, 1, 1, 8, 2,
							 2, 8, 4, 4, 8, 6,
							 7, 1, 5, 5, 1, 3 };*/

//Adjacency Matrix
std::vector<std::vector<GLuint>> trussEdges = { { 1, 2, 3 },
												{ 0, 2 },
												{ 0, 1, 3, 4 },
												{ 0, 2, 4, 5 },
												{ 2, 3, 5, 6 },
												{ 3, 4, 6 },
												{ 4, 5 } };



//The tblock to fall on the Truss
MeshEBuf* block;
//The truss simulating Truss physics
MeshEBuf* truss;
MeshEBuf* trussDeformed;

std::vector<std::vector<GLfloat>> trussAng;

// Reference to the window object being created by GLFW.
GLFWwindow* window;
#pragma endregion

#pragma region Variables for FPS and Physics Timestep Calculations
int frame = 0;
double time = 0;
double prevtime = 0;
double accumulator = 0.0;
int fps = 0;
double fpsTime = 0.0;
//The number of milliseconds for physics to update
double physicsStep = 0.012;
#pragma endregion

#pragma region Function Declarations
void init();
void update(float dt);
void checkTime();
void renderScene();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
#pragma endregion

void main()
{
	/*std::vector<std::vector<GLuint>> sqEdges = { { 1, 2 },
												 { 0, 2, 3 },
												 { 0, 1, 3 },
												 { 1, 2 } };*/
	//A bunch of arrays of angles in radians
	//figured out with repect to the x-axis
	trussAng = std::vector<std::vector<GLfloat>>(trussEdges.size());
	for (int i = 0; i < trussEdges.size(); i++)
	{
		for (int j = 0; j < trussEdges[i].size(); j++)
		{
			glm::vec3 edge = vertsTruss[i] - vertsTruss[trussEdges[i][j]];
			float dotF = glm::dot(glm::vec3(1, 0, 0), edge);
			float magI = 1;
			float magJ = glm::length(edge);
			float magITimeJ = magI * magJ;
			float totCos = dotF/magITimeJ;
			
			trussAng[i].push_back(acos(totCos));
		}
	}

	/*for (int i = 0; i < ebuf.size(); i++)
	{
		ebuf[i] -= 1;
	}*/

	if (!glfwInit())
		FAIL;

	// Creates a window given (width, height, title, monitorPtr, windowPtr).
	// Don't worry about the last two, as they have to do with controlling which monitor to display on and having a reference to other windows. Leaving them as nullptr is fine.
	window = glfwCreateWindow(800, 800, "Truss Issues", nullptr, nullptr);

	// Makes the OpenGL context current for the created window.
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	// Sets the number of screen updates to wait before swapping the buffers.
	// Setting this to zero will disable VSync, which allows us to actually get a read on our FPS. Otherwise we'd be consistently getting 60FPS or lower, 
	// since it would match our FPS to the screen refresh rate.
	// Set to 1 to enable VSync.
	glfwSwapInterval(0);

	// Initializes most things needed before the main loop
	
	init();	

	// Sends the funtion as a funtion pointer along with the window to which it should be applied to.
	//glfwSetKeyCallback(window, key_callback);

	// Enter the main loop.
	while (!glfwWindowShouldClose(window))
	{
		//deals with the physics timestep
		checkTime();

		// Call the render function.
		renderScene();

		// Swaps the back buffer to the front buffer
		// Remember, you're rendering to the back buffer, then once rendering is complete, you're moving the back buffer to the front so it can be displayed.
		glfwSwapBuffers(window);

		//add one to the frame counter
		frame++;

		// Checks to see if any events are pending and then processes them.
		glfwPollEvents();
	}

	// After the program is over, cleanup your data!
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteProgram(program);
	
	// Frees up GLFW memory
	glfwTerminate();
}

// Initialization code
void init()
{
#pragma region Initialize Glew and Shaders
	// Initializes the glew library
	glewInit();

	// Enables the depth test, which you will want in most cases. You can disable this in the render loop if you need to.
	glEnable(GL_DEPTH_TEST);

	// Read in the shader code from a file.
	std::string vertShader = readShader("VertexShader.glsl");
	std::string fragShader = readShader("FragmentShader.glsl");

	// createShader consolidates all of the shader compilation code
	vertex_shader = createShader(vertShader, GL_VERTEX_SHADER);
	fragment_shader = createShader(fragShader, GL_FRAGMENT_SHADER);

	// A shader is a program that runs on your GPU instead of your CPU. In this sense, OpenGL refers to your groups of shaders as "programs".
	// Using glCreateProgram creates a shader program and returns a GLuint reference to it.
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);		// This attaches our vertex shader to our program.
	glAttachShader(program, fragment_shader);	// This attaches our fragment shader to our program.

	// This links the program, using the vertex and fragment shaders to create executables to run on the GPU.
	glLinkProgram(program);
	// End of shader and program creation

	// This gets us a reference to the uniform variable in the vertex shader, which is called "MVP".
	// We're using this variable as a 4x4 transformation matrix
	// Only 2 parameters required: A reference to the shader program and the name of the uniform variable within the shader code.
	//mvpLoc = glGetUniformLocation(program, "MVP");

		// This is not necessary, but I prefer to handle my vertices in the clockwise order. glFrontFace defines which face of the triangles you're drawing is the front.
		// Essentially, if you draw your vertices in counter-clockwise order, by default (in OpenGL) the front face will be facing you/the screen. If you draw them clockwise, the front face 
		// will face away from you. By passing in GL_CW to this function, we are saying the opposite, and now the front face will face you if you draw in the clockwise order.
		// If you don't use this, just reverse the order of the vertices in your array when you define them so that you draw the points in a counter-clockwise order.
		//glFrontFace(GL_CW);

	// This is also not necessary, but more efficient and is generally good practice. By default, OpenGL will render both sides of a triangle that you draw. By enabling GL_CULL_FACE, 
	// we are telling OpenGL to only render the front face. This means that if you rotated the triangle over the X-axis, you wouldn't see the other side of the triangle as it rotated.
	glEnable(GL_CULL_FACE);

	// Determines the interpretation of polygons for rasterization. The first parameter, face, determines which polygons the mode applies to.
	// The face can be either GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
	// The mode determines how the polygons will be rasterized. GL_POINT will draw points at each vertex, GL_LINE will draw lines between the vertices, and 
	// GL_FILL will fill the area inside those lines.
	glPolygonMode(GL_FRONT, GL_FILL);

#pragma endregion

	//block = new MeshEBuf(&vertsSq, &ebufSq, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 0.25, program);
	//truss = new MeshEBuf(&vertsTruss, &ebufTruss, &trussAng, &trussEdges, glm::vec3(0.0f, -0.5f, 0), glm::vec3(0, 0, 0), 0.35, program);
	truss = new MeshEBuf(&vertsTruss2, &ebufTruss2, nullptr, nullptr, glm::vec3(0.0f, -0.5f, 0), glm::vec3(0, 0, 0), 0.35, program);
	trussDeformed = new MeshEBuf(&vertsTrussDeformed, &ebufTruss2, nullptr, nullptr, glm::vec3(0.0f, -0.5f, 0), glm::vec3(0, 0, 0), 0.35, program);
}

// Functions called between every frame. game logic
#pragma region util_functions
// This runs once every physics timestep.
void update(float dt)
{
	//std::cout << "Block: " << block->transform.position.x << ", " << block->transform.position.y << ", " << block->transform.position.z << std::endl;
	//std::cout << "Truss: " << truss->transform.position.x << ", " << truss->transform.position.y << ", " << truss->transform.position.z << std::endl;
	//if (!(block->transform.position.y < (truss->transform.position.y + 0.22f) && block->transform.position.y > (truss->transform.position.y - 0.22f)))
	//{
	//	//std::cout << "WAT!!" << std::endl;
	//	block->update(dt);
	//}
	//truss->update(dt);
}

// This function runs every frame
void renderScene()
{
	// Clear the color buffer and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Clear the screen to white
	glClearColor(0.0, 0.0, 1.0, 1.0);

	//block->render(glm::vec3(1, 0, 0), GL_TRIANGLE_STRIP);
	truss->render(glm::vec3(0, 1, 0), GL_TRIANGLE_STRIP);
	trussDeformed->render(glm::vec3(0, 1, 0), GL_TRIANGLE_STRIP);
}

//Code taken from https://github.com/IGME-RIT/physics-physicsTimestep
void checkTime()
{
	//current time
	time = glfwGetTime();
	//time since last update
	double dt = time - prevtime;

	//if more time has passed than the physics timestep
	if (dt > physicsStep)
	{
		//Calculate FPS:
			//number of frames(frame) since the last time fps was calculated and divide by time
			//last time fps was calculated = (time - fpsTime)
		if (time - fpsTime > 1.0)
		{
			fps = frame / (time - fpsTime);
			//Set fpsTime to equal time, so we have a reference for when fps was calculated
			fpsTime = time;
			//reset frame counter
			frame = 0;
			//turns fps into a string
			std::string s = "FPS: " + std::to_string(fps);
			//changes the window title to display the current fps
			glfwSetWindowTitle(window, s.c_str());
		}

		//gives a reference for when the last physics timestep was run
		prevtime = time;

		//limit dt, so that if there is something happening a bunch of times while the player can't see
			//would freeze before
		if (dt > 0.25)
			dt = 0.25;

		//accumulator keeps track of the amount of time that needs to be updated based on dt
			//won't update at dt intervals, but at physicsStep intervals 
		accumulator += dt;

		//runs update until the accumulator no longer has any time left
		//or until the time left is less than physics step,
			//at which point it saves the leftover time for the next checkTime() call
		while (accumulator >= physicsStep)
		{
			// Call to update() which will update the gameobjects.
			update(physicsStep);
			accumulator -= physicsStep;
		}
	}
}


// This function is used to handle key inputs.
// It is a callback funciton. i.e. glfw takes the pointer to this function (via function pointer) and calls this function every time a key is pressed in the during event polling.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		glfwDestroyWindow(window);
}