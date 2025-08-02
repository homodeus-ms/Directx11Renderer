#include "pch.h"
#include "LUT.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include "Texture/Texture3D.h"

LUT::LUT()
	: Super(EResourceType::LUT)
{
}

LUT::~LUT()
{
}

void LUT::Load(const wstring& path)
{
    std::filesystem::path filepath(path);

    std::ifstream file(filepath);
    if (!file.is_open())
    {
        LOG(Error, "Failed to open LUT file");
        return;
    }

    std::string line;
    bool readingData = false;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);

        if (line.find("LUT_3D_SIZE") != std::string::npos)
        {
            iss.ignore(64, ' '); // "LUT_3D_SIZE " 건너뛰기
            iss >> _LUTSize;
            _LUTData.reserve(_LUTSize * _LUTSize * _LUTSize);
        }
        else if (std::isdigit(line[0]) || line[0] == '-' || line[0] == '+')
        {
            Vec3 color;
            iss >> color.x >> color.y >> color.z;
            _LUTData.push_back(color);
        }
        // 다른 메타 정보는 무시 가능 (예: TITLE, DOMAIN_MIN/MAX 등)
    }

    if (_LUTData.size() != size_t(_LUTSize * _LUTSize * _LUTSize))
    {
        LOG(Error, "LUTSize Different");
    }

    _LUTTexture = make_shared<Texture3D>();
    _LUTTexture->CreateTexture(_LUTSize, _LUTData);
}

shared_ptr<SRVBindingInfo> LUT::GetSRVBindingInfo()
{
    if (_LUTTexture == nullptr)
    {
        LOG(Warning, "_LUTTexture is NULL");
        return nullptr;
    }

    return _LUTTexture->GetSRVBindingInfo();
}
