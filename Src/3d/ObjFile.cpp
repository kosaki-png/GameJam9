#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <codecvt> 
#include <cstdio>
#include <sstream>
#include <string>
#include <stdio.h>
#include <iomanip>
#include "../Base/DirectGraphics.h"
#include "../Base/Utility.h"
#include "../3d/ObjFile.h"

bool ObjFile::Load(const char* file_name, ID3D11Device *device, VertexShader* vertex_shader)
{
	char file_path[256];
	std::vector<std::string> out_material_list;
	int len = (int)strlen(file_name);
	int path_tail_point = 0;

	for (int i = len - 1; i >= 0; i--)
	{
		if (file_name[i] == '/')
		{
			path_tail_point = i;
			break;
		}
	}

	strncpy_s(file_path, file_name, path_tail_point + 1);

	if (CreateMesh(out_material_list, file_path, file_name) == false)
	{
		return false;
	}

	if (LoadMaterialFile(out_material_list, file_path) == false)
	{
		return false;
	}

	if (CreateVertexBuffer(device) == false)
	{
		return false;
	}

	if (CreateIndexBuffer(device) == false)
	{
		return false;
	}

	if (CreateInputLayout(device, vertex_shader) == false)
	{
		return false;
	}

	return true;

	dirty = true;
}

void ObjFile::Update()
{
	float tmp = 0;
	if (!parent)
	{
		tmp = 180.0f;
	}

	// ���[�J���}�g���N�X�ݒ�
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(position.X, position.Y, position.Z);
	DirectX::XMMATRIX rotate_x = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.X));
	DirectX::XMMATRIX rotate_y = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.Y + tmp));
	DirectX::XMMATRIX rotate_z = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.Z));
	DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(scale.X, scale.Y, scale.Z);
	m_LocalMat = scale_mat * rotate_x * rotate_y * rotate_z * translate;

	// ���[���h�s��̎Z�o
	UpdateWorldMatrix();
	
}

void ObjFile::Render(DirectGraphics* graphics)
{
	graphics->SetUpContext();
	int count = 0;
	UINT strides = sizeof(CustomVertex);
	UINT offsets = 0;

	// �C���f�b�N�X�o�b�t�@�̐� = �}�e���A���̐��������b�V����`�悷��
	for (auto index : m_Indices)
	{
		// IA(InputAssemblerStage)�ɓ��̓��C�A�E�g��ݒ肷��
		graphics->GetContext()->IASetInputLayout(m_InputLayout);
		// IA�ɐݒ肷�钸�_�o�b�t�@�̎w��
		graphics->GetContext()->IASetVertexBuffers(
			0,						// �o�b�t�@���M�̃X���b�g�ԍ�
			1,						// �o�b�t�@�̐�
			&m_VertexBuffer,		// ���_�o�b�t�@
			&strides,				// �o�b�t�@�Ɏg�p���Ă���\���̂̃T�C�Y
			&offsets);				// �J�n�I�t�Z�b�g

		graphics->GetContext()->IASetIndexBuffer(
			m_IndexBuffers[count],
			DXGI_FORMAT_R32_UINT,
			0);

		// ���[���h�}�g���N�X���R���X�^���g�o�b�t�@�ɐݒ�
		XMStoreFloat4x4(&graphics->GetConstantBufferData()->World, XMMatrixTranspose(m_WorldMat));

		graphics->SetMaterial(&m_Materials[index.first]);

		// �R���X�^���g�o�b�t�@�X�V
		graphics->GetContext()->UpdateSubresource(graphics->GetConstantBuffer(), 0, NULL, graphics->GetConstantBufferData(), 0, 0);

		ID3D11Buffer* constant_buffer = graphics->GetConstantBuffer();
		// �R���e�L�X�g�ɃR���X�^���g�o�b�t�@��ݒ�
		graphics->GetContext()->VSSetConstantBuffers(0, 1, &constant_buffer);
		graphics->GetContext()->PSSetConstantBuffers(0, 1, &constant_buffer);

		// �}�e���A���Ƀe�N�X�`��������Ȃ�ݒ肷��
		if (m_Textures.count(m_Materials[index.first].TextureKeyWord) > 0)
		{
			graphics->SetTexture(m_Textures[m_Materials[index.first].TextureKeyWord]);
		}
		else
		{
			graphics->SetTexture(nullptr);
		}

		// �`��
		graphics->GetContext()->DrawIndexed(
			(UINT)index.second.size(),		// ���_��
			0,						// �I�t�Z�b�g
			0);						// �J�n���_�̃C���f�b�N�X

		count++;
	}
}

bool ObjFile::CreateMesh(std::vector<std::string>& out_material_list, const char* file_path, const char* file_name)
{
	FILE* fp = nullptr;
	fopen_s(&fp, file_name, "r");

	if (fp == nullptr)
	{
		return false;
	}

	std::vector<Vector3> vertices;
	std::vector<Vector3> textures;
	std::vector<Vector3> normals;
	std::map<std::string, int> index_list;
	std::string current_mat_name = "";
	const int LineBufferLength = 1024;
	char buffer[LineBufferLength];

	while (fgets(buffer, LineBufferLength, fp) != nullptr)
	{
		// �R�����g�͖���
		if (buffer[0] == '#')
		{
			continue;
		}

		char* parse_point = strchr(buffer, ' ');
		if (parse_point == nullptr)
		{
			continue;
		}

		Replace('\n', '\0', buffer);

		// ���_�֘A
		if (buffer[0] == 'v')
		{
			// ���_���W
			if (buffer[1] == ' ')
			{
				ParseVKeywordTag(vertices, &parse_point[1]);
			}
			// �e�N�X�`�����W
			else if (buffer[1] == 't')
			{
				ParseVKeywordTag(textures, &parse_point[1]);
				// ���𔽓]������
				//textures[textures.size() - 1].X = (1.0f - textures[textures.size() - 1].X);
				textures[textures.size() - 1].Y = (1.0f - textures[textures.size() - 1].Y);
			}
			// �@�����W
			else if (buffer[1] == 'n')
			{
				ParseVKeywordTag(normals, &parse_point[1]);
			}
		}
		// �ʏ��
		else if (buffer[0] == 'f')
		{
			ParseFKeywordTag(m_Vertices, index_list, current_mat_name, vertices, textures, normals, &parse_point[1]);
		}
		else if (strstr(buffer, "mtllib") == buffer)
		{
			Replace('\n', '\0', buffer);
			// �g�p���Ă���mtl�t�@�C���Ƃ��Ēǉ�
			out_material_list.push_back(&buffer[strlen("mtllib") + 1]);
		}
		else if (strstr(buffer, "usemtl") == buffer)
		{
			Replace('\n', '\0', buffer);
			current_mat_name = &buffer[strlen("usemtl") + 1];		
		}
	}

	fclose(fp);

	return true;
}

bool ObjFile::CreateVertexBuffer(ID3D11Device* device)
{
	//���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(CustomVertex) * (UINT)m_Vertices.size();	// �o�b�t�@�̃T�C�Y
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// �g�p���@
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND�ݒ�
	buffer_desc.CPUAccessFlags = 0;						// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
	buffer_desc.MiscFlags = 0;							// ���\�[�X�I�v�V�����̃t���O
	buffer_desc.StructureByteStride = 0;				// �\���̂̃T�C�Y

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = &m_Vertices[0];	// �o�b�t�@�̒��g�̐ݒ�
	sub_resource.SysMemPitch = 0;			// texture�f�[�^���g�p����ۂɎg�p���郁���o
	sub_resource.SysMemSlicePitch = 0;		// texture�f�[�^���g�p����ۂɎg�p���郁���o

	// �o�b�t�@�쐬
	if (FAILED(device->CreateBuffer(
		&buffer_desc,				// �o�b�t�@���
		&sub_resource,				// ���\�[�X���
		&m_VertexBuffer)))			// �쐬���ꂽ�o�b�t�@�̊i�[��
	{
		return false;
	}

	return true;
}

bool ObjFile::CreateIndexBuffer(ID3D11Device* device)
{
	int count = 0;
	for (auto index : m_Indices)
	{
		m_IndexBuffers.emplace_back();

		//���_�o�b�t�@�쐬
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = sizeof(UINT) * (UINT)index.second.size();	// �o�b�t�@�̃T�C�Y
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;							// �g�p���@
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					// BIND�ݒ�
		buffer_desc.CPUAccessFlags = 0;										// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
		buffer_desc.MiscFlags = 0;											// ���\�[�X�I�v�V�����̃t���O
		buffer_desc.StructureByteStride = 0;								// �\���̂̃T�C�Y

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &index.second[0];							// �o�b�t�@�̒��g�̐ݒ�
		sub_resource.SysMemPitch = 0;										// texture�f�[�^���g�p����ۂɎg�p���郁���o
		sub_resource.SysMemSlicePitch = 0;									// texture�f�[�^���g�p����ۂɎg�p���郁���o

		// �o�b�t�@�쐬
		if (FAILED(device->CreateBuffer(
			&buffer_desc,					// �o�b�t�@���
			&sub_resource,					// ���\�[�X���
			&m_IndexBuffers[count])))		// �쐬���ꂽ�o�b�t�@�̊i�[��
		{
			return false;
		}

		count++;
	}

	return true;
}

bool ObjFile::CreateInputLayout(ID3D11Device* device, VertexShader* vertex_shader)
{
	D3D11_INPUT_ELEMENT_DESC vertex_desc[]{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//���_���C�A�E�g�쐬
	if (FAILED(device->CreateInputLayout(
		vertex_desc,				// ���C�A�E�g�ݒ�
		ARRAYSIZE(vertex_desc),		// �z��T�C�Y
		vertex_shader->GetData(),	// ���C�A�E�g�Ɗ֘A�t���钸�_�V�F�[�_�̃f�[�^
		vertex_shader->GetSize(),	// ���C�A�E�g�Ɗ֘A�t���钸�_�V�F�[�_�̃T�C�Y
		&m_InputLayout)))			// �쐬���ꂽ���_���C�A�E�g�̊i�[��
	{
		return false;
	}

	return true;
}

void ObjFile::ParseVKeywordTag(std::vector<Vector3>& data, char* buff)
{
	std::vector<std::string> split_strings;
	Split(' ', buff, split_strings);

	int count = 0;
	float values[3] = { 0.0f };

	for (std::string str : split_strings)
	{
		values[count] = (float)atof(str.c_str());
		count++;
	}

	data.push_back(Vector3(values[0], values[1], values[2]));
}

void ObjFile::ParseFKeywordTag(std::vector<CustomVertex>& out_custom_vertices, std::map<std::string, int>& index_list, std::string current_material, std::vector<Vector3>& vertices, std::vector<Vector3>& textures, std::vector<Vector3>& normals, char* buffer)
{
	int count = 0;
	int vertex_info[3] =
	{
		-1, -1, -1, 
	};
	std::vector<std::string> space_split;

	Split(' ', buffer, space_split);

	for (int i = 0; i < space_split.size(); i++)
	{
		CustomVertex vertex;
		ParseShashKeywordTag(vertex_info, (char*)space_split[i].c_str());

		for (int i = 0; i < 3; i++)
		{
			if (vertex_info[i] == -1)
			{
				continue;
			}

			int id = vertex_info[i];

			switch (i)
			{
			case 0:
				vertex.Position = vertices[id];
				break;
			case 1:
				vertex.TexturePos = Vector2(textures[id].X, textures[id].Y);
				break;
			case 2:
				vertex.Normal = normals[id];
				break;
			}
		}

// �œK��
#define OPTIMIZATION (0)
#if OPTIMIZATION
		std::string key = "";

		for (int i = 0; i < 3; i++)
		{
			std::ostringstream sout;
			sout << std::setfill('0') << std::setw(5) << vertex_info[i];
			key += sout.str();
		}

		if (index_list.count(key) > 0)
		{
			m_Indices[current_material].push_back(index_list[key]);
		}
		else
		{
			// ���_�o�b�t�@���X�g�ɒǉ�
			out_custom_vertices.push_back(vertex);
			m_Indices[current_material].push_back(out_custom_vertices.size() - 1);
			index_list[key] = out_custom_vertices.size() - 1;
		}
#else
		// ���_�o�b�t�@���X�g�ɒǉ�
		out_custom_vertices.push_back(vertex);
		m_Indices[current_material].push_back((UINT)out_custom_vertices.size() - 1);

#endif
	}
}

void ObjFile::ParseShashKeywordTag(int* list, char* buffer)
{
	int counter = 0;
	std::vector<std::string> slash_split;
	Split('/', buffer, slash_split);

	for (std::string str : slash_split)
	{
		if (str.size() > 0)
		{
			list[counter] = atoi(str.c_str()) - 1;
		}
		counter++;

	}
}

bool ObjFile::LoadMaterialFile(std::vector<std::string> file_list, std::string file_path)
{
	char buffer[1024];

	for (auto mat_file_name : file_list)
	{
		FILE* fp = nullptr;
		std::string name = file_path;
		name += mat_file_name;

		fopen_s(&fp, name.c_str(), "r");

		if (fp == nullptr)
		{
			return false;
		}
		std::string current_material_name = "";

		while (fgets(buffer, 1024, fp) != nullptr)
		{
			if (strstr(buffer, "newmtl") == buffer)
			{
				Replace('\n', '\0', buffer);
				current_material_name = &buffer[strlen("newmtl") + 1];
			}
			else if (strstr(buffer, "map_Kd") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::string texture_name = &buffer[strlen("map_Kd") + 1];
				// �t�@�C���p�X���݂ŕۑ�
				m_Materials[current_material_name].TextureName = file_path + texture_name;

				std::vector<std::string> split;
				Split('.', (char*)texture_name.c_str(), split);
				if (split.size() > 1)
				{
					m_Materials[current_material_name].TextureKeyWord = split[0];
					// �ǂݍ���
					LoadTexture(split[0], m_Materials[current_material_name].TextureName);
				}
			}
			else if (strstr(buffer, "Ka") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::vector<std::string> split;
				Split(' ', &buffer[strlen("Ka") + 1], split);

				for (int i = 0; i < split.size(); i++)
				{
					m_Materials[current_material_name].Ambient[i] = (float)atof(split[i].c_str());
				}
			}
			else if (strstr(buffer, "Kd") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::vector<std::string> split;
				Split(' ', &buffer[strlen("Kd") + 1], split);

				for (int i = 0; i < split.size(); i++)
				{
					m_Materials[current_material_name].Diffuse[i] = (float)atof(split[i].c_str());
				}
			}
			else if (strstr(buffer, "Ks") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::vector<std::string> split;
				Split(' ', &buffer[strlen("Ks") + 1], split);

				for (int i = 0; i < split.size(); i++)
				{
					m_Materials[current_material_name].Specular[i] = (float)atof(split[i].c_str());
				}
			}
		}

		fclose(fp);
	}

	return true;
}

bool ObjFile::LoadTexture(std::string key_word, std::string file_name)
{
	if (m_Textures.count(key_word) > 0 &&
		m_Textures[key_word] != nullptr)
	{
		return true;
	}

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::wstring wstr_file_name = cv.from_bytes(file_name);

	if (FAILED(DirectX::CreateWICTextureFromFile(
		DirectGraphics::GetInstance()->GetDevice(),			// ID3D11Device
		wstr_file_name.c_str(),									// �t�@�C����(wchar_t�^�Ȃ̂Œ���)
		nullptr,											// �ʏ�̃e�N�X�`���f�[�^
		&m_Textures[key_word])))							// �V�F�[�_�Ŏg�p���邱�Ƃ��ł���e�N�X�`���f�[�^
	{
		return false;
	}

	return true;
}

void ObjFile::UpdateWorldMatrix()
{
	// �e������Ȃ�
	if (parent)
	{
		// ���g�̃��[�J���p���Ɛe�̃��[���h�ϊ��s��̊|���Z
		m_WorldMat = m_LocalMat * parent->GetWorldMatrix();
	}
	else
	{
		// ���[�J���s������̂܂܃��[���h�s���
		m_WorldMat = m_LocalMat;
	}
}

void ObjFile::RegistChildObj(ObjFile* spChildObj)
{
	// �ǉ�
	m_ChildObjVector.push_back(spChildObj);

	spChildObj->SetParent(this);
}

