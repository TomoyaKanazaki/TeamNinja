//============================================================
//
//	コインヘッダー [coin.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _COIN_H_
#define _COIN_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectModel.h"

//************************************************************
//	クラス定義
//************************************************************
// コインクラス
class CCoin : public CObjectModel
{
public:

	// コンストラクタ
	CCoin();

	// デストラクタ
	~CCoin() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// メンバ関数
	void Collision						// 当たり判定処理
	(
		D3DXVECTOR3& rPos,				// 位置
		const float fRadius,			// 半径
		const float fHeight			// 高さ
	);

	// 静的メンバ関数
	static CCoin* Create(const D3DXVECTOR3& rPos);		// 生成処理
	static CListManager<CCoin>* GetList(void);			// リスト構造の取得処理
	static HRESULT LoadSetup(const char* pPass);		// セットアップ

private:

	// 静的メンバ変数
	static CListManager<CCoin>* m_pList;			// リスト構造

	// メンバ変数
	CListManager<CCoin>::AIterator m_iterator;		// イテレーター
};

#endif	// _COIN_H_
