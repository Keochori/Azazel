#pragma once
#include <objbase.h>
#include <sstream>
#include <iomanip>
#include "Graphics/Diagnostics/DXASSERT.h"

using AssetGUID = std::string;

static AssetGUID GenerateGUID()
{
	GUID guid;
	HRASSERT(CoCreateGuid(&guid));

    std::stringstream ss;

    ss << std::hex
        << std::setw(8) << std::setfill('0') << guid.Data1
        << "-"
        << std::setw(4) << guid.Data2
        << "-"
        << std::setw(4) << guid.Data3
        << "-"
        << std::setw(2) << (int)guid.Data4[0]
        << std::setw(2) << (int)guid.Data4[1]
        << "-"
        << std::setw(2) << (int)guid.Data4[2]
        << std::setw(2) << (int)guid.Data4[3]
        << std::setw(2) << (int)guid.Data4[4]
        << std::setw(2) << (int)guid.Data4[5]
        << std::setw(2) << (int)guid.Data4[6]
        << std::setw(2) << (int)guid.Data4[7];

    return ss.str();
}