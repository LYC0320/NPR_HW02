#include <iostream>
#include <vector>
#include <math.h>
#include <GLUT/glut.h>
#include <GLM/glm/glm.hpp>
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <GLM/glm/gtc/type_ptr.hpp>

#include "MyTools.h"

using namespace glm;
using namespace std;

bool first = true;

void resize(int w, int h) 
{
	// we ignore the params and do:
	glutReshapeWindow(width, height);
}

void Display(void)
{
	glClearColor(139.0f / 255.0f, 139.0f / 255.0f, 122.0f / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (first)
	{
		genHorizontalPoints(7);
		genVerticalPoints(3);

		completeGraph();

		generateQuads();

		offsetInternalPoints();

		scaleQuads();
		drawAllQuads();
		drawQuadBoundary();
		save_screen_bmp("stone.bmp");
		save_screen_ppm("stone.ppm");

		save_screen_bmp("../../Rendering/Resources/stone.bmp");
		save_screen_ppm("../../Rendering/Resources/stone.ppm");

		first = false;
	}
	else
	{
		drawAllQuads();
		drawQuadBoundary();
	}
	glutReshapeFunc(resize);
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Layout Creation");
	glutDisplayFunc(Display);
	
	glutMainLoop();

	return 0;
}