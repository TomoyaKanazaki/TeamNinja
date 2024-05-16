//============================================================
//
// startUIヘッダー [startUI.h]
// Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _STARTUI_H_
#define _STARTUI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object2D.h"

//************************************************************
//	クラス定義
//************************************************************
// startUIクラス
class CStartUI : public CObject2D
{
public:

	// 状態
	enum EState
	{
		STATE_ZOOM = 0,		// ズーム状態
		STATE_BOUND,		// バウンド状態
		STATE_STOP,			// 停止状態
		STATE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CStartUI(void);

	// デストラクタ
	~CStartUI() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CStartUI* Create(void);	// 生成

private:

	// メンバ関数
	void State(void);		// 状態処理
	void Zoom(void);		// ズーム処理
	void Bound(void);		// バウンド処理

	// メンバ変数
	D3DXVECTOR3 m_sizeMove;	// サイズの移動量
	EState m_state;			// 状態
};

#endif	// _EFFECT2D_H_
