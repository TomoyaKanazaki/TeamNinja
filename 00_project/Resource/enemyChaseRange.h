//============================================================
//
//	敵の追跡範囲ヘッダー [enemyChaseRange.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_CHASE_RANGE_H_
#define _ENEMY_CHASE_RANGE_H_

//************************************************************
// 前方宣言
//************************************************************
class CObjectMeshCube;		// メッシュキューブ

//************************************************************
//	クラス定義
//************************************************************
// 敵の追跡範囲クラス
class CEnemyChaseRange
{
public:

	// コンストラクタ
	CEnemyChaseRange();

	// デストラクタ
	~CEnemyChaseRange();

	// オーバーライド関数
	HRESULT Init(void);		// 初期化
	void Uninit(void);		// 終了

	// メンバ関数
	bool ChaseRange(const D3DXVECTOR3& rInitPos, D3DXVECTOR3* pPos);					// 追跡範囲処理
	bool InsideTargetPos(const D3DXVECTOR3& rInitPos, const D3DXVECTOR3& rTargetPos);	// ターゲットが範囲内にいるかの処理

	// 静的メンバ関数
	static CEnemyChaseRange* Create(const D3DXVECTOR3& rPos, const float fWidth, const float fDepth);		// 生成処理

	// ゲッター
	float GetWidth(void) const { return m_fWidth; }		// 幅取得
	float GetDepth(void) const { return m_fDepth; }		// 奥行取得

private:

	// メンバ変数
	float m_fWidth;		// 幅
	float m_fDepth;		// 奥行

#ifdef _DEBUG

	CObjectMeshCube* m_pRangeCube;		// 範囲キューブ

#endif // _DEBUG
};

#endif	// _ENEMY_CHASE_RANGE_H_
