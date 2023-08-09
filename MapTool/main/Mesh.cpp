#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	if (m_vertexBuffer) { m_vertexBuffer->Release(); m_vertexBuffer = NULL; }
	if (m_indexBuffer) { m_indexBuffer->Release(); m_indexBuffer = NULL; }
}

void Mesh::Initialize(ID3D11Device* _device)
{
	CreateVertexBuffer(_device);
	CreateIndexBuffer(_device);
}

void Mesh::Render()
{
	//RECT rect;
	//rect.left = (int)m_MeshRect.left;
	//rect.top = (int)m_MeshRect.top;
	//rect.right = (int)m_MeshRect.right;
	//rect.bottom = (int)m_MeshRect.bottom;

	//HPEN DefalutPen = (HPEN)SelectObject(Data::g_hdc, Data::hRedPen);
	//FillRect(Data::g_hdc, &rect, Data::hbrushBlue);
	//SelectObject(Data::g_hdc, DefalutPen);

	//Rectangle(Data::g_hdc, m_MeshRect.left, m_MeshRect.top, m_MeshRect.right, m_MeshRect.bottom);
}

void Mesh::SelectedRender()
{
	RECT rect;
	rect.left = (int)m_MeshRect.left;
	rect.top = (int)m_MeshRect.top;
	rect.right = (int)m_MeshRect.left + 20;
	rect.bottom = (int)m_MeshRect.top + 20;

	FillRect(Data::g_hdc, &rect, Data::hbrushBlue);
}

void Mesh::CreateVertexBuffer(ID3D11Device* _device)
{
	// Create a vertex buffer
	{
		const float Width = WIDTH / 15.f * 2;
		const float Height = HEIGHT / 10.f * 2;

		float left = (m_XNumber * Width / WIDTH) - 1.0f;
		float right = ((m_XNumber + 1) * Width / WIDTH) - 1.0f;
		float top = 1.0f - (m_YNumber * Height / HEIGHT);
		float bottom = 1.0f - ((m_YNumber + 1) * Height / HEIGHT);

		m_MeshRect.left = m_XNumber * Width / 2;
		m_MeshRect.right = (m_XNumber + 1) * Width / 2;
		m_MeshRect.top = m_YNumber * Height / 2;
		m_MeshRect.bottom = (m_YNumber + 1) * Height / 2;

		if (mbIsSelected)
		{
			vertices =
			{
				{ { left,  top, 0.0f, 1.0f }, { 1.f, 1.f, 1.f, 1.f},{0.f, 0.f}},
				{ { right,  top, 0.0f, 1.0f }, { 1.f, 1.f, 1.f, 1.f },{ 1.f, 0.f }},
				{ { left, bottom, 0.0f, 1.0f }, { 1.f, 1.f, 1.f, 1.f },{ 0.f, 1.f }},
				{ { right, bottom, 0.0f, 1.0f }, { 1.f, 1.f, 1.f, 1.f },{ 1.f, 1.f }},
			};
		}
		else
		{
			vertices =
			{
				{ { left,  top, 0.0f, 1.0f }, { 1.f, 1.f, 1.f, 0.5f},{0.f, 0.f}},
				{ { right,  top, 0.0f, 1.0f }, { 1.f, 1.f, 1.f, 0.5f },{ 1.f, 0.f }},
				{ { left, bottom, 0.0f, 1.0f }, { 1.f, 1.f, 1.f, 0.5f },{ 0.f, 1.f }},
				{ { right, bottom, 0.0f, 1.0f }, { 1.f, 1.f, 1.f, 0.5f },{ 1.f, 1.f }},
			};
		}

		

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;                // write access access by CPU and GPU
		bufferDesc.ByteWidth = UINT(sizeof(Vertex) * 4);             // size is the VERTEX struct * 3
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
		//bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
		bufferDesc.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0, };
		vertexBufferData.pSysMem = vertices.data();
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;

		const HRESULT hr = _device->CreateBuffer(&bufferDesc, &vertexBufferData, &m_vertexBuffer);
		if (FAILED(hr)) {
			std::cout << "CreateBuffer() failed. " << std::hex << hr << std::endl;
		};
	}
}

void Mesh::CreateIndexBuffer(ID3D11Device* _device)
{
	// Create an index buffer
	{
		indices =
		{
			 0, 1 ,2,
			 2, 1, 3
		};

		m_indexCount = UINT(indices.size());

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bufferDesc.ByteWidth = UINT(sizeof(uint16_t) * indices.size());
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;       // use as a vertex buffer
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
		bufferDesc.StructureByteStride = sizeof(uint16_t);

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = indices.data();
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;

		_device->CreateBuffer(&bufferDesc, &indexBufferData, &m_indexBuffer);
	}
}
