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
class CShadow;			// 影クラス
class COrbit;			// 軌跡クラス
class CCloneAngleUI;	// 分身出す方向UI
class CGauge2D;			// ゲージクラス
class CCheckPoint;		// チェックポイントクラス

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
		MODEL_MAX			// この列挙型の総数
	};

	// モーション列挙
	enum EMotion
	{
		MOTION_IDOL = 0,	// 待機モーション
		MOTION_CAUTIOUS,	// 警戒モーション
		MOTION_DASH,		// 歩行モーション
		MOTION_STEALTHWALK,	// 忍び足モーション
		MOTION_JUMP_MINI,	// 小ジャンプモーション
		MOTION_JUMP_HIGH,	// 大ジャンプモーション
		MOTION_FALL,		// 落下モーション
		MOTION_LANDING,		// 着地モーション
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
	void SetMove(const D3DXVECTOR3& rMove)			{ m_move = rMove; }		// 移動量設定
	D3DXVECTOR3 GetMove(void) const					{ return m_move; }		// 移動量取得
	void SetEnableJump(const bool bJump)			{ m_bJump = bJump; }	// ジャンプ状況設定
	bool IsJump(void) const							{ return m_bJump; }		// ジャンプ状況設定
	bool HitKnockBack(const int nDamage, const D3DXVECTOR3& rVecKnock);		// ノックバックヒット
	bool Hit(const int nDamage);		// ヒット
	void SetSpawn(void);				// 出現設定
	void SetState(const EState state);	// 状態設定
	EState GetState(void) const;		// 状態取得
	float GetRadius(void) const;		// 半径取得
	float GetHeight(void) const;		// 縦幅取得
	bool GimmickHighJump(void);			// ギミックのハイジャンプ
	void GimmickLowJump(void);			// ギミックの小ジャンプ
	bool GimmickLand(void);				// ギミックの飛び降り着地

	// メンバ関数 (金崎朋弥)
	int GetTension() const;		// 士気力の値を取得
	void RecoverCheckPoint();	// チェックポイントでの回復処理
	void RecoverJust();			// ジャストアクションでの回復処理
	void SetCheckPoint(CCheckPoint* checkpoint)	{ m_pCheckPoint = checkpoint; }	// チェックポイントを取得する処理
	D3DXVECTOR3 GetCenterPos() const	{ return m_posCenter; }	// プレイヤーの中心座標を取得

private:
	// メンバ関数
	EMotion UpdateSpawn(const float fDeltaTime);	// スポーン状態時の更新
	EMotion UpdateNormal(const float fDeltaTime);	// 通常状態時の更新
	void UpdateOldPosition(void);	// 過去位置の更新
	EMotion UpdateMove(void);		// 移動量・目標向きの更新
	void UpdateGravity(void);		// 重力の更新
	void UpdateSaveTeleport(void);	// 保存位置の更新

	bool UpdateLanding(D3DXVECTOR3& rPos);							// 着地状況の更新
	void UpdatePosition(D3DXVECTOR3& rPos, const float fDeltaTime);	// 位置の更新
	void UpdateRotation(D3DXVECTOR3& rRot);							// 向きの更新
	void UpdateMotion(int nMotion, const float fDeltaTime);			// モーション・キャラクターの更新
	bool UpdateFadeOut(const float fAdd);	// フェードアウト状態時の更新
	bool UpdateFadeIn(const float fSub);	// フェードイン状態時の更新

	// メンバ関数 (金崎追加)
	void ControlClone(D3DXVECTOR3& rPos, D3DXVECTOR3& rRot);	// 分身の処理
	void SaveReset();		// 直前のチェックポイントに帰る
	void CallClone();		// 分身を呼び戻す処理
	void CollisionActor();	// アクターの当たり判定

#ifdef _DEBUG

	void DebugJumpControl(void);	// ジャンプ操作
	void DebugMoveControl(void);	// キーボードの移動操作
	void DebugCloneControl(void);	// キーボードの分身操作

#endif // _DEBUG

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
	float		m_fScalar;			// 移動量

	// メンバ変数 (金崎追加)
	CGauge2D* m_pTensionGauge;		// 士気力ゲージのポインタ
	CCheckPoint* m_pCheckPoint;		// セーブしたチェックポイント
	D3DXVECTOR3 m_posCenter;		// 中心座標

};

#endif	// _PLAYER_H_
