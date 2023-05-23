#pragma once
#include<vector>
#include<memory>
#include<array>
#include"Common/Angle.h"
#include"Common/Vec.h"

namespace KuroEngine
{
	class TextureBuffer;
}

class GameScene;

class PauseUI
{
	//�|�[�Y�N����
	bool m_isActive = false;

	//����
	enum PAUSE_ITEM
	{
		RESUME,	//�Q�[�����ĊJ
		RETRY,	//���g���C
		FAST_TRAVEL,	//�t�@�X�g�g���x��
		SETTING,	//�ݒ�
		RETURN_TO_TITLE,	//�^�C�g���ɖ߂�
		PAUSE_ITEM_NUM,
	}m_item;

	//���ڂ̏��
	enum ITEM_STATUS 
	{ 
		DEFAULT,	//�f�t�H���g
		SELECT,	//�I��
		ITEM_STATUS_NUM 
	};

	//���j���[�̏��
	enum MENU_STATUS
	{
		DEFAULT_MENU,	//�ʏ탁�j���[
		CONFIRM_MENU,	//�ŏI�m�F
		MENU_STATUS_NUM
	}m_menuStatus;

	//�f�t�H���g�̃��j���[
	struct DefaultMenu
	{
		//���ڂ��Ƃ̉摜
		std::array<std::array<std::shared_ptr<KuroEngine::TextureBuffer>, ITEM_STATUS_NUM>, PAUSE_ITEM_NUM>m_itemTexArray;
		//�I�𒆂̍��ڂɂ̂ݏo��e�摜
		std::shared_ptr<KuroEngine::TextureBuffer>m_selectItemShadowTex;

		//�X�e�[�W���摜
		std::vector<std::shared_ptr<KuroEngine::TextureBuffer>>m_stageNameTex;
		std::shared_ptr<KuroEngine::TextureBuffer>m_stageNameDefaultTex;
		//�X�e�[�W���̑��������摜
		std::shared_ptr<KuroEngine::TextureBuffer>m_underLineTex;
		//�X�e�[�W���̃C���f�b�N�X
		int m_stageNameIdx = 0;

		//�Ԃ̉摜
		std::shared_ptr<KuroEngine::TextureBuffer>m_flowerTex;
		//���W�Ԃ̐��e�N�X�`��
		static const int FLOWER_NUM_TEX_SIZE = 10;
		std::array<std::shared_ptr<KuroEngine::TextureBuffer>, FLOWER_NUM_TEX_SIZE>m_flowerNumTexArray;
		//���W�Ԃ́u x �v�e�N�X�`��
		std::shared_ptr<KuroEngine::TextureBuffer>m_flowerMulTex;
	}m_defaultMenu;

	//�ŏI�m�F
	struct ConfirmMenu
	{
		//�A�C�R���摜
		std::shared_ptr<KuroEngine::TextureBuffer>m_iconTex;

		//�������A�͂�
		enum CONFIRM_ITEM { NO, YES, CONFIRM_ITEM_NUM, NONE }m_confirmItem;
		//�u�������v�u�͂��v�摜
		std::array<std::array<std::shared_ptr<KuroEngine::TextureBuffer>, ITEM_STATUS_NUM>, CONFIRM_ITEM_NUM>m_answerTexArray;
		//�u�������v�u�͂��v�̑I�𑤂ɏo��e�摜
		std::shared_ptr<KuroEngine::TextureBuffer>m_answerShadowTex;

		//�u��낵���ł����H�v�摜
		std::shared_ptr<KuroEngine::TextureBuffer>m_mindTex;

		//�I�񂾍��ڂɉ���������摜
		std::array<std::shared_ptr<KuroEngine::TextureBuffer>, PAUSE_ITEM_NUM>m_questionTexArray;
	}m_confirmMenu;

	//sin�J�[�u���[�g
	float m_sinCurveRateT = 0.0f;

	//�p�[�����m�C�Y
	float m_perlinNoiseRateT = 0.0f;
	//�p�[�����m�C�Y�̃V�[�h
	KuroEngine::Vec2<int>m_perlinNoiseSeed = { 0,0 };
	//�I�𒆂̉e�摜�̃p�[�����m�C�Y��]
	KuroEngine::Angle m_selectItemShadowSpin;
	//�I�𒆂̉e�摜�̃p�[�����m�C�Y���W�I�t�Z�b�g
	KuroEngine::Vec2<float> m_selectItemShadowOffset;

	//�|�[�Y�ɓ��钼�O�̃^�C���X�P�[��
	float m_latestTimeScale = 0.0f;

	void OnActive();
	void OnNonActive();

public:
	PauseUI();
	void Init();
	void Update(GameScene* arg_gameScene, float arg_timeScale = 1.0f);
	void Draw(int arg_totalGetFlowerNum);
	void SetInverseActive();

	const bool& IsActive()const { return m_isActive; }
};