#pragma once
#include "DirectXMath.h"
#include "Graphics/Bindables/ConstantBuffer.hpp"
#include "Graphics/Bindables/InputLayout.h"

class Object;

using namespace DirectX;

class Renderer
{
public:
	Renderer(ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aContext);
	~Renderer() = default;

	void Render(ComPtr<ID3D11DeviceContext>& aContext, const XMMATRIX aViewMatrix, const std::vector<std::shared_ptr<Object>>& aObjects);

private:
	ConstantBuffer<WVPBuffer> myWVPBuffer;
	ConstantBuffer<MaterialBuffer> myMaterialBuffer;
	InputLayout myInputLayout;
};

