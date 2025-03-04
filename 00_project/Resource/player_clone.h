//============================================================
//
//	プレイヤーの分身ヘッダー [player_clone.h]
//	Author：小原立暉
//	Adder ：藤田勇一
//	Adder ：金崎朋弥
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
		MOTION_DASH,		// 歩行モーション
		MOTION_STEALTHWALK,	// 忍び足モーション
		MOTION_FALL,		// 落下モーション
		MOTION_LANDING,		// 着地モーション
		MOTION_JUMP_IDOL,	// ジャンプ台待機モーション
		MOTION_JUMP_WALK,	// ジャンプ台移動モーション
		MOTION_CATAPULT,	// ジャンプ台打ち上げモーション
		MOTION_LADDER,		// 梯子/橋先頭モーション
		MOTION_BRIDGE,		// 橋モーション
		MOTION_OPEN,		// 扉上げモーション
		MOTION_MAX			// この列挙型の総数
	};

	// 行動パターン
	enum EAction
	{
		ACTION_MOVE = 0,		// 歩行
		ACTION_FALL_TO_WAIT,	// 落とし穴警戒
		ACTION_FALL,			// 落とし穴落下
		ACTION_JUMPTABLE,		// ジャンプ台
		ACTION_HEAVYDOOR,		// 重い扉
		ACTION_STEP,			// 梯子
		ACTION_BRIDGE,			// 橋
		ACTION_BUTTON,			// ボタン
		ACTION_VANISH,			// 消滅
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

	bool HitKnockBack(const int nDamage, const D3DXVECTOR3& rVecKnock);	// ノックバックヒット
	bool Hit(const int nDamage);				// ヒット
	void SetGimmick(CGimmickAction* gimmick);	// ギミックのポインタを受け取る
	void DeleteGimmick();						// 所持しているギミックを削除
	void SetField(CField* field);				// フィールドのポインタを受け取る
	void DeleteField(CField* field);			// 所持しているフィールドを削除

	EAction GetAction() const			{ return m_Action; }	// 行動を取得
	CGimmickAction* GetGimmick() const { return m_pGimmick; }	// 所持ギミックを取得
	CField* GetField() const { return m_pField; }				// 所持フィールドを取得
	void SetAction(EAction action)		{ m_Action = action; }	// 行動を設定
	void AddFrags(const char cFrag);							// 文字列(フラグ)の追加
	void SabFrags(const char cFrag);							// 文字列(フラグ)の削除
	bool GetFrags(const char cFrag);							// 文字列(フラグ)の取得

	// 静的メンバ関数
	static CPlayerClone* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);	// 生成(歩行型)
	static CPlayerClone* Create(CGimmickAction* gimmick);							// 生成(直接ギミック)
	static void Delete();															// 全消去処理 (金崎追加)
	static void Delete(const int nNum);												// 選択消去処理 (金崎追加)
	static void Delete(const EAction act);											// 選択消去処理 (金崎追加)
	static void Delete(CPlayerClone* pClone);										// 選択消去処理 (金崎追加)
	static CListManager<CPlayerClone>* GetList(void);								// リスト取得
	static float GetRadius();														// 半径の取得
	static float GetHeight();														// 身長の取得

	// 静的メンバ関数 (藤田追加)
	static void VanishAll(void);	// 全消滅

	// メンバ関数 (藤田追加)
	void Vanish(void);	// 消滅

private:
	// メンバ関数
	EMotion UpdateMove(const float fDeltaTime);			// 移動行動時の更新
	EMotion UpdateFallToWait(const float fDeltaTime);	// 落とし穴警戒
	EMotion UpdateFall(const float fDeltaTime);			// 落とし穴落下
	EMotion UpdateJumpTable(const float fDeltaTime);	// ジャンプ台行動時の更新
	EMotion UpdateHeavyDoor(const float fDeltaTime);	// 重い扉行動時の更新
	EMotion UpdateStep(const float fDeltaTime);			// 梯子行動時の更新
	EMotion UpdateBridge(const float fDeltaTime);		// 橋行動時の更新
	EMotion UpdateButton(const float fDeltaTime);		// ボタン行動時の更新

	void UpdateOldPosition(void);			// 過去位置の更新
	void UpdateGravity(void);				// 重力の更新
	void UpdateRotation(D3DXVECTOR3& rRot);	// 向きの更新
	void UpdateLanding(D3DXVECTOR3& rPos, EMotion* rCurMotion = nullptr);	// 着地状況の更新

	void UpdateMotion(int nMotion, const float fDeltaTime);		// モーション・オブジェクトキャラクターの更新
	bool UpdateFadeOut(const float fAdd);	// フェードアウト状態時の更新
	bool UpdateFadeIn(const float fSub);	// フェードイン状態時の更新

	bool UpdateActive(const float fDeltaTime); // アクティブ状態での処理

	// メンバ関数 (金崎追加)
	void CheckGimmick();	// ギミックの接触判定
	void Approach();		// 目標位置に向かう処理
	CPlayerClone* Block();	// 分身出させないよの処理
	bool CollisionActor(D3DXVECTOR3& pos);	// アクターとの当たり判定
	bool CollisionWall();	// 壁との当たり判定

	// メンバ関数 (藤田追加)
	CPlayerClone* GetGimmickNextClone();	// ギミックの次の分身取得

	// 静的メンバ変数
	static CListManager<CPlayerClone>* m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CPlayerClone>::AIterator m_iterator;	// イテレーター

	COrbit* m_pOrbit;			// 軌跡の情報
	D3DXVECTOR3 m_move;			// 移動量
	EAction m_Action;			// 現在行動
	EAction m_OldAction;		// 過去行動
	float m_fDeleteTimer;		// 自動消滅タイマー
	float m_fGimmickTimer;		// ギミック受付時間タイマー
	CGimmickAction* m_pGimmick;	// ギミックのポインタ
	std::string m_sFrags;		// ギミックフラグの文字列
	int m_nIdxGimmick;			// ギミック内の管理番号
	float m_fFallStart;			// 落とし穴の落ちる前の高さ
	bool m_bFind;				// 発見フラグ
	D3DXVECTOR3 m_size;			// サイズ
	CField* m_pField;			// フィールドフラグ

	// メンバ変数 (藤田追加)
	CField *m_pCurField;	// 現在の地面
	CField *m_pOldField;	// 過去の地面
	D3DXVECTOR3	m_oldPos;	// 過去位置
	D3DXVECTOR3	m_destRot;	// 目標向き
	bool m_bJump;			// ジャンプ状況

	// メンバ変数（小原追加）
	bool m_bActorDelete;	// アクターの消去
};

#endif	// _PLAYER_CLONE_H_
