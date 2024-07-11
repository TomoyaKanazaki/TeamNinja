//============================================================
//
//	遷移ポイントヘッダー [transpoint.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TRANSPOINT_H_
#define _TRANSPOINT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectModel.h"

//************************************************************
//	クラス定義
//************************************************************
// 遷移ポイントクラス
class CTransPoint : public CObjectModel
{
public:
	// コンストラクタ
	explicit CTransPoint(const char* pPass);

	// デストラクタ
	~CTransPoint() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CTransPoint *Create	// 生成
	( // 引数
		const char* pPass,					// 遷移先マップパス
		const D3DXVECTOR3& rPos,			// 位置
		const D3DXVECTOR3& rRot = VEC3_ZERO	// 向き
	);
	static CListManager<CTransPoint>* GetList(void);	// リスト取得
	static HRESULT LoadSetup(const char* pPass);		// セットアップ

	// メンバ関数


private:
	// 静的メンバ変数
	static CListManager<CTransPoint>* m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CTransPoint>::AIterator m_iterator;	// イテレーター
	const char* m_pTransMapPass;	// 遷移先マップパス
};

#endif	// _TRANSPOINT_H_
