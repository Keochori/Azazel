#pragma once
#include "Texture.h"
#include "Scene/Types/Color.h"
#include "Graphics/Bindables/Sampler.h"
#include <algorithm>
#include <memory>

struct Material
{
	Material(const Sampler& aSampler) 
	{
		mySampler = aSampler;
	}

	void SetAlbedoColor(int aR, int aG, int aB, int aA)
	{
		myAlbedoColor = Color(aR, aG, aB, aA);
	}

	void SetAlbedoColor(int aR, int aG, int aB)
	{
		myAlbedoColor = Color(aR, aG, aB);
	}

	std::shared_ptr<Texture> myAlbedoTexture;
	Color myAlbedoColor;

	Sampler mySampler;
};