#pragma once
#include"KuroEngineDevice.h"
#include"ForUser/DebugCamera.h"

class GameScene : public KuroEngine::BaseScene
{
	std::shared_ptr<KuroEngine::TextureBuffer>m_ddsTex;
	std::shared_ptr<KuroEngine::TextureBuffer>m_pngTex;
	KuroEngine::DebugCamera m_debugCam;
	
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override{};
	void OnFinalize()override {};
public:
	GameScene();
};