//============================================================
//
//	神器ヘッダー [godItem.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GODITEM_H_
#define _GODITEM_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectModel.h"

//************************************************************
//	クラス定義
//************************************************************
// 神器クラス
class CGodItem : public CObjectModel
{
public:

	// 種類
	enum EType
	{
		TYPE_MIRROR = 0,	// 八咫鏡
		TYPE_SWORD,			// 草薙剣
		TYPE_MAGATAMA,		// 八尺瓊勾玉
		TYPE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CGodItem();

	// デストラクタ
	~CGodItem() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// メンバ関数
	bool Collision						// 当たり判定処理
	(
		D3DXVECTOR3& rPos,				// 位置
		const float fRadius,			// 半径
		const float fHeight				// 高さ
	);

	// セット・ゲット関係
	void SetType(const EType type);		// 種類の設定処理
	EType GetType(void) const;			// 種類の取得処理

	// 静的メンバ関数
	static CGodItem* Create(const D3DXVECTOR3& rPos, const EType type);		// 生成処理
	static CListManager<CGodItem>* GetList(void);			// リスト構造の取得処理
	static HRESULT LoadSetup(const char* pPass);			// セットアップ

private:

	// 静的メンバ関数
	static bool DuplicationCheck(const EType type);		// 重複チェック処理

	// 静的メンバ変数
	static CListManager<CGodItem>* m_pList;			// リスト構造

	// メンバ変数
	CListManager<CGodItem>::AIterator m_iterator;	// イテレーター
	EType m_type;									// 種類
};

#endif	// _COIN_H_
