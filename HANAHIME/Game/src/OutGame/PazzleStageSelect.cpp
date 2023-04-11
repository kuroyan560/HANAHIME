#include"PazzleStageSelect.h"
#include"FrameWork/WinApp.h"

PazzleStageSelect::PazzleStageSelect()
{
	m_baseStageSelectPos = KuroEngine::WinApp::Instance()->GetExpandWinCenter() / 2.0f;


	m_numTexArray.resize(10);
	KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(m_numTexArray.data(), "resource/user/tex/Number.png", 10, { 10,1 });
}

void PazzleStageSelect::Init()
{
}

void PazzleStageSelect::Update()
{
	if (KuroEngine::UsersInput::Instance()->KeyOnTrigger(DIK_RIGHT))
	{
		++m_nowStageNum.x;
	}
	if (KuroEngine::UsersInput::Instance()->KeyOnTrigger(DIK_LEFT))
	{
		--m_nowStageNum.x;
	}
	if (KuroEngine::UsersInput::Instance()->KeyOnTrigger(DIK_UP))
	{
		--m_nowStageNum.y;
	}
	if (KuroEngine::UsersInput::Instance()->KeyOnTrigger(DIK_DOWN))
	{
		++m_nowStageNum.y;
	}


	int stageYMaxNum = static_cast<int>(m_stageSelectArray.size());
	if (m_nowStageNum.y < 0)
	{
		m_nowStageNum.y = stageYMaxNum - 1;
	}
	if (stageYMaxNum <= m_nowStageNum.y)
	{
		m_nowStageNum.y = 0;
	}


	int stageXMaxNum = static_cast<int>(m_stageSelectArray[m_nowStageNum.y].size());
	//左上端から左に行こうとしたら右下端に向かう
	if (m_nowStageNum.x < 0 && m_nowStageNum.y == 0)
	{
		m_nowStageNum.y = stageYMaxNum - 1;
		m_nowStageNum.x = stageXMaxNum - 1;
	}
	//右下端から右に行こうとしたら左上端に向かう
	if (stageXMaxNum <= m_nowStageNum.x && m_nowStageNum.y == stageYMaxNum - 1)
	{
		m_nowStageNum.y = 0;
		m_nowStageNum.x = 0;
	}
	//左の最大値行こうとしたら上に行く
	if (m_nowStageNum.x < 0)
	{
		--m_nowStageNum.y;
		m_nowStageNum.x = stageXMaxNum - 1;
	}
	//右の最大値行こうとしたら下に行く
	if (stageXMaxNum <= m_nowStageNum.x)
	{
		++m_nowStageNum.y;
		m_nowStageNum.x = 0;
	}

	//クリアしたかどうか
	if (m_stageSelectArray[m_nowStageNum.y][m_nowStageNum.x].m_isClearFlag)
	{
		bool debug = false;
	}
}

void PazzleStageSelect::Draw()
{
	//Y軸を考慮した数字のカウントに必要
	int yNum = 0;

	KuroEngine::Vec2<float>texSize(64.0f, 64.0f);
	//数字の描写
	for (int y = 0; y < m_stageSelectArray.size(); ++y)
	{
		for (int x = 0; x < m_stageSelectArray[y].size(); ++x)
		{
			KuroEngine::Vec2<float>pos(static_cast<float>(x), static_cast<float>(y));
			//桁用意
			std::vector<int>timeArray = CountNumber(yNum + x);
			//一桁
			if (yNum + x < 10)
			{
				KuroEngine::DrawFunc2D::DrawGraph(m_baseStageSelectPos + pos * texSize, m_numTexArray[timeArray[1]]);
			}
			//二桁
			else
			{
				KuroEngine::Vec2<float>basePos(pos * texSize + m_baseStageSelectPos);
				//桁の間を真ん中に持っていく処理
				basePos -= KuroEngine::Vec2<float>(15.0f, 0.0f);
				KuroEngine::DrawFunc2D::DrawGraph(basePos, m_numTexArray[timeArray[0]]);
				KuroEngine::DrawFunc2D::DrawGraph(basePos + KuroEngine::Vec2<float>(30.0f, 0.0f), m_numTexArray[timeArray[1]]);
			}
		}
		yNum += static_cast<int>(m_stageSelectArray[y].size());
	}
	//現在選択中
	KuroEngine::DrawFunc2D::DrawBox2D(m_baseStageSelectPos + m_nowStageNum.Float() * texSize, m_baseStageSelectPos + m_nowStageNum.Float() * texSize + texSize, KuroEngine::Color(255, 255, 0, 255), false);
}

int PazzleStageSelect::GetNumber()
{
	size_t num = 0;
	for (int y = 0; y < m_nowStageNum.y; ++y)
	{
		num += m_stageSelectArray[y].size();
	}
	num += m_nowStageNum.x;
	return static_cast<int>(num);
}