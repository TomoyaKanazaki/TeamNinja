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
//	前方宣言
//************************************************************
class CMapModel;		// マップモデルクラス
class CStage;			// ステージクラス

//************************************************************
//	クラス定義
//************************************************************
class CGimmickHeavyDoor : public CGimmickAction
{
public:

	// 扉の状態列挙
	enum EState
	{
		STATE_NONE = 0,		// 何もしてない
		STATE_CLOSE,		// 扉閉じてる
		STATE_OPEN,			// 扉上げてる
		STATE_FULLY,		// 扉全開
		STATE_MAX
	};

	CGimmickHeavyDoor();
	~CGimmickHeavyDoor();

	HRESULT Init(void);	// 初期化
	void Uninit(void);		// 終了
	void Update(const float fDeltaTime);	// 更新
	void Draw(CShader* pShader = nullptr);	// 描画

private:

	void OpenTheDoor(void);	// 扉を上げる
	void CloseTheDoor(void);	// 扉を下げる

	// メンバ変数
	CMapModel* m_pMapModel;	// マップモデルの情報
	CStage* m_pStage;		// ステージの情報

	D3DXVECTOR3 m_move;		// 移動量
	int m_nDoorCounter;		// 扉の開閉カウンター
	int m_nNumBound;		// 跳ねた回数

	EState m_state;			// 扉の状態

};

#endif
