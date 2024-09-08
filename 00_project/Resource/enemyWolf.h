//============================================================
//
//	狼敵ヘッダー [enemyWolf.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_WOLF_H_
#define _ENEMY_WOLF_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "enemyAttack.h"

//************************************************************
// 前方宣言
//************************************************************
class CEnemyNav;		// 敵のナビゲーション

//************************************************************
//	クラス定義
//************************************************************
// 狼敵クラス
class CEnemyWolf : public CEnemyAttack
{
public:

	// モーション列挙
	enum EMotion
	{
		MOTION_IDOL = 0,	// 待機モーション
		MOTION_RUN,			// 走行モーション
		MOTION_FOUND,		// 発見モーション
		MOTION_BITE,		// 噛みつきモーション
		MOTION_TURN,		// 振り向きモーション
		MOTION_FALL,		// 落下モーション
		MOTION_LANDING,		// 着地モーション
		MOTION_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEnemyWolf();

	// デストラクタ
	~CEnemyWolf() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	void SetData(void) override;	// 情報の設定処理

	float GetRadius(void) const override;			// 半径の取得処理
	float GetHeight(void) const override;			// 高さの取得処理
	float GetSpeed(void) const override;			// 速度の取得処理

	// 静的メンバ関数
	static CEnemyWolf* Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const float fMoveWidth,		// 移動幅
		const float fMoveDepth,		// 移動奥行
		const float fChaseWidth,	// 追跡幅
		const float fChaseDepth		// 追跡奥行
	);
	static CEnemyWolf* Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rRot,				// 向き
		const std::vector<D3DXVECTOR3> route,	// ルートの配列
		const float fChaseWidth,				// 追跡幅
		const float fChaseDepth					// 追跡奥行
	);

private:

	// オーバーライド関数
	int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override;	// 状態の更新処理
	void UpdateMotion(int nMotion, const float fDeltaTime) override;	// モーションの更新処理
	void UpdateLanding(D3DXVECTOR3* pPos) override;	// 着地更新
	void NavMoitonSet(int* pMotion);				// ナビゲーションによるモーションの設定処理

	// メンバ関数
	int Original(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override;	// それぞれの独自処理
	int Warning				// 警告処理
	(
		D3DXVECTOR3* pPos,		// 位置
		D3DXVECTOR3* pRot,		// 向き
		const float fDeltaTime,	// デルタタイム
		const float fRotRev		// 向きの補正数
	) override;
	int Stalk				// 追跡処理
	(
		D3DXVECTOR3* pPos,		// 位置
		D3DXVECTOR3* pRot, 		// 向き
		const float fDeltaTime,	// デルタタイム
		const float fRotRev		// 向きの補正数
	) override;
	int Attack(const D3DXVECTOR3& rPos) override;	// 攻撃処理
	int BlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime, const float fRotRev) override;		// 空白攻撃処理
	int Upset(void) override;						// 動揺処理
	int Stance(void) override;						// 構え処理
	bool BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight) override;			// 元の位置に戻る処理

	// メンバ変数
	CEnemyNav* m_pNav;					// ナビゲーションの情報
};

#endif	// _ENEMY_CHASE_H_
