#pragma once

struct Color
{
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
		myG = std::clamp(aR, 0, 255);
		myB = std::clamp(aR, 0, 255);
		myA = std::clamp(aR, 0, 255);
	}

	Color(int aR, int aG, int aB)
	{
		myR = std::clamp(aR, 0, 255);
		myG = std::clamp(aR, 0, 255);
		myB = std::clamp(aR, 0, 255);
		myA = 255;
	}

	int myR;
	int myG;
	int myB;
	int myA;
};