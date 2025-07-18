#pragma once

constexpr uint8 MAX_DIRECTIONAL_LIGHT_COUNT = 1;
constexpr uint8 MAX_SPOT_LIGHT_COUNT = 3;
constexpr uint8 MAX_POINT_LIGHT_COUNT = 3;
constexpr uint8 MAX_LIGHTS_COUNT = MAX_DIRECTIONAL_LIGHT_COUNT + MAX_SPOT_LIGHT_COUNT + MAX_POINT_LIGHT_COUNT;

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
	uint32 isOn = 1;    // 1 : true, 0 : false
};

struct SpotLightDesc : public LightDesc
{
	Vec3 position{ 0.f, 0.f, 0.f };
	float range = 100;
	Vec3 direction{0.f, 0.f, 1.f};
	float spotPower = 1;
	Vec3 attenuation = { 1.f, 0.1f, 0.05f }; // constant, linear, quadratic
	uint32 isOn = 1;
};

struct PointLightDesc : public LightDesc
{
	Vec3 position{ 0.f, 0.f, 0.f };
	float range = 50.f;
	Vec3 attenuation = { 1.f, 0.1f, 0.05f }; // constant, linear, quadratic
	uint32 isOn = 1;
};
