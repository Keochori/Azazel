#pragma once
#include "DirectXMath.h"
#include "Graphics/Bindables/ConstantBuffer.hpp"
#include "Graphics/Bindables/InputLayout.h"

class Object;

using namespace DirectX;

class Renderer
{
public:
	Renderer(ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aContext, float aAspectRatio);
	~Renderer() = default;

	void Render(ComPtr<ID3D11DeviceContext>& aContext, const XMMATRIX aViewMatrix, const std::vector<std::shared_ptr<Object>>& aObjects);
	void SetAspectRatio(float aApsect);

private:
	float myAspectRatio;
	ConstantBuffer<WVPBuffer> myWVPBuffer;
	ConstantBuffer<MaterialBuffer> myMaterialBuffer;
	InputLayout myInputLayout;
};

