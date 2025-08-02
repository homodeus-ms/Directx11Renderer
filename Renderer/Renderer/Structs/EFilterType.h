#pragma once
#include "Headers/Types.h"

enum class EFilterType : uint8
{
	CopyFilter,
	BloomPreFilter,
	BlurX_Gaussian,
	BlurY_Gaussian,
	DownSampling,
	UpSampling,
	Combine,
	Gaussian,
	Bloom,
	LUT_ColorGrading,
	End,
};
