//============================================================
//
//	敵の持ち物ヘッダー [enemy_item.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_ITEM_H_
#define _ENEMY_ITEM_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectModel.h"

//************************************************************
//	クラス定義
//************************************************************
// 敵の持ち物クラス
class CEnemyItem : public CObjectModel
{
public:

	// 種類列挙
	enum EType
	{
		TYPE_KATANA = 0,		// 刀
		TYPE_MAX				// この列挙型の総数
	};

	// コンストラクタ
	CEnemyItem();

	// デストラクタ
	virtual ~CEnemyItem() override;

	// オーバーライド関数
	virtual HRESULT Init(void) override;	// 初期化
	virtual void Uninit(void) override;		// 終了
	virtual void Update(const float fDeltaTime) override;	// 更新
	virtual void Draw(CShader* pShader = nullptr) override;	// 描画

	// メンバ関数
	void Offset(const D3DXMATRIX& rMtx);	// オフセット処理

	// セット・ゲット関数
	void SetType(const EType type) { m_type = type; }		// 種類の設定処理
	EType GetType(void) const { return m_type; }			// 種類の取得処理

	// 静的メンバ関数
	static CEnemyItem* Create			// 生成
	( // 引数
		const EType type,				// 種類
		const D3DXVECTOR3& rOffset,		// オフセット
		const D3DXMATRIX& rMtx			// マトリックス情報
	);

private:

	// メンバ変数
	D3DXVECTOR3 m_offset;	// オフセット
	EType m_type;			// 種類
};

#endif	// _ACTOR_H_
