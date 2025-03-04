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
#include "effekseerControl.h"
#include "effekseerManager.h"

//************************************************************
//	前方宣言
//************************************************************
class CShadow;			// 影クラス
class COrbit;			// 軌跡クラス
class CCheckPoint;		// チェックポイントクラス
class CPlayerBackUI;	// プレイヤー回帰UI
class CPlayerDodge;		// プレイヤー回避エフェクト
class CField;			// フィールドクラス
class CObject2D;		// オブジェクト2D

//************************************************************
//	クラス定義
//************************************************************
// プレイヤークラス
class CPlayer : public CObjectChara
{
public:

	// 静的メンバ変数宣言
	static constexpr int MAX_BODYORBIT = 2;		// 体の軌跡の総数
	static constexpr int MAX_JUMPORBIT = 4;		// ジャンプ軌跡の総数

	// 種類列挙
	enum EType
	{
		TYPE_TITLE = 0,	// タイトル画面
		TYPE_SELECT,	// セレクト画面
		TYPE_GAME,		// ゲーム画面
		TYPE_MAX		// この列挙型の総数
	};

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
		MOTION_DODGE,		// 回避モーション
		MOTION_DEATH,		// 死亡モーション
		MOTION_DAMAGE,		// ダメージモーション
		MOTION_SAVE,		// チェックポイントモーション
		MOTION_GET,			// 神器取得モーション
		MOTION_START,		// スタートモーション
		MOTION_STAND,		// 仁王立ちモーション
		MOTION_GOAL,		// ゴールモーション
		MOTION_SELECT_IN,	// セレクト開始モーション
		MOTION_SELECT_OUT,	// セレクト終了モーション
		MOTION_DROWNING,	// 水没モーション
		MOTION_SNEAK,		// 忍び足待機モーション
		MOTION_MAX			// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態

		// タイトル画面状態
		STATE_TITLE_IDOL,	// 仁王立ち状態
		STATE_TITLE_MOVE,	// 移動状態

		// セレクト画面状態
		STATE_SELECT_SPAWN,		// スポーン状態
		STATE_SELECT_NORMAL,	// 移動状態
		STATE_SELECT_WAIT,		// 待機状態
		STATE_SELECT_ENTER,		// 入場状態
		STATE_SELECT_OPEN,		// 解放状態

		// ゲーム画面状態
		STATE_SPAWN,	// スポーン状態
		STATE_START,	// スタート状態
		STATE_NORMAL,	// 通常状態
		STATE_GODITEM,	// 神器獲得状態
		STATE_GOAL,		// ゴール状態
		STATE_DODGE,	// 回避状態
		STATE_DEATH,	// 死亡状態
		STATE_DAMAGE,	// ダメージ状態
		STATE_DROWN,	// 溺死状態
		STATE_BACKWAIT,	// チェックポイントリセット待機状態

		STATE_MAX	// この列挙型の総数
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

	// 仮想関数
	virtual EMotion UpdateState(const float fDeltaTime);	// 状態更新
	virtual void SetSpawn(void);					// 出現設定
	virtual void SetStart(void) { assert(false); }	// 演出開始設定
	virtual void SetEnter(void) { assert(false); }	// 入場設定

	// 静的メンバ関数
	static CPlayer *Create	// 生成
	( // 引数
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);
	static CListManager<CPlayer> *GetList(void);	// リスト取得
	static HRESULT LoadSetup(const char* pPass);	// セットアップ

	// メンバ関数
	void SetDestRotation(const D3DXVECTOR3& rRot)	{ m_destRot = rRot; }	// 目標向き設定
	D3DXVECTOR3 GetDestRotation(void) const			{ return m_destRot; }	// 目標向き取得
	void SetMove(const D3DXVECTOR3& rMove)			{ m_move = rMove; }		// 移動量設定
	D3DXVECTOR3 GetMove(void) const					{ return m_move; }		// 移動量取得
	void SetEnableJump(const bool bJump)			{ m_bJump = bJump; }	// ジャンプ状況設定
	bool IsJump(void) const							{ return m_bJump; }		// ジャンプ状況設定
	bool HitKnockBack(const int nDamage, const D3DXVECTOR3& rVecKnock);		// ノックバックヒット
	bool Hit(const int nDamage);				// ヒット
	void TimeUp(void);							// タイムアップ
	void SetEnableGodItem(const bool bGet);		// 神器獲得設定
	void SetResult();							// リザルト設定
	void SetState(const EState state);			// 状態設定
	EState GetState(void) const;				// 状態取得
	float GetRadius(void) const;				// 半径取得
	float GetHeight(void) const;				// 縦幅取得
	bool GimmickHighJump(const int nNumClone);	// ギミックのハイジャンプ
	void GimmickLowJump(void);					// ギミックの小ジャンプ
	bool GimmickLand(void);						// ギミックの飛び降り着地
	D3DXVECTOR3 GetVec3Sizing() const override
	{ return D3DXVECTOR3(GetRadius(), GetHeight(), GetRadius()); } // サイズの取得

	// メンバ関数 (金崎朋弥)
	void RecoverCheckPoint();	// チェックポイントでの回復処理
	void RecoverItem();			// アイテムでの回復処理
	D3DXVECTOR3 GetCenterPos() const	{ return m_posCenter; }	// プレイヤーの中心座標を取得
	void SetClone(bool bClone) { m_bClone = bClone; }			// 分身操作可能フラグの設定
	void AddFrags(const char cFrag);							// 文字列(フラグ)の追加
	void SabFrags(const char cFrag);							// 文字列(フラグ)の削除
	D3DXVECTOR3 GetOldPosition() const { return m_oldPos; }		// 過去位置の取得
	CField* GetField() const { return m_pCurField; }			// フィールドの取得

protected:
	// メンバ関数
	EMotion UpdateNone(const float fDeltaTime);	// 何もしない状態時の更新
	EMotion UpdateMove(void);					// 移動量・目標向きの更新
	void UpdateGravity(const float fDeltaTime);	// 重力の更新
	bool UpdateLanding(D3DXVECTOR3& rPos, const float fDeltaTime);	// 着地状況の更新
	void UpdatePosition(D3DXVECTOR3& rPos, const float fDeltaTime);	// 位置の更新
	void UpdateRotation(D3DXVECTOR3& rRot, const float fDeltaTime);	// 向きの更新
	bool UpdateFadeOut(const float fAdd);	// フェードアウト状態時の更新
	bool UpdateFadeIn(const float fSub);	// フェードイン状態時の更新
	bool CollisionWall(D3DXVECTOR3& rPos);	// 壁との当たり判定
	void SetJump(const bool bJump) { m_bJump = bJump; }	// ジャンプ状況設定

	// メンバ関数 (金崎追加)
	bool ControlClone(D3DXVECTOR3& rPos, D3DXVECTOR3& rRot, const float fDeltaTime);	// 分身の処理

	// メンバ関数 (小原追加)
	void CollisionActor(D3DXVECTOR3& pos, bool& rLand);		// アクターの当たり判定
	void CollisionEnemy(D3DXVECTOR3& pos);					// 敵の当たり判定

private:
	// メンバ関数
	EMotion UpdateSpawn(const float fDeltaTime);	// スポーン状態時の更新
	EMotion UpdateStart(const float fDeltaTime);	// スタート状態時の更新
	EMotion UpdateNormal(const float fDeltaTime);	// 通常状態時の更新
	EMotion UpdateGodItem(const float fDeltaTime);	// 神器獲得状態時の更新
	EMotion UpdateGoal(const float fDeltaTime);		// ゴール状態時の更新
	EMotion UpdateDodge(const float fDeltaTime);	// 回避状態時の更新
	EMotion UpdateDeath(const float fDeltaTime);	// 死亡状態時の更新
	EMotion UpdateDamage(const float fDeltaTime);	// ダメージ状態時の更新
	EMotion UpdateDrown(const float fDeltaTime);	// 溺死状態時の更新
	EMotion UpdateBackWait(const float fDeltaTime);	// チェックポイントリセット待機時の更新
	void UpdateOldPosition(void);					// 過去位置の更新
	void UpdateMotion(int nMotion, const float fDeltaTime);	// モーション・キャラクターの更新
	void ResetStack();								// スタック状態のリセット
	void CheckPointBack(const float fDeltaTime);	// チェックポイント回帰処理

	// メンバ関数 (金崎追加)
	void DeleteClone();		// 分身を呼び戻す処理
	bool CreateGimmick(const float fDeltaTime);	// 直接ギミックを生成する処理
	bool Dodge(D3DXVECTOR3& rPos, CInputPad* pPad);	// 回避処理
	void FloorEdgeJump(); // 床際のジャンプ処理

	// メンバ関数 (小原追加)
	void CollisionCoin(const D3DXVECTOR3& rPos);			// コインとの当たり判定
	void CollisionGodItem(const D3DXVECTOR3& rPos);			// 神器との当たり判定
	void CollisionTouchActor(const D3DXVECTOR3& rPos);		// タッチアクターとの当たり判定

#ifdef _DEBUG

	void DebugJumpControl(void);	// ジャンプ操作
	void DebugMoveControl(void);	// キーボードの移動操作
	void DebugCloneControl(void);	// キーボードの分身操作

#endif // _DEBUG

	// 静的メンバ変数
	static CListManager<CPlayer> *m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CPlayer>::AIterator m_iterator;	// イテレーター

	COrbit			*m_apBodyOrbit[MAX_BODYORBIT];	// 体の軌跡の情報
	COrbit			*m_apJumpOrbit[MAX_JUMPORBIT];	// ジャンプ時の軌跡の情報
	CPlayerBackUI	*m_pBackUI;				// 回帰UIの情報
	CPlayerDodge	*m_pDodge;				// 回避エフェクトの情報
	D3DXVECTOR3		m_oldPos;				// 過去位置
	D3DXVECTOR3		m_move;					// 移動量
	D3DXVECTOR3		m_destRot;				// 目標向き
	D3DXVECTOR3		m_posInit;				// 初期位置
	EState			m_state;				// 状態
	int				m_nCounterState;		// 状態管理カウンター
	int				m_nWalkCount;			// 歩行音カウント
	bool			m_bJump;				// ジャンプ状況
	float			m_fScalar;				// 移動量
	bool			m_bGimmickClone;		// ギミッククローンの生成フラグ
	float			m_fGimmickTimer;		// ギミッククローンの生成タイマー
	float			m_fTempStick;			// スティックの入力角を保存する変数

	// メンバ変数 (金崎追加)
	D3DXVECTOR3 m_posCenter;				// 中心座標
	bool m_bClone;							// 分身操作可能フラグ
	bool m_bGetCamera;						// カメラの取得
	float m_fCameraRot;						// カメラの角度
	float m_fStickRot;						// スティックの角度
	std::string m_sFrags;					// ギミックフラグの文字列
	CField* m_pCurField;					// 現在の地面
	CField* m_pOldField;					// 過去の地面
	CEffekseer::CEffectData* m_pEffectFirefly;	// エフェクト情報

	// メンバ変数（小原追加）
	D3DXVECTOR3 m_posTeleport;	// テレポート位置
	int m_nCounterTeleport;		// テレポートカウント
};

#endif	// _PLAYER_H_
