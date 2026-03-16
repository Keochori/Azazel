#pragma once
#include "Graphics/Diagnostics/DXGIInfoManager.h"
#include "Core/LOG.h"
#include <windows.h>
#include <assert.h>
#include <vector>

static void DXAssertMessages(const char* aFile, int aLine)
{
	bool error = false;
	std::vector<DXGIInfoMessage> messages = DXGIInfoManager::GetInstance().GetMessages();
	for (DXGIInfoMessage& message : messages)
	{
		switch (message.mySeverity)
		{
			case eSeverity::Warning:
				LOG::Print(message.myDescription, aFile, aLine, LogType::eWarning);
				break;

			case eSeverity::Error:
				LOG::Print(message.myDescription, aFile, aLine, LogType::eError);
				error = true;
				break;
		}
	}

	DXGIInfoManager::GetInstance().UpdateInfoQueuePosition();
	assert(!error);
}

static void HRESULTCheck(HRESULT aHr, const char* aFile, int aLine)
{
	bool HRFailed = FAILED(aHr);

	DXAssertMessages(aFile, aLine);

	assert(!HRFailed);
}

#define HRASSERT(hr) HRESULTCheck(hr, __FILE__, __LINE__)
#define DXASSERT(func) (func); DXAssertMessages(__FILE__, __LINE__)


