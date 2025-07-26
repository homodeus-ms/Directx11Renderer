#pragma once
#include "Headers/Types.h"

enum class EShaderStage : uint8
{
	None = 0,
	VsStage = 1 << 0,
	PsStage = 1 << 1,
	GsStage = 1 << 2,
	Both = VsStage | PsStage,

	End = static_cast<uint8>(Both) + 1,
};

inline EShaderStage operator|(EShaderStage a, EShaderStage b)
{
	return static_cast<EShaderStage>(static_cast<uint8>(a) | static_cast<uint8>(b));
}

inline EShaderStage operator&(EShaderStage a, EShaderStage b)
{
	return static_cast<EShaderStage>(static_cast<uint8>(a) & static_cast<uint8>(b));
}

//inline bool IsValid(EShaderStage stage)
//{
//	uint8 num = static_cast<uint8>(stage);
//	return num > static_cast<uint8>(EShaderStage::None) && num < static_cast<uint8>(EShaderStage::End);
//}

inline bool IsStageVS(EShaderStage stage)
{
	return (stage & EShaderStage::VsStage) == EShaderStage::VsStage;;
}

inline bool IsStagePS(EShaderStage stage)
{
	return (stage & EShaderStage::PsStage) == EShaderStage::PsStage;
}

inline bool IsStageGS(EShaderStage stage)
{
	return (stage & EShaderStage::GsStage) == EShaderStage::GsStage;
}