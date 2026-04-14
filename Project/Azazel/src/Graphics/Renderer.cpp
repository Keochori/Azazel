#include "pch.h"
#include "Renderer.h"
#include "RenderData.h"
#include "Assets/Model.h"
#include "Assets/Material.h"
#include "Graphics/Diagnostics/DXASSERT.h"

Renderer::Renderer(ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aContext, float aAspectRatio) :
	myWVPBuffer(eBindType::VS, WVPBuffer(XMMatrixIdentity())),
	myMaterialBuffer(eBindType::PS, MaterialBuffer(false, false, XMFLOAT4(0, 0, 0, 0))),
	myDefaultSampler(Sampler()),
	myInputLayout({ 
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"ALBEDOTEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0} })
{
	myWVPBuffer.Create(aDevice);
	myWVPBuffer.Bind(aContext);

	myMaterialBuffer.Create(aDevice);
	myMaterialBuffer.Bind(aContext);

	myDefaultSampler.Create(aDevice);
	myDefaultSampler.Bind(aContext);

	myInputLayout.Create(aDevice);
	myInputLayout.Bind(aContext);

	myAspectRatio = aAspectRatio;
}

void Renderer::Render(ComPtr<ID3D11DeviceContext>& aContext, const XMMATRIX aViewMatrix, const std::vector<RenderData>& aRenderData)
{
	for (RenderData renderData : aRenderData)
	{
		if (!renderData.myModel)
			continue;

		renderData.myModel->myVertexBuffer.Bind(aContext);
		renderData.myModel->myIndexBuffer.Bind(aContext);

		// Update material data
		int hasMaterial = false;
		int hasAlbedoTexture = false;
		if (renderData.myMaterial)
		{
			hasMaterial = true;
			std::shared_ptr<Material> material = renderData.myMaterial;

			if (material->myAlbedoTexture)
			{
				hasAlbedoTexture = true;
				material->mySampler.Bind(aContext);
				material->myAlbedoTexture->mySRV.Bind(aContext);
			}

			constexpr float inverseRGB = 1.0f / 255.0f;
			myMaterialBuffer.UpdateData(aContext, MaterialBuffer(hasMaterial, hasAlbedoTexture, DirectX::XMFLOAT4(
				material->myAlbedoColor.myR * inverseRGB,
				material->myAlbedoColor.myG * inverseRGB,
				material->myAlbedoColor.myB * inverseRGB,
				material->myAlbedoColor.myA * inverseRGB
			)));
		}
		else
		{
			myMaterialBuffer.UpdateData(aContext, MaterialBuffer(hasMaterial, hasAlbedoTexture, DirectX::XMFLOAT4()));
		}

		// Update WVPBuffer
		Transform& transform = renderData.myTransform;
		XMMATRIX transformMatrix = 
			XMMatrixScaling(transform.myScale.x, transform.myScale.y, transform.myScale.z) *
			XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(transform.myRotation.x), XMConvertToRadians(transform.myRotation.y), XMConvertToRadians(transform.myRotation.z)) *
			XMMatrixTranslation(transform.myPosition.x, transform.myPosition.y, transform.myPosition.z);
		XMMATRIX viewMatrix = aViewMatrix;
		XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), myAspectRatio, 0.5f, 1000.f);

		myWVPBuffer.UpdateData(aContext,
			{
				XMMatrixTranspose(
					transformMatrix *
					viewMatrix *
					perspectiveMatrix
				)
			});

		// Draw
		DXASSERT(aContext->DrawIndexed((UINT)renderData.myModel->myIndexBuffer.GetIndices().size(), 0u, 0u));
	}
}

void Renderer::SetAspectRatio(float aAspect)
{
	if (aAspect <= 0)
		return;
	myAspectRatio = aAspect;
}
