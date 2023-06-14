#pragma once
#include <unordered_map>
#include"DrawCommon.h"
#include"TextureManager.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"ConstantBuffer.h"
#include"myMath.h"

struct Vertex
{
	myMath::Vector3 position; // �ʒu���W
	myMath::Vector3 normal; // �@��
	myMath::Vector2 uv; // uv���W
	myMath::Vector3 tangent; // �ڋ��
	myMath::Vector4 color; // ���_�F
};

struct Mesh
{
	std::vector<Vertex> vertices;	//���_�f�[�^�z��
	std::vector<uint16_t> indices;	//�C���f�b�N�X�z��
	std::wstring diffuseMap;		//�e�N�X�`���̃t�@�C��
};

struct aiMesh;
struct aiMaterial;

struct ImportSettings
{
	const wchar_t* filename = nullptr;	//�t�@�C���p�X
	std::vector<Mesh>& meshes;			//�o�͐�̃��b�V���z��
	bool inverseU = false;				//U���W�𔽓]�����邩
	bool inverseV = false;				//V���W�𔽓]�����邩
};

class AssimpLoder
{
public:

	static bool Load(ImportSettings setting); // ���f�������[�h����

private:

	static void LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV);
	static void LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src);
};