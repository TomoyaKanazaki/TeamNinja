//============================================================
//
//	勾玉UIヘッダー [goditemUI.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GODITEMUI_H_
#define _GODITEMUI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "godItem.h"

//************************************************************
//	前方宣言
//************************************************************
class CAnim2D;	// アニメ2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// 神器UIクラス
class CGodItemUI : public CObject
{
public:

	// 勾玉マークの情報
	struct SBody
	{
		CAnim2D* pMark;	// 勾玉本体の情報
		bool bGet;			// 取得状況
	};

	// コンストラクタ
	CGodItemUI();

	// デストラクタ
	~CGodItemUI();

	// メンバ関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	void Get(const CGodItem::EType type);			// 取得処理

	// 静的メンバ関数
	static CGodItemUI* Create(void);				// 生成
	static CListManager<CGodItemUI>* GetList();		// リスト取得

private:
	// オーバーライド関数
	void Release(void) override { CObject::Release(); }	// 破棄

	// メンバ変数
	CListManager<CGodItemUI>::AIterator m_iterator;	// イテレーター

	CAnim2D* m_apGround[CGodItem::TYPE_MAX];	// 下地の情報
	SBody m_aBody[CGodItem::TYPE_MAX];			// 本体の情報

	// 静的メンバ変数
	static CListManager<CGodItemUI>* m_pList;	// オブジェクトリスト
};

#endif	// _GODITEMUI_H_
