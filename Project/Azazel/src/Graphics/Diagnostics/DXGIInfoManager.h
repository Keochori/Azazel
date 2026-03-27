#pragma once
#include <wrl.h>

using namespace Microsoft::WRL;

struct IDXGIInfoQueue;

enum class eSeverity
{
	Warning,
	Error,
	Other
};

struct DXGIInfoMessage
{
	std::string myDescription;
	eSeverity mySeverity = eSeverity::Other;
};

class DXGIInfoManager
{
public:
	DXGIInfoManager() = default;
	~DXGIInfoManager() = default;
	DXGIInfoManager(const DXGIInfoManager&) = delete;
	DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;

	void Init();
	static DXGIInfoManager& GetInstance();

	void UpdateInfoQueuePosition();
	std::vector<DXGIInfoMessage> GetMessages() const;

private:
	UINT64 currentPos = 0u;
	ComPtr<IDXGIInfoQueue> myDXGIInfoQueue;
};

