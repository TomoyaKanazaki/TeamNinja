//============================================================
//
//	壁ヘッダー [wall.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _WALL_H_
#define _WALL_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectMeshWall.h"

//==========================================
//  前方宣言
//==========================================
class CCamera;

//************************************************************
//	クラス定義
//************************************************************
// 壁クラス
class CWall : public CObjectMeshWall
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_SOIL = 0,	// 土テクスチャ
		TYPE_WEED_SOIL,	// 草土テクスチャ
		TYPE_WEED,		// 草テクスチャ
		TYPE_WATER,		// 水テクスチャ
		TYPE_PLASTER,	// 城の漆喰テクスチャ
		TYPE_WOOD,		// 木のテクスチャ
		TYPE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CWall();

	// デストラクタ
	~CWall() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// メンバ関数(小原追加)
	bool Collision				// 当たり判定処理
	(
		D3DXVECTOR3& rPos,			// 位置
		D3DXVECTOR3& rPosOld,		// 前回の位置
		const float fRadius,		// 半径
		const float fHeight,		// 高さ
		D3DXVECTOR3& rMove,			// 移動量
		bool* pJump = nullptr		// ジャンプ状況
	);

	// 静的メンバ関数
	static CWall *Create	// 生成
	( // 引数
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR2& rSize,	// 大きさ
		const D3DXCOLOR& rCol,		// 色
		const POSGRID2& rPart,		// 分割数
		const POSGRID2& rTexPart	// テクスチャ分割数
	);
	static CListManager<CWall> *GetList(void);	// リスト取得

	// メンバ関数
	void SetType(const EType type);					// 種類設定
	EType GetType(void) const { return m_type; }	// 種類取得

private:

	// メンバ関数
	void Invisible(); // 透明化処理
	float InvisibleVtx(const D3DXVECTOR3& posVtx, const D3DXVECTOR3& posPlayer, const D3DXVECTOR3& posPlayerWorld, SMeshWall meshWall, CCamera* pCamera); // 頂点計算処理

	// 静的メンバ変数
	static CListManager<CWall> *m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CWall>::AIterator m_iterator;	// イテレーター
	EType m_type;	// 種類
};

#endif	// _WALL_H_
