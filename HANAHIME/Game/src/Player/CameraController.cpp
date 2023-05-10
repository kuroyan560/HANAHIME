#include "CameraController.h"
#include"Render/RenderObject/Camera.h"
#include"../../../../src/engine/ForUser/Object/Model.h"
#include"CollisionDetectionOfRayAndMesh.h"

void CameraController::OnImguiItems()
{
	ImGui::Text("NowParameter");
	ImGui::SameLine();

	//パラメータ初期化ボタン
	if (ImGui::Button("Initialize"))
	{
		m_nowParam = m_initializedParam;
	}

	//現在のパラメータ表示
	if (ImGui::BeginChild("NowParam"))
	{
		ImGui::Text("posOffsetZ : %.2f", m_nowParam.m_posOffsetZ);
		float degree = static_cast<float>(KuroEngine::Angle::ConvertToDegree(m_nowParam.m_xAxisAngle));
		ImGui::Text("xAxisAngle : %.2f", degree);
		degree = static_cast<float>(KuroEngine::Angle::ConvertToDegree(m_nowParam.m_yAxisAngle));
		ImGui::Text("yAxisAngle : %.2f", degree);
		ImGui::EndChild();
	}

}

CameraController::CameraController()
	:KuroEngine::Debugger("CameraController", true, true)
{
	AddCustomParameter("posOffsetZ", { "InitializedParameter","posOffsetZ" }, PARAM_TYPE::FLOAT, &m_initializedParam.m_posOffsetZ, "InitializedParameter");
	AddCustomParameter("xAxisAngle", { "InitializedParameter","xAxisAngle" }, PARAM_TYPE::FLOAT, &m_initializedParam.m_xAxisAngle, "InitializedParameter");

	AddCustomParameter("gazePointOffset", { "gazePointOffset" }, PARAM_TYPE::FLOAT_VEC3, &m_gazePointOffset, "UpdateParameter");
	AddCustomParameter("posOffsetDepthMin", { "posOffsetDepth","min" }, PARAM_TYPE::FLOAT, &m_posOffsetDepthMin, "UpdateParameter");
	AddCustomParameter("posOffsetDepthMax", { "posOffsetDepth","max" }, PARAM_TYPE::FLOAT, &m_posOffsetDepthMax, "UpdateParameter");
	AddCustomParameter("xAxisAngleMin", { "xAxisAngle","min" }, PARAM_TYPE::FLOAT, &m_xAxisAngleMin, "UpdateParameter");
	AddCustomParameter("xAxisAngleMax", { "xAxisAngle","max" }, PARAM_TYPE::FLOAT, &m_xAxisAngleMax, "UpdateParameter");
	AddCustomParameter("camFowardPosLerpRate", { "PosLerpRate" }, PARAM_TYPE::FLOAT, &m_camForwardPosLerpRate, "UpdateParameter");
	AddCustomParameter("camFollowLerpRate", { "FollowLerpRate" }, PARAM_TYPE::FLOAT, &m_camFollowLerpRate, "UpdateParameter");

	LoadParameterLog();
}

void CameraController::AttachCamera(std::shared_ptr<KuroEngine::Camera> arg_cam)
{
	//操作対象となるカメラのポインタを保持
	m_attachedCam = arg_cam;
	//コントローラーのトランスフォームを親として設定
	m_cameraLocalTransform.SetParent(&m_camParentTransform);
}

void CameraController::Init()
{
	m_nowParam = m_initializedParam;
	m_verticalControl = ANGLE;
}

void CameraController::Update(KuroEngine::Vec3<float>arg_scopeMove, KuroEngine::Transform arg_targetPos, float arg_playerRotY, float arg_cameraZ, const std::weak_ptr<Stage>arg_nowStage)
{
	using namespace KuroEngine;

	//カメラがアタッチされていない
	if (m_attachedCam.expired())return;

	//トランスフォームを保存。
	m_oldCameraWorldPos = m_cameraLocalTransform.GetPosWorldByMatrix();

	//左右カメラ操作
	m_nowParam.m_yAxisAngle = arg_playerRotY;

	//上下カメラ操作
	switch (m_verticalControl)
	{
	case ANGLE:
		m_nowParam.m_xAxisAngle -= arg_scopeMove.y * 0.3f;
		//if (m_nowParam.m_xAxisAngle <= m_xAxisAngleMin)m_verticalControl = DIST;
		break;

	case DIST:
		m_nowParam.m_posOffsetZ += arg_scopeMove.y * 6.0f;
		if (m_nowParam.m_posOffsetZ <= m_posOffsetDepthMin)m_verticalControl = ANGLE;
		break;
	}

	//上限値超えないようにする
	m_nowParam.m_posOffsetZ = arg_cameraZ;
	m_nowParam.m_xAxisAngle = std::clamp(m_nowParam.m_xAxisAngle, m_xAxisAngleMin, m_xAxisAngleMax);


	//操作するカメラのトランスフォーム（前後移動）更新
	Vec3<float> localPos = { 0,0,0 };
	localPos.z = m_nowParam.m_posOffsetZ;
	localPos.y = m_gazePointOffset.y + tan(-m_nowParam.m_xAxisAngle) * m_nowParam.m_posOffsetZ;
	m_cameraLocalTransform.SetPos(Math::Lerp(m_cameraLocalTransform.GetPos(), localPos, m_camForwardPosLerpRate));
	m_cameraLocalTransform.SetRotate(Vec3<float>::GetXAxis(), m_nowParam.m_xAxisAngle);

	//コントローラーのトランスフォーム（対象の周囲、左右移動）更新
	m_camParentTransform.SetRotate(Vec3<float>::GetYAxis(), m_nowParam.m_yAxisAngle);
	m_camParentTransform.SetPos(Math::Lerp(m_camParentTransform.GetPos(), arg_targetPos.GetPosWorld(), m_camFollowLerpRate));






	//使用するカメラの座標を補間して適用。
	m_attachedCam.lock()->GetTransform().SetPos(m_cameraLocalTransform.GetPosWorldByMatrix());

	////通常の地形を走査
	//for (auto& terrian : arg_nowStage.lock()->GetTerrianArray())
	//{
	//	//モデル情報取得
	//	auto model = terrian.GetModel().lock();

	//	//メッシュを走査
	//	for (auto& modelMesh : model->m_meshes)
	//	{

	//		//当たり判定に使用するメッシュ
	//		auto checkHitMesh = terrian.GetCollisionMesh()[static_cast<int>(&modelMesh - &model->m_meshes[0])];

	//		//判定↓============================================

	//		//当たり判定を実行
	//		auto moveVec = m_cameraLocalTransform.GetPosWorldByMatrix() - arg_targetPos.GetPos();
	//		CollisionDetectionOfRayAndMesh::MeshCollisionOutput output = CollisionDetectionOfRayAndMesh::Instance()->MeshCollision(arg_targetPos.GetPos(), moveVec.GetNormal(), checkHitMesh);

	//		if (output.m_isHit && 0 < output.m_distance && output.m_distance < moveVec.Length()) {

	//			m_attachedCam.lock()->GetTransform().SetPos(output.m_pos + output.m_normal);

	//		}

	//		//=================================================
	//	}
	//}

	//現在の座標からプレイヤーに向かう回転を求める。
	Vec3<float> playerDir = arg_targetPos.GetPos() - m_attachedCam.lock()->GetTransform().GetPosWorld();
	playerDir.Normalize();

	//仮のXベクトルから上ベクトルを得る。
	Vec3<float> axisY = { 0,1,0 };

	//プレイヤーの法線との外積から仮のXベクトルを得る。
	Vec3<float> axisX = axisY.Cross(playerDir);

	//本当のXベクトルを得る。
	Vec3<float> axisZ = -axisY.Cross(axisX);

	//姿勢を得る。
	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();
	matWorld.r[0] = { axisX.x, axisX.y, axisX.z, 0.0f };
	matWorld.r[1] = { axisY.x, axisY.y, axisY.z, 0.0f };
	matWorld.r[2] = { axisZ.x, axisZ.y, axisZ.z, 0.0f };

	XMVECTOR rotate, scale, position;
	DirectX::XMMatrixDecompose(&scale, &rotate, &position, matWorld);
	rotate = DirectX::XMQuaternionNormalize(rotate);

	//回転を適用。
	m_attachedCam.lock()->GetTransform().SetRotate(rotate);


	//m_attachedCam.lock()->GetTransform() = m_cameraLocalTransform;

}

void CameraController::TerrianMeshCollision(const std::weak_ptr<Stage> arg_nowStage)
{

	//通常の地形を走査
	for (auto& terrian : arg_nowStage.lock()->GetTerrianArray())
	{
		//モデル情報取得
		auto model = terrian.GetModel().lock();

		//メッシュを走査
		for (auto& modelMesh : model->m_meshes)
		{

			//当たり判定に使用するメッシュ
			auto checkHitMesh = terrian.GetCollisionMesh()[static_cast<int>(&modelMesh - &model->m_meshes[0])];

			//判定↓============================================

			//当たり判定を実行
			//auto eyePos = m_oldAttackCam.GetPosWorld();
			//auto moveVec = m_attachedCam.lock()->GetTransform().GetPosWorld() - m_oldAttackCam.GetPosWorld();
			//CollisionDetectionOfRayAndMesh::MeshCollisionOutput output = CollisionDetectionOfRayAndMesh::Instance()->MeshCollision(eyePos, moveVec.GetNormal(), checkHitMesh);

			//if (output.m_isHit && 0 < output.m_distance && output.m_distance < moveVec.Length()) {

				//m_attachedCam.lock()->GetTransform() = m_oldAttackCam;
				//m_camParentTransform = m_oldCamParentTransform;

			//}

			//=================================================
		}
	}

}
