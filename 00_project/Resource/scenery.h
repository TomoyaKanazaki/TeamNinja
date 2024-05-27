//============================================================
//
//	景色ヘッダー [scenery.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCENERY_H_
#define _SCENERY_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectMeshCylinder.h"

//************************************************************
//	クラス定義
//************************************************************
// 景色クラス
class CScenery : public CObjectMeshCylinder
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_MOUNTAIN_SMALL_00 = 0,	// 山テクスチャ
		TYPE_MOUNTAIN_SMALL_01,		// 山テクスチャ
		TYPE_MOUNTAIN_BIG,			// 山テクスチャ
		TYPE_MAX					// この列挙型の総数
	};

	// コンストラクタ
	CScenery();

	// デストラクタ
	~CScenery() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CScenery *Create	// 生成
	( // 引数
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXCOLOR& rCol,		// 色
		const POSGRID2& rPart,		// 分割数
		const POSGRID2& rTexPart,	// テクスチャ分割数
		const float fRadius,		// 半径
		const float fHeight			// 縦幅
	);
	static CListManager<CScenery> *GetList(void);	// リスト取得

	// メンバ関数
	void SetType(const EType type);					// 種類設定
	EType GetType(void) const { return m_type; }	// 種類取得

private:
	// 静的メンバ変数
	static CListManager<CScenery> *m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CScenery>::AIterator m_iterator;	// イテレーター
	EType m_type;	// 種類
};

#endif	// _SCENERY_H_
