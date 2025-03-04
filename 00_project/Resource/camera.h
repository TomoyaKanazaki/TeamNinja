//============================================================
//
//	カメラヘッダー [camera.h]
//	Author：藤田勇一
//	Adder : 金崎朋弥
//	Adder : 小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
// 前方宣言
//************************************************************
class CPlayer;	// プレイヤー

//************************************************************
//	クラス定義
//************************************************************
// カメラクラス
class CCamera
{
public:
	// カメラ列挙
	enum EType
	{
		TYPE_MAIN = 0,	// メインカメラ
		TYPE_MODELUI,	// モデルUI表示カメラ
		TYPE_MAX		// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,		// なにもしない状態
		STATE_CONTROL,		// 操作状態
		STATE_ROTATE,		// 回転状態
		STATE_FOLLOW,		// 追従状態
		STATE_AROUND,		// 回り込み
		STATE_TELEPHOTO,	// 望遠
		STATE_GODITEM,		// 神器獲得
		STATE_RESULT,		// リザルト
		STATE_SELECT,		// 選択
		STATE_OPEN,			// 解放
		STATE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CCamera();

	// デストラクタ
	~CCamera();

	// カメラ揺れ構造体
	struct SSwing
	{
	public:
		// コンストラクタ
		SSwing() {}
		SSwing(const float ShiftLength, const float SubAngle, const float SubLength)
		{
			shiftPos		= VEC3_ZERO;	// 位置ずれ量
			fShiftAngle		= 0.0f;			// 位置をずらす角度
			fShiftLength	= ShiftLength;	// 位置をずらす距離
			fSubAngle		= SubAngle;		// ずらす角度の減算量
			fSubLength		= SubLength;	// ずらす距離の減算量
		}

		// デストラクタ
		~SSwing() {}

		// メンバ変数
		D3DXVECTOR3	shiftPos;		// 位置ずれ量
		float		fShiftAngle;	// 位置をずらす角度
		float		fShiftLength;	// 位置をずらす距離
		float		fSubAngle;		// ずらす角度の減算量
		float		fSubLength;		// ずらす距離の減算量
	};

	// カメラ構造体
	struct SCamera
	{
		D3DXVECTOR3		posV;			// 現在の視点
		D3DXVECTOR3		posR;			// 現在の注視点
		D3DXVECTOR3		destPosV;		// 目標の視点
		D3DXVECTOR3		destPosR;		// 目標の注視点
		D3DXVECTOR3		posOldV;		// 現在の視点
		D3DXVECTOR3		posOldR;		// 現在の注視点
		D3DXVECTOR3		vecU;			// 上方向ベクトル
		D3DXVECTOR3		rot;			// 現在の向き
		D3DXVECTOR3		destRot;		// 目標の向き
		float			fDis;			// 現在の視点と注視点の距離
		float			fDestDis;		// 目標の視点と注視点の距離
		SSwing			swingInfo;		// カメラ揺れ情報
		D3DVIEWPORT9	viewportInfo;	// ビューポート情報
		D3DXMATRIX		mtxProjection;	// プロジェクションマトリックス
		D3DXMATRIX		mtxView;		// ビューマトリックス
	};

	// スタートカメラ構造体
	struct SStart
	{
		// 状態
		enum EState
		{
			STATE_LAND = 0,	// 着地待ち状態
			STATE_ROUND,	// 回り込み状態
			STATE_BACK,		// 戻り状態
			STATE_MAX		// この列挙型の総数
		};

		int nCount;			// カウント
		EState state;		// 状態
	};

	// 解放カメラ構造体
	struct SOpen
	{
		// 状態列挙
		enum EState
		{
			STATE_ROTA_UP = 0,	// 上回転状態
			STATE_MOVE,			// 移動状態
			STATE_ROTA_DOWN,	// 下回転状態
			STATE_MAX			// この列挙型の総数
		};

		float fCurTime;	// カウント
		EState state;	// 状態
	};

	// 回転カメラ構造体
	struct SRota
	{
		// 状態列挙
		enum EState
		{
			STATE_ROTA = 0,	// 回転状態
			STATE_WAIT,		// 待機状態
			STATE_RUN,		// 移動状態
			STATE_MAX		// この列挙型の総数
		};

		float fCurTime;	// カウント
		EState state;	// 状態
	};

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(const float fDeltaTime);	// 更新
	void Reset(void);						// 再設定
	void SetCamera(const EType type);		// カメラ設定
	SCamera GetCamera(const EType type);	// カメラ取得
	void SetState(const EState state);		// カメラ状態設定
	EState GetState(void) const;			// カメラ状態取得
	void SetSwing(const EType type, const SSwing swing);	// カメラ揺れ設定
	void SetEnableUpdate(const bool bUpdate);				// 更新状況設定
	bool IsUpdate(void) const	{ return m_bUpdate; }		// 更新状況取得
	void SetRotateRun(void)		{ m_rotaInfo.state = SRota::STATE_RUN; }	// カメラの走り状態設定

	void SetDestRotate(void);		// カメラ目標位置設定 (回転)
	void SetDestRotateWait(void);	// カメラ目標位置設定 (回転:待機)
	void SetDestFollow(void);		// カメラ目標位置設定 (追従)
	void SetDestAround(void);		// カメラ目標位置設定 (回り込み)
	void SetDestTelephoto(void);	// カメラ目標位置設定 (望遠)
	void SetDestGodItem(void);		// カメラ目標位置設定 (神器獲得)
	void SetDestResult(void);		// カメラ目標位置設定 (リザルト)
	void SetDestSelect(void);		// カメラ目標位置設定 (選択)
	void SetDestOpen(void);			// カメラ目標位置設定 (解放)
	void SetPositionV(const D3DXVECTOR3& rPos);	// 視点設定
	void SetPositionR(const D3DXVECTOR3& rPos);	// 注視点設定
	void SetRotation(const D3DXVECTOR3& rRot);	// 向き設定
	void SetDistance(const float fDis);			// 距離設定
	D3DXVECTOR3 GetPositionV(void) const		{ return m_aCamera[TYPE_MAIN].posV; }		// 現在の視点取得
	D3DXVECTOR3 GetDestPositionV(void) const	{ return m_aCamera[TYPE_MAIN].destPosV; }	// 目標の視点取得
	D3DXVECTOR3 GetPositionR(void) const		{ return m_aCamera[TYPE_MAIN].posR; }		// 現在の注視点取得
	D3DXVECTOR3 GetDestPositionR(void) const	{ return m_aCamera[TYPE_MAIN].destPosR; }	// 目標の注視点取得
	D3DXVECTOR3 GetRotation(void) const			{ return m_aCamera[TYPE_MAIN].rot; }		// 現在の向き取得
	D3DXVECTOR3 GetDestRotation(void) const		{ return m_aCamera[TYPE_MAIN].destRot; }	// 目標の向き取得
	float GetDistance(void) const				{ return m_aCamera[TYPE_MAIN].fDis; }		// 現在の距離取得
	float GetDestDistance(void) const			{ return m_aCamera[TYPE_MAIN].fDestDis; }	// 目標の距離取得

	// 金崎追加
	D3DXVECTOR3 CalcPlayerPos(); // プレイヤーを二次元座標に変換する処理
	D3DXVECTOR3 CalcWorldToScreen(const D3DXVECTOR3& pos); // ワールド座標をスクリーン座標に変換する
	bool OnScreen(const D3DXVECTOR3& pos); // スクリーン内判定
	bool OnScreen(const D3DXVECTOR3& pos, D3DXVECTOR3& posOut); // スクリーン座標を返すスクリーン内判定

	// 小原追加
	void StartReset(void);					// スタート状態のリセット処理
	void StartCamera(void);					// スタート演出時のカメラ
	void StartLand(CPlayer* pPlayer);		// 着地待ちカメラ
	void StartRound(CPlayer* pPlayer);		// 回り込みカメラ
	void EnterGame(CPlayer* pPlayer);		// ゲーム遷移処理

	// 静的メンバ関数
	static CCamera *Create(void);				// 生成
	static void Release(CCamera *&prCamera);	// 破棄

private:
	// メンバ関数
	void Rotate(const float fDeltaTime);		// カメラの更新 (回転)
	void RotateRota(const float fDeltaTime);	// カメラの更新 (回転:回転)
	void RotateWait(const float fDeltaTime);	// カメラの更新 (回転:待機)
	void RotateMove(const float fDeltaTime);	// カメラの更新 (回転:移動)
	void Follow(const float fDeltaTime);		// カメラの更新 (追従)
	void Control(const float fDeltaTime);		// カメラの更新 (操作)
	void GodItem(const float fDeltaTime);		// カメラの更新 (神器獲得)
	void Result(const float fDeltaTime);		// カメラの更新 (リザルト)
	void Select(const float fDeltaTime);		// カメラの更新 (選択)
	void Open(const float fDeltaTime);			// カメラの更新 (解放)
	void OpenRotUp(const float fDeltaTime);		// カメラの更新 (解放:上回転)
	void OpenMove(const float fDeltaTime);		// カメラの更新 (解放:移動)
	void OpenRotDown(const float fDeltaTime);	// カメラの更新 (解放:下回転)
	void Move(const float fDeltaTime);			// 位置の更新 (操作)
	void Distance(const float fDeltaTime);		// 距離の更新 (操作)
	void Rotation(const float fDeltaTime);		// 向きの更新 (操作)
	void Swing(const float fDeltaTime);			// カメラ揺れの更新

	// 金崎追加
	void CalcAround(const D3DXVECTOR3& pos, const D3DXVECTOR3& size);	// 回り込みの計算
	void Around(const float fDeltaTime);	// 回り込み
	void Telephoto(const float fDeltaTime);	// 望遠
	void CollisionWallV();	// 視点と壁の当たり判定
	void CollisionWallR();	// 注視点と壁の当たり判定

	// メンバ変数
	SCamera	m_aCamera[TYPE_MAX];	// カメラの情報
	SStart	m_startInfo;	// スタートカメラの情報
	SOpen	m_openInfo;		// 解放カメラの情報
	SRota	m_rotaInfo;		// 回転カメラの情報
	EState	m_state;		// 状態
	bool	m_bUpdate;		// 更新状況
	float	m_fFov;			// 視野角
};

#endif	// _CAMERA_H_
