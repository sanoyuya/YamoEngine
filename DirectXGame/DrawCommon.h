#pragma once
#include"myMath.h"

struct PosUvColor
{
	myMath::Vector3 pos;//ÀW
	myMath::Vector2 uv;//uvÀW
	myMath::Vector4 color;//F
};

enum class BlendMode
{
	None,//m[uh
	Alpha,//¼§¾¬
	Add,//ÁZ¬
	Sub,//¸Z¬
	Mul,//æZ¬
	Inv//F½]
};

class DrawCommon
{
};