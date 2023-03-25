#pragma once
#include<memory>
#include"Common/PerlinNoise.h"
#include"ForUser/Debugger.h"

namespace KuroEngine
{
	class TextureBuffer;
	class ComputePipeline;
}

//水彩画風に２つのテクスチャをブレンド
class WaterPaintBlend : public KuroEngine::Debugger
{
	static const int THREAD_PER_NUM = 32;

	//パイプライン
	static std::shared_ptr<KuroEngine::ComputePipeline>s_pipeline;
	void GeneratePipeline();

	//パーリンノイズの設定
	KuroEngine::NoiseInitializer m_noiseInitializer;
	//境界ぼかしに使うノイズテクスチャ
	std::shared_ptr<KuroEngine::TextureBuffer>m_noiseTex;
	//結果の描画先
	std::shared_ptr<KuroEngine::TextureBuffer>m_resultTex;

	void OnImguiItems()override;

public:
	WaterPaintBlend();

	//グラフィックスマネージャに登録
	void Register(
		std::shared_ptr<KuroEngine::TextureBuffer>arg_baseTex,
		std::shared_ptr<KuroEngine::TextureBuffer>arg_blendTex,
		std::shared_ptr<KuroEngine::TextureBuffer>arg_maskTex);

	//結果のテクスチャ取得
	std::shared_ptr<KuroEngine::TextureBuffer>& GetResultTex() { return m_resultTex; }
};