//============================================================
//
// 重い扉ヘッダー [gimmick_heavydoor.h]
// Author：佐藤根詩音
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GIMMICK_HEAVYDOOR_H_
#define _GIMMICK_HEAVYDOOR_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "gimmick_action.h"

//************************************************************
//	クラス定義
//************************************************************
class CGimmickHeavyDoor : public CGimmickAction
{
public:
	CGimmickHeavyDoor();
	~CGimmickHeavyDoor();

	HRESULT Init(void);	// 初期化
	void Uninit(void);		// 終了
	void Update(const float fDeltaTime);	// 更新
	void Draw(CShader* pShader = nullptr);	// 描画

private:
	void PushDoor(void);		// ドア押す

};

#endif
