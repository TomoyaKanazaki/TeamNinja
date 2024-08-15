//============================================================
//
//	神器獲得演出マネージャーヘッダー [godItemManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GODITEM_MANAGER_H_
#define _GODITEM_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;		// オブジェクト2Dクラス
class CScrollString2D;	// 文字送り文字列2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// 神器獲得演出マネージャークラス
class CGodItemManager : public CObject
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_FADEOUT,	// フェードアウト状態
		STATE_LINE,		// 下線出現状態
		STATE_TITLE,	// タイトル出現状態
		STATE_NAME,		// 名前出現状態
		STATE_WAIT,		// 待機状態
		STATE_FADEIN,	// フェードイン状態
		STATE_FALL,		// UI消失状態
		STATE_END,		// 終了状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CGodItemManager();

	// デストラクタ
	~CGodItemManager() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// メンバ関数
	void SetState(const EState state)	{ m_state = state; }	// 状態設定
	EState GetState(void) const			{ return m_state; }		// 状態取得

	// 静的メンバ関数
	static CGodItemManager *Create(void);	// 生成

private:
	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CGodItemManager::*AFuncUpdateState)(const float fDeltaTime);

	// 静的メンバ変数
	static AFuncUpdateState m_aFuncUpdateState[];	// 状態更新関数
	static CGodItemManager* m_pThisClass;			// 自身のインスタンス

	// オーバーライド関数
	void Release(void) override { CObject::Release(); }	// 破棄

	// メンバ関数
	void UpdateFadeOut(const float fDeltaTime);	// フェードアウト更新
	void UpdateLine(const float fDeltaTime);	// 下線出現更新
	void UpdateTitle(const float fDeltaTime);	// タイトル出現更新
	void UpdateName(const float fDeltaTime);	// 名前出現更新
	void UpdateWait(const float fDeltaTime);	// 待機更新
	void UpdateFadeIn(const float fDeltaTime);	// フェードイン更新
	void UpdateFall(const float fDeltaTime);	// UI消失更新
	void UpdateEnd(const float fDeltaTime);		// 終了更新
	void UpdateSkip(void);	// スキップ操作更新
	void SkipStaging(void);	// 演出スキップ

	// メンバ変数
	CObject2D* m_pFade;			// フェード情報
	CObject2D* m_pTitle;		// タイトル情報
	CObject2D* m_pLine;			// 下線情報
	CScrollString2D* m_pName;	// 名前情報
	EState m_state;				// 状態
	float m_fCurTime;			// 現在の待機時間
};

#endif	// _GODITEM_MANAGER_H_
