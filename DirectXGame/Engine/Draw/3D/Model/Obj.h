#pragma once
#include"DrawCommon.h"
#include"DirectXBase.h"
#include<vector>
#include"DrawOversight.h"
#include <unordered_map>
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"ConstantBuffer.h"
#include"Model.h"

class Obj
{
public:

	static void Create(const char* filePath, bool smoothing, ModelData* data, bool flipX = false, bool flipY = false, bool flipZ = false);

private:

	static void LoadMaterial(const std::string& directoryPath, const std::string& filename, ModelData* data);
	static void LoadTexture(const std::string& directoryPath, const std::string& filename, ModelData* data);
	static void CalculateSmoothedVertexNormals(ModelData* data);
	static int8_t GetFlip(bool flip);

	//コピーコンストラクタ・代入演算子削除
	Obj& operator=(const Obj&) = delete;
	Obj(const Obj&) = delete;
};