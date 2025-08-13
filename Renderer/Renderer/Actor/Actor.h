#pragma once
#include "Components/Component.h"
#include "Graphics/Buffer/ConstantBuffer.h"
#include "Structs/EShaderStage.h"
#include "Structs/ShaderParameterTypes.h"

class Transform;
class BasicMesh;
class RenderComponentBase;
class BasicMeshRenderer;
class StaticMeshRenderer;
class StaticMesh;
class MaterialBase;

enum class EActorType : uint8
{
	Actor,
	Pawn,
	CameraActor,
	LightActor,
	DebugActor,
	ReflectActor,
};

class Actor : public enable_shared_from_this<Actor>
{
public:
	Actor(EActorType actorType = EActorType::Actor, const string& actorName = "Actor");
	virtual ~Actor();

	virtual void Construct();
	virtual void BeginPlay();
	virtual void Tick();
	virtual void LateTick();
	virtual void FixedTick();

	void Render();
	void RenderShadowMap(bool bForPointLight, int32 instanceCount = 0);
	void RenderDrawNormal();

	EActorType GetActorType() const { return _actorType; }
	EResourceType GetMeshType() const { return _meshType; }
	const string& GetName() const { return _actorName; }

	// Component
	shared_ptr<Component> GetFixedComponent(EComponentType type);
	shared_ptr<Transform> GetTransform();
	shared_ptr<Transform> GetOrAddTransform();
	void AddComponent(shared_ptr<Component> component);
	bool IsRenderedActor();

	// Mesh, Material
	vector<shared_ptr<MaterialBase>> GetMaterials();

	void SetBasicMesh(const shared_ptr<BasicMesh>& mesh);
	void SetBasicMaterial(const shared_ptr<MaterialBase>& material);
	void SetStaticMesh(const shared_ptr<StaticMesh>& staticMesh);
	void ChangeMaterialType(EMaterialType type);
	shared_ptr<BasicMeshRenderer> GetOrAddBasicMeshRenderer();
	shared_ptr<StaticMeshRenderer> GetOrAddStaticMeshRenderer();

	// Transform
	bool IsTransformChanged() { return _bTransformChanged; }
	virtual void SetTransformChanged(bool bChanged) { _bTransformChanged = bChanged; }

	// ShadowMap
	bool IsCastShadowedActor() { return _bCastShadow; }
	void SetIsCastShadowedActor(bool bShadowed) { _bCastShadow = bShadowed; }

	// Draw Normal For Debug
	bool ShouldDrawNormal() { return _bDrawNormal; }
	void SetDrawNormal(bool bDraw) { _bDrawNormal = bDraw; };

protected:

	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	
	// Renderer Cache
	shared_ptr<RenderComponentBase> _renderer = nullptr;

	EActorType _actorType;
	EResourceType _meshType{};
	string _actorName{};

private:
	bool _bCastShadow = true;
	bool _bTransformChanged = true;
	bool _bIsRenderedActor = true;
	bool _bDrawNormal = false;
};

