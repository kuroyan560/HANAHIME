#include "Enemy.h"
#include"../../Player/Player.h"
#include"FrameWork/UsersInput.h"

void MiniBug::Update(Player &arg_player)
{
	bool findFlag = m_sightArea.IsFind(arg_player.GetTransform().GetPos(), 180.0f);
	if (findFlag)
	{
		m_nowStatus = MiniBug::ATTACK;
	}

	if (m_decisionFlag != m_prevDecisionFlag)
	{
		//思考
		//敵発見時(プレイヤーが視界に入った)
		if (findFlag)
		{
			m_nowStatus = MiniBug::ATTACK;
		}
		//敵発見時(プレイヤーがもぐっているかつ動いている時に発見した)
		else if (false)
		{
			m_nowStatus = MiniBug::NOTICE;
		}
		//帰還(敵を見つけた後、見失った)
		else if (false)
		{
			m_nowStatus = MiniBug::RETURN;
		}
		//巡回(何も起きていないorルートに帰還したら)
		else if (false)
		{
			m_nowStatus = MiniBug::SERACH;
		}
		m_prevDecisionFlag = m_decisionFlag;
	}

	//敵発見時(プレイヤーが視界に入った)
	if (KuroEngine::UsersInput::Instance()->KeyOnTrigger(DIK_1))
	{
		m_nowStatus = MiniBug::ATTACK;
	}
	//敵発見時(プレイヤーがもぐっているかつ動いている時に発見した)
	else if (KuroEngine::UsersInput::Instance()->KeyOnTrigger(DIK_2))
	{
		m_nowStatus = MiniBug::NOTICE;
	}
	//帰還(敵を見つけた後、見失った)
	else if (KuroEngine::UsersInput::Instance()->KeyOnTrigger(DIK_3))
	{
		m_nowStatus = MiniBug::RETURN;
	}
	//巡回(何も起きていないorルートに帰還したら)
	else if (KuroEngine::UsersInput::Instance()->KeyOnTrigger(DIK_4))
	{
		m_nowStatus = MiniBug::SERACH;
	}


	//初期化
	if (m_nowStatus != m_prevStatus)
	{
		int index = 0;
		float min = 10000000000.0f;
		float prevMin = 0.0f;
		switch (m_nowStatus)
		{
			//最も近い制御地点からループする
		case MiniBug::SERACH:
			m_patrol.Init(0, false);
			m_pos = m_patrol.GetLimitPos(0);

			break;
		case MiniBug::ATTACK:
			m_attackIntervalTimer.Reset(120);
			m_sightArea.Init(&m_transform);
			break;
		case MiniBug::NOTICE:

			break;
		case MiniBug::RETURN:
			//最も近い制御点に戻る
			m_aPointPos = m_pos;
			for (int i = 0; i < m_posArray.size(); ++i)
			{
				min = std::min(m_posArray[i].Distance(m_pos), min);
				if (min != prevMin)
				{
					index = i;
				}
			}
			m_bPointPos = m_posArray[index];
			break;
		default:
			break;
		}
		m_thinkTimer.Reset(120);
		m_prevStatus = m_nowStatus;
	}

	bool debug = false;
	float distance = 0.0f;

	//行動
	KuroEngine::Vec3<float>vel = { 0.0f,0.0f,0.0f };
	switch (m_nowStatus)
	{
	case MiniBug::SERACH:
		vel = m_patrol.Update(m_pos);
		break;
	case MiniBug::ATTACK:

		//見つけた時のリアクション時間
		if (!m_attackIntervalTimer.UpdateTimer())
		{
			//注視
			debug = true;
			//何かしらのアクションを書く
			break;
		}

		distance = arg_player.GetTransform().GetPos().Distance(vel);
		//プレイヤーと一定以上距離が離れた場合
		if (30.0f <= distance)
		{
			//暫く止まり、何もなければ思考を切り替える。
			if (m_thinkTimer.UpdateTimer())
			{
				m_nowStatus = MiniBug::RETURN;
			}
			m_aPointPos = m_pos;
		}
		//プレイヤーを追尾中
		else
		{
			m_thinkTimer.Reset(120);
			vel = track.Update(m_pos, arg_player.GetTransform().GetPos());
		}

		//プレイヤーと一定距離まで近づいたら攻撃予備動作を入れる
		if (distance <= 5.0f)
		{
			m_attackFlag = true;
		}
		//攻撃予備動作が終わって攻撃を行った。
		if (m_attackFlag && m_attackIntervalTimer.UpdateTimer())
		{
			//プレイヤーと敵の当たり判定の処理をここに書く

			m_attackIntervalTimer.Reset(120);
			m_attackFlag = false;
		}

		break;
	case MiniBug::NOTICE:
		//暫く待って動かなかったら別の場所に向かう
		//動いたら注視する
		//判定
		break;
	case MiniBug::RETURN:
		//期間中
		m_thinkTimer.Reset(120);
		vel = m_trackPlayer.Update();
		if (false)
		{
			m_nowStatus = MiniBug::SERACH;
		}
		break;
	default:
		break;
	}

	//共通処理



	//草の当たり判定
	arg_player.CheckHitGrassSphere(m_transform.GetPosWorld(), m_transform.GetUpWorld(), m_transform.GetScale().Length());

	//座標移動
	m_pos += vel;
	m_transform.SetPos(m_pos);

	DirectX::XMVECTOR dir = { 0.0f,1.0f,0.0f,0.0f };
	m_transform.SetRotate(DirectX::XMQuaternionRotationAxis(dir, KuroEngine::Angle::ConvertToRadian(-90.0f)));

}

void MiniBug::DebugDraw(KuroEngine::Camera &camera)
{
#ifdef _DEBUG

	switch (m_nowStatus)
	{
	case MiniBug::SERACH:
		m_patrol.DebugDraw();
		break;
	case MiniBug::ATTACK:
		break;
	case MiniBug::NOTICE:
		break;
	case MiniBug::RETURN:
		break;
	default:
		break;
	}

	m_sightArea.DebugDraw(camera);

#endif // _DEBUG

}

void DossunRing::Update(Player &arg_player)
{
	if (m_sightArea.IsFind(arg_player.m_sphere))
	{
		m_findPlayerFlag = true;
	}
	//攻撃が終わってプレイヤーが見えなくなったら攻撃終了
	else if (!m_attackFlag)
	{
		m_findPlayerFlag = false;
	}

	//亜種の場合は常に攻撃するモーションを入れる
	switch (m_nowStatus)
	{
	case DossunRing::ALWAYS:
		m_findPlayerFlag = true;
		break;
	default:
		break;
	}

	if (!m_findPlayerFlag)
	{
		return;
	}
	//以降プレイヤーが発見された処理---------------------------------------

	//攻撃予備動作中
	if (m_attackInterval.UpdateTimer() && !m_attackFlag)
	{
		m_attackTimer.Reset(m_maxAttackTime);
		m_attackFlag = true;
	}

	//攻撃開始
	if (m_attackFlag)
	{
		//当たり判定の広がり
		m_hitBoxRadius = m_attackTimer.GetTimeRate() * m_hitBoxRadiusMax;
		//広がり切ったらインターバルに戻る
		if (m_attackTimer.UpdateTimer())
		{
			m_hitBoxRadius = 0.0f;
			m_attackInterval.Reset(m_maxAttackIntervalTime);
			m_attackFlag = false;
		}

		//プレイヤーと敵の当たり判定の処理をここに書く
		if (false)
		{

		}
	}
}

void DossunRing::DebugDraw(KuroEngine::Camera &camera)
{
#ifdef _DEBUG

	m_sightArea.DebugDraw(camera);

#endif // _DEBUG

}