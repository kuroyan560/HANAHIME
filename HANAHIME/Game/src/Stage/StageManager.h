#pragma once
#include"Common/Singleton.h"
#include"ForUser/Debugger.h"

#include<memory>
namespace KuroEngine
{
	class Model;
	class Camera;
	class LightManager;
}

class Stage;

class StageManager : public KuroEngine::DesignPattern::Singleton<StageManager>,public KuroEngine::Debugger
{
	friend class KuroEngine::DesignPattern::Singleton<StageManager>;
	StageManager();

	//スカイドームの大きさ
	float m_skydomeScaling = 1.0f;
	//森林円柱の半径
	float m_woodsRadius = 1.0f;
	//森林円柱の高さ
	float m_woodsHeight = 1.0f;
	//地面の大きさ
	float m_groundScaling = 1.0f;

	float m_terrianScaling = 1.0f;
	float m_oldTerrianScaling = m_terrianScaling;

	//デバッグ用テストステージ
	std::shared_ptr<Stage>m_testStage;
	std::shared_ptr<Stage>m_homeStage;

	//現在のステージ
	std::shared_ptr<Stage>m_nowStage;

	//Imguiデバッグ関数オーバーライド
	void OnImguiItems()override;

public:
	void Draw(KuroEngine::Camera& arg_cam, KuroEngine::LightManager& arg_ligMgr);

	//現在のステージのゲッタ
	std::weak_ptr<Stage>GetNowStage() { return m_nowStage; }
};

