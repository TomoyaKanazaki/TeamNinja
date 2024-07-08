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

	bool CollisionPlayer() override;		// プレイヤーとの当たり判定(矩形)
	bool DistancePlayer();					// プレイヤーとの当たり判定(円形)

	// セット・ゲット関係
	void SetNumClone(const int nNum) { m_nNumClone = nNum; }	// 分身の総数の設定処理
	int GetNumClone(void) const { return m_nNumClone; }			// 分身の総数の取得処理
	void AddNumClone() { ++m_nNumClone; }						// 分身の総数を加算
	void SetEnableActive(const bool bAct) { m_bActive = bAct; }	// 発動条件の設定処理
	bool IsActive(void) const { return m_bActive; }				// 発動条件の取得処理
	bool GetMoment() const { return m_bMoment; }				// 発動中フラグの取得
	void SetMoment(bool moment) { m_bMoment = moment; }			// 発動中フラグの設定
	virtual bool IsFall() { return bool(); }					// 落としギミック用のフラグ取得
	static CListManager<CGimmickAction>* GetList(void);			// リスト取得
	void SetActionPoint(const D3DXVECTOR3& pos);				// 発動判定位置の設定
	D3DXVECTOR3 GetActionPoint() const { return m_posAction; }	// 発動判定位置の取得

	// 仮想関数
	virtual D3DXVECTOR3 CalcWaitPoint(const int Idx)	// 各分身毎の待機位置を算出
	{ assert(false); return GetVec3Position(); }
	virtual D3DXVECTOR3 CalcWaitRotation(const int Idx, const CPlayerClone* pClone)	// 各分身毎の待機向きを算出
	{ assert(false); return GetVec3Rotation(); }

private:

	// 静的メンバ変数
	static CListManager<CGimmickAction>* m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CGimmickAction>::AIterator m_iterator;	// イテレーター
	int m_nNumClone;			// 範囲に入っている分身の数
	bool m_bActive;				// 発動状況
	bool m_bMoment;				// 発動中フラグ
	D3DXVECTOR3 m_posAction;	// アクションポイント(待機座標)

};

#endif	// _GIMMICK_ACTION_H_
