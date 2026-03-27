#include "pch.h"
#include "DXGIInfoManager.h"
#include "dxgidebug.h"
#include <memory>

DXGIInfoManager& DXGIInfoManager::GetInstance()
{
	static DXGIInfoManager ourInstance;
	return ourInstance;
}

void DXGIInfoManager::Init()
{
	// Load the dll
	const HMODULE dxgidebugDLL = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (!dxgidebugDLL)
	{
		LOG_ERROR("Couldn't load dxgidebug.dll");
		return;
	}

	// Define function signature of DXGIGetDebugInterface
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// Load the function
	DXGIGetDebugInterface dxgiGetDebugInterfaceFunc = reinterpret_cast<DXGIGetDebugInterface>(GetProcAddress(dxgidebugDLL, "DXGIGetDebugInterface"));
	if (!dxgiGetDebugInterfaceFunc)
	{
		LOG_ERROR("Couldn't load DXGIGetDebugInterface function correctly");
		return;
	}

	if (FAILED(dxgiGetDebugInterfaceFunc(__uuidof(IDXGIInfoQueue), &myDXGIInfoQueue)))
	{
		LOG_ERROR("Couldn't use DXGIGetDebugInterface function correctly");
		return;
	}

	UpdateInfoQueuePosition();
}

void DXGIInfoManager::UpdateInfoQueuePosition()
{
	currentPos = myDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<DXGIInfoMessage> DXGIInfoManager::GetMessages() const
{
	std::vector<DXGIInfoMessage> messages;
	UINT64 end = myDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (UINT64 i = currentPos; i < end; i++)
	{
		// Get size of message
		SIZE_T messageLength = 0u;
		if (FAILED(myDXGIInfoQueue->GetMessageW(DXGI_DEBUG_ALL, i, nullptr, &messageLength)))
		{
			LOG_ERROR("Couldn't fetch message length from DXGIInfoQueue");
			continue;
		}

		// Allocate memory for message
		std::unique_ptr<char[]> messageBuffer = std::make_unique<char[]>(messageLength);
		DXGI_INFO_QUEUE_MESSAGE* message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(messageBuffer.get());

		// Get message
		if (FAILED(myDXGIInfoQueue->GetMessageW(DXGI_DEBUG_ALL, i, message, &messageLength)))
		{
			LOG_ERROR("Couldn't fetch message from DXGIInfoQueue");
			continue;
		}

		DXGIInfoMessage dxgiInfoMessage;
		dxgiInfoMessage.myDescription = message->pDescription;

		switch (message->Severity)
		{
			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING:
				dxgiInfoMessage.mySeverity = eSeverity::Warning;
				break;

			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR:
				dxgiInfoMessage.mySeverity = eSeverity::Error;
				break;

			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION:
				dxgiInfoMessage.mySeverity = eSeverity::Error;
				break;
		}
		messages.emplace_back(dxgiInfoMessage);
	}
	return messages;
}
