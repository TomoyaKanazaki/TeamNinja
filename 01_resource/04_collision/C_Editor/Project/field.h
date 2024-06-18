//============================================================
//
//	地面ヘッダー [field.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FIELD_H_
#define _FIELD_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectMeshField.h"

//************************************************************
//	クラス定義
//************************************************************
// 地面クラス
class CField : public CObjectMeshField
{
public:
	// テクスチャ列挙
	enum ETexture
	{
		TEXTURE_ASH = 0,	// 火山灰テクスチャ
		TEXTURE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CField();

	// デストラクタ
	~CField() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CField *Create	// 生成
	( // 引数
		const ETexture texture,		// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR2& rSize,	// 大きさ
		const D3DXCOLOR& rCol,		// 色
		const POSGRID2& rPart		// 分割数
	);
};

#endif	// _FIELD_H_
