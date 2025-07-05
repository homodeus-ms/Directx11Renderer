#pragma once

template<typename T>
class Geometry
{
public:
	Geometry() {}
	~Geometry() {}

	const vector<T>& GetVertices() const { return _vertices; }
	uint32 GetVerticesCount() const { return _vertices.size(); }
	void* GetVertexData() const { return _vertices.data(); }

	const vector<uint32>& GetIndices() const { return _indices; }
	uint32 GetIndicesCount() const { return _indices.size(); }
	void* GetIndexData() const { return _indices.data(); }

	void AddVertex(const T& vertex) { _vertices.push_back(vertex); }
	void AddVertices(const vector<T>& vertices) { _vertices.insert(_vertices.end(), vertices.begin(), vertices.end()); }
	void SetVertices(const vector<T>& vertices) { _vertices = vertices; }

	void AddIndex(uint32 index) { _indices.push_back(index); }
	void AddIndicies(const vector<uint32>& indices) { _indices.insert(_indices.end(), indices.begin(), indices.end()); }
	void SetIndicies(const vector<uint32>& indices) { _indices = indices; }

private:
	vector<T> _vertices;
	vector<uint32> _indices;
};

