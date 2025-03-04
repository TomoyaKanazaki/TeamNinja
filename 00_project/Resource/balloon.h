//============================================================
//
//	吹き出しヘッダー [balloon.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _BALLOON_H_
#define _BALLOON_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectBillboard.h"

//************************************************************
//	クラス定義
//************************************************************
// 吹き出しクラス
class CBalloon : public CObjectBillboard
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_OPEN ,	// 解放状態
		STATE_NORMAL,	// 通常状態
		STATE_END,		// 終了状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CBalloon();

	// デストラクタ
	~CBalloon();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定

	// 静的メンバ関数
	static CBalloon *Create(const D3DXVECTOR3& rPosParent);	// 生成

	// メンバ関数
	void SetFirstDisp(void);		// 吹き出し初表示設定
	void SetDisp(const bool bDisp);	// 吹き出し表示設定
	bool IsSizeDisp(void) const;	// 吹き出し拡大状況

private:
	// メンバ変数
	EState m_state;		// 状態
	float m_fCurTime;	// 現在時間
	float m_fScale;		// 拡大率
	float m_fDestScale;	// 目標拡大率
};

#endif	// _BALLOON_H_
