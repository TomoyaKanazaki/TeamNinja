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
#include "effekseerControl.h"
#include "effekseerManager.h"

//************************************************************
//	前方宣言
//************************************************************
class CBalloon;			// 吹き出しクラス
class CBalloonManager;	// 吹き出しマネージャークラス

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
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定

	// 静的メンバ関数
	static CTransPoint *Create(const char* pPass, const D3DXVECTOR3& rPos);			// 生成
	static CTransPoint* Collision(const D3DXVECTOR3& rPos, const float fRadius);	// 遷移ポイントとの当たり判定
	static CListManager<CTransPoint>* GetList(void);	// リスト取得
	static HRESULT LoadSetup(const char* pPass);		// セットアップ

	// メンバ関数
	HRESULT LoadOpen(void);	// 解放フラグ読込
	HRESULT SaveOpen(void);	// 解放フラグ保存
	const std::string GetTransMapPass(void) { return m_sTransMapPass; }	// 遷移先マップパス取得

private:
	// 静的メンバ変数
	static CListManager<CTransPoint>* m_pList;	// オブジェクトリスト
	static CBalloonManager* m_pBalloonManager;	// 吹き出しマネージャー情報

	// メンバ関数
	HRESULT CreateStageTexture(void);	// ステージ情報テクスチャ作成

	// メンバ変数
	CListManager<CTransPoint>::AIterator m_iterator;	// イテレーター
	const std::string m_sTransMapPass;		// 遷移先マップパス
	CEffekseer::CEffectData* m_pEffectData;	// 保持するエフェクト情報
	CBalloon* m_pBalloon;	// 吹き出し情報
	bool m_bOpen;			// ステージ解放フラグ
};

#endif	// _TRANSPOINT_H_
