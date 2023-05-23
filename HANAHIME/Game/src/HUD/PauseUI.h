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
	//ポーズ起動中
	bool m_isActive = false;

	//項目
	enum PAUSE_ITEM
	{
		RESUME,	//ゲームを再開
		RETRY,	//リトライ
		FAST_TRAVEL,	//ファストトラベル
		SETTING,	//設定
		RETURN_TO_TITLE,	//タイトルに戻る
		PAUSE_ITEM_NUM,
	}m_item;

	//項目の状態
	enum ITEM_STATUS 
	{ 
		DEFAULT,	//デフォルト
		SELECT,	//選択中
		ITEM_STATUS_NUM 
	};

	//メニューの状態
	enum MENU_STATUS
	{
		DEFAULT_MENU,	//通常メニュー
		CONFIRM_MENU,	//最終確認
		MENU_STATUS_NUM
	}m_menuStatus;

	//デフォルトのメニュー
	struct DefaultMenu
	{
		//項目ごとの画像
		std::array<std::array<std::shared_ptr<KuroEngine::TextureBuffer>, ITEM_STATUS_NUM>, PAUSE_ITEM_NUM>m_itemTexArray;
		//選択中の項目にのみ出る影画像
		std::shared_ptr<KuroEngine::TextureBuffer>m_selectItemShadowTex;

		//ステージ名画像
		std::vector<std::shared_ptr<KuroEngine::TextureBuffer>>m_stageNameTex;
		std::shared_ptr<KuroEngine::TextureBuffer>m_stageNameDefaultTex;
		//ステージ名の装飾下線画像
		std::shared_ptr<KuroEngine::TextureBuffer>m_underLineTex;
		//ステージ名のインデックス
		int m_stageNameIdx = 0;

		//花の画像
		std::shared_ptr<KuroEngine::TextureBuffer>m_flowerTex;
		//収集花の数テクスチャ
		static const int FLOWER_NUM_TEX_SIZE = 10;
		std::array<std::shared_ptr<KuroEngine::TextureBuffer>, FLOWER_NUM_TEX_SIZE>m_flowerNumTexArray;
		//収集花の「 x 」テクスチャ
		std::shared_ptr<KuroEngine::TextureBuffer>m_flowerMulTex;
	}m_defaultMenu;

	//最終確認
	struct ConfirmMenu
	{
		//アイコン画像
		std::shared_ptr<KuroEngine::TextureBuffer>m_iconTex;

		//いいえ、はい
		enum CONFIRM_ITEM { NO, YES, CONFIRM_ITEM_NUM, NONE }m_confirmItem;
		//「いいえ」「はい」画像
		std::array<std::array<std::shared_ptr<KuroEngine::TextureBuffer>, ITEM_STATUS_NUM>, CONFIRM_ITEM_NUM>m_answerTexArray;
		//「いいえ」「はい」の選択側に出る影画像
		std::shared_ptr<KuroEngine::TextureBuffer>m_answerShadowTex;

		//「よろしいですか？」画像
		std::shared_ptr<KuroEngine::TextureBuffer>m_mindTex;

		//選んだ項目に応じた質問画像
		std::array<std::shared_ptr<KuroEngine::TextureBuffer>, PAUSE_ITEM_NUM>m_questionTexArray;
	}m_confirmMenu;

	//sinカーブレート
	float m_sinCurveRateT = 0.0f;

	//パーリンノイズ
	float m_perlinNoiseRateT = 0.0f;
	//パーリンノイズのシード
	KuroEngine::Vec2<int>m_perlinNoiseSeed = { 0,0 };
	//選択中の影画像のパーリンノイズ回転
	KuroEngine::Angle m_selectItemShadowSpin;
	//選択中の影画像のパーリンノイズ座標オフセット
	KuroEngine::Vec2<float> m_selectItemShadowOffset;

	//ポーズに入る直前のタイムスケール
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