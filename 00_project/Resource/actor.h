//============================================================
//
//	アクターヘッダー [actor.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ACTOR_H_
#define _ACTOR_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectModel.h"
#include "listManager.h"
#include "collisionModel.h"

//************************************************************
//	クラス定義
//************************************************************
// アクタークラス
class CActor : public CObjectModel
{
public:

	// コンストラクタ
	CActor();

	// デストラクタ
	virtual ~CActor() override;

	// オーバーライド関数
	virtual HRESULT Init(void) override;	// 初期化
	virtual void Uninit(void) override;		// 終了
	virtual void Update(const float fDeltaTime) override;	// 更新
	virtual void Draw(CShader* pShader = nullptr) override;	// 描画

	// メンバ関数
	void Collision						// 当たり判定処理
	(
		D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rPosOld,		// 前回の位置
		const float fRadius,			// 半径
		const float fHeight,			// 高さ
		D3DXVECTOR3& rMove,				// 移動量
		bool& bJump						// ジャンプ状況
	);

	// 静的メンバ関数
	static CActor* Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXVECTOR3& rScale = VEC3_ONE	// 大きさ
	);
	static CListManager<CActor>* GetList(void);	// リスト構造の取得処理

private:

	// 静的メンバ変数
	static CListManager<CActor>* m_pList;		// リスト構造

	// メンバ変数
	CListManager<CActor>::AIterator m_iterator;	// イテレーター
	std::vector<CCollision*> m_collision;		// 当たり判定のリスト
};

#endif	// _ACTOR_H_
