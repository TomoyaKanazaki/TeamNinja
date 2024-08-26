//============================================================
//
//	吹き出しマネージャーヘッダー [balloonManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _BALLOON_MANAGER_H_
#define _BALLOON_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "godItem.h"

//************************************************************
//	前方宣言
//************************************************************
class CTransPoint;		// 遷移ポイントクラス
class CObject2D;		// オブジェクト2Dクラス
class CRoll2D;			// 回転オブジェクト2Dクラス
class CScale2D;			// 拡縮オブジェクト2Dクラス
class CAnim2D;			// アニメーション2Dクラス
class CScrollText2D;	// 文字送りテキスト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// 吹き出しマネージャークラス
class CBalloonManager : public CObject
{
public:
	// 定数
	static constexpr int NUM_STAR = 2;	// 手裏剣の数

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 無し
		STATE_STAG,		// 演出
		STATE_END,		// 終了
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CBalloonManager(CTransPoint* pParent, const bool bOpen);

	// デストラクタ
	~CBalloonManager();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CBalloonManager *Create(CTransPoint* pParent, const bool bOpen);	// 生成

	// メンバ関数
	void SetStag(void);	// 演出開始設定
	bool IsNone(void)	{ return (m_state == STATE_NONE); }	// 演出状況取得

private:
	// オーバーライド関数
	void Release(void) override { CObject::Release(); }	// 破棄

	// メンバ変数
	CAnim2D* m_apGodItem[CGodItem::TYPE_MAX];	// 神器アイコン
	CTransPoint* m_pParent;			// 遷移ポイント
	CObject2D* m_pStage;			// ステージ画面
	CObject2D* m_pFrame;			// フレーム
	CRoll2D* m_apStar[NUM_STAR];	// 手裏剣
	CScale2D* m_pCont;				// 操作方法
	CScrollText2D* m_pShadow;		// ステージ名の影
	CScrollText2D* m_pName;			// ステージ名
	EState m_state;					// 状態
	const bool m_bOpen;				// ステージ解放フラグ
};

#endif	// _BALLOON_MANAGER_H_
