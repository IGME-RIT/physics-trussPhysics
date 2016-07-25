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
#pragma once
#include <iostream>
#include <fstream>
#include "glew\glew.h"

std::string readShader(std::string fileName);
GLuint createShader(std::string sourceCode, GLenum shaderType);
