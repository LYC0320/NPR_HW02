#pragma once

#ifndef MyTools_H
#define MyTools_H

#include <GLM/glm/glm.hpp>
#include <GLUT/glut.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace glm;
using namespace std;

const float width = 600, height = 600;

vector<vec2> horizontalPoints, verticalPoints;
vector<vector<vec2>> newQuadPoints;
int verticalBaseCount;

unordered_map<int, vector<int>> g, q;

float maxDisturbanceX = 3.0f, maxDisturbanceY = 3.0f;
float scaleCoefficient = 0.15f, disturbanceCoefficient = 0.25f;

float myNormalize(float f, float l)
{
	return f / l * 2.0f - 1.0f;
}

float mylength(vec2 a, vec2 b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

void myDrawLine(GLfloat size, vec2 pos1, vec2 pos2)
{
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth((GLfloat)size);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(pos1.x, pos1.y);
	glVertex2f(pos2.x, pos2.y);
	glEnd();
	glDisable(GL_LINE_SMOOTH);
}

void genHorizontalPoints(int count)
{
	verticalBaseCount = count + 1;

	horizontalPoints.push_back(vec2(-1.0f, 1.0f));
	horizontalPoints.push_back(vec2(1.0f, 1.0f));

	float averageIntervalH = height / (float)count; // 平均間隔

	float baseIntervalH = averageIntervalH/2; // 基底間隔

	float varIntervalH = baseIntervalH; // 變動間隔

	srand(time(NULL));

	

	for(int i = 0; i < count; i++)
	{
		float y = rand() % (int)varIntervalH;

		varIntervalH = baseIntervalH + (varIntervalH - y); //基底間隔 + 剩下的間隔 = 下個迭代的的變動間隔

		y += baseIntervalH; // 基底間隔 + 變動間隔

		y = 2.0f / height*y; // Normalize

		maxDisturbanceY = min(y, maxDisturbanceY);

		y = horizontalPoints[i*2].y - y; // 加上個點開始

		horizontalPoints.push_back(vec2(-1.0f, y));
		horizontalPoints.push_back(vec2(1.0f, y));
	}

	horizontalPoints.push_back(vec2(-1.0f, -1.0f));
	horizontalPoints.push_back(vec2(1.0f, -1.0f));
}

void genVerticalPoints(int baseCount)
{
	int verticalIndex = 0;
	int specialIndex, specialIndex2;

	srand(time(NULL));


	for (int i = 0; i < verticalBaseCount; i++)
	{

		if (i == 0)
		{
			verticalPoints.push_back(vec2(horizontalPoints[i * 2].x, horizontalPoints[i * 2].y));
			verticalPoints.push_back(vec2(horizontalPoints[i * 2 + 2].x, horizontalPoints[i * 2 + 2].y));
			specialIndex = verticalPoints.size() - 1;
		}
		else
		{
			verticalPoints.push_back(vec2(horizontalPoints[i * 2 + 2].x, horizontalPoints[i * 2 + 2].y));
		}

		verticalIndex = verticalPoints.size();

		if (i == 0)
		{
			g[verticalPoints.size() - 2].push_back(verticalPoints.size() - 1);
			g[verticalPoints.size() - 1].push_back(verticalPoints.size() - 2);
		}
		else
		{
			g[specialIndex].push_back(verticalPoints.size() - 1);
			g[verticalPoints.size() - 1].push_back(specialIndex);
		}

		int verticalPointCount = rand() % 3 + baseCount;
		float averageIntervalW = width / (float)verticalPointCount;
		float baseIntervalW = averageIntervalW / 2.0f;
		float varIntervalW = baseIntervalW;

		float averageIntervalW2 = width / verticalPointCount;
		float baseIntervalW2 = averageIntervalW2 / 2.0f;
		float varIntervalW2 = baseIntervalW2;

		for(int j = 0; j < verticalPointCount; j++)
		{
			float y = horizontalPoints[i * 2].y;
			float x = rand() % (int)varIntervalW;
			varIntervalW = baseIntervalW + (varIntervalW - x); //基底間隔 + 剩下的間隔 = 下個迭代的的變動間隔
			x += baseIntervalW; // 基底間隔 + 變動間隔
			x = 2.0f / height*x; // Normalize

			maxDisturbanceX = min(x, maxDisturbanceX);

			if (i == 0)
			{
				x += verticalPoints[verticalIndex - 2].x;
			}
			else if (i != 0 && j == 0)
			{
				x += verticalPoints[specialIndex].x;
				specialIndex = verticalPoints.size() - 1;
			}
			else if (i != 0 && j != 0)
			{
				x += verticalPoints[verticalIndex - 2].x;
			}

			verticalPoints.push_back(vec2(x, y));

			float y2 = horizontalPoints[i * 2 + 2].y;
			float x2 = rand() % (int)varIntervalW2;

			varIntervalW2 = baseIntervalW2 + (varIntervalW2 - x2); //基底間隔 + 剩下的間隔 = 下個迭代的的變動間隔
			x2 += baseIntervalW2; // 基底間隔 + 變動間隔
			x2 = 2.0f / height*x2; // Normalize

			x2 += verticalPoints[verticalIndex - 1].x;

			verticalPoints.push_back(vec2(x2, y2));

			verticalIndex = verticalPoints.size();

			g[verticalIndex - 2].push_back(verticalIndex - 1);
			g[verticalIndex - 1].push_back(verticalIndex - 2);
			
		}

		if (i == 0)
		{
			verticalPoints.push_back(vec2(horizontalPoints[i * 2 + 1].x, horizontalPoints[i * 2 + 1].y));
			verticalPoints.push_back(vec2(horizontalPoints[i * 2 + 3].x, horizontalPoints[i * 2 + 3].y));
			specialIndex2 = verticalPoints.size() - 1;
		}
		else
		{
			verticalPoints.push_back(vec2(horizontalPoints[i * 2 + 3].x, horizontalPoints[i * 2 + 3].y));
		}

		verticalIndex = verticalPoints.size();

		if (i == 0)
		{
			g[verticalPoints.size() - 2].push_back(verticalPoints.size() - 1);
			g[verticalPoints.size() - 1].push_back(verticalPoints.size() - 2);
		}
		else
		{
			g[specialIndex2].push_back(verticalPoints.size() - 1);
			g[verticalPoints.size() - 1].push_back(specialIndex2);
			specialIndex2 = verticalPoints.size() - 1;
		}
		
	}
}

void offsetInternalPoints()
{
	int index = 0;

	srand(time(NULL));

	while (index <= verticalPoints.size() - 1)
	{

		if (verticalPoints[index].x > -1.0f && verticalPoints[index].x < 1.0f && verticalPoints[index].y > -1.0f && verticalPoints[index].y < 1.0f)
		{
			int randX = rand() % 100;
			int randY = rand() % 100;

			float randXF = ((float)randX - 50.0f) / 100.0f;
			float randYF = ((float)randY - 50.0f) / 100.0f;

			verticalPoints[index].x += randXF*maxDisturbanceX*disturbanceCoefficient;
			verticalPoints[index].y += randYF*maxDisturbanceY*disturbanceCoefficient;
		}

		index++;
	}

}

void drawQuadBoundary()
{
	for(int i = 0; i < newQuadPoints.size(); i++)
	{
		if (newQuadPoints[i].size() != 0)
		{
			myDrawLine(3.0f, newQuadPoints[i].back(), newQuadPoints[i][0]);

			for (int j = 0; j < newQuadPoints[i].size() - 1; j++)
			{
				myDrawLine(3.0f, newQuadPoints[i][j], newQuadPoints[i][j + 1]);
			}
		}
	}
}

void drawAllQuads()
{
	for (int i = 0; i < newQuadPoints.size(); i++)
	{
		//glEnable(GL_POLYGON_SMOOTH);
		//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		if (newQuadPoints[i].size() != 0)
		{
			glBegin(GL_TRIANGLE_FAN);
			glColor3f(169.0f / 255.0f, 169.0f / 255.0f, 169.0f / 255.0f);
			glVertex2f(newQuadPoints[i].back().x, newQuadPoints[i].back().y);

			for (int j = 0; j < newQuadPoints[i].size() - 1; j++)
			{
				glVertex2f(newQuadPoints[i][j].x, newQuadPoints[i][j].y);
			}

			glEnd();
		}
	}
}

void drawBoundary()
{
	myDrawLine(5.0f, vec2(-1.0f, 1.0f), vec2(1.0f, 1.0f));
	myDrawLine(5.0f, vec2(1.0f, 1.0f), vec2(1.0f, -1.0f));
	myDrawLine(5.0f, vec2(1.0f, -1.0f), vec2(-1.0f, -1.0f));
	myDrawLine(5.0f, vec2(-1.0f, -1.0f), vec2(-1.0f, 1.0f));
}

void completeGraph() // 橫向
{
	for (int i = 0; i < verticalPoints.size(); i++)
	{
		float minX = 3.0f;
		int minJ = verticalPoints.size();

		for (int j = 0; j < verticalPoints.size(); j++)
		{

			if (verticalPoints[i].y == verticalPoints[j].y &&
				verticalPoints[j].x - verticalPoints[i].x > 0 &&
				verticalPoints[j].x - verticalPoints[i].x < minX)
			{
				minX = verticalPoints[j].x - verticalPoints[i].x;
				minJ = j;
			}
		}

		if (minJ < verticalPoints.size())
		{
			g[i].push_back(minJ);
			g[minJ].push_back(i);
		}
	}
}

void trace(int currentIndex, int &stage, int startIndex)
{
	for (int j = 0; j < g[currentIndex].size(); j++)
	{

		// 往下找
		if (stage == 0)
		{
			if (verticalPoints[currentIndex].y - verticalPoints[g[currentIndex][j]].y > 0.0f)
			{
				q[startIndex].push_back(g[currentIndex][j]);
				trace(g[currentIndex][j], ++stage, startIndex);
				break;
			}
		}

		// 往右找
		if (stage == 1)
		{
			if (verticalPoints[currentIndex].y == verticalPoints[g[currentIndex][j]].y && verticalPoints[g[currentIndex][j]].x - verticalPoints[currentIndex].x > 0.0f)
			{
				q[startIndex].push_back(g[currentIndex][j]);
				trace(g[currentIndex][j], ++stage, startIndex);
				break;
			}
		}

		if (stage == 2)
		{
			// 先往上找
			if (verticalPoints[g[currentIndex][j]].y - verticalPoints[currentIndex].y > 0.0f)
			{
				q[startIndex].push_back(g[currentIndex][j]);
				trace(g[currentIndex][j], ++stage, startIndex);
				break;
			}
			
			// 再往右找
			if (j == g[currentIndex].size() - 1)
			{
				for (int k = 0; k < g[currentIndex].size(); k++)
				{
					if (verticalPoints[currentIndex].y == verticalPoints[g[currentIndex][k]].y && verticalPoints[g[currentIndex][k]].x - verticalPoints[currentIndex].x > 0.0f)
					{
						q[startIndex].push_back(g[currentIndex][k]);
						trace(g[currentIndex][k], stage, startIndex);
						break;
					}
				}

				break;
			}
		}

		// 往左找
		if(stage == 3)
		{
			if (verticalPoints[currentIndex].y == verticalPoints[g[currentIndex][j]].y  && verticalPoints[currentIndex].x - verticalPoints[g[currentIndex][j]].x > 0.0f)
			{
				q[startIndex].push_back(g[currentIndex][j]);

				if (g[currentIndex][j] == startIndex)
				{
					break;	// 結束
				}
				else
					trace(g[currentIndex][j], stage, startIndex);
			}
		}
	}
}

void generateQuads()
{

	for (int i = 0; i < verticalPoints.size(); i++)
	{
		// 最右邊的點不用trace
		if (verticalPoints[i].x != 1.0f)
		{
			int stage = 0;
			trace(i, stage, i);
		}
	}

	vector<vector<vec2>> temp(q.size(), vector<vec2>(0, vec2(0.0f, 0.0f)));
	newQuadPoints = temp;
}

void scaleQuads()
{
	int count = 0;

	for (auto it = q.begin(); it != q.end(); ++it)
	{
		vec2 center = vec2(0.0f, 0.0f);

		for(int j = 0; j < q[it->first].size(); j++)
		{
			center += verticalPoints[q[it->first][j]];
		}

		center /= q[it->first].size();

		for (int j = 0; j < q[it->first].size(); j++)
		{
			vec2 newPoints = verticalPoints[q[it->first][j]];

			newPoints += (center - newPoints)*scaleCoefficient;

			newQuadPoints[count].push_back(newPoints);
		}

		count++;
	}
}

void printG()
{
	for (int i = 0; i < verticalPoints.size(); i++)
	{
		cout << "G[" << i << "]:";

		for (int j = 0; j < g[i].size(); j++)
		{
			cout << g[i][j] << " ";
		}

		cout << endl;
	}

	cout << "========================" << endl;
}

void printQ()
{
	for (int i = 0; i < verticalPoints.size(); i++)
	{
		cout << "Q[" << i << "]:";

		for (int j = 0; j < q[i].size(); j++)
		{
			cout << q[i][j] << " " << "(" << verticalPoints[q[i][j]].x << "," << verticalPoints[q[i][j]].y << ")" << " ";
		}

		cout << endl;
	}
}

void save_screen_bmp(const char *spath)
{

	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);

	int x, y, w, h;
	x = vp[0];
	y = vp[1];
	w = vp[2];
	h = vp[3];

	int j;

	unsigned char *bottomup_pixel = (unsigned char *)malloc(w*h * 3 * sizeof(unsigned char));
	unsigned char *topdown_pixel = (unsigned char *)malloc(w*h * 3 * sizeof(unsigned char));


	//Byte alignment (that is, no alignment)
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, bottomup_pixel);

	
	for (j = 0; j<h; j++)
		memcpy(&topdown_pixel[j * w * 3], &bottomup_pixel[(h - j - 1) * w * 3], w * 3 * sizeof(unsigned char));

	stbi_write_bmp(spath, 600, 600, 3, topdown_pixel);

	free(bottomup_pixel);
	free(topdown_pixel);
}

void save_screen_ppm(const char *spath)
{

	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);

	int x, y, w, h;
	x = vp[0];
	y = vp[1];
	w = vp[2];
	h = vp[3];

	int j;

	unsigned char *bottomup_pixel = (unsigned char *)malloc(w*h * 3 * sizeof(unsigned char));
	unsigned char *topdown_pixel = (unsigned char *)malloc(w*h * 3 * sizeof(unsigned char));


	//Byte alignment (that is, no alignment)
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, bottomup_pixel);
	for (j = 0; j<h; j++)
		memcpy(&topdown_pixel[j*w * 3], &bottomup_pixel[(h - j - 1)*w * 3], w * 3 * sizeof(unsigned char));


	FILE *f0 = fopen(spath, "wb");
	if (f0 == NULL)
	{
		printf("[Error] : SaveScreen(), Cannot open %s for writting.\n", spath);
		exit(-1);
	}

	fprintf(f0, "P6\n%d %d\n255\n", w, h);
	fwrite(topdown_pixel, sizeof(unsigned char), w*h * 3, f0);
	fclose(f0);

	free(bottomup_pixel);
	free(topdown_pixel);
}

#endif