#pragma once

struct Color
{
	int myR;
	int myG;
	int myB;
	int myA;

	Color()
	{
		myR = 255;
		myG = 255;
		myB = 255;
		myA = 255;
	}

	Color(int aR, int aG, int aB, int aA)
	{
		myR = std::clamp(aR, 0, 255);
		myG = std::clamp(aG, 0, 255);
		myB = std::clamp(aB, 0, 255);
		myA = std::clamp(aA, 0, 255);
	}

	Color(int aR, int aG, int aB)
	{
		myR = std::clamp(aR, 0, 255);
		myG = std::clamp(aG, 0, 255);
		myB = std::clamp(aB, 0, 255);
		myA = 255;
	}
};