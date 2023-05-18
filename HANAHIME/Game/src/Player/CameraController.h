#pragma once
#include"Common/Vec.h"
#include"Common/Angle.h"
#include"Common/Transform.h"
#include"ForUser/Debugger.h"
#include"../Stage/Stage.h"

#include<memory>
namespace KuroEngine
{
	class Camera;
}

class CameraController : public KuroEngine::Debugger
{
	void OnImguiItems()override;

	//対象と注視点の相対的な座標オフセット
	KuroEngine::Vec3<float>m_gazePointOffset = { 0,0.5f,0 };
	//追従対象との相対的な座標オフセットの最小と最大
	float m_posOffsetDepthMin = -10.0f;
	float m_posOffsetDepthMax = -0.1f;
	//X軸角度（高さ傾き）の最小と最大
	KuroEngine::Angle m_xAxisAngleMin = KuroEngine::Angle(-40);
	KuroEngine::Angle m_xAxisAngleMax = KuroEngine::Angle(40);
	//カメラの角度 天井にいるときバージョン
	KuroEngine::Angle m_xAxisAngleMinCeiling = KuroEngine::Angle(-40);
	KuroEngine::Angle m_xAxisAngleMaxCeiling = KuroEngine::Angle(40);

	//カメラをZ方向に回転させる量。
	float m_rotateZ;

	//地形に当たっているか
	bool m_isHitTerrian;

	//下側の地形に当たっているか。当たっていたら注視点をずらすやつをやる。
	bool m_isHitUnderGroundTerrian;

	//プレイヤーのY軸回転を保存しておく変数。プレイヤーが横の壁に居るときは注視点の移動をY軸回転で行うので、注視点移動が終わったら動かした量を戻すため。
	float m_playerRotYStorage;
	float m_playerRotYLerp;
	const float PLAYER_TARGET_MOVE_SIDE = 0.8f;		//プレイヤーの横面の注視点移動のときの動かせる限界。


	float m_rotateYLerpAmount;
	float m_cameraXAngleLerpAmount;

	//カメラの回転を制御するクォータニオン
	KuroEngine::Quaternion m_cameraQ;

	//操作するカメラのポインタ
	std::weak_ptr<KuroEngine::Camera>m_attachedCam;

	KuroEngine::Vec3<float> m_oldCameraWorldPos;


public:
	//コンストラクタ
	CameraController();

	void AttachCamera(std::shared_ptr<KuroEngine::Camera>arg_cam);

	void Init(const KuroEngine::Vec3<float>& arg_up, float arg_rotateY);
	void Update(KuroEngine::Vec3<float>arg_scopeMove, KuroEngine::Transform arg_targetPos, float& arg_playerRotY, float arg_cameraZ, const std::weak_ptr<Stage>arg_nowStage, bool arg_isCameraUpInverse, bool arg_isCameraDefaultPos, bool& arg_isHitUnderGround, bool arg_isMovePlayer);

	std::weak_ptr<KuroEngine::Camera> GetCamera() { return m_attachedCam; }

	KuroEngine::Quaternion GetCameraQ() { return m_cameraQ; }

private:

	//3次元ベクトルを2次元に射影する関数
	KuroEngine::Vec2<float> Project3Dto2D(KuroEngine::Vec3<float> arg_vector3D, KuroEngine::Vec3<float> arg_basis1, KuroEngine::Vec3<float> arg_basis2) {

		//基底ベクトルを正規化
		arg_basis1.Normalize();
		arg_basis2.Normalize();

		//3次元ベクトルを2次元ベクトルに射影
		float x = arg_vector3D.Dot(arg_basis1);
		float y = arg_vector3D.Dot(arg_basis2);

		return KuroEngine::Vec2<float>(x, y);
	}

};