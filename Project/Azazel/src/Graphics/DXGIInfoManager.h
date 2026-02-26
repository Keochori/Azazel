#pragma once

struct IDXGIInfoQueue;

enum class Severity
{
	eWarning,
	eError,
	eOther
};

struct DXGIInfoMessage
{
	std::string myDescription;
	Severity mySeverity = Severity::eOther;
};

class DXGIInfoManager
{
public:
	DXGIInfoManager();
	~DXGIInfoManager();

	void UpdateInfoQueuePosition();
	std::vector<DXGIInfoMessage> GetMessages() const;

private:
	UINT64 currentPos = 0u;
	IDXGIInfoQueue* myDXGIInfoQueue;
};

