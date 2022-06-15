#include "StaticMesh.h"

StaticMesh::StaticMesh(Vertex1* Vertexes, UINT vertexCount, UINT* Indicies, UINT indexCount, Accelerator* Accel)
{
	SetTag(EngineObjTag::NonEngine);
	SetName("Unnamed Static Mesh");

	m_Indicies = vertexCount / 2;
	IndexCount = indexCount;

	TrisCount = vertexCount / 3;


	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(Vertexes) * vertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData = {};
	initialVertexData.pSysMem = Vertexes;

	HRESULT res = Accel->GetDevice()->CreateBuffer(&VertexBufferDesc, &initialVertexData, &VertexBuffer);

	if (!SUCCEEDED(res))
	{
		std::cout << "Vertex Buffer Creation failed!" << std::endl;
		return;
	}

	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(int) * indexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initalIndexData = {};
	initalIndexData.pSysMem = &Indicies; //??

	res = Accel->GetDevice()->CreateBuffer(&IndexBufferDesc, &initalIndexData, &IndexBuffer);

	if (!SUCCEEDED(res))
	{
		std::cout << "Index Buffer Creation failed!" << std::endl;
		return;
	}

	

}

StaticMesh::StaticMesh(Vertex1* Vertexes, UINT vertexCount, UINT* Indicies, UINT indexCount, Accelerator* Accel, std::string& Name)
{
	SetTag(EngineObjTag::NonEngine);
	SetName(Name);

	m_Indicies = vertexCount / 2;
	IndexCount = indexCount;

	TrisCount = vertexCount / 3;


	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(Vertexes) * vertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData = {};
	initialVertexData.pSysMem = Vertexes;

	HRESULT res = Accel->GetDevice()->CreateBuffer(&VertexBufferDesc, &initialVertexData, &VertexBuffer);

	if (!SUCCEEDED(res))
	{
		std::cout << "Vertex Buffer Creation failed!" << std::endl;
		return;
	}

	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(int) * indexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initalIndexData = {};
	initalIndexData.pSysMem = &Indicies; //??

	res = Accel->GetDevice()->CreateBuffer(&IndexBufferDesc, &initalIndexData, &IndexBuffer);

	if (!SUCCEEDED(res))
	{
		std::cout << "Index Buffer Creation failed!" << std::endl;
		return;
	}

}

void StaticMesh::CalculateTangents(Vertex1* vertices, UINT vertexCount, UINT* indices, UINT indexCount)
{
	XMFLOAT3* tan1 = new XMFLOAT3[vertexCount * 2];
	XMFLOAT3* tan2 = tan1 + vertexCount;

	ZeroMemory(tan1, vertexCount * sizeof(XMFLOAT3) * 2);

	int triangleCount = indexCount / 3;
	for (UINT i = 0; i < indexCount; i += 3)
	{
		int i1 = indices[i];
		int i2 = indices[i + 2];
		int i3 = indices[i + 1];
		auto v1 = vertices[i1].Position;
		auto v2 = vertices[i2].Position;
		auto v3 = vertices[i3].Position;

		auto w1 = vertices[i1].UV;
		auto w2 = vertices[i2].UV;
		auto w3 = vertices[i3].UV;

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;
		float r = 1.0F / (s1 * t2 - s2 * t1);

		XMFLOAT3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);

		XMFLOAT3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r);

		XMStoreFloat3(&tan1[i1], XMLoadFloat3(&tan1[i1]) + XMLoadFloat3(&sdir));
		XMStoreFloat3(&tan1[i2], XMLoadFloat3(&tan1[i2]) + XMLoadFloat3(&sdir));
		XMStoreFloat3(&tan1[i3], XMLoadFloat3(&tan1[i3]) + XMLoadFloat3(&sdir));

		XMStoreFloat3(&tan2[i1], XMLoadFloat3(&tan2[i1]) + XMLoadFloat3(&tdir));
		XMStoreFloat3(&tan2[i2], XMLoadFloat3(&tan2[i2]) + XMLoadFloat3(&tdir));
		XMStoreFloat3(&tan2[i3], XMLoadFloat3(&tan2[i3]) + XMLoadFloat3(&tdir));
	}

	for (int a = 0; a < vertexCount; a++)
	{
		auto n = vertices[a].Normal;
		auto t = tan1[a];

		// Gram-Schmidt orthogonalize
		auto dot = XMVector3Dot(XMLoadFloat3(&n), XMLoadFloat3(&t));

		XMStoreFloat3(&vertices[a].Tangent, XMVector3Normalize(XMLoadFloat3(&t) - XMLoadFloat3(&n) * dot));
	}

	delete[] tan1;
}