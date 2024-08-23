//============================================================
//
//	草ヘッダー [weed.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _WEED_H_
#define _WEED_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object3D.h"

//************************************************************
//	クラス定義
//************************************************************
// 草クラス
class CWeed : public CObject3D
{
public:
	// コンストラクタ
	CWeed();

	// デストラクタ
	~CWeed() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CWeed *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);
	static CListManager<CWeed>* GetList(void);	// リスト取得

	// メンバ関数
	bool CollisionPlayer(void);					// プレイヤーとの当たり判定
	void UpdateSwing(const float fDeltaTime);	// 風でなびく更新

private:

	// 静的メンバ変数
	static CListManager<CWeed>* m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CWeed>::AIterator m_iterator;	// イテレーター
	D3DXVECTOR3 m_offset;	// 上頂点オフセット
	bool m_bChange;			// 自然揺れと踏みつけの遷移フラグ
	float m_fCurLength;		// 現在の距離
	float m_fDestLength;	// 目標の距離
	float m_fCurAngle;		// 現在の角度
	float m_fDestAngle;		// 目標の角度
	float m_fGapRate;		// 揺れの範囲
};

#endif	// _WEED_H_
