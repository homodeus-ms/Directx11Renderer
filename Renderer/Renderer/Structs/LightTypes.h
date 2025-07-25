#pragma once

constexpr uint8 MAX_DIRECTIONAL_LIGHT_COUNT = 1;
constexpr uint8 MAX_SPOT_LIGHT_COUNT = 3;
constexpr uint8 MAX_POINT_LIGHT_COUNT = 3;
constexpr uint8 MAX_LIGHTS_COUNT = MAX_DIRECTIONAL_LIGHT_COUNT + MAX_SPOT_LIGHT_COUNT + MAX_POINT_LIGHT_COUNT;
constexpr uint8 MAX_SHADOW_MAP_COUNT = 4;
constexpr uint8 MAX_CUBE_SHADOW_MAP_COUNT = 1;

enum class ELightType : uint8
{
	Directional,
	Spot,
	Point,
};

struct LightDesc
{
	Color ambient = Vec4(1.f);
	Color diffuse = Vec4(1.f);
	Color specular = Vec4(1.f);
	Color emissive = { 0.f, 0.f, 0.f, 1.f };
};

struct DirectionalLightDesc : public LightDesc
{
	Vec3 direction{ 0.f, 0.f, 1.f };
	int32 shadowMapIndex = -1;
	uint32 isOn = 1;    // 1 : true, 0 : false
	float pad[3]{};
};

struct SpotLightDesc : public LightDesc
{
	Vec3 position{ 0.f, 0.f, 0.f };
	int32 shadowMapIndex = -1;
	Vec3 direction{ 0.f, 0.f, 1.f };
	float range = 80.f;
	Vec3 attenuation = { 1.f, 0.1f, 0.05f }; // constant, linear, quadratic
	float spotPower = 8.f;
	uint32 isOn = 1;
	float pad[3];
};

struct PointLightDesc : public LightDesc
{
	Vec3 position{ 0.f, 0.f, 0.f };
	uint32 bShadowMapUsing = 0;
	Vec3 attenuation = { 1.f, 0.1f, 0.05f }; // constant, linear, quadratic
	float range = 80.f;
	uint32 isOn = 1;
	float pad[3];
};
