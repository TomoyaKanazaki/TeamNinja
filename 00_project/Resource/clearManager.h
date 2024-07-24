#if 1
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
	virtual void Update(const float fDeltaTime)	= 0;	// 更新

	// 静的メンバ関数
	static CClearManager *Create(const CRetentionManager::EWin win);	// 生成
	static void Release(CClearManager *&prClearManager);	// 破棄
};

#endif	// _CLEARMANAGER_H_
#endif
