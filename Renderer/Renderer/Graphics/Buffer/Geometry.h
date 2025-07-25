#pragma once

#include "VertexData.h"

template<typename T>
class Geometry
{
public:
	Geometry() {}
	~Geometry() {}

	const vector<D3D11_INPUT_ELEMENT_DESC>& GetInputDesc() const { return T::descs; }

	uint32 GetVertexCount() const { return static_cast<uint32>(_vertices.size()); }
	void* GetVertexData() const { return _vertices.data(); }
	const vector<T>& GetVertices() const { return _vertices; }

	uint32 GetIndexCount() const { return static_cast<uint32>(_indices.size()); }
	void* GetIndexData() const { return _indices.data(); }
	const vector<uint32>& GetIndices() const { return _indices; }
	
	void AddVertex(const T& vertex) { _vertices.push_back(vertex); }
	void AddVertices(const vector<T>& vertices) 
	{ 
		_vertices.insert(_vertices.end(), vertices.begin(), vertices.end()); 
	}
	void SetVertices(const vector<T>& vertices) 
	{
		_vertices = vertices; 
	}

	void AddIndex(uint32 index) { _indices.push_back(index); }
	void AddIndices(const vector<uint32>& indices) { _indices.insert(_indices.end(), indices.begin(), indices.end()); }
	void SetIndices(const vector<uint32>& indices) { _indices = indices; }

private:
	vector<T> _vertices;
	vector<uint32> _indices;
};
