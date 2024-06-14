//============================================================
//
//	プレイヤーの分身ヘッダー [player_clone.h]
//	Author：小原立暉
//	Adder ：藤田勇一
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
class CField;	// 地面クラス
class CGimmickAction;	// アクションギミッククラス
class CField;	// フィールドクラス

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
		MODEL_MAX			// この列挙型の総数
	};

	// モーション列挙
	enum EMotion
	{
		MOTION_IDOL = 0,	// 待機モーション
		MOTION_CAUTIOUS,	// 警戒モーション
		MOTION_DASH,		// 歩行モーション
		MOTION_STEALTHWALK,	// 忍び足モーション
		MOTION_FALL,		// 落下モーション
		MOTION_LANDING,		// 着地モーション
		MOTION_JUMP_IDOL,	// ジャンプ台モーション
		MOTION_CATAPULT,	// 打ち上げモーション
		MOTION_MAX			// この列挙型の総数
	};

	// 行動パターン
	enum EAction
	{
		ACTION_CHASE = 0,		// 通常の行動
		ACTION_MOVE,			// 歩行
		ACTION_MOVE_TO_WAIT,	// 待機位置への移動
		ACTION_WAIT,			// ギミック待機
		ACTION_FALL_TO_WAIT,	// 落とし穴警戒
		ACTION_FALL,			// 落とし穴落下
		ACTION_JUMPTABLE,		// ジャンプ台
		ACTION_JUMPOFF,			// 飛び降り
		ACTION_HEAVYDOOR,		// 重い扉
		ACTION_MAX
	};

	// ギミック対応状態
	enum EGimmick
	{
		GIMMICK_IGNORE = 0, // ギミックを無視する状態
		GIMMICK_REACTION, // ギミックに反応する状態
		GIMMICK_ACTION, // ギミックに反応した状態
		GIMMICK_MAX
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

	bool HitKnockBack(const int nDamage, const D3DXVECTOR3& rVecKnock);	// ノックバックヒット
	bool Hit(const int nDamage);				// ヒット
	void SetGimmick(CGimmickAction* gimmick);	// ギミックのポインタを受け取る
	void DeleteGimmick() { m_pGimmick = nullptr; } // 所持しているギミックを削除

	EAction GetAction() const			{ return m_Action; }	// 行動を取得
	CGimmickAction* GetGimmick() const	{ return m_pGimmick; }	// 所持ギミックを取得
	void SetAction(EAction action)		{ m_Action = action; }	// 行動を設定
	void AddFrags(const char cFrag);							// 文字列(フラグ)の追加
	void SabFrags(const char cFrag);							// 文字列(フラグ)の削除
	bool GetFrags(const char cFrag);							// 文字列(フラグ)の取得
	EGimmick IsGimmickFrag() const { return m_eGimmick; }			// ギミックフラグの取得
	void SetGimmickFrag(EGimmick gimmick) { m_eGimmick = gimmick; }	//ギミックフラグの設定

	// 静的メンバ関数
	static CPlayerClone* Create();													// 生成
	static CPlayerClone* Create(const D3DXVECTOR3& move);							// 生成(歩行型)
	static CPlayerClone* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);	// 生成(歩行型)
	static CPlayerClone* Create(CGimmickAction* gimmick);							// 生成(直接ギミック)
	static void Delete(const int nNum);												// 消去処理
	static void Delete(const EAction act = ACTION_CHASE);							// 選択消去処理 (金崎追加)
	static CListManager<CPlayerClone>* GetList(void);								// リスト取得
	static void CallBack();															// 分身を呼び戻す

private:
	// メンバ関数
	EMotion UpdateMove(const float fDeltaTime);			// 移動行動時の更新
	EMotion UpdateChase(const float fDeltaTime);		// 追従行動時の更新
	EMotion UpdateMoveToWait(const float fDeltaTime);	// 待機位置への移動時の更新
	EMotion UpdateWait(const float fDeltaTime);			// ギミック待機
	EMotion UpdateFallToWait(const float fDeltaTime);	// 落とし穴警戒
	EMotion UpdateFall(const float fDeltaTime);			// 落とし穴落下
	EMotion UpdateJumpTable(const float fDeltaTime);	// ジャンプ台行動時の更新

	void UpdateOldPosition(void);			// 過去位置の更新
	void UpdateGravity(void);				// 重力の更新
	void UpdateRotation(D3DXVECTOR3& rRot);	// 向きの更新
	void UpdateLanding(D3DXVECTOR3& rPos, EMotion* rCurMotion = nullptr);	// 着地状況の更新

	void UpdateMotion(int nMotion, const float fDeltaTime);		// モーション・オブジェクトキャラクターの更新
	bool UpdateFadeOut(const float fAdd);	// フェードアウト状態時の更新
	bool UpdateFadeIn(const float fSub);	// フェードイン状態時の更新

	void UpdateIgnore(); // 無視する状態での更新
	void UpdateReAction(); // 反応する状態での更新
	void UpdateAction(); // 反応した状態での更新

	// メンバ関数 (金崎追加)
	CPlayerClone::EMotion ChasePrev(D3DXVECTOR3* pPosThis, D3DXVECTOR3* pRotThis);	// 前についていく処理
	CPlayerClone::EMotion Chase	// ついていく処理
	( // 引数
		D3DXVECTOR3* pPosThis,			// 自身の位置
		D3DXVECTOR3* pRotThis,			// 自身の向き
		const D3DXVECTOR3& rPosPrev,	// ついていくやつの位置
		const D3DXVECTOR3& rRotPrev		// ついていくやつの向き
	);
	void ViewTarget(const D3DXVECTOR3& rPosThis, const D3DXVECTOR3& rPosPrev); // 目標の方向を向く処理
	CPlayerClone* Block();				// 分身出させないよの処理
	D3DXVECTOR3 CalcStartPos() const;	// 初期位置を算出
	D3DXVECTOR3 CalcPrevBack(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot) const;	// 一つ前の対象の後ろを算出
	bool CollisionActor();				// アクターとの当たり判定
	void CheckGimmick();			// ギミックとの当たり判定

	// 静的メンバ変数
	static CListManager<CPlayerClone>* m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CPlayerClone>::AIterator m_iterator;	// イテレーター
	CShadow* m_pShadow;			// 影の情報
	COrbit* m_pOrbit;			// 軌跡の情報
	D3DXVECTOR3 m_move;			// 移動量
	EAction m_Action;			// 行動
	float m_fDeleteTimer;		// 自動消滅タイマー
	float m_fGimmickTimer;		// ギミック受付時間タイマー
	CGimmickAction* m_pGimmick;	// ギミックのポインタ
	std::string m_sFrags;		// ギミックフラグの文字列
	int m_nIdxGimmick;			// ギミック内の管理番号
	float m_fFallStart;			// 落とし穴の落ちる前の高さ
	EGimmick m_eGimmick;		// ギミックフラグ
	bool m_bFind;				// 発見フラグ
	D3DXVECTOR3 m_size;			// サイズ
	CField* m_pField;			// フィールドフラグ

	// メンバ変数 (藤田追加)
	CField *m_pCurField;	// 現在の地面
	CField *m_pOldField;	// 過去の地面
	D3DXVECTOR3	m_oldPos;	// 過去位置
	D3DXVECTOR3	m_destRot;	// 目標向き
	bool m_bJump;			// ジャンプ状況
};

#endif	// _PLAYER_CLONE_H_
