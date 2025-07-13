#pragma once

enum class EResourceType : int8
{
	None = -1,
	Mesh = 0,
	StaticMesh,
	SkeletalMesh,
	Shader,
	Texture,
	Material,
	Animation,
	XML,

	End
};

enum
{
	RESOURCE_TYPE_COUNT = static_cast<uint8>(EResourceType::End)
};

class ResourceBase : public enable_shared_from_this<ResourceBase>
{
public:
	ResourceBase(EResourceType type);
	virtual ~ResourceBase();

	EResourceType GetType() { return _type; }

	void SetName(const wstring& name) { _name = name; }
	const wstring& GetName() { return _name; }
	uint32 GetID() { return _id; }

protected:
	virtual void Load(const wstring& path) {}
	virtual void Save(const wstring& path) {}

protected:
	EResourceType _type = EResourceType::None;
	wstring _name;
	wstring _path;
	uint32 _id = 0;
};

