#include<windows.h>
#include"KuroEngineDevice.h"
#include"Common/Transform.h"
#include"Common/Transform2D.h"
#include"Common/Color.h"

#include"ForUser/DrawFunc/2D/DrawFunc2D.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D_Shadow.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D_FillTex.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D_Color.h"

#include"ForUser/DrawFunc/3D/DrawFunc3D.h"

#include"ForUser/DrawFunc/BillBoard/DrawFuncBillBoard.h"

#include"GameScene.h"

#ifdef _DEBUG
int main()
#else
//Windows�A�v���ł̃G���g���[�|�C���g�imain�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
	//�G���W���ݒ�i���[�U�[�ݒ�j========
	KuroEngine::EngineOption engineOption;
	engineOption.m_windowName = "�ؕP";
	engineOption.m_windowSize = { 1280,720 };
	//engineOption.m_windowSize = { 1920,1080 };
	engineOption.m_fullScreen = false;
	engineOption.m_showCursor = false;
	engineOption.m_iconPath = nullptr;
	engineOption.m_backBuffClearColor = KuroEngine::Color(0, 0, 0, 0);
	engineOption.m_useHDR = false;
	engineOption.m_frameRate = 60;
	//================================

	//�G���W���N���i���s������I���j
	KuroEngine::KuroEngineDevice engine;
	if (engine.Initialize(engineOption))return 1;

	//�V�[�����X�g�i���[�U�[�ݒ�j=========
	std::map<std::string, KuroEngine::BaseScene*>sceneList =
	{
		{"ProtoType",new GameScene()}
	};
	std::string awakeScene = "ProtoType";	//�J�n���̃V�[���L�[
	//================================

	//�G���W���ɃV�[�����X�g��n��
	engine.SetSceneList(sceneList, awakeScene);

	bool winEnd = false;

	//���[�v
	while (1)
	{
		//���b�Z�[�W������H
		MSG msg{};
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);	//�L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&msg);	//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
			if (msg.message == WM_QUIT)
			{
				//�E�B���h�E������ꂽ
				winEnd = true;
				break;
			}
		}

		//�I�����b�Z�[�W������ / �V�[���}�l�[�W���̏I���@�Ń��[�v�𔲂���
		if (winEnd || engine.End())
		{
			break;
		}

		engine.Update();
		engine.Draw();

		//�ÓI�N���X�������iDirty�t���O�n�j
		KuroEngine::Transform::DirtyReset();
		KuroEngine::Transform2D::DirtyReset();

		KuroEngine::DrawFunc2D::CountReset();
		KuroEngine::DrawFunc2D_Shadow::CountReset();
		KuroEngine::DrawFunc2D_FillTex::CountReset();
		KuroEngine::DrawFunc2D_Mask::CountReset();
		KuroEngine::DrawFunc2D_Color::CountReset();

		KuroEngine::DrawFunc3D::CountReset();

		KuroEngine::DrawFuncBillBoard::CountReset();
	}

	return 0;
}