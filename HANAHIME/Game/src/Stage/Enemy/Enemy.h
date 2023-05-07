#pragma once
#include"../StageParts.h"
#include"../../AI/EnemyPatrol.h"
#include"../../AI/EnemySearch.h"

class MiniBug :public StageParts
{
public:
	MiniBug(std::weak_ptr<KuroEngine::Model>arg_model, KuroEngine::Transform arg_initTransform, StageParts *arg_parent, std::vector<KuroEngine::Vec3<float>>posArray)
		:StageParts(MINI_BUG, arg_model, arg_initTransform, arg_parent), m_patrol(posArray, 0)
	{
		m_nowStatus = NOTICE;
		m_prevStatus = NONE;
		m_sightArea.Init(&m_transform);
		track.Init(0.01f);
		m_posArray = posArray;
	}
	void Update(Player& arg_player)override;

	void DebugDraw(KuroEngine::Camera &camera);

private:
	std::shared_ptr<KuroEngine::Model>m_model;

	enum Status
	{
		NONE,
		SERACH,//循環中
		ATTACK,//プレイヤーを追跡
		NOTICE,//プレイヤーらしきものを見つけた
		RETURN,//循環エリアに戻る
	};

	Status m_nowStatus;
	Status m_prevStatus;

	KuroEngine::Vec3<float>m_pos;
	PatrolBasedOnControlPoint m_patrol;
	SightSearch m_sightArea;

	
	//攻撃処理---------------------------------------

	bool m_attackFlag;
	HeadNextPoint m_trackPlayer;
	KuroEngine::Vec3<float>m_aPointPos;
	KuroEngine::Vec3<float>m_bPointPos;
	KuroEngine::Timer m_attackIntervalTimer;
	KuroEngine::Timer m_attackCoolTimer;
	KuroEngine::Timer m_readyToGoToPlayerTimer;
	TrackEndPoint track;

	//攻撃処理---------------------------------------



	//思考処理
	KuroEngine::Timer m_thinkTimer;
	bool m_decisionFlag,m_prevDecisionFlag;

	//制御点
	std::vector<KuroEngine::Vec3<float>>m_posArray;
};

class DossunRing : public StageParts
{
public:
	enum Status
	{
		NORMAL,//プレイヤーを見つけたら攻撃開始
		ALWAYS //常に一定間隔で攻撃している
	};

	DossunRing(std::weak_ptr<KuroEngine::Model>arg_model, KuroEngine::Transform arg_initTransform, StageParts *arg_parent, Status status)
		:StageParts(DOSSUN_RING, arg_model, arg_initTransform, arg_parent)
	{
		m_hitBoxRadiusMax = 10.0f;
		m_hitBoxRadius = 0.0f;
		m_findPlayerFlag = false;
		m_nowStatus = status;

		//視界の判定---------------------------------------
		m_sightHitBox.m_centerPos = &m_transform.GetPos();
		m_sightHitBox.m_radius = &m_hitBoxRadiusMax;
		m_sightArea.Init(m_sightHitBox);
		//視界の判定---------------------------------------

		m_maxAttackIntervalTime = 60 * 2;
		m_maxAttackTime = 60 * 5;

		m_attackInterval.Reset(m_maxAttackIntervalTime);
		m_attackTimer.Reset(m_maxAttackTime);
	}
	void Update(Player& arg_player)override;

	void DebugDraw(KuroEngine::Camera &camera);

private:

	Status m_nowStatus;

	//攻撃フェーズ---------------
	int m_maxAttackIntervalTime;//予備動作時間
	int m_maxAttackTime;		//攻撃時間

	KuroEngine::Timer m_attackInterval;	//予備動作
	bool m_attackFlag;					//攻撃中
	KuroEngine::Timer m_attackTimer;	//攻撃の広がり

	float m_hitBoxRadius;		//攻撃の当たり判定(円)
	float m_hitBoxRadiusMax;	//攻撃の当たり判定(最大値)
	//攻撃フェーズ---------------

	Sphere m_sightHitBox;
	CircleSearch m_sightArea;

	bool m_findPlayerFlag;

};