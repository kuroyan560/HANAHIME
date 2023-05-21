#pragma once
#include<array>
#include<memory>
#include"Common/Vec.h"
#include"Common/Angle.h"
#include"ForUser/Timer.h"
#include"ForUser/ImpactShake.h"

namespace KuroEngine
{
	class TextureBuffer;
}

class PlayerHpUI
{
	//HPUI�̉��o�X�e�[�^�X
	enum HP_UI_STATUS { HP_UI_APPEAR, HP_UI_DRAW, HP_UI_DISAPPEAR, HP_UI_DAMAGE }m_hpUiStatus;

	//HP���[�t�̐�
	static const int LEAF_NUM = 5;
	static const int NUM_TEX = LEAF_NUM + 1;
	//�t���σe�N�X�`��
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, LEAF_NUM>m_leafTexArray;
	//�����e�N�X�`��
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, NUM_TEX>m_numTexArray;
	//�uHP�v�e�N�X�`��
	std::shared_ptr<KuroEngine::TextureBuffer>m_hpStrTex;

	//HPUI�̒��S���W�I�t�Z�b�g
	KuroEngine::Vec2<float>m_hpCenterOffset;
	//HPUI�̔��a�g�嗦
	float m_hpRadiusExpand = 1.0f;
	//HPUI�̉摜�̊g�嗦
	float m_hpTexExpand = 1.0f;
	//HPUI�̉�]�p�x
	KuroEngine::Angle m_leafSpin = KuroEngine::Angle(0);
	//HPUI�̓o�ꉉ�o�^�C�}�[
	KuroEngine::Timer m_appearTimer;
	//HPUI�̐U��
	KuroEngine::ImpactShake m_impactShake;
	//HPUI�̐S�����o�^�C�}�[
	KuroEngine::Timer m_beatTimer;

	//HP�t�ȊO�̃A���t�@�l
	float m_strAlpha = 0.0f;
	//HP�t�ȊO�̃I�t�Z�b�gX
	float m_strOffsetX = 0.0f;

	//�_���[�W���̃t���b�V��
	bool m_isNoDamageTime;
	KuroEngine::Timer m_damageFlashTimer;
	bool m_damageFlash;

	void SetHpUIStatus(HP_UI_STATUS arg_status);

public:
	PlayerHpUI();
	void Init();
	void Update(float arg_timeScale, int arg_defaultHp, int arg_nowHp, const KuroEngine::Timer& arg_noDamageTimer);
	void Draw(int arg_defaultHp, int arg_nowHp, bool arg_isHitStop);

	void OnDamage()
	{
		SetHpUIStatus(HP_UI_DAMAGE);
	}
};
