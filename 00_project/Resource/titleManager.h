//============================================================
//
//	タイトルマネージャーヘッダー [titleManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TITLEMANAGER_H_
#define _TITLEMANAGER_H_

//************************************************************
//	前方宣言
//************************************************************
class CTitleLogo2D;	// タイトルロゴ2Dクラス
class CBlink2D;		// 点滅オブジェクト2Dクラス
class CLogoManager;	// ロゴマネージャー

//************************************************************
//	クラス定義
//************************************************************
// タイトルマネージャークラス
class CTitleManager
{
public:
	// 定数
	static constexpr int NUM_LOGO = 4;	// 文字数

	// 状態列挙
	enum EState
	{
		STATE_LOGO = 0,	// ロゴ表示
		STATE_TITLE,	// タイトル表示
		STATE_WAIT,		// 操作待機
		STATE_RUN,		// 忍者疾走
		STATE_END,		// 終了
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CTitleManager();

	// デストラクタ
	~CTitleManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(const float fDeltaTime);	// 更新

	// 静的メンバ関数
	static CTitleManager *Create(void);	// 生成
	static void Release(CTitleManager *&prTitleManager);	// 破棄

private:
	// メンバ関数
	void UpdateSkip(void);	// スキップ操作更新
	void SkipStaging(void);	// 演出スキップ

	// メンバ変数
	CLogoManager* m_pLogoManager;		// ロゴマネージャー
	CTitleLogo2D* m_apLogo[NUM_LOGO];	// タイトル情報
	CBlink2D* m_pStart;	// 開始操作情報
	EState m_state;		// 状態
	float m_fCurTime;	// 現在の経過時間
};

#endif	// _TITLEMANAGER_H_
