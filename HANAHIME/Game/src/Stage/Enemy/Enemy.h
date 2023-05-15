#pragma once
#include"../StageParts.h"
#include"../../AI/EnemyPatrol.h"
#include"../../AI/EnemySearch.h"
#include"../Grass.h"
#include"ForUser/DrawFunc/BillBoard/DrawFuncBillBoard.h"
#include"EnemyDataReferenceForCircleShadow.h"

enum ENEMY_ATTACK_PATTERN
{
	ENEMY_ATTACK_PATTERN_NORMAL,//プレイヤーを見つけたら攻撃開始
	ENEMY_ATTACK_PATTERN_ALWAYS, //常に一定間隔で攻撃している
	ENEMY_ATTACK_PATTERN_INVALID
};

class MiniBug :public StageParts
{
public:
	MiniBug(std::weak_ptr<KuroEngine::Model>arg_model, KuroEngine::Transform arg_initTransform, std::vector<KuroEngine::Vec3<float>>posArray, bool loopFlag)
		:StageParts(MINI_BUG, arg_model, arg_initTransform), m_deadTimer(120)
	{

		return;

		m_sightArea.Init(&m_transform);
		track.Init(0.5f);
		m_posArray = posArray;

		m_nowStatus = SEARCH;
		m_prevStatus = NONE;
		m_limitIndex = 0;
		m_deadFlag = false;
		m_startDeadMotionFlag = false;
		m_deadTimer.Reset(120);

		m_tex.resize(MAX);
		m_tex[FIND] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer("resource/user/tex/reaction/Find.png");
		m_tex[HIT] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer("resource/user/tex/reaction/Attack.png");
		m_tex[LOOK] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer("resource/user/tex/reaction/hatena.png");
		m_tex[FAR_AWAY] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer("resource/user/tex/reaction/hatena.png");
		m_tex[DEAD] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer("resource/user/tex/reaction/dead.png");

		m_reaction = std::make_unique<Reaction>(m_tex);

		m_hitBox.m_centerPos = &m_transform.GetPos();
		m_hitBox.m_radius = &m_hitBoxSize;

		m_hitBoxSize = 3.0f;

		if (posArray.size() == 0 || posArray.size() == 1)
		{
			std::vector<KuroEngine::Vec3<float>>limitPosArray;
			limitPosArray.emplace_back(arg_initTransform.GetPos());
			m_patrol = std::make_unique<PatrolBasedOnControlPoint>(limitPosArray, 0, loopFlag);
			m_posArray = m_patrol->GetLimitPosArray();
		}
		else
		{
			m_patrol = std::make_unique<PatrolBasedOnControlPoint>(posArray, 0, loopFlag);
		}

		m_shadowInfluenceRange = SHADOW_INFLUENCE_RANGE;

		//丸影用に敵のデータの参照を渡す。
		EnemyDataReferenceForCircleShadow::Instance()->SetData(&m_transform, &m_shadowInfluenceRange, &m_deadFlag);

	}

	void OnInit()override;
	void Update(Player &arg_player)override;
	void Draw(KuroEngine::Camera& arg_cam, KuroEngine::LightManager& arg_ligMgr)override;

	void DebugDraw(KuroEngine::Camera &camera);

private:
	std::shared_ptr<KuroEngine::Model>m_enemyModel;

	enum Status
	{
		NONE,
		SEARCH,//循環中
		ATTACK,//プレイヤーを追跡
		NOTICE,//プレイヤーらしきものを見つけた
		RETURN,//循環エリアに戻る
	};

	Status m_nowStatus;
	Status m_prevStatus;

	KuroEngine::Vec3<float>m_pos, m_prevPos;
	float m_scale;
	KuroEngine::Vec3<float>m_dir;
	std::unique_ptr<PatrolBasedOnControlPoint> m_patrol;
	SightSearch m_sightArea;
	int m_limitIndex;

	float m_hitBoxSize;

	//丸影用
	float m_shadowInfluenceRange;
	const float SHADOW_INFLUENCE_RANGE = 6.0f;

	class AttackMotion
	{
		KuroEngine::Vec3<float> m_aPointPos;
		KuroEngine::Vec3<float> m_bPointPos;

		HeadNextPoint m_go;
		HeadNextPoint m_back;

		bool m_flag;
		bool m_doneFlag = true;
	public:
		void Init(const KuroEngine::Vec3<float> &aPointPos, const KuroEngine::Vec3<float> &bPointPos, float speed)
		{
			m_aPointPos = aPointPos;
			m_bPointPos = bPointPos;

			m_go.Init(m_aPointPos, m_bPointPos, speed);
			m_back.Init(m_bPointPos, m_aPointPos, speed);
			m_doneFlag = true;
			m_flag = true;
		};

		bool IsDone()
		{
			return m_doneFlag;
		}

		void Done() {
			m_doneFlag = true;
		}
		void UnDone() {
			m_doneFlag = false;
		}

		KuroEngine::Vec3<float>GetVel(const KuroEngine::Vec3<float> &pos)
		{
			if (m_go.IsArrive(pos))
			{
				m_flag = false;
			}
			if (m_back.IsArrive(pos))
			{
				m_flag = true;
				m_doneFlag = true;
			}

			if (m_flag)
			{
				return m_go.Update();
			}
			else
			{
				return m_back.Update();
			}
		};
	};


	//攻撃処理---------------------------------------

	//予備動作
	KuroEngine::Timer m_attackIntervalTimer;
	KuroEngine::Timer m_attackCoolTimer;
	KuroEngine::Timer m_readyToGoToPlayerTimer;
	KuroEngine::Vec3<float>m_attackOffsetVel;

	AttackMotion m_jumpMotion;
	AttackMotion m_attackMotion;



	bool m_attackFlag;
	HeadNextPoint m_trackPlayer;
	KuroEngine::Vec3<float>m_aPointPos;
	KuroEngine::Vec3<float>m_bPointPos;
	TrackEndPoint track;

	//攻撃処理---------------------------------------

	//移動処理---------------------------------------
	KuroEngine::Vec3<float>m_larpPos;
	KuroEngine::Quaternion m_larpRotation;
	//移動処理---------------------------------------

	//死亡処理------------------------------------------------------------------------------

	bool m_deadFlag;
	bool m_startDeadMotionFlag;
	KuroEngine::Timer m_deadTimer;

	Sphere m_hitBox;

	//死亡処理------------------------------------------------------------------------------



	//思考処理
	KuroEngine::Timer m_thinkTimer;
	bool m_decisionFlag, m_prevDecisionFlag;

	//制御点
	std::vector<KuroEngine::Vec3<float>>m_posArray;


	//リアクション表記---------------------------------------

	enum ReactionEnum
	{
		FIND,
		HIT,
		LOOK,
		FAR_AWAY,
		DEAD,
		MAX
	};

	class Reaction
	{
	public:
		Reaction(std::vector<std::shared_ptr<KuroEngine::TextureBuffer>> buffer)
		{
			m_timer.Reset(120);
			m_timer.ForciblyTimeUp();
			m_tex = buffer;
		}

		void Init(int index)
		{
			m_index = index;
			m_timer.Reset(120);
		}

		void Update(const KuroEngine::Vec3<float> &pos)
		{
			m_pos = KuroEngine::Math::Ease(KuroEngine::Out, KuroEngine::Back, m_timer.GetTimeRate(), pos, pos + KuroEngine::Vec3<float>(0.0f, 10.0f, 0.0f));
			m_timer.UpdateTimer();
		}

		void Draw(KuroEngine::Camera &camera)
		{
			if (!m_timer.IsTimeUp())
			{
				KuroEngine::DrawFuncBillBoard::Graph(camera, m_pos, { 5.0f,5.0f }, m_tex[m_index]);
			}
			else
			{
				m_pos = { 0.0f,0.0f,0.0f };
			}
		}

		bool Done()
		{
			return m_timer.IsTimeUp();
		}

	private:
		int m_index;
		KuroEngine::Vec3<float>m_pos;
		KuroEngine::Timer m_timer;
		std::vector<std::shared_ptr<KuroEngine::TextureBuffer>>m_tex;
	};
	std::unique_ptr<Reaction> m_reaction;

	std::vector<std::shared_ptr<KuroEngine::TextureBuffer>>m_tex;

	//リアクション表記---------------------------------------



	KuroEngine::Quaternion Lerp(const KuroEngine::Quaternion &a, const KuroEngine::Quaternion &b, float mul)
	{
		KuroEngine::Vec4<float> base;
		base.x = a.m128_f32[0];
		base.y = a.m128_f32[1];
		base.z = a.m128_f32[2];
		base.w = a.m128_f32[3];

		KuroEngine::Vec4<float> base2;
		base2.x = b.m128_f32[0];
		base2.y = b.m128_f32[1];
		base2.z = b.m128_f32[2];
		base2.w = b.m128_f32[3];

		KuroEngine::Vec4<float>ease = KuroEngine::Math::Lerp(base, base2, mul);

		KuroEngine::Quaternion result;
		result.m128_f32[0] = ease.x;
		result.m128_f32[1] = ease.y;
		result.m128_f32[2] = ease.z;
		result.m128_f32[3] = ease.w;

		return result;

	};
};

class DossunRing : public StageParts
{
public:
	DossunRing(std::weak_ptr<KuroEngine::Model>arg_model, KuroEngine::Transform arg_initTransform, ENEMY_ATTACK_PATTERN status)
		:StageParts(DOSSUN_RING, arg_model, arg_initTransform)
	{

		return;

		m_hitBoxRadiusMax = 10.0f;
		m_hitBoxRadius = 0.0f;
		m_findPlayerFlag = false;
		m_nowStatus = status;

		m_sightRange = m_hitBoxRadiusMax;

		//視界の判定---------------------------------------
		m_sightHitBox.m_centerPos = &m_transform.GetPos();
		m_sightHitBox.m_radius = &m_sightRange;
		m_sightArea.Init(m_sightHitBox);
		//視界の判定---------------------------------------

		m_maxAttackIntervalTime = 60 * 2;
		m_maxAttackTime = 60 * 5;

		m_attackInterval.Reset(m_maxAttackIntervalTime);
		m_attackTimer.Reset(m_maxAttackTime);

		//死亡処理---------------------------------------
		m_deadFlag = false;
		m_startDeadMotionFlag = false;
		m_deadTimer.Reset(120);
		//死亡処理---------------------------------------

		m_hitBox.m_centerPos = &m_transform.GetPos();
		m_hitBox.m_radius = &m_hitBoxRadius;

		m_hitBoxModel =
			KuroEngine::Importer::Instance()->LoadModel("resource/user/model/", "RedSphere.glb");

		m_shadowInfluenceRange = SHADOW_INFLUENCE_RANGE;

		//丸影用に敵のデータの参照を渡す。
		EnemyDataReferenceForCircleShadow::Instance()->SetData(&m_transform, &m_shadowInfluenceRange, &m_deadFlag);

	}
	void Update(Player &arg_player)override;
	void Draw(KuroEngine::Camera &arg_cam, KuroEngine::LightManager &arg_ligMgr)override;

	void DebugDraw(KuroEngine::Camera &camera);

private:

	ENEMY_ATTACK_PATTERN m_nowStatus;
	float m_sightRange;


	//攻撃フェーズ---------------
	int m_maxAttackIntervalTime;//予備動作時間
	int m_maxAttackTime;		//攻撃時間

	KuroEngine::Timer m_attackInterval;	//予備動作
	bool m_attackFlag;					//攻撃中
	KuroEngine::Timer m_attackTimer;	//攻撃の広がり

	float m_hitBoxRadius;		//攻撃の当たり判定(円)
	float m_hitBoxRadiusMax;	//攻撃の当たり判定(最大値)
	//攻撃フェーズ---------------

	Sphere m_hitBox;
	Sphere m_sightHitBox;
	CircleSearch m_sightArea;

	bool m_findPlayerFlag;

	//死亡処理---------------------------------------
	bool m_deadFlag;
	bool m_startDeadMotionFlag;
	KuroEngine::Timer m_deadTimer;
	float m_scale;
	//死亡処理---------------------------------------


	//移動処理---------------------------------------
	KuroEngine::Vec3<float>m_larpPos;
	KuroEngine::Quaternion m_larpRotation;
	//移動処理---------------------------------------

	//丸影用
	float m_shadowInfluenceRange;
	const float SHADOW_INFLUENCE_RANGE = 6.0f;


	std::shared_ptr<KuroEngine::Model>m_hitBoxModel;

};