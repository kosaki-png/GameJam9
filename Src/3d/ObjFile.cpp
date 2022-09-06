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

	// ローカルマトリクス設定
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(position.X, position.Y, position.Z);
	DirectX::XMMATRIX rotate_x = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.X));
	DirectX::XMMATRIX rotate_y = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.Y + tmp));
	DirectX::XMMATRIX rotate_z = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.Z));
	DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(scale.X, scale.Y, scale.Z);
	m_LocalMat = scale_mat * rotate_x * rotate_y * rotate_z * translate;

	// ワールド行列の算出
	UpdateWorldMatrix();
	
}

void ObjFile::Render(DirectGraphics* graphics)
{
	graphics->SetUpContext();
	int count = 0;
	UINT strides = sizeof(CustomVertex);
	UINT offsets = 0;

	// インデックスバッファの数 = マテリアルの数だけメッシュを描画する
	for (auto index : m_Indices)
	{
		// IA(InputAssemblerStage)に入力レイアウトを設定する
		graphics->GetContext()->IASetInputLayout(m_InputLayout);
		// IAに設定する頂点バッファの指定
		graphics->GetContext()->IASetVertexBuffers(
			0,						// バッファ送信のスロット番号
			1,						// バッファの数
			&m_VertexBuffer,		// 頂点バッファ
			&strides,				// バッファに使用している構造体のサイズ
			&offsets);				// 開始オフセット

		graphics->GetContext()->IASetIndexBuffer(
			m_IndexBuffers[count],
			DXGI_FORMAT_R32_UINT,
			0);

		// ワールドマトリクスをコンスタントバッファに設定
		XMStoreFloat4x4(&graphics->GetConstantBufferData()->World, XMMatrixTranspose(m_WorldMat));

		graphics->SetMaterial(&m_Materials[index.first]);

		// コンスタントバッファ更新
		graphics->GetContext()->UpdateSubresource(graphics->GetConstantBuffer(), 0, NULL, graphics->GetConstantBufferData(), 0, 0);

		ID3D11Buffer* constant_buffer = graphics->GetConstantBuffer();
		// コンテキストにコンスタントバッファを設定
		graphics->GetContext()->VSSetConstantBuffers(0, 1, &constant_buffer);
		graphics->GetContext()->PSSetConstantBuffers(0, 1, &constant_buffer);

		// マテリアルにテクスチャがあるなら設定する
		if (m_Textures.count(m_Materials[index.first].TextureKeyWord) > 0)
		{
			graphics->SetTexture(m_Textures[m_Materials[index.first].TextureKeyWord]);
		}
		else
		{
			graphics->SetTexture(nullptr);
		}

		// 描画
		graphics->GetContext()->DrawIndexed(
			(UINT)index.second.size(),		// 頂点数
			0,						// オフセット
			0);						// 開始頂点のインデックス

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
		// コメントは無視
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

		// 頂点関連
		if (buffer[0] == 'v')
		{
			// 頂点座標
			if (buffer[1] == ' ')
			{
				ParseVKeywordTag(vertices, &parse_point[1]);
			}
			// テクスチャ座標
			else if (buffer[1] == 't')
			{
				ParseVKeywordTag(textures, &parse_point[1]);
				// 軸を反転させる
				//textures[textures.size() - 1].X = (1.0f - textures[textures.size() - 1].X);
				textures[textures.size() - 1].Y = (1.0f - textures[textures.size() - 1].Y);
			}
			// 法線座標
			else if (buffer[1] == 'n')
			{
				ParseVKeywordTag(normals, &parse_point[1]);
			}
		}
		// 面情報
		else if (buffer[0] == 'f')
		{
			ParseFKeywordTag(m_Vertices, index_list, current_mat_name, vertices, textures, normals, &parse_point[1]);
		}
		else if (strstr(buffer, "mtllib") == buffer)
		{
			Replace('\n', '\0', buffer);
			// 使用しているmtlファイルとして追加
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
	//頂点バッファ作成
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(CustomVertex) * (UINT)m_Vertices.size();	// バッファのサイズ
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// 使用方法
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND設定
	buffer_desc.CPUAccessFlags = 0;						// リソースへのCPUのアクセス権限についての設定
	buffer_desc.MiscFlags = 0;							// リソースオプションのフラグ
	buffer_desc.StructureByteStride = 0;				// 構造体のサイズ

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = &m_Vertices[0];	// バッファの中身の設定
	sub_resource.SysMemPitch = 0;			// textureデータを使用する際に使用するメンバ
	sub_resource.SysMemSlicePitch = 0;		// textureデータを使用する際に使用するメンバ

	// バッファ作成
	if (FAILED(device->CreateBuffer(
		&buffer_desc,				// バッファ情報
		&sub_resource,				// リソース情報
		&m_VertexBuffer)))			// 作成されたバッファの格納先
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

		//頂点バッファ作成
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = sizeof(UINT) * (UINT)index.second.size();	// バッファのサイズ
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;							// 使用方法
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					// BIND設定
		buffer_desc.CPUAccessFlags = 0;										// リソースへのCPUのアクセス権限についての設定
		buffer_desc.MiscFlags = 0;											// リソースオプションのフラグ
		buffer_desc.StructureByteStride = 0;								// 構造体のサイズ

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &index.second[0];							// バッファの中身の設定
		sub_resource.SysMemPitch = 0;										// textureデータを使用する際に使用するメンバ
		sub_resource.SysMemSlicePitch = 0;									// textureデータを使用する際に使用するメンバ

		// バッファ作成
		if (FAILED(device->CreateBuffer(
			&buffer_desc,					// バッファ情報
			&sub_resource,					// リソース情報
			&m_IndexBuffers[count])))		// 作成されたバッファの格納先
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

	//頂点レイアウト作成
	if (FAILED(device->CreateInputLayout(
		vertex_desc,				// レイアウト設定
		ARRAYSIZE(vertex_desc),		// 配列サイズ
		vertex_shader->GetData(),	// レイアウトと関連付ける頂点シェーダのデータ
		vertex_shader->GetSize(),	// レイアウトと関連付ける頂点シェーダのサイズ
		&m_InputLayout)))			// 作成された頂点レイアウトの格納先
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

// 最適化
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
			// 頂点バッファリストに追加
			out_custom_vertices.push_back(vertex);
			m_Indices[current_material].push_back(out_custom_vertices.size() - 1);
			index_list[key] = out_custom_vertices.size() - 1;
		}
#else
		// 頂点バッファリストに追加
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
				// ファイルパス込みで保存
				m_Materials[current_material_name].TextureName = file_path + texture_name;

				std::vector<std::string> split;
				Split('.', (char*)texture_name.c_str(), split);
				if (split.size() > 1)
				{
					m_Materials[current_material_name].TextureKeyWord = split[0];
					// 読み込み
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
		wstr_file_name.c_str(),									// ファイル名(wchar_t型なので注意)
		nullptr,											// 通常のテクスチャデータ
		&m_Textures[key_word])))							// シェーダで使用することができるテクスチャデータ
	{
		return false;
	}

	return true;
}

void ObjFile::UpdateWorldMatrix()
{
	// 親がいるなら
	if (parent)
	{
		// 自身のローカル姿勢と親のワールド変換行列の掛け算
		m_WorldMat = m_LocalMat * parent->GetWorldMatrix();
	}
	else
	{
		// ローカル行列をそのままワールド行列へ
		m_WorldMat = m_LocalMat;
	}
}

void ObjFile::RegistChildObj(ObjFile* spChildObj)
{
	// 追加
	m_ChildObjVector.push_back(spChildObj);

	spChildObj->SetParent(this);
}

