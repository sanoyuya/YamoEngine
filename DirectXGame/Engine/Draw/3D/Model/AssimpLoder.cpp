#include "AssimpLoder.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<assimp/cimport.h>
#include <filesystem>
#include"myMath.h"

std::wstring GetDirectoryPath(const std::wstring& origin)
{
	std::filesystem::path p = origin.c_str();
	return p.remove_filename().c_str();
}

std::string ToUTF8(const std::wstring& value)
{
	auto length = WideCharToMultiByte(
		CP_UTF8, 0U, value.data(), -1, nullptr, 0, nullptr,nullptr);

	auto buffer = new char[length];

	WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, buffer, length, nullptr, nullptr);

	std::string result(buffer);
	delete[] buffer;
	buffer = nullptr;

	return result;
}

std::wstring ToWideString(const std::string& str)
{
	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(num1);

#ifdef _DEBUG
	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);
#endif // _DEBUG

	return wstr;
}

bool AssimpLoder::Load(ImportSettings settings, ModelData* data)
{
	//書いてないならスキップ
	if (settings.filename == nullptr)
	{
		return false;
	}

	auto& meshs = settings.meshes;
	/*auto inverseU = settings.inverseU;
	auto inverseV = settings.inverseV;*/

	std::string path = "Resources/" + ToUTF8(settings.filename) + "/" + ToUTF8(settings.filename) + ".gltf";

	Assimp::Importer importer;

	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;

	auto scene = importer.ReadFile(path, flag);


	if (scene == nullptr)
	{
		//エラーがでたら表示
		OutputDebugStringA(importer.GetErrorString());
		assert(0);
		return false;
	}

	//読み込んだデータをMesh構造体に変換
	meshs.clear();
	meshs.resize(scene->mNumMeshes);
	for (size_t i = 0; i < meshs.size(); i++)
	{
		const auto pMesh = scene->mMeshes[i];
		LoadMesh(meshs[i], pMesh, settings.inverseU, settings.inverseV, data);
		const auto pMaterial = scene->mMaterials[i];
		LoadTexture(settings.filename, meshs[i], pMaterial, data);
	}

	scene = nullptr;

	return true;
}

void AssimpLoder::LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV, ModelData* data)
{
	aiVector3D zero3D(0.0f, 0.0f, 0.0f);
	aiColor4D zeroColor(0.0f, 0.0f, 0.0f, 0.0f);

	dst.vertices.resize(src->mNumVertices);

	for (auto i = 0u; i < src->mNumVertices; ++i)
	{
		auto position = &(src->mVertices[i]);
		auto normal = &(src->mNormals[i]);
		auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;
	/*	auto tangent = (src->HasTangentsAndBitangents()) ? &(src->mTangents[i]) : &zero3D;
		auto color = (src->HasVertexColors(0)) ? &(src->mColors[0][i]) : &zeroColor;*/

		// 反転オプションがあったらUVを反転させる
		if (inverseU)
		{
			uv->x = 1 - uv->x;
		}
		if (inverseV)
		{
			uv->y = 1 - uv->y;
		}

		PosNormalUv vertex = {};
		vertex.pos = myMath::Vector3(position->x, position->y, position->z);
		vertex.normal = myMath::Vector3(normal->x, normal->y, normal->z);
		vertex.uv = myMath::Vector2(uv->x, uv->y);
	/*	vertex.tangent = myMath::Vector3(tangent->x, tangent->y, tangent->z);
		vertex.color = myMath::Vector4(color->r, color->g, color->b, color->a);*/

		dst.vertices[i] = vertex;
		data->vertices.push_back(vertex);
	}

	dst.indices.resize(src->mNumFaces * 3);

	for (auto i = 0u; i < src->mNumFaces; ++i)
	{
		const auto& face = src->mFaces[i];

		dst.indices[i * 3 + 0] = static_cast<uint32_t>(face.mIndices[0]);
		dst.indices[i * 3 + 1] = static_cast<uint32_t>(face.mIndices[1]);
		dst.indices[i * 3 + 2] = static_cast<uint32_t>(face.mIndices[2]);
	}

	data->maxVert = static_cast<uint32_t>(data->vertices.size());
	data->maxIndex = static_cast<uint32_t>(dst.indices.size());

	//頂点バッファ・インデックス生成
	data->vertexBuffer = std::make_unique<VertexBuffer>();
	data->vertexBuffer->Create(data->maxVert, sizeof(PosNormalUv));

	data->indexBuffer = std::make_unique<IndexBuffer>();
	data->indexBuffer->Create(data->maxIndex);

	//頂点バッファへのデータ転送
	data->vertexBuffer->Update(data->vertices.data());

	//インデックスバッファへのデータ転送
	data->indexBuffer->Update(dst.indices.data());

	data->constBuffMaterial = std::make_unique<ConstantBuffer>();
	data->constBuffMaterial->Create(sizeof(ConstBuffDataMaterial));
	ConstBuffDataMaterial tmpMaterial = data->modelMaterial.GetConstBuffMaterial();
	data->constBuffMaterial->Update(&tmpMaterial);
}

void AssimpLoder::LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src, ModelData* data)
{
	aiString path;
	if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
	{
		// テクスチャパスは相対パスで入っているので、ファイルの場所とくっつける
		auto dir = GetDirectoryPath(filename);
		auto file = std::string(path.C_Str());
		dst.diffuseMap = dir + ToWideString(file);

		std::string filepath = "Resources/" + ToUTF8(filename) + "/" + ToUTF8(filename) + ".png";
		uint32_t handl = TextureManager::Load(filepath);
		data->textureData = TextureManager::GetTextureData(handl);
	}
	else
	{
		dst.diffuseMap.clear();
	}
}