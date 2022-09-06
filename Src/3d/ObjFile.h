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
	* @brief ���f���ǂݍ���@n
	* ���f���f�[�^��ǂݍ���
	* @return �쐬�̐��� ����(true)
	* @param[in] device �f�o�C�X
	* @param[in] vertex_shader ���f���Ɗ֘A�t����VertexShader
	*/
	bool Load(const char* file_name, ID3D11Device *device, VertexShader* vertex_shader);

	void Update();

	/**
	* @brief ���f���`��֐�@n
	* �w�肳�ꂽ���Ń��f����`�悷��
	* @return �쐬�̐��� ����(true)
	* @param[in] graphics �`��Ǘ��N���X
	* @param[in] pos �`����W
	* @param[in] scale �g�k��
	* @param[in] degree �p�x
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
	* @brief ���b�V������@n
	* �t�@�C�����烁�b�V���𐶐�����
	* @return �쐬�̐��� ����(true)
	* @param[in] file_name �t�@�C����
	*/
	bool CreateMesh(std::vector<std::string>& out_material_list, const char* file_path, const char* file_name);

	/**
	* @brief VertexBuffer�쐬@n
	* VertexBuffer���쐬����
	* @return �쐬�̐��� ����(true)
	* @param[in] device DirectX11�O���t�B�N�X�f�o�C�X
	*/
	bool CreateVertexBuffer(ID3D11Device* device);

	/**
	* @brief IndexBuffer�쐬@n
	* IndexBuffer���쐬����
	* @return �쐬�̐��� ����(true)
	* @param[in] device DirectX11�O���t�B�N�X�f�o�C�X
	*/
	bool CreateIndexBuffer(ID3D11Device* device);

	/**
	* @brief InputLayout�쐬@n
	* InputLayout���쐬����
	* @return �쐬�̐��� ����(true)
	* @param[in] device DirectX11�O���t�B�N�X�f�o�C�X
	* @param[in] vertex_shader InputLayout�Ɗ֘A�t���钸�_�V�F�[�_
	*/
	bool CreateInputLayout(ID3D11Device* device, VertexShader* vertex_shader);

	/**
	* @brief InputLayout�쐬@n
	* InputLayout���쐬����
	* @param[in] device DirectX11�O���t�B�N�X�f�o�C�X
	* @param[in] vertex_shader InputLayout�Ɗ֘A�t���钸�_�V�F�[�_
	*/
	void ParseVKeywordTag(std::vector<Vector3>& out_vertices, char* buff);

	/**
	* @brief InputLayout�쐬@n
	* InputLayout���쐬����
	* @param[in] device DirectX11�O���t�B�N�X�f�o�C�X
	* @param[in] vertex_shader InputLayout�Ɗ֘A�t���钸�_�V�F�[�_
	*/
	void ParseFKeywordTag(std::vector<CustomVertex>& out_custom_vertices, std::map<std::string, int>& index_list, std::string current_material_name, std::vector<Vector3>& vertices, std::vector<Vector3>& textures, std::vector<Vector3>& normals, char* buffer);

	/**
	* @brief InputLayout�쐬@n
	* InputLayout���쐬����
	* @param[in] device DirectX11�O���t�B�N�X�f�o�C�X
	* @param[in] vertex_shader InputLayout�Ɗ֘A�t���钸�_�V�F�[�_
	*/
	void ParseShashKeywordTag(int* list, char* buffer);

	bool LoadMaterialFile(std::vector<std::string> file_list, std::string file_path);

	bool LoadTexture(std::string key_word, std::string file_name);

private:
	ID3D11Buffer* m_VertexBuffer;								//!< @brief ���_�o�b�t�@(Shader���M�p)
	std::vector<ID3D11Buffer*> m_IndexBuffers;					//!< @brief �C���f�b�N�X�o�b�t�@(Shader���M�p)
	ID3D11InputLayout* m_InputLayout;							//!< @brief ���̓��C�A�E�g
	std::vector<CustomVertex> m_Vertices;						//!< @brief ���_�o�b�t�@
	std::map <std::string, std::vector<UINT>> m_Indices;		//!< @brief �C���f�b�N�X�o�b�t�@
	std::map<std::string, ObjMaterial> m_Materials;
	std::map< std::string, ID3D11ShaderResourceView*> m_Textures;

	Vector3 position = Vector3(0.0f, 0.0f, 0.0f);	// ���W
	Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);	// �p�x
	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);		// �傫��

	bool dirty = false;

protected:
	DirectX::XMMATRIX m_WorldMat = DirectX::XMMatrixIdentity();   // ���[���h��Ԃ���Ƃ������̎p��
	DirectX::XMMATRIX m_LocalMat = DirectX::XMMatrixIdentity();   // ���g�̐e��Ԃ���Ƃ������̎p��
	std::vector<ObjFile*> m_ChildObjVector;

	ObjFile* parent = nullptr;

public:		// �e�q�֌W
	void UpdateWorldMatrix();  // �e��Ԃ̃��[���h�p�����玩�g�̎p�����Z�o
	DirectX::XMMATRIX GetWorldMatrix() { return m_WorldMat; };   // ���[���h�ϊ��s����擾

	void RegistChildObj(ObjFile* spChildObj);   // �q�I�u�W�F�N�g��o�^
	void SetParent(ObjFile* object) { parent = object; }	// �e�I�u�W�F�N�g�̃|�C���^�[�擾

};

#endif
