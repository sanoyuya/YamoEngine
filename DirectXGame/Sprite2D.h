#pragma once
#include"myMath.h"
#include"DirectX_.h"
#include"WindowsApp.h"
#include"TextureManager.h"
#include"DrawCommon.h"
#include<array>
#include"Pipeline.h"
#include"IndexBuffer.h"
#include"VertexBuffer.h"
#include"ConstantBuffer.h"
#include"SpriteCommon.h"

class Sprite2D
{
private:

	HRESULT result;
	char PADING[4];

	//頂点バッファ
	std::unique_ptr<VertexBuffer> vertexBuffer;
	//頂点マップ
	PosUvColor* vertMap;

	//インデックスバッファ
	std::unique_ptr<IndexBuffer> indexBuffer;

	//インデックスバッファをマッピング
	uint16_t* indexMap;

	// 定数バッファ
	std::unique_ptr<ConstantBuffer> constBuffMaterial;

	//定数バッファのマッピング用ポインタ
	myMath::Matrix4 constBuffMap;

	//プロジェクション行列
	static myMath::Matrix4 matProjection;

	int blendMode = (int)BlendMode::Alpha;//初期値半透明合成

	TextureData* texture;

public:

	Sprite2D() {}
	virtual ~Sprite2D() {}

	static void StaticInitialize();

	/// <summary>
	/// スプライトの初期化処理
	/// </summary>
	void Sprite2DInitialize(uint32_t handle);

	/// <summary>
	/// スプライトの描画
	/// </summary>
	/// <param name="textureData">テクスチャデータ</param>
	/// <param name="position">座標</param>
	/// <param name="color">色</param>
	/// <param name="scale">大きさ</param>
	/// <param name="rotation">回転</param>
	/// <param name="anchorpoint">中心点</param>
	/// <param name="flipX">X反転</param>
	/// <param name="flipY">Y反転</param>
	void DrawSprite2D(myMath::Vector2 position, myMath::Vector4 color = { 1.0f,1.0f ,1.0f ,1.0f }, myMath::Vector2 scale = { 1.0f,1.0f }, float rotation = 0.0f, myMath::Vector2 anchorpoint = { 0.5f,0.5f }, bool flipX = false, bool flipY = false);

	/// <summary>
	/// スプライトの切り抜き描画
	/// </summary>
	/// <param name="textureData">テクスチャデータ</param>
	/// <param name="position">座標</param>
	/// <param name="clipCenter">切り取る中心座標</param>
	/// <param name="clipRadius">切り取る大きさ(半径)</param>
	/// <param name="color">色</param> 
	/// <param name="scale">大きさ</param> 
	/// <param name="rotation">回転</param>
	/// <param name="flipX">X反転</param>
	/// <param name="flipY">Y反転</param>
	void DrawSpriteClip2D(myMath::Vector2 position, myMath::Vector2 clipCenter, myMath::Vector2 clipRadius, myMath::Vector4 color = { 1.0f,1.0f ,1.0f ,1.0f }, myMath::Vector2 scale = { 1.0f,1.0f }, float rotation = 0.0f, bool flipX = false, bool flipY = false);

	/// <summary>
	/// 横連番スプライト描画
	/// </summary>
	/// <param name="textureData">テクスチャデータ</param>
	/// <param name="position">座標</param>
	/// <param name="sheetsNum">枚数</param>
	/// <param name="nowNum">何枚目か</param>
	/// <param name="color">色</param>
	/// <param name="scale">大きさ</param>
	/// <param name="rotation">回転</param>
	/// <param name="anchorpoint">中心点</param>
	/// <param name="flipX">X反転</param>
	/// <param name="flipY">Y反転</param>
	void DrawAnimationSpriteX2D(myMath::Vector2 position, uint16_t sheetsNum, uint16_t& nowNum, myMath::Vector4 color = { 1.0f,1.0f ,1.0f ,1.0f }, myMath::Vector2 scale = { 1.0f,1.0f }, float rotation = 0.0f, myMath::Vector2 anchorpoint = { 0.5f,0.5f }, bool flipX = false, bool flipY = false);

	/// <summary>
	/// 縦連番スプライト描画
	/// </summary>
	/// <param name="textureData">テクスチャデータ</param>
	/// <param name="position">座標</param>
	/// <param name="sheetsNum">枚数</param>
	/// <param name="nowNum">何枚目か</param>
	/// <param name="color">色</param>
	/// <param name="scale">大きさ</param>
	/// <param name="rotation">回転</param>
	/// <param name="anchorpoint">中心点</param>
	/// <param name="flipX">X反転</param>
	/// <param name="flipY">Y反転</param>
	void DrawAnimationSpriteY2D(myMath::Vector2 position, uint16_t sheetsNum, uint16_t& nowNum, myMath::Vector4 color = { 1.0f,1.0f ,1.0f ,1.0f }, myMath::Vector2 scale = { 1.0f,1.0f }, float rotation = 0.0f, myMath::Vector2 anchorpoint = { 0.5f,0.5f }, bool flipX = false, bool flipY = false);

	/// <summary>
	/// 縦横連番スプライト描画
	/// </summary>
	/// <param name="textureData">テクスチャデータ</param>
	/// <param name="position">座標</param>
	/// <param name="sheetsNumX">横の枚数</param>
	/// <param name="sheetsNumY">縦の枚数</param>
	/// <param name="nowNum">何枚目か</param>
	/// <param name="color">色</param>
	/// <param name="scale">大きさ</param>
	/// <param name="rotation">回転</param>
	/// <param name="anchorpoint">中心点</param>
	/// <param name="flipX">X反転</param>
	/// <param name="flipY">Y反転</param>
	void DrawAnimationSpriteXY2D(myMath::Vector2 position, uint16_t sheetsNumX, uint16_t sheetsNumY, uint16_t& nowNum, myMath::Vector4 color = { 1.0f,1.0f ,1.0f ,1.0f }, myMath::Vector2 scale = { 1.0f,1.0f }, float rotation = 0.0f, myMath::Vector2 anchorpoint = { 0.5f,0.5f }, bool flipX = false, bool flipY = false);

	/// <summary>
	/// ブレンドモードのセット
	/// </summary>
	/// <param name="mode">モード</param>
	void SetSprite2DBlendMode(BlendMode mode);

private:
	void CreateVertexIndexBuffer();
	void CreateConstBuff();
	void Update(myMath::Vector2 position, myMath::Vector2 scale, float rotation);
};