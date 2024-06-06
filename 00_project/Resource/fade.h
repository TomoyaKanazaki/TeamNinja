//============================================================
//
//	フェードヘッダー [fade.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FADE_H_
#define _FADE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "scene.h"
#include "object2D.h"

//************************************************************
//	クラス定義
//************************************************************
// フェードクラス
class CFade : public CObject2D
{
public:
	// 定数
	static constexpr int	PRIORITY = 7;	// フェード優先順位
	static constexpr float	LEVEL = 1.0f;	// フェードα値加減量

	// フェード状態列挙
	enum EFade
	{
		FADE_NONE = 0,	// 何もしていない状態
		FADE_WAIT,		// フェード余韻状態
		FADE_IN,		// フェードイン状態
		FADE_OUT,		// フェードアウト状態
		FADE_IRISOUT,	// アイリスアウト状態
		FADE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CFade();

	// デストラクタ
	~CFade();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CFade *Create(void);	// 生成

	// メンバ関数
	EFade GetState(void) const { return m_fade; }	// フェード状態取得

	void SetFade	// フェード開始設定
	( // 引数
		const float fAddOut	= LEVEL,	// アウトのα値増加量
		const float fSubIn	= LEVEL,	// インのα値減少量
		const int nPriority	= PRIORITY	// 優先順位
	);
	void SetModeFade	// 次シーン設定 (フェードのみ)
	( // 引数
		const CScene::EMode mode,			// 次シーン
		const float fWaitTime	= 0.0f,		// 余韻時間
		const float fAddOut		= LEVEL,	// アウトのα値増加量
		const float fSubIn		= LEVEL		// インのα値減少量
	);
	void SetLoadFade	// 次シーン設定 (ロード画面付き)
	( // 引数
		const CScene::EMode mode,			// 次シーン
		const float fWaitTime	= 0.0f,		// 余韻時間
		const float fAddOut		= LEVEL,	// アウトのα値増加量
		const float fSubIn		= LEVEL		// インのα値減少量
	);

private:
	// メンバ変数
	std::function<HRESULT(CScene::EMode)> m_pFuncSetMode;	// モード設定関数ポインタ
	CScene::EMode m_modeNext;	// 次シーン
	EFade m_fade;		// フェード状態
	float m_fWaitTime;	// 現在の余韻時間
	float m_fSubIn;		// インのα値減少量
	float m_fAddOut;	// アウトのα値増加量

	CObject2D *m_pCircle;	// アイリスアウト切り抜き型
};

#endif	// _FADE_H_
