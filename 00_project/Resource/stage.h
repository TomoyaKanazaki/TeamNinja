//============================================================
//
//	ステージヘッダー [stage.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _STAGE_H_
#define _STAGE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "scene.h"

//************************************************************
//	前方宣言
//************************************************************
class CField;	// 地面クラス

//************************************************************
//	クラス定義
//************************************************************
// ステージクラス
class CStage
{
public:
	// 神器列挙
	enum EGodItem
	{
		GOD_MIRROR = 0,	// 八咫鏡
		GOD_SWORD,		// 草薙剣
		GOD_MAGATAMA,	// 八尺瓊勾玉
		GOD_MAX			// この列挙型の総数
	};

	// 制限列挙
	enum ELimit
	{
		LIMIT_NONE = 0,	// 範囲制限なし
		LIMIT_BOX,		// 矩形範囲
		LIMIT_CIRCLE,	// 円範囲
		LIMIT_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CStage();

	// デストラクタ
	~CStage();

	// 読込パス構造体
	struct SPass
	{
		std::string sStage;		// ステージ情報パス
		std::string sEnemyAtc;	// 攻撃敵情報パス
		std::string sCoin;		// コイン情報パス
		std::string sGodItem;	// 神器情報パス
		std::string sActor;		// アクター情報パス
		std::string sGimmick;	// ギミック情報パス
		std::string sPoint;		// ポイント情報パス
		std::string sTrans;		// 遷移ポイント情報パス
	};

	// 範囲構造体
	struct SLimit
	{
		ELimit mode;		// 制限モード
		D3DXVECTOR3 center;	// 中心座標
		float fNear;		// 制限位置 (前)
		float fFar;			// 制限位置 (後)
		float fRight;		// 制限位置 (右)
		float fLeft;		// 制限位置 (左)
		float fRadius;		// 制限位置 (半径)
		float fField;		// 制限位置 (地面)
		float fKillY;		// 制限位置 (削除)
	};

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	SPass Regist(const char* pMapPass);		// マップパス登録
	HRESULT BindStage(const char* pPass);	// ステージ割当 (パス)
	HRESULT BindStage(const SPass& rPass);	// ステージ割当 (情報)

	void SetInitMapPass(const std::string& rMapPass)	{ m_sInitMapPass = rMapPass; }	// 初期化マップパス設定
	std::string GetInitMapPass(void) const				{ return m_sInitMapPass; }		// 初期化マップパス取得
	void SetLimit(const SLimit& rLimit)	{ m_limit = rLimit; }	// 範囲設定
	SLimit GetLimit(void) const			{ return m_limit;	}	// 範囲取得

	bool LandLimitPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight);	// 範囲外の着地判定
	bool LandFieldPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rOldPos, D3DXVECTOR3& rMove, CField** ppCollField = nullptr);	// 地面着地
	bool LandFieldPositionTop(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, CField** ppCollField = nullptr);						// 一番上の地面着地
	bool CollisionWall(D3DXVECTOR3& rPos, D3DXVECTOR3& rPosOld, const float fRadius, const float fHeight, D3DXVECTOR3& rMove, bool* pJump = nullptr);	// 壁との当たり判定
	void LimitPosition(D3DXVECTOR3& rPos, const float fRadius);	// 位置補正
	bool CollisionKillY(const D3DXVECTOR3& rPos);				// キルY座標との当たり判定
	bool IsFieldPositionRange(const D3DXVECTOR3& rPos);			// メッシュの範囲内取得
	float GetFieldPositionHeight(const D3DXVECTOR3& rPos);		// メッシュの着地位置取得

	// 静的メンバ関数
	static CStage *Create(void);			// 生成
	static void Release(CStage *&prStage);	// 破棄
	static HRESULT LoadPass(const char* pMapPass, SPass* pPassInfo);	// パス情報読込

private:
	// メンバ関数
	HRESULT LoadSetup(const char* pPass);					// セットアップ
	HRESULT LoadLimit(const char* pString, FILE *pFile);	// 範囲情報の読込
	HRESULT LoadField(const char* pString, FILE *pFile);	// 地面情報の読込
	HRESULT LoadWall(const char* pString, FILE *pFile);		// 壁情報の読込
	HRESULT LoadScenery(const char* pString, FILE *pFile);	// 景色情報の読込
	HRESULT LoadSky(const char* pString, FILE *pFile);		// 空情報の読込
	HRESULT LoadLiquid(const char* pString, FILE* pFile);	// 液体情報の読込
	HRESULT LoadChanger(const char* pString, FILE* pFile);	// カメラ変更地点情報の読込

	// メンバ変数
	std::map<std::string, SPass> m_mapPass;	// マップパス連想配列
	std::string m_sInitMapPass;	// 初期化時に生成するマップのパス
	SLimit m_limit;	// 範囲情報
};

#endif	// _STAGE_H_
