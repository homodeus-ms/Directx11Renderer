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
	LightDesc(Color A, Color D, Color S, Color E = { 0.f, 0.f, 0.f, 1.f })
		: ambient(A), diffuse(D), specular(S), emissive(E)
		, color(D.x, D.y, D.z)
		, radiance((color.x + color.y + color.z) * 8 / 3.f) {}
	
	Color ambient;
	Color diffuse;
	Color specular;
	Color emissive;
	// For PBR
	Vec3 color{};
	float radiance = 1.f;
	float radianceFactor = 2.f;
	
};

struct DirectionalLightDesc : public LightDesc
{
	DirectionalLightDesc()
		: LightDesc(Color(0.f, 0.f, 0.f, 1.f), Color(0.f, 0.f, 0.f, 1.f), Color(0.f, 0.f, 0.f, 1.f)) {}
	
	DirectionalLightDesc(Color A, Color D, Color S, Color E = { 0.f, 0.f, 0.f, 1.f })
		: LightDesc(A, D, S, E) {}

	Vec3 direction{ 0.f, 0.f, 1.f };
	int32 shadowMapIndex = -1;
	uint32 isOn = 1;    // 1 : true, 0 : false
	float pad[2]{};
};

struct SpotLightDesc : public LightDesc
{
	SpotLightDesc() 
		: LightDesc(Color(0.f, 0.f, 0.f, 1.f), Color(0.f, 0.f, 0.f, 1.f), Color(0.f, 0.f, 0.f, 1.f)) {}
	
	SpotLightDesc(Color A, Color D, Color S, Color E = { 0.f, 0.f, 0.f, 1.f })
		: LightDesc(A, D, S, E) {}

	Vec3 position{ 0.f, 0.f, 0.f };
	int32 shadowMapIndex = -1;
	Vec3 direction{ 0.f, 0.f, 1.f };
	float range = 80.f;
	Vec3 attenuation = { 1.f, 0.1f, 0.05f }; // constant, linear, quadratic
	float spotPower = 3.f;
	uint32 isOn = 1;
	float pad[2]{};
};

struct PointLightDesc : public LightDesc
{
	PointLightDesc()
		: LightDesc(Color(0.f, 0.f, 0.f, 1.f), Color(0.f, 0.f, 0.f, 1.f), Color(0.f, 0.f, 0.f, 1.f)) {}

	PointLightDesc(Color A, Color D, Color S, Color E = { 0.f, 0.f, 0.f, 1.f })
		: LightDesc(A, D, S, E) {}

	Vec3 position{ 0.f, 0.f, 0.f };
	uint32 bShadowMapUsing = 0;
	Vec3 attenuation = { 1.f, 0.1f, 0.05f }; // constant, linear, quadratic
	float range = 80.f;
	uint32 isOn = 1;
	float pad[2]{};
};
