//============================================================
//
// プレイヤー回帰UIヘッダー [playerbackUI.h]
// Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYERBACKUI_H_
#define _PLAYERBACKUI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object2D.h"

//************************************************************
//	クラス定義
//************************************************************
// CPlayerBackUIクラス
class CPlayerBackUI : public CObject2D
{
public:

	// 状態
	enum EState
	{
		STATE_NONE = 0,		// 通常状態
		STATE_ADD,			// 加算状態
		STATE_SUB,			// 減算状態
		STATE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CPlayerBackUI();

	// デストラクタ
	~CPlayerBackUI() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// セット・ゲット関係
	void SetState(const EState state)	{ m_state = state; }		// 状態設定処理

	// 静的メンバ関数
	static CPlayerBackUI* Create(void);	// 生成

private:

	// メンバ関数
	void AddFunc(const float fDeltaTime);	// 加算状態処理
	void SubFunc(const float fDeltaTime);	// 減算状態処理

	// メンバ変数
	EState m_state;		// 状態
};

#endif	// _EFFECT2D_H_
