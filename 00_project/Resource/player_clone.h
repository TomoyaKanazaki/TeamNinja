//============================================================
//
//	プレイヤーの分身ヘッダー [player_clone.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_CLONE_H_
#define _PLAYER_CLONE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectChara.h"

//************************************************************
//	前方宣言
//************************************************************
class CShadow;	// 影クラス
class COrbit;	// 軌跡クラス
class CGimmick;	//　ギミッククラス

//************************************************************
//	クラス定義
//************************************************************
// プレイヤーの分身クラス
class CPlayerClone : public CObjectChara
{
public:
	// モデル列挙
	enum EModel
	{
		MODEL_WAIST = 0,	// 腰
		MODEL_BODY,			// 体
		MODEL_HEAD,			// 頭
		MODEL_ARMUL,		// 左上腕
		MODEL_ARMUR,		// 右上腕
		MODEL_ARMDL,		// 左下腕
		MODEL_ARMDR,		// 右下腕
		MODEL_HANDL,		// 左手
		MODEL_HANDR,		// 右手
		MODEL_LEGUL,		// 左太もも
		MODEL_LEGUR,		// 右太もも
		MODEL_LEGDL,		// 左脛
		MODEL_LEGDR,		// 右脛
		MODEL_FOOTL,		// 左足
		MODEL_FOOTR,		// 右足
		MODEL_SWORDL,		// 左剣
		MODEL_SWORDR,		// 右剣
		MODEL_MAX			// この列挙型の総数
	};

	// モーション列挙
	enum EMotion
	{
		MOTION_IDOL = 0,	// 待機モーション
		MOTION_MAX			// この列挙型の総数
	};

	// 行動パターン
	enum EAction
	{
		ACTION_NONE, // 通常の行動
		ACTION_MOVE, // 歩行
		ACTION_MAX
	};

	// コンストラクタ
	CPlayerClone();

	// デストラクタ
	~CPlayerClone() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetEnableUpdate(const bool bUpdate) override;	// 更新状況設定
	void SetEnableDraw(const bool bDraw) override;		// 描画状況設定

	bool HitKnockBack(const int nDamage, const D3DXVECTOR3& rVecKnock);		// ノックバックヒット
	bool Hit(const int nDamage);		// ヒット
	EAction GetAction() const { return m_Action; } // 行動を取得
	void SetGimmick(CGimmick* gimmick) { m_pGimmick = gimmick; } // ギミックのポインタを受け取る

	// 静的メンバ関数
	static CPlayerClone* Create();							// 生成
	static CPlayerClone* Create(const D3DXVECTOR3& move);	// 生成(歩行型)
	static void Delete(const int nNum);						// 消去処理
	static void Delete(const EAction act = ACTION_NONE);	// 全消去処理 (金崎追加)
	static CListManager<CPlayerClone>* GetList(void);		// リスト取得

private:
	// メンバ関数
	EMotion UpdateNormal(void);		// 通常状態時の更新
	void UpdateMotion(int nMotion, const float fDeltaTime);	// モーション・オブジェクトキャラクターの更新
	bool UpdateFadeOut(const float fAdd);				// フェードアウト状態時の更新
	bool UpdateFadeIn(const float fSub);				// フェードイン状態時の更新

	// メンバ関数 (金崎追加)
	void ChasePrev(); // 前についていく処理
	void Chase(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot); // ついていく処理
	void ViewTarget(const D3DXVECTOR3& rPos); // 目標の方向を向く処理

	// 静的メンバ変数
	static CListManager<CPlayerClone>* m_pList;		// オブジェクトリスト

	// メンバ変数
	CListManager<CPlayerClone>::AIterator m_iterator;	// イテレーター
	CShadow* m_pShadow;			// 影の情報
	COrbit* m_pOrbit;			// 軌跡の情報
	D3DXVECTOR3 m_move; // 移動量
	EAction m_Action; // 行動
	float m_fDeleteTimer; // 自動消滅タイマー
	float m_fChargeTimer; // ため時間タイマー
	CGimmick* m_pGimmick; // ギミックのポインタ

};

#endif	// _PLAYER_H_
