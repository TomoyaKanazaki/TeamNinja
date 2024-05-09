//============================================================
//
//	プレイヤーヘッダー [player.h]
//	Author：藤田勇一
//  Adder : 金崎朋弥
// 
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectChara.h"
#include "scene.h"
#include "retentionManager.h"

//************************************************************
//	前方宣言
//************************************************************
class CShadow;	// 影クラス
class COrbit;	// 軌跡クラス
class CGauge2D; // ゲージクラス

//************************************************************
//	クラス定義
//************************************************************
// プレイヤークラス
class CPlayer : public CObjectChara
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
		MOTION_MOVE,		// 移動モーション
		MOTION_MAX			// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_SPAWN,	// スポーン状態
		STATE_NORMAL,	// 通常状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CPlayer();

	// デストラクタ
	~CPlayer() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader *pShader = nullptr) override;		// 描画
	void SetEnableUpdate(const bool bUpdate) override;	// 更新状況設定
	void SetEnableDraw(const bool bDraw) override;		// 描画状況設定

	// 静的メンバ関数
	static CPlayer *Create(CScene::EMode mode);		// 生成
	static CListManager<CPlayer> *GetList(void);	// リスト取得

	// メンバ関数
	void SetDestRotation(const D3DXVECTOR3& rRot)	{ m_destRot = rRot; }	// 目標向き設定
	D3DXVECTOR3 GetDestRotation(void) const			{ return m_destRot; }	// 目標向き取得
	bool HitKnockBack(const int nDamage, const D3DXVECTOR3& rVecKnock);		// ノックバックヒット
	bool Hit(const int nDamage);		// ヒット
	void SetSpawn(void);				// 出現設定
	void SetState(const EState state);	// 状態設定
	EState GetState(void) const;		// 状態取得
	float GetRadius(void) const;		// 半径取得
	float GetHeight(void) const;		// 縦幅取得

	// メンバ関数 (金崎朋弥)
	int GetTension() const; // 士気力の値を取得

private:
	// メンバ関数
	EMotion UpdateSpawn(void);		// スポーン状態時の更新
	EMotion UpdateNormal(void);		// 通常状態時の更新
	void UpdateOldPosition(void);	// 過去位置の更新
	EMotion UpdateMove(void);		// 移動量・目標向きの更新
	void UpdateGravity(void);		// 重力の更新

	bool UpdateLanding(D3DXVECTOR3& rPos);				// 着地状況の更新
	void UpdatePosition(D3DXVECTOR3& rPos);				// 位置の更新
	void UpdateRotation(D3DXVECTOR3& rRot);				// 向きの更新
	void UpdateMotion(int nMotion, const float fDeltaTime);	// モーション・オブジェクトキャラクターの更新
	bool UpdateFadeOut(const float fAdd);				// フェードアウト状態時の更新
	bool UpdateFadeIn(const float fSub);				// フェードイン状態時の更新

	// メンバ関数 (金崎追加)
	void Move(); // 操作処理
	void Inertial(); // 運動の第一法則
	void LoadParameter(); // 定数値の読み込み
	void ControlClone(); // 分身の処理

	// 静的メンバ変数
	static CListManager<CPlayer> *m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CPlayer>::AIterator m_iterator;	// イテレーター
	CShadow		*m_pShadow;			// 影の情報
	COrbit		*m_pOrbit;			// 軌跡の情報
	D3DXVECTOR3	m_oldPos;			// 過去位置
	D3DXVECTOR3	m_move;				// 移動量
	D3DXVECTOR3	m_destRot;			// 目標向き
	EState		m_state;			// 状態
	int			m_nCounterState;	// 状態管理カウンター
	bool		m_bJump;			// ジャンプ状況

	// メンバ変数 (金崎追加)
	CGauge2D* m_pTensionGauge; // 士気力ゲージのポインタ
	unsigned int m_nMaxTension; // 士気力の最大値
	unsigned int m_nInitTension; // 士気力の初期値
	unsigned int m_nSpeedTension; // 士気力ゲージの増減速度
	bool m_bCreateClone; // 分身生成モードフラグ
	unsigned int m_nNumClone; // 生成する分身の数
	unsigned int m_nMaxClone; // 一度に分身できる上限

};

#endif	// _PLAYER_H_
