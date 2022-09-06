#ifndef OBJ_FILE_H_
#define OBJ_FILE_H_

#include <vector>
#include <map>
#include <d3d11.h>
#include "../Base/DirectGraphics.h"
#include "../ShaderBase/VertexShader.h"
#include "../Base/GraphicsUtility.h"

class ObjFile
{
public:
	/** Constructor */
	ObjFile() :
		m_VertexBuffer(nullptr),
		m_InputLayout(nullptr)
	{
		m_Vertices.clear();
	}

	/** Destructor */
	~ObjFile()
	{
		for (auto texture : m_Textures)
		{
			if (texture.second != nullptr)
			{
				texture.second->Release();
			}
		}

		if (m_VertexBuffer != nullptr)
		{
			m_VertexBuffer->Release();
			m_VertexBuffer = nullptr;
		}

		for (auto buffer : m_IndexBuffers)
		{
			buffer->Release();
		}
		m_IndexBuffers.clear();

		if (m_InputLayout != nullptr)
		{
			m_InputLayout->Release();
			m_InputLayout = nullptr;
		}
	}

	/**
	* @brief モデル読み込み@n
	* モデルデータを読み込む
	* @return 作成の成否 成功(true)
	* @param[in] device デバイス
	* @param[in] vertex_shader モデルと関連付けるVertexShader
	*/
	bool Load(const char* file_name, ID3D11Device *device, VertexShader* vertex_shader);

	void Update();

	/**
	* @brief モデル描画関数@n
	* 指定された情報でモデルを描画する
	* @return 作成の成否 成功(true)
	* @param[in] graphics 描画管理クラス
	* @param[in] pos 描画座標
	* @param[in] scale 拡縮率
	* @param[in] degree 角度
	*/
	void Render(DirectGraphics* graphics);

	// setter, getter
	void SetPosition(Vector3 position) { this->position = position; dirty = true; }
	Vector3 GetPosition() { return position; dirty = true;};

	void SetRotation(Vector3 rotation) { this->rotation = rotation; dirty = true; }
	Vector3 GetRotation() { return rotation; dirty = true; }

	void SetScale(Vector3 scale) { this->scale = scale; dirty = true; }
	Vector3 GetScale() { return scale; }

private:
	/**
	* @brief メッシュ生成@n
	* ファイルからメッシュを生成する
	* @return 作成の成否 成功(true)
	* @param[in] file_name ファイル名
	*/
	bool CreateMesh(std::vector<std::string>& out_material_list, const char* file_path, const char* file_name);

	/**
	* @brief VertexBuffer作成@n
	* VertexBufferを作成する
	* @return 作成の成否 成功(true)
	* @param[in] device DirectX11グラフィクスデバイス
	*/
	bool CreateVertexBuffer(ID3D11Device* device);

	/**
	* @brief IndexBuffer作成@n
	* IndexBufferを作成する
	* @return 作成の成否 成功(true)
	* @param[in] device DirectX11グラフィクスデバイス
	*/
	bool CreateIndexBuffer(ID3D11Device* device);

	/**
	* @brief InputLayout作成@n
	* InputLayoutを作成する
	* @return 作成の成否 成功(true)
	* @param[in] device DirectX11グラフィクスデバイス
	* @param[in] vertex_shader InputLayoutと関連付ける頂点シェーダ
	*/
	bool CreateInputLayout(ID3D11Device* device, VertexShader* vertex_shader);

	/**
	* @brief InputLayout作成@n
	* InputLayoutを作成する
	* @param[in] device DirectX11グラフィクスデバイス
	* @param[in] vertex_shader InputLayoutと関連付ける頂点シェーダ
	*/
	void ParseVKeywordTag(std::vector<Vector3>& out_vertices, char* buff);

	/**
	* @brief InputLayout作成@n
	* InputLayoutを作成する
	* @param[in] device DirectX11グラフィクスデバイス
	* @param[in] vertex_shader InputLayoutと関連付ける頂点シェーダ
	*/
	void ParseFKeywordTag(std::vector<CustomVertex>& out_custom_vertices, std::map<std::string, int>& index_list, std::string current_material_name, std::vector<Vector3>& vertices, std::vector<Vector3>& textures, std::vector<Vector3>& normals, char* buffer);

	/**
	* @brief InputLayout作成@n
	* InputLayoutを作成する
	* @param[in] device DirectX11グラフィクスデバイス
	* @param[in] vertex_shader InputLayoutと関連付ける頂点シェーダ
	*/
	void ParseShashKeywordTag(int* list, char* buffer);

	bool LoadMaterialFile(std::vector<std::string> file_list, std::string file_path);

	bool LoadTexture(std::string key_word, std::string file_name);

private:
	ID3D11Buffer* m_VertexBuffer;								//!< @brief 頂点バッファ(Shader送信用)
	std::vector<ID3D11Buffer*> m_IndexBuffers;					//!< @brief インデックスバッファ(Shader送信用)
	ID3D11InputLayout* m_InputLayout;							//!< @brief 入力レイアウト
	std::vector<CustomVertex> m_Vertices;						//!< @brief 頂点バッファ
	std::map <std::string, std::vector<UINT>> m_Indices;		//!< @brief インデックスバッファ
	std::map<std::string, ObjMaterial> m_Materials;
	std::map< std::string, ID3D11ShaderResourceView*> m_Textures;

	Vector3 position = Vector3(0.0f, 0.0f, 0.0f);	// 座標
	Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);	// 角度
	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);		// 大きさ

	bool dirty = false;

protected:
	DirectX::XMMATRIX m_WorldMat = DirectX::XMMatrixIdentity();   // ワールド空間を基準とした時の姿勢
	DirectX::XMMATRIX m_LocalMat = DirectX::XMMatrixIdentity();   // 自身の親空間を基準とした時の姿勢
	std::vector<ObjFile*> m_ChildObjVector;

	ObjFile* parent = nullptr;

public:		// 親子関係
	void UpdateWorldMatrix();  // 親空間のワールド姿勢から自身の姿勢を算出
	DirectX::XMMATRIX GetWorldMatrix() { return m_WorldMat; };   // ワールド変換行列を取得

	void RegistChildObj(ObjFile* spChildObj);   // 子オブジェクトを登録
	void SetParent(ObjFile* object) { parent = object; }	// 親オブジェクトのポインター取得

};

#endif
