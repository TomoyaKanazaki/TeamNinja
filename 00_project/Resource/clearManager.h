//============================================================
//
//	クリアマネージャーヘッダー [clearManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _CLEARMANAGER_H_
#define _CLEARMANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "retentionManager.h"
#include "scene.h"

//************************************************************
//	クラス定義
//************************************************************
// クリアマネージャークラス
class CClearManager
{
public:
	// コンストラクタ
	CClearManager();

	// デストラクタ
	virtual ~CClearManager();

	// 純粋仮想関数
	virtual HRESULT Init(void)	= 0;	// 初期化
	virtual void Uninit(void)	= 0;	// 終了
	virtual void Update(const float fDeltaTime)			= 0;	// 更新
	virtual void SetAllMove(const D3DXVECTOR3& rMove)	= 0;	// 全UIオブジェクト移動
	virtual void SkipStaging(void)						= 0;	// 演出スキップ
	virtual bool IsSkipOK(void) const					= 0;	// スキップ可能状況取得
	virtual bool IsEndState(void) const					= 0;	// 終了状況取得

	// 静的メンバ関数
	static CClearManager *Create(const CRetentionManager::EWin win);	// 生成
	static void Release(CClearManager *&prClearManager);				// 破棄

	// メンバ関数
	void SetNext(const CScene::EMode next)	{ m_modeNext = next; }	// 遷移先シーン設定
	CScene::EMode GetNext(void) const		{ return m_modeNext; }	// 遷移先シーン取得

private:
	// メンバ変数
	CScene::EMode m_modeNext;	// 遷移先シーン	// TODO：将来的にステージパスにする
};

#endif	// _CLEARMANAGER_H_
