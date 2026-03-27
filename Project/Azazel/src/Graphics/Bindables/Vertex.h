#pragma once

struct Vertex
{
	Vertex(float aX, float aY, float aZ)
	{
		x = aX;
		y = aY;
		z = aZ;

		u = 0;
		v = 0;
	}

	Vertex(float aX, float aY, float aZ, float aU, float aV)
	{
		x = aX;
		y = aY;
		z = aZ;

		u = aU;
		v = aV;
	}

	float x;
	float y;
	float z;

	float u;
	float v;
};