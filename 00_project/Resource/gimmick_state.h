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
	virtual void SetChar(CPlayerClone* pClone) = 0; // 文字列(フラグ)を追加
	virtual void DeleteChar(CPlayerClone* pClone) = 0; // 文字列(フラグ)を削除

private:

};

#endif	// _GIMMICK_ACTION_H_
