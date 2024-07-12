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
	static CTransPoint *Create(const char* pPass, const D3DXVECTOR3& rPos);			// 生成
	static CTransPoint* Collision(const D3DXVECTOR3& rPos, const float fRadius);	// 遷移ポイントとの当たり判定
	static CListManager<CTransPoint>* GetList(void);	// リスト取得
	static HRESULT LoadSetup(const char* pPass);		// セットアップ

	// メンバ関数
	const std::string GetTransMapPass(void) { return m_sTransMapPass; }	// 遷移先マップパス取得

private:
	// 静的メンバ変数
	static CListManager<CTransPoint>* m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CTransPoint>::AIterator m_iterator;	// イテレーター
	const std::string m_sTransMapPass;	// 遷移先マップパス
};

#endif	// _TRANSPOINT_H_
