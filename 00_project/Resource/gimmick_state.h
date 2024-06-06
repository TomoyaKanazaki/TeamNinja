//============================================================
//
// 状態ギミックヘッダー [gimmick_state.h]
// Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GIMMICK_STATE_H_
#define _GIMMICK_STATE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "gimmick.h"

//===========================================
//  前方宣言
//===========================================
class CPlayerClone;

//************************************************************
//	クラス定義
//************************************************************
// 状態ギミッククラス
class CGimmickState : public CGimmick
{
public:

	// 各ギミックのフラグ
	enum EFrag
	{
		GRAVEL = 'g', // 砂利道
		BOOB = 'b', // ドブ
		WATER = 'w', // 水場
		CLEEN = 'c', // 掃除したての床
		MAX
	};

	// コンストラクタ
	CGimmickState();

	// デストラクタ
	virtual ~CGimmickState() override;

	// オーバーライド関数
	virtual HRESULT Init(void) override;	// 初期化
	virtual void Uninit(void) override;		// 終了
	virtual void Update(const float fDeltaTime) override;	// 更新
	virtual void Draw(CShader* pShader = nullptr) override;	// 描画

	void CollisionClone() override;			// クローンとの当たり判定

protected:

	// 純粋仮想関数
	virtual void HitClone(CPlayerClone* pClone) = 0; // 分身に当たっていた時の処理
	virtual void MissClone(CPlayerClone* pClone) = 0; // 分身に当たっていない場合の処理

private:

};

#endif	// _GIMMICK_ACTION_H_
