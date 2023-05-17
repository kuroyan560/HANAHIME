#pragma once
#include<memory>
#include"Common/Transform2D.h"
#include"Common/Transform.h"
#include"Common/Vec.h"
#include<vector>
#include<array>

namespace KuroEngine
{
	class TextureBuffer;
	class Camera;
}

//目的地を示すマップピン
class MapPinUI
{
	struct Content
	{
		bool m_active = true;
		std::shared_ptr<KuroEngine::TextureBuffer>m_tex;
		KuroEngine::Transform2D m_transform;
		float m_alpha = 1.0f;
		KuroEngine::Angle m_angle;
		Content(std::string arg_texPath, KuroEngine::Transform2D* arg_parent);
		Content(std::shared_ptr<KuroEngine::TextureBuffer>arg_tex, KuroEngine::Transform2D* arg_parent)
			:m_tex(arg_tex)
		{
			m_transform.SetParent(arg_parent);
		}
	};

	//ピン
	std::shared_ptr<Content>m_smallSquare;
	std::shared_ptr<Content>m_middleSquare;
	std::shared_ptr<Content>m_largeSquare;

	//矢印ピン
	std::shared_ptr<Content>m_arrowPin;

	//距離の数字
	std::array<std::shared_ptr	<KuroEngine::TextureBuffer>, 10>m_numTex;
	static const int NUM_DIGIT_MAX = 4;
	std::array<std::shared_ptr<Content>,NUM_DIGIT_MAX>m_distanceNum;
	//メートル
	std::shared_ptr<Content>m_meter;

	enum PIN_MODE { PIN_MODE_IN_SCREEN, PIN_MODE_OUT_SCREEN, PIN_MODE_NUM };
	std::array<std::vector<std::weak_ptr<Content>>, PIN_MODE_NUM>m_mapPinUI;

	//マップピンの描画サイズ
	float m_pinSize = 62.0f;
	//矢印全体を描画するための座標クランプ量
	float m_arrowClampOffset = 65.0f;
	//矢印の描画オフセット
	float m_arrowDrawOffset = 6.0f;
	//距離表記の描画オフセットY
	float m_meterDrawOffsetY = 16.0f;
	//距離の数字の字間オフセット
	float m_distStrDrawSpace = -2.0f;
	//距離とmの字間
	float m_meterStrDrawSpace = 3.0f;

	//UI全体のトランスフォーム
	KuroEngine::Transform2D m_canvasTransform;

	void UpdateDistance(PIN_MODE arg_pinMode, float arg_distance);
	void UpdateArrowDir(KuroEngine::Vec2<float>arg_destPos2D, KuroEngine::Vec2<float>arg_winSize, float arg_clampOffset);

public:
	MapPinUI();
	void Draw(KuroEngine::Camera& arg_cam, KuroEngine::Vec3<float>arg_destinationPos, KuroEngine::Vec3<float>arg_playerPos);
};

