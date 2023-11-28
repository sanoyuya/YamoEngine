#include "Building.h"

void Building::Initialize()
{
	builing_ = std::make_unique<Model>();
	buildingTex_ = builing_->CreateObjModel("Resources/building2");
	builing_->SetModel(buildingTex_);
	buildingTrans_.Initialize();

	buildingBack_= std::make_unique<Model>();
	buildingBackTex_ = builing_->CreateObjModel("Resources/cube");
	buildingBack_->SetModel(buildingBackTex_);
	buildingBackTrans_.Initialize();
}

void Building::Update()
{
	if (isSet == false)
	{
		buildingBackTrans_.translation = { buildingTrans_.translation.x,buildingTrans_.translation.y + buildingSize_.y,buildingTrans_.translation.z };
		buildingBackTrans_.scale = { buildingTrans_.scale.x * buildingSize_.x - backCubeSubSize_,buildingTrans_.scale.y * buildingSize_.y - backCubeSubSize_,buildingTrans_.scale.z * buildingSize_.z - backCubeSubSize_ };
		buildingBackTrans_.rotation = buildingTrans_.rotation;
		isSet = true;
	}

	buildingTrans_.TransUpdate(sCamera_);
	buildingBackTrans_.TransUpdate(sCamera_);
}

void Building::Draw()
{
	buildingBack_->DrawModel(&buildingBackTrans_, { 78.0f / 255.0f, 175.0f / 255.0f, 254.0f / 255.0f,1.0f });
	builing_->DrawModel(&buildingTrans_);
}

std::string Building::GetName()
{
	return name_;
}

const Transform& Building::GetTransform()
{
	return buildingTrans_;
}

const CollisionData& Building::GetCollisionData()
{
	return collisionData_;
}

void Building::SetPosition(const myMath::Vector3& position)
{
	buildingTrans_.translation = position;
}

void Building::SetRotation(const myMath::Vector3& rotation)
{
	buildingTrans_.rotation = rotation;
}

void Building::SetScale(const myMath::Vector3& scale)
{
	buildingTrans_.scale = scale;
}

void Building::SetColliderSize(const float size)
{
	collisionData_.radius = size;
}