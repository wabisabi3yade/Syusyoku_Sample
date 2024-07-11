#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(const Mesh& _other)
{
	Copy(_other);
}

Mesh& Mesh::operator=(const Mesh& _other)
{
	Copy(_other);
	return *this;
}

std::vector<Vertex>& Mesh::GetVerticies()
{
	return verticies;
}

std::vector<u_int>& Mesh::GetIndicies()
{
	return indicies;
}

u_int Mesh::GetIndexNum()
{
	return static_cast<u_int>(indicies.size());
}

void Mesh::SetName(const std::string& _name)
{
	name = _name;
}

void Mesh::SetMaterialID(u_int _materialID)
{
	materialID = _materialID;
}

void Mesh::Copy(const Mesh& _other)
{
	if (this == &_other) return;

	// 頂点
	verticies.clear();
	for (Vertex vertex : _other.verticies)
		verticies.push_back(vertex);

	// インデックス
	indicies.clear();
	for (u_int index : _other.indicies)
		indicies.push_back(index);

	name = _other.name;

	materialID = _other.materialID;
}
