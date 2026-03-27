#pragma once
#include "Texture.h"
#include "Scene/Color.h"
#include "Graphics/Bindables/Sampler.h"
#include <memory>

struct Material
{
	Material(const Sampler& aSampler) 
	{
		mySampler = aSampler;
	}

	void SetAlbedoColor(int aR, int aG, int aB, int aA)
	{
		myAlbedoColor.myR = std::clamp(aR, 0, 255);
		myAlbedoColor.myG = std::clamp(aG, 0, 255);
		myAlbedoColor.myB = std::clamp(aB, 0, 255);
		myAlbedoColor.myA = std::clamp(aA, 0, 255);
	}

	void SetAlbedoColor(int aR, int aG, int aB)
	{
		myAlbedoColor.myR = std::clamp(aR, 0, 255);
		myAlbedoColor.myG = std::clamp(aG, 0, 255);
		myAlbedoColor.myB = std::clamp(aB, 0, 255);
	}

	std::shared_ptr<Texture> myAlbedoTexture;
	Color myAlbedoColor;

	Sampler mySampler;
};