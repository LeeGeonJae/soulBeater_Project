#pragma once
#include "defalut.h"

class Texture;

class Mesh
{
public:
	Mesh();
	~Mesh();

public:
	void Initialize(ID3D11Device* _device);
	void Render();

	void SelectedRender();

public:
	void SetTexture(Texture* tex) { if (tex != nullptr) m_texture = tex; }

	ID3D11Buffer* const* GetVertexBuffer() { return &m_vertexBuffer; }
	ID3D11Buffer* GetIndexBuffer() { return m_indexBuffer; }
	UINT GetIndexCount() { return m_indexCount; }
	Texture* GetTexture() { return m_texture; }
	RECT_F GetMeshRect() { return m_MeshRect; }

	void SetXYPosition(int _x, int _y) { m_XNumber = _x; m_YNumber = _y; }
	void AddXYPosition(int _x, int _y) { m_XNumber += _x; m_YNumber += _y; }

	inline void SetIsSelected(bool bIsSelected, ID3D11DeviceContext* deviceContext);

private:
	void CreateVertexBuffer(ID3D11Device* _device);
	void CreateIndexBuffer(ID3D11Device* _device);

private:
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	UINT m_indexCount;

	Texture* m_texture = {};

	vector<uint16_t> indices = {};
	vector<Vertex> vertices = {};

	int m_XNumber;
	int m_YNumber;
	RECT_F m_MeshRect;

	bool mbIsSelected;
};

void Mesh::SetIsSelected(bool bIsSelected, ID3D11DeviceContext* deviceContext)
{
	mbIsSelected = bIsSelected;

	for (Vertex& vertex : vertices)
	{
		vertex.color.a = mbIsSelected ? 1.f : 0.5f;
	}
	deviceContext->UpdateSubresource(m_vertexBuffer, 0, nullptr, vertices.data(), 0, 0);
}