//植えた草の情報
struct PlantGrass
{
    float3 m_pos;
    int m_texIdx;
    float3 m_normal;
    float m_sineLength;
    float m_appearY;
    float m_appearYTimer;
};

//全草むらで共通する定数バッファ用のパラメータ
struct CommonGrassInfo
{
    //判定を飛ばす距離
    float m_checkClipOffset;
	//周辺に既に草が生えているか確認する際の範囲
    float m_checkRange;
    //草むら登場時のイージング速度
    float m_appearEaseSpeed;
    //草を揺らす際のSine量 つまり風
    float m_sineWave;
    //プレイヤーの座標
    float3 m_playerPos;
};

//草むら以外の外部オブジェクトのトランスフォーム情報
struct TransformData
{
    float3 m_camPos;
    int m_seed;
    int m_grassCount;
};

//ランダム
int RandomIntInRange(int arg_minVal, int arg_maxVal, int arg_seed)
{
    return frac(sin(dot(float2(arg_seed, arg_seed + 1), float2(12.9898, 78.233))) * 43758.5453) * (arg_maxVal - arg_minVal + 1) + arg_minVal;
}