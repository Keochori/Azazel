#pragma once
#include <d3d11.h>
#include <wrl.h> 

using namespace Microsoft::WRL;

class ImGuiManager
{
public:
	ImGuiManager(HWND& aHWND, ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aContext);
	~ImGuiManager() = default;

	void NewFrame();
	void Render();
private:
};

