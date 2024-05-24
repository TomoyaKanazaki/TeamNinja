//============================================================
//
// アクションギミックヘッダー [gimmick_action.h]
// Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GIMMICK_ACTION_H_
#define _GIMMICK_ACTION_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "gimmick.h"

//************************************************************
//	クラス定義
//************************************************************
// アクションギミッククラス
class CGimmickAction : public CGimmick
{
public:

	// コンストラクタ
	CGimmickAction();

	// デストラクタ
	virtual ~CGimmickAction() override;

	// オーバーライド関数
	virtual HRESULT Init(void) override;	// 初期化
	virtual void Uninit(void) override;		// 終了
	virtual void Update(const float fDeltaTime) override;	// 更新
	virtual void Draw(CShader* pShader = nullptr) override;	// 描画

	virtual void CollisionClone(void);		// クローンとの当たり判定

	// セット・ゲット関係
	void SetNumClone(const int nNum) { m_nNumClone = nNum; }	// 分身の総数の設定処理
	int GetNumClone(void) const { return m_nNumClone; }			// 分身の総数の取得処理
	void SetNumActive(const int nNum) { m_nNumActive = nNum; }	// 発動可能人数の設定処理
	int GetNumActive(void) const { return m_nNumActive; }		// 発動可能人数の取得処理
	void SetEnableActive(const bool bAct) { m_bActive = bAct; }	// 発動条件の設定処理
	bool IsActive(void) const { return m_bActive; }				// 発動条件の取得処理

private:

	// メンバ変数
	int m_nNumClone;	// 範囲に入っている分身の数
	int m_nNumActive;	// 発動可能な分身の数
	bool m_bActive;		// 発動状況
};

#endif	// _GIMMICK_ACTION_H_
