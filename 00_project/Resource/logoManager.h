//============================================================
//
//	ロゴマネージャーヘッダー [logoManager.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _LOGOMANAGER_H_
#define _LOGOMANAGER_H_

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;		// オブジェクト2D

//************************************************************
//	クラス定義
//************************************************************
// ロゴマネージャークラス
class CLogoManager
{
public:

	// ポリゴンの種類
	enum EPoly
	{
		POLYGON_BACK = 0,	// 背景
		POLYGON_LOGO,		// ロゴ
		POLYGON_MAX			// この列挙型の総数
	};

	// 状態
	enum EState
	{
		STATE_APPEAR = 0,	// 出現状態
		STATE_WAIT,			// 待機状態
		STATE_DISAPPEAR,	// 消滅状態
		STATE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CLogoManager();

	// デストラクタ
	~CLogoManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(const float fDeltaTime);	// 更新

	// 静的メンバ関数
	static CLogoManager* Create(void);	// 生成
	static void Release(CLogoManager*& prResultManager);	// 破棄

private:

	// メンバ関数
	void UpdateAppear(void);	// 出現状態処理
	void UpdateWait(void);		// 待機状態処理
	void UpdateSkip(void);		// スキップ操作更新

	// メンバ変数
	CObject2D* m_apLogo[POLYGON_MAX];	// ロゴ情報
	EState m_state;						// 状態
	int m_nStateCount;					// 状態カウント
};

#endif	// _RESULTMANAGER_H_
