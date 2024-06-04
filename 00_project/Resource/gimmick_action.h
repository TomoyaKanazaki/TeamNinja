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

	void CollisionClone() override;			// クローンとの当たり判定
	bool CollisionPlayer() override;		// プレイヤーとの当たり判定

	// セット・ゲット関係
	void SetNumClone(const int nNum) { m_nNumClone = nNum; }	// 分身の総数の設定処理
	int GetNumClone(void) const { return m_nNumClone; }			// 分身の総数の取得処理
	void SetEnableActive(const bool bAct) { m_bActive = bAct; }	// 発動条件の設定処理
	bool IsActive(void) const { return m_bActive; }				// 発動条件の取得処理
	bool GetMoment() const { return m_bMoment; }				// 発動中フラグの取得
	void SetMoment(bool moment) { m_bMoment = moment; }			// 発動中フラグの設定

private:

	// メンバ変数
	int m_nNumClone;	// 範囲に入っている分身の数
	bool m_bActive;		// 発動状況
	bool m_bMoment;		// 発動中フラグ

};

#endif	// _GIMMICK_ACTION_H_
