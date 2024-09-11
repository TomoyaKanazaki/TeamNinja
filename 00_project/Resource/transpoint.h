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
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_CAMERA,	// カメラ状態
		STATE_FADE,		// フェード状態
		STATE_NORMAL,	// 通常状態
		STATE_MAX		// この列挙型の総数
	};

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
	static CTransPoint *Collision(const D3DXVECTOR3& rPos, const float fRadius);	// 遷移ポイントとの当たり判定
	static CTransPoint* GetOpenTransPoint(void) { return m_pOpenTransPoint; }		// 解放された遷移ポイント取得

	static CListManager<CTransPoint>* GetList(void);	// リスト取得
	static HRESULT LoadSetup(const char* pPass);		// セットアップ
	static HRESULT LoadOpen(const char* pPass, bool* pOpen);		// 解放フラグ読込
	static HRESULT SaveOpen(const char* pPass, const bool bOpen);	// 解放フラグ保存
	static HRESULT LoadScreenShot(const char* pPass, std::string* pStrPath);	// スクショパス読込
	static HRESULT SaveScreenShot(const char* pPass);							// スクショパス保存

	// メンバ関数
	const std::string GetTransMapPass(void) { return m_sTransMapPass; }	// 遷移先マップパス取得

private:
	// 静的メンバ変数
	static CListManager<CTransPoint>* m_pList;	// オブジェクトリスト
	static CBalloonManager* m_pBalloonManager;	// 吹き出しマネージャー情報
	static CTransPoint* m_pOpenTransPoint;		// 解放された遷移ポイント

	// メンバ関数
	HRESULT CreateStageTexture(void);	// ステージ情報テクスチャ作成

	// メンバ変数
	CListManager<CTransPoint>::AIterator m_iterator;	// イテレーター
	const std::string m_sTransMapPass;		// 遷移先マップパス
	std::string m_sScreenShotPass;			// スクショパス
	CEffekseer::CEffectData* m_pEffectData;	// 保持するエフェクト情報
	CBalloon* m_pBalloon;	// 吹き出し情報
	EState m_state;			// 状態
	bool m_bOpen;			// ステージ解放フラグ
	float m_fCurTime;		// 現在の時間
};

#endif	// _TRANSPOINT_H_
