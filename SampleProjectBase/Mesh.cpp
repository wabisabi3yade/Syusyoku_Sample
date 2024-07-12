#include "pch.h"
#include "Mesh.h"

Mesh::Mesh() : name(""), materialID(0), topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	pVertexBuffer = std::make_unique<VertexBuffer>();
	pIndexBuffer = std::make_unique<IndexBuffer>();
}

Mesh::Mesh(const Mesh& _other)
{
	Copy(_other);
}

Mesh& Mesh::operator=(const Mesh& _other)
{
	Copy(_other);
	return *this;
}

void Mesh::InitBuffer()
{
	assert(static_cast<int>(verticies.size()) > 0 || 
		static_cast<int>(indicies.size()) > 0);

	// 頂点バッファ作成
	u_int size = static_cast<u_int>(verticies.size() * sizeof(Vertex));
	pVertexBuffer->CreateBuffer(size, 0, verticies.data());

	// インデックスバッファ作成
	size = static_cast<u_int>(indicies.size() * sizeof(u_int));
	pIndexBuffer->CreateBuffer(size, 0, indicies.data());
}

std::vector<Vertex>& Mesh::GetVerticies()
{
	return verticies;
}

const ID3D11Buffer& Mesh::GetVertexBuffer() const
{
	return pVertexBuffer->GetBuffer();
}

const ID3D11Buffer& Mesh::GetIndexBuffer() const
{
	return pIndexBuffer->GetBuffer();
}

std::vector<u_int>& Mesh::GetIndicies()
{
	return indicies;
}

u_int Mesh::GetIndexNum() const
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

	// バッファ作成
	InitBuffer();

	name = _other.name;
	materialID = _other.materialID;
	topology = _other.topology;
}
