#pragma once
#include "DirectXMath.h"
#include "Graphics/Bindables/ConstantBuffer.hpp"
#include "Graphics/Bindables/InputLayout.h"
#include "Graphics/Bindables/Sampler.h"

using namespace DirectX;

struct RenderData;

class Renderer
{
public:
	Renderer(ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aContext, float aAspectRatio);
	~Renderer() = default;

	void Render(ComPtr<ID3D11DeviceContext>& aContext, const XMMATRIX aViewMatrix, const std::vector<RenderData>& aRenderData);
	void SetAspectRatio(float aApsect);

private:
	float myAspectRatio;
	ConstantBuffer<WVPBuffer> myWVPBuffer;
	ConstantBuffer<MaterialBuffer> myMaterialBuffer;
	Sampler myDefaultSampler;
	InputLayout myInputLayout;
};

