#pragma once

#define MAX_BONES_PER_VERTEX 4 // When changing this number, apply changes to input assembly and vertex shader accordingly

struct Vertex
{
	Vertex(float aX, float aY, float aZ) : Vertex(aX, aY, aZ, 0.0f, 0.0f) {}

	Vertex(float aX, float aY, float aZ, float aU, float aV)
	{
		myX = aX;
		myY = aY;
		myZ = aZ;

		myU = aU;
		myV = aV;

		for (int i = 0; i < MAX_BONES_PER_VERTEX; i++)
		{
			myBoneIDs[i] = 0u;
			myWeights[i] = 0.0f;
		}
	}

	float myX;
	float myY;
	float myZ;

	float myU;
	float myV;

	unsigned int myBoneIDs[MAX_BONES_PER_VERTEX];
	float myWeights[MAX_BONES_PER_VERTEX];
};