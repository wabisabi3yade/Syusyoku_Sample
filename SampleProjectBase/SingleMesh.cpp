#include "pch.h"
#include "SingleMesh.h"

void SingleMesh::InitSetting(const std::string& _name, D3D11_PRIMITIVE_TOPOLOGY _topology)
{
	name = _name;
	topology = _topology;
}

SingleMesh::SingleMesh() : name(""), materialID(0), topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	pVertexBuffer = std::make_unique<VertexBuffer>();
	pIndexBuffer = std::make_unique<IndexBuffer>();
}

SingleMesh::SingleMesh(const SingleMesh& _other)
{
	Copy(_other);
}

SingleMesh& SingleMesh::operator=(const SingleMesh& _other)
{
	Copy(_other);
	return *this;
}

void SingleMesh::InitBuffer()
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

std::vector<Vertex>& SingleMesh::GetVerticies()
{
	return verticies;
}

const VertexBuffer& SingleMesh::GetVertexBuffer() const
{
	return *pVertexBuffer;
}

const IndexBuffer& SingleMesh::GetIndexBuffer() const
{
	return *pIndexBuffer;
}

std::vector<u_int>& SingleMesh::GetIndicies()
{
	return indicies;
}

u_int SingleMesh::GetVertexNum() const
{
	return static_cast<u_int>(verticies.size());
}

u_int SingleMesh::GetIndexNum() const
{
	return static_cast<u_int>(indicies.size());
}

u_int SingleMesh::GetMaterialID() const
{
	return materialID;
}

DirectX::SimpleMath::Vector3 SingleMesh::GetCenterPos()
{
	return centerPos;
}

DirectX::SimpleMath::Vector3 SingleMesh::GetSize()
{
	return size;
}

D3D11_PRIMITIVE_TOPOLOGY SingleMesh::GetTopology() const
{
	return topology;
}

void SingleMesh::SetName(const std::string& _name)
{
	name = _name;
}

void SingleMesh::SetMaterialID(u_int _materialID)
{
	materialID = _materialID;
}

void SingleMesh::SetTopology(D3D11_PRIMITIVE_TOPOLOGY _topology)
{
	topology = _topology;
}

void SingleMesh::SetCenterPosition(const DirectX::SimpleMath::Vector3& _centerPos)
{
	centerPos = _centerPos;
}

void SingleMesh::SetSize(const DirectX::SimpleMath::Vector3& _size)
{
	// 0以下にならないように最小値を制限
	size = Vec3::Max(_size, 0.0f);
}

void SingleMesh::Copy(const SingleMesh& _other)
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
	centerPos = _other.centerPos;
	size = _other.size;
}