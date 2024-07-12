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
class CActor;	// アクタークラス

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
	void Uninit(void);	// 終了
	void Update(const float fDeltaTime);	// 更新
	void Draw(CShader* pShader = nullptr);	// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// 大きさ設定
	D3DXVECTOR3 CalcWaitPoint(const int Idx, const CPlayerClone* pClone) override;		// 各分身毎の待機位置を算出
	D3DXVECTOR3 CalcWaitRotation(const int Idx, const CPlayerClone* pClone) override;	// 各分身毎の待機向きを算出

private:

	void OpenTheDoor(D3DXVECTOR3& rPos);	// 扉を上げる
	void CloseTheDoor(D3DXVECTOR3& rPos);	// 扉を下げる

	// メンバ変数
	CActor* m_pGateModel;	// アクター(枠)の情報
	CActor* m_pDoorModel;	// アクター(扉)の情報
	D3DXVECTOR3	m_oldPosDoor;	// 過去位置
	D3DXVECTOR3 m_move;	// 移動量
	EState m_state;		// 扉の状態

};

#endif
