#include "pch.h"
#include "Renderer.h"
#include "Scene/Object.h"
#include "Graphics/Diagnostics/DXASSERT.h"

Renderer::Renderer(ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aContext) :
	myWVPBuffer(eBindType::VS, WVPBuffer(XMMatrixIdentity())),
	myInputLayout({ {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}, }),
	myColorBuffer(eBindType::PS, ColorBuffer({
				DirectX::XMVectorSet(1.0f,0.0f,1.0f,0.0f),
				DirectX::XMVectorSet(1.0f,0.0f,0.0f,0.0f),
				DirectX::XMVectorSet(0.0f,1.0f,0.0f,0.0f),
				DirectX::XMVectorSet(0.0f,0.0f,1.0f,0.0f),
				DirectX::XMVectorSet(1.0f,1.0f,0.0f,0.0f),
				DirectX::XMVectorSet(0.0f,1.0f,1.0f,0.0f)
		}))
{
	myWVPBuffer.Create(aDevice);
	myWVPBuffer.Bind(aContext);

	myInputLayout.Create(aDevice);
	myInputLayout.Bind(aContext);

	myColorBuffer.Create(aDevice);
	myColorBuffer.Bind(aContext);
}

void Renderer::Render(ComPtr<ID3D11DeviceContext>& aContext, const XMMATRIX aViewMatrix, std::vector<std::shared_ptr<Object>> aObjects)
{
	for (std::shared_ptr<Object> object : aObjects)
	{
		if (!object->GetMesh())
			continue;
		object->GetMesh()->myVertexBuffer.Bind(aContext);
		object->GetMesh()->myIndexBuffer.Bind(aContext);

		// Update WVPBuffer
		Transform& transform = object->GetTransform();
		XMMATRIX transformMatrix = 
			XMMatrixScaling(transform.myScale.x, transform.myScale.y, transform.myScale.z) *
			XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(transform.myRotation.x), XMConvertToRadians(transform.myRotation.y), XMConvertToRadians(transform.myRotation.z)) *
			XMMatrixTranslation(transform.myPosition.x, transform.myPosition.y, transform.myPosition.z);
		XMMATRIX viewMatrix = aViewMatrix;
		XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 1500.0f / 902.0f, 0.5f, 1000.f);

		myWVPBuffer.UpdateData(aContext,
			{
				XMMatrixTranspose(
					transformMatrix *
					viewMatrix *
					perspectiveMatrix
				)
			});

		// Draw
		DXASSERT(aContext->DrawIndexed((UINT)object->GetMesh()->myIndexBuffer.GetIndices().size(), 0u, 0u));
	}
}
