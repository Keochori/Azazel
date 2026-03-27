#pragma once
#include "Graphics/Bindables/SRV.h"

struct Texture
{
	Texture(DirectX::ScratchImage aImage) : mySRV(std::move(aImage)) {}
	SRV mySRV;
};