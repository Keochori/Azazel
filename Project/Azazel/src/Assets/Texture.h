#pragma once
#include "Graphics/Bindables/SRV.h"

struct Texture
{
	SRV mySRV;
	Texture(DirectX::ScratchImage aImage) : mySRV(std::move(aImage)) {}
};