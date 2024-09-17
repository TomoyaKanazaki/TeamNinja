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
#include "objectCircle2D.h"

//************************************************************
//	前方宣言
//************************************************************
class CFadeState;	// フェード状態クラス

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
	HRESULT ChangeState(CFadeState *pState);	// 状態変更
	bool SubAlpha(const float fDeltaTime);		// α値減少
	bool AddAlpha(const float fDeltaTime);		// α値増加
	void TransNextMode(void);	// 次シーン遷移
	bool IsFade(void);			// フェード中かの判定取得
	bool IsFadeIn(void);		// フェードイン中かの判定取得

	void SetFade	// フェード開始設定
	( // 引数
		const float fAddOut	= LEVEL,		// アウトのα値増加量
		const float fSubIn	= LEVEL,		// インのα値減少量
		const int nPriority	= PRIORITY,		// 優先順位
		const D3DXCOLOR& rCol = XCOL_ABLACK	// 色
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

	void SetCropPosition(const D3DXVECTOR3& rPos)	{ m_pCrop->SetVec3Position(rPos); }		// 切り抜き型位置設定
	D3DXVECTOR3 GetCropPosition(void) const			{ return m_pCrop->GetVec3Position(); }	// 切り抜き型位置取得
	void SetCropRadius(const float fRadius)			{ m_pCrop->SetRadius(fRadius); }		// 切り抜き型半径設定
	float GetCropRadius(void)						{ return m_pCrop->GetRadius(); }		// 切り抜き型半径取得
	float CalcCropRadius(const D3DXVECTOR3& rPos);	// 切り抜き半径計算

private:
	// メンバ変数
	std::function<HRESULT(CScene::EMode)> m_pFuncSetMode;	// モード設定関数ポインタ
	CObjectCircle2D *m_pCrop;	// 切り抜き型情報
	CFadeState *m_pState;		// 状態
	CScene::EMode m_modeNext;	// 次シーン
	float m_fSubIn;		// インのα値減少量
	float m_fAddOut;	// アウトのα値増加量
};

#endif	// _FADE_H_
