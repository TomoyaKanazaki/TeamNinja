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

//************************************************************
// 前方宣言
//************************************************************
class CCollisionCube;		// キューブの当たり判定
class CCollisionCylinder;	// シリンダーの当たり判定
class CCollisionSphere;		// スフィアの当たり判定
class CCollisionPolygon;	// ポリゴンの当たり判定

//************************************************************
//	クラス定義
//************************************************************
// アクタークラス
class CActor : public CObjectModel
{
public:

	// 種類列挙
	enum EType
	{
		TYPE_ROCK_S = 0,		// 岩(小)
		TYPE_ROCK_M,			// 岩(中)
		TYPE_ROCK_B,			// 岩(大)
		TYPE_MOSS_ROCK_S,		// 苔岩(小)
		TYPE_MOSS_ROCK_M,		// 苔岩(中)
		TYPE_MOSS_ROCK_B,		// 苔岩(大)
		TYPE_BIRCHTREE_B,		// 白樺の木(大)
		TYPE_BIRCHTREE_M,		// 白樺の木(中)
		TYPE_COM_TREE_B,		// 普通の木(大)
		TYPE_COM_TREE_M,		// 普通の木(中)
		TYPE_WILLOW_B,			// 柳の木(大)
		TYPE_WILLOW_M,			// 柳の木(中)
		TYPE_WOODLOG,			// 原木
		TYPE_TREESTOMP,			// 木の幹
		TYPE_LILYPAD,			// 蓮の葉
		TYPE_PLANT1,			// 4つしかつながっていない草
		TYPE_PLANT2,			// 多くつながっている草
		TYPE_BUSH,				// 草むら
		TYPE_HOUSE1,			// 家1
		TYPE_HOUSE2,			// 家2
		TYPE_HOUSE3,			// 家3
		TYPE_HOUSE4,			// 家4
		TYPE_HOUSE5,			// 家5
		TYPE_HOUSE6,			// 家6
		TYPE_HOUSE7,			// 家7
		TYPE_HOUSE8,			// 家8
		TYPE_HOUSE9,			// 家9
		TYPE_HOUSE10,			// 家10
		TYPE_LANTERN,			// ランタン
		TYPE_GATE,				// 門(枠部分)
		TYPE_DOOR,				// 門(扉部分)
		TYPE_KAKEJIKU,			// 掛け軸
		TYPE_PIER,				// 橋
		TYPE_PILLER1,			// 城に配置する形の柱
		TYPE_PILLER2,			// ただ四角いだけの柱
		TYPE_STAIRS,			// 階段
		TYPE_B_LATTICE,			// 竹の格子
		TYPE_ROOFFLOOR1,		// 骨組みみたいな屋根裏の木
		TYPE_ROOFFLOOR2,		// レンガ柄風の屋根裏の木
		TYPE_MAX				// この列挙型の総数
	};

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
	void Collision						// 当たり判定処理(判定を返すオーバーライド)
	(
		D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rPosOld,		// 前回の位置
		const float fRadius,			// 半径
		const float fHeight,			// 高さ
		D3DXVECTOR3& rMove,				// 移動量
		bool& bJump,					// ジャンプ状況
		bool& bHit						// 衝突判定
	);
	void ClearCollision(void);			// 当たり判定の消去処理
	void SetModel(const EType type);	// モデルの割り当て処理

	// セット・ゲット関数
	void SetType(const EType type) { m_type = type; }		// 種類の設定処理
	EType GetType(void) const { return m_type; }			// 種類の取得処理
	std::vector<CCollisionCube*> GetCube(void) const { return m_cube; }					// キューブの情報の取得処理
	std::vector<CCollisionCylinder*> GetCylinder (void) const { return m_cylinder; }	// シリンダーの情報の取得処理
	std::vector<CCollisionSphere*> GetSphere(void) const { return m_sphere; }			// スフィアの情報の取得処理
	std::vector<CCollisionPolygon*> GetPolygon(void) const { return m_polygon; }		// ポリゴンの情報の取得処理

	// 静的メンバ関数
	static CActor* Create	// 生成
	( // 引数
		const EType type,						// 種類
		const D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXVECTOR3& rScale = VEC3_ONE	// 拡大率
	);
	static CListManager<CActor>* GetList(void);		// リスト構造の取得処理
	static HRESULT LoadSetup(const char* pPass);	// セットアップ

private:

	// メンバ関数
	void BindCollision(void);						// 当たり判定の割り当て処理

	// 静的メンバ変数
	static CListManager<CActor>* m_pList;			// リスト構造

	// メンバ変数
	CListManager<CActor>::AIterator m_iterator;		// イテレーター
	EType m_type;									// 種類
	std::vector<CCollisionCube*> m_cube;			// キューブの可変長配列
	std::vector<CCollisionCylinder*> m_cylinder;	// シリンダーの可変長配列
	std::vector<CCollisionSphere*> m_sphere;		// スフィアの可変長配列
	std::vector<CCollisionPolygon*> m_polygon;		// ポリゴンの可変長配列
};

#endif	// _ACTOR_H_
