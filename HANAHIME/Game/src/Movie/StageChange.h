#pragma once
#include"Common/Transform.h"
#include"../../../src/engine/Render/RenderObject/SpriteMesh.h"
#include"../../../src/engine/ForUser/Timer.h"

/// <summary>
/// ���ړ����̑J�ډ��o
/// </summary>
class SceneChange
{
public:
	SceneChange();

	void Update();
	void Draw();

	void Start();
	bool IsHide();

private:
	bool m_startFlag;	//�J�n�t���O
	bool m_blackOutFlag;//�Ó]�����u��

	//�Ó]�Ɩ��]�̎��Ԋ֘A----------------------------------------
	KuroEngine::Timer m_time;
	int m_countTimeUpNum;
	//�Ó]�Ɩ��]�̎��Ԋ֘A----------------------------------------

	//�e�N�X�`���֘A�̏��----------------------------------------
	float m_alpha;
	KuroEngine::Vec2<float>m_size;
	std::shared_ptr<KuroEngine::TextureBuffer> m_blackTexBuff;
	//�e�N�X�`���֘A�̏��----------------------------------------

};