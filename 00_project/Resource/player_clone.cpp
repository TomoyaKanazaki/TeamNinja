//============================================================
//
//	プレイヤーの分身処理 [player_clone.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "player_clone.h"
#include "shadow.h"
#include "input.h"
#include "sound.h"
#include "manager.h"
#include "useful.h"
#include "player.h"
#include "orbit.h"
#include "multiModel.h"
#include "stage.h"

#include "collision.h"
#include "gimmick_action.h"
#include "effekseerControl.h"
#include "effekseerManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\CHARACTER\\player_clone.txt";	// セットアップテキスト相対パス

	const int	PRIORITY	= 3;		// プレイヤーの優先順位
	const int	BLEND_FRAME	= 5;		// モーションのブレンドフレーム
	const float	MOVE		= 150.0f;	// 移動量
	const float	GRAVITY		= 60.0f;	// 重力
	const float	REV_ROTA	= 0.15f;	// 向き変更の補正係数
	const float	ADD_MOVE	= 0.08f;	// 非アクション時の速度加算量
	const float	JUMP_REV	= 0.16f;	// 通常状態時の空中の移動量の減衰係数
	const float	LAND_REV	= 0.16f;	// 通常状態時の地上の移動量の減衰係数
	const int	BLEND_FRAME_OTHER	= 5;		// モーションの基本的なブレンドフレーム
	const int	BLEND_FRAME_LAND	= 15;		// モーション着地のブレンドフレーム
	const int	CAUTIOUS_TRANS_LOOP	= 7;		// 警戒モーションに遷移する待機ループ数
	const float	SPAWN_ADD_ALPHA		= 0.03f;	// スポーン状態時の透明度の加算量

	const D3DXVECTOR3 DMG_ADDROT	= D3DXVECTOR3(0.04f, 0.0f, -0.02f);	// ダメージ状態時のプレイヤー回転量
	const D3DXVECTOR3 SHADOW_SIZE	= D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// 影の大きさ

	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), XCOL_GREEN);	// オフセット情報
	const int ORBIT_PART = 10;	// 分割数

	const float DISTANCE = 45.0f; // プレイヤーとの距離
	const float TIMER = 10.0f; // 自動消滅タイマー

	const float DASH_SPEED = 30.0f; // ダッシュモーションになる速度
	const float STEALTH_SPEED = 1.0f; // 忍び足モーションになる速度
	const float FALL_SPEED = 0.2f; // 落とし穴待機時の移動速度倍率
	const float FALL = 150.0f; // 落とし穴による落下
	const float FALL_DELETE = 250.0f; // 落とし穴に落ちて消えるまでの距離
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CPlayerClone>* CPlayerClone::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CPlayerClone] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayerClone::CPlayerClone() : CObjectChara(CObject::LABEL_AVATAR, CObject::DIM_3D, PRIORITY),
	m_pShadow		(nullptr),		// 影の情報
	m_pOrbit		(nullptr),		// 軌跡の情報
	m_move			(VEC3_ZERO),	// 移動量
	m_Action		(ACTION_CHASE),	// 行動
	m_fDeleteTimer	(0.0f),			// 自動消滅タイマー
	m_fChargeTimer	(0.0f),			// ため時間タイマー
	m_pGimmick		(nullptr),		// ギミックのポインタ
	m_sFrags		({}),			// ギミックフラグの文字列
	m_nIdxGimmick	(-1),			// ギミック内の管理番号
	m_oldPos		(VEC3_ZERO),	// 過去位置
	m_destRot		(VEC3_ZERO),	// 目標向き
	m_bJump			(false),		// ジャンプ状況
	m_fFallStart	(0.0f)			// 落とし穴の落ちる前の高さ
{

}

//============================================================
//	デストラクタ
//============================================================
CPlayerClone::~CPlayerClone()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayerClone::Init(void)
{
	// メンバ変数を初期化
	m_pShadow		= nullptr;		// 影の情報
	m_pOrbit		= nullptr;		// 軌跡の情報
	m_move			= VEC3_ZERO;	// 移動量
	m_Action		= ACTION_CHASE;	// 行動
	m_fDeleteTimer	= 0.0f;			// 自動消滅タイマー
	m_fChargeTimer	= 0.0f;			// ため時間タイマー
	m_pGimmick		= nullptr;		// ギミックのポインタ
	m_sFrags		= {};			// ギミックフラグの文字列
	m_nIdxGimmick	= -1;			// ギミック内の管理番号
	m_oldPos		= VEC3_ZERO;	// 過去位置
	m_destRot		= VEC3_ZERO;	// 目標向き
	m_bJump			= true;			// ジャンプ状況

	// オブジェクトキャラクターの初期化
	if (FAILED(CObjectChara::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キャラクター情報の割当
	BindCharaData(SETUP_TXT);

	// 影の生成
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, SHADOW_SIZE, this);
	if (m_pShadow == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 軌跡の生成
	m_pOrbit = COrbit::Create
	( // 引数
		GetParts(MODEL_BODY)->GetPtrMtxWorld(),	// 親マトリックス
		ORBIT_OFFSET,	// オフセット情報
		ORBIT_PART		// 分割数
	);
	if (m_pOrbit == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// マテリアルを変更
	SetAllMaterial(material::Green());

	// サイズを調整
	SetVec3Scaling(D3DXVECTOR3(0.8f, 0.8f, 0.8f));

	// プレイヤー位置に設定
	SetVec3Position(GET_PLAYER->GetVec3Position());

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CPlayerClone>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayerClone::Uninit(void)
{
	// 影の終了
	m_pShadow->DeleteObjectParent();	// 親オブジェクトを削除
	SAFE_UNINIT(m_pShadow);

	// 軌跡の終了
	SAFE_UNINIT(m_pOrbit);

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトキャラクターの終了
	CObjectChara::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayerClone::Update(const float fDeltaTime)
{
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション

	// 過去位置の更新
	UpdateOldPosition();

	// 各種行動を起こす
	switch (m_Action)
	{
	case ACTION_MOVE: // 歩行

		// 移動行動時の更新 (待機モーションなら破棄済み)
		currentMotion = UpdateMove(fDeltaTime);
		if (currentMotion == MOTION_IDOL)
		{ // 破棄されてる場合

			// 関数を抜ける
			return;
		}

		break;

	case ACTION_CHASE: // 追従

		// 追従行動時の更新
		currentMotion = UpdateChase(fDeltaTime);

		break;

	case ACTION_MOVE_TO_WAIT: // 待機位置への移動

		// 待機位置への移動状態の更新
		currentMotion = UpdateMoveToWait(fDeltaTime);

		break;

	case ACTION_WAIT: // ギミック待機

		// ギミック待機状態の更新
		currentMotion = UpdateWait(fDeltaTime);

		break;

	case ACTION_FALL: // 落とし穴警戒

		// ギミック待機状態の更新
		currentMotion = UpdateFall(fDeltaTime);

		break;

	case ACTION_JUMPTABLE: // ジャンプ台状態

		// ギミック台状態の更新
		currentMotion = UpdateJumpTable(fDeltaTime);

		break;

	default:
		assert(false);
		break;
	}

	// 影の更新
	m_pShadow->Update(fDeltaTime);

	// 軌跡の更新
	m_pOrbit->Update(fDeltaTime);

	// モーション・オブジェクトキャラクターの更新
	UpdateMotion(currentMotion, fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CPlayerClone::Draw(CShader* pShader)
{
	CToonShader* pToonShader = CToonShader::GetInstance();	// トゥーンシェーダー情報
	if (pToonShader->IsEffectOK())
	{ // エフェクトが使用可能な場合

		// オブジェクトキャラクターの描画
		CObjectChara::Draw(pToonShader);
	}
	else
	{ // エフェクトが使用不可能な場合

		// エフェクトエラー
		assert(false);

		// オブジェクトキャラクターの描画
		CObjectChara::Draw(pShader);
	}
}

//============================================================
//	更新状況の設定処理
//============================================================
void CPlayerClone::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);		// 自身
	m_pShadow->SetEnableUpdate(bUpdate);	// 影
}

//============================================================
//	描画状況の設定処理
//============================================================
void CPlayerClone::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);		// 自身
	m_pShadow->SetEnableDraw(bDraw);	// 影
}

//============================================================
//	ノックバックヒット処理
//============================================================
bool CPlayerClone::HitKnockBack(const int /*nDamage*/, const D3DXVECTOR3& /*rVecKnock*/)
{
	if (IsDeath()) { return false; }	// 死亡済み

	return true;
}

//============================================================
//	ヒット処理
//============================================================
bool CPlayerClone::Hit(const int nDamage)
{
	if (IsDeath()) { return false; }	// 死亡済み

	return true;
}

//==========================================
//  ギミックのポインタを取得する
//==========================================
void CPlayerClone::SetGimmick(CGimmickAction* gimmick)
{
	// 引数をポインタに設定する
	m_pGimmick = gimmick;

	// ギミック内での管理番号を取得する
	m_nIdxGimmick = m_pGimmick->GetNumClone() - 1;

	// ギミック待機状態になる
	m_Action = ACTION_MOVE_TO_WAIT;
}

//===========================================
//  文字列(フラグ)の追加
//===========================================
void CPlayerClone::AddFrags(const char cFrag)
{
	// 文字列内を検索に同じ文字が存在したら関数を抜ける
	if (m_sFrags.find(cFrag) != std::string::npos) { return; }

	// 文字列に受け取ったフラグを追加する
	m_sFrags += cFrag;
}

//=========================================
//  文字列(フラグ)の削除
//===========================================
void CPlayerClone::SabFrags(const char cFrag)
{
	// 文字列内を検索し番号を取得する
	size_t nIdx = m_sFrags.find(cFrag);

	// 文字列内にフラグが存在しなかった場合関数を抜ける
	if (nIdx == std::string::npos) { return; }

	// 文字列からフラグを削除する
	m_sFrags.erase(nIdx);
}

//===========================================
//  文字列(フラグ)の追加
//===========================================
bool CPlayerClone::GetFrags(const char cFrag)
{
	// 文字列内を検索した結果を返す
	return m_sFrags.find(cFrag) != std::string::npos;
}

//============================================================
//	生成処理
//============================================================
CPlayerClone* CPlayerClone::Create(void)
{
	// ポインタを宣言
	CPlayerClone* pPlayer = new CPlayerClone;	// プレイヤー情報
	if (pPlayer == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// プレイヤーの初期化
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// プレイヤーの破棄
			SAFE_DELETE(pPlayer);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pPlayer;
	}
}

//==========================================
//  生成処理(歩行)
//==========================================
CPlayerClone* CPlayerClone::Create(const D3DXVECTOR3& move)
{
	// ポインタを宣言
	CPlayerClone* pPlayer = new CPlayerClone;	// プレイヤー情報

	// 生成に失敗した場合nullを返す
	if (pPlayer == nullptr) { return nullptr; }

	// プレイヤーの初期化
	if (FAILED(pPlayer->Init()))
	{ // 初期化に失敗した場合

		// プレイヤーの破棄
		SAFE_DELETE(pPlayer);
		return nullptr;
	}

	// 向きを設定
	D3DXVECTOR3 rot = VEC3_ZERO;		// 向き
	rot.y = atan2f(-move.x, -move.z);	// 向きを移動量から求める
	pPlayer->SetVec3Rotation(rot);		// 向き設定

	// 移動量を設定
	pPlayer->m_move = move;

	// 行動を設定
	pPlayer->m_Action = ACTION_MOVE;

	// 自動消滅タイマーを設定
	pPlayer->m_fDeleteTimer = TIMER;

	// 確保したアドレスを返す
	return pPlayer;
}

//==========================================
//  生成処理(歩行)
//==========================================
CPlayerClone* CPlayerClone::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move)
{
	// ポインタを宣言
	CPlayerClone* pPlayer = new CPlayerClone;	// プレイヤー情報

	// 生成に失敗した場合nullを返す
	if (pPlayer == nullptr) { return nullptr; }

	// プレイヤーの初期化
	if (FAILED(pPlayer->Init()))
	{ // 初期化に失敗した場合

		// プレイヤーの破棄
		SAFE_DELETE(pPlayer);
		return nullptr;
	}

	// 位置を設定
	pPlayer->SetVec3Position(pos);

	// 向きを設定
	D3DXVECTOR3 rot = VEC3_ZERO;		// 向き
	rot.y = atan2f(-move.x, -move.z);	// 向きを移動量から求める
	pPlayer->SetVec3Rotation(rot);		// 向き設定

	// 移動量を設定
	pPlayer->m_move = move;

	// 行動を設定
	pPlayer->m_Action = ACTION_MOVE;

	// 自動消滅タイマーを設定
	pPlayer->m_fDeleteTimer = TIMER;

	// 確保したアドレスを返す
	return pPlayer;
}

//============================================================
// 消去処理
//============================================================
void CPlayerClone::Delete(const int nNum)
{
	// リスト情報がない場合停止する
	if (m_pList == nullptr) { assert(false); return; }

	// 現在の総数よりも上の数字が指定されていた場合停止する
	if (m_pList->GetNumAll() <= nNum) { assert(false); return; }

	// 分身を取得
	CPlayerClone* pAvatar = *m_pList->GetIndex(nNum);

	// 消去のエフェクトを生成する
	GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", pAvatar->GetVec3Position(), pAvatar->GetVec3Rotation(), VEC3_ZERO, 25.0f);

	// 分身の終了
	pAvatar->Uninit();
}

//============================================================
//  全消去処理 (金崎追加)
//============================================================
void CPlayerClone::Delete(const EAction act)
{
	// リスト情報がない場合停止する
	if (m_pList == nullptr) { assert(false); return; }

	// 総数を取得
	int nNum = m_pList->GetNumAll();

	// 削除する分身の番号を保存する変数
	bool* bDelete = new bool[nNum];

	// 削除フラグを立てる
	for (int i = 0; i < nNum; ++i)
	{
		// 削除フラグをオフ
		bDelete[i] = false;

		// 分身を取得
		CPlayerClone* pAvatar = *m_pList->GetIndex(i);

		// 分身の削除フラグをオン
		if (pAvatar->GetAction() == act)
		{
			bDelete[i] = true;
		}
	}

	// フラグの立っている分身を削除する
	for (int i = nNum - 1; i >= 0; --i)
	{
		// 削除
		if (bDelete[i]) { Delete(i); }
	}

	// 削除フラグを削除
	delete[] bDelete;
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CPlayerClone>* CPlayerClone::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//==========================================
//  分身を追従に変更
//==========================================
void CPlayerClone::CallBack()
{
	// リスト情報がない場合停止する
	if (m_pList == nullptr) { return; }

	// 総数を取得
	int nNum = m_pList->GetNumAll();

	// 追従と歩行以外の行動を追従に変更
	for (int i = 0; i < nNum; ++i)
	{
		// 分身を取得
		CPlayerClone* pClone = *m_pList->GetIndex(i);

		// 追従させたくない場合は次に進む
		if (pClone->GetAction() == ACTION_MOVE) { continue; }
		if (pClone->GetAction() == ACTION_CHASE) { continue; }
		if (pClone->GetAction() == ACTION_FALL) { continue; }

		// ギミックの保有分身数を減らす
		pClone->m_pGimmick->SetNumClone(pClone->m_pGimmick->GetNumClone() - 1);

		// 保存しているギミックを初期化する
		pClone->m_pGimmick = nullptr;

		// ギミック内管理番号をリセットする
		pClone->m_nIdxGimmick = -1;

		// 移動量をリセットする
		pClone->m_move = VEC3_ZERO;

#ifdef _DEBUG
		// マテリアルを変更
		pClone->SetAllMaterial(material::Green());
#endif

		// 追従状態にする
		pClone->SetAction(ACTION_CHASE);
	}
}

//============================================================
//	移動行動時の更新処理
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateMove(const float fDeltaTime)
{
	D3DXVECTOR3 posClone = GetVec3Position();	// クローン位置
	EMotion currentMotion = MOTION_DASH;		// 現在のモーション

	// 重力の更新
	UpdateGravity();

	// 着地判定
	UpdateLanding(posClone, &currentMotion);

	// 移動
	posClone += m_move * fDeltaTime;

	// 消滅
	m_fDeleteTimer -= fDeltaTime;
	if (m_fDeleteTimer <= 0.0f)
	{
		// 消去のエフェクトを生成する
		GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", GetVec3Position(), GetVec3Rotation(), VEC3_ZERO, 25.0f);
		Uninit();
		return MOTION_IDOL;
	}

	// 位置を反映
	SetVec3Position(posClone);

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	追従行動時の更新処理
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateChase(const float fDeltaTime)
{
	D3DXVECTOR3 posClone = GetVec3Position();	// クローン位置
	D3DXVECTOR3 rotClone = GetVec3Rotation();	// クローン向き
	EMotion currentMotion = MOTION_DASH;		// 現在のモーション

	// 重力の更新
	UpdateGravity();

	// 一つ前を追いかけて、その際のモーションを返す
	currentMotion = ChasePrev(&posClone, &rotClone);

	// 着地判定
	UpdateLanding(posClone, &currentMotion);

	// 移動
	posClone += m_move * fDeltaTime;

	// 向きの更新
	UpdateRotation(rotClone);

	// 位置を反映
	SetVec3Position(posClone);

	// 向きを反映
	SetVec3Rotation(rotClone);

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	待機位置への移動時の更新処理
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateMoveToWait(const float fDeltaTime)
{
	// ギミックがnullの場合関数を抜ける
	if (m_pGimmick == nullptr) { assert(false); return MOTION_IDOL; }

	// ギミックが落とし穴だった場合関数を抜ける
	if (m_pGimmick->GetType() == CGimmick::TYPE_FALL)
	{
		// 移動量を減少させる
		m_move.x *= FALL_SPEED;
		m_move.z *= FALL_SPEED;

		// 落とし穴警戒状態にする
		m_Action = ACTION_FALL;
		return MOTION_STEALTHWALK; 
	}

	// TODO：Gimmick移動どうしよかね
#if 1
	// ギミックの位置に移動する
	SetVec3Position(m_pGimmick->GetVec3Position());

	// ギミック待機状態にする
	m_Action = ACTION_WAIT;
#else
	int nNumAct = m_pGimmick->GetNumActive();
	float fRate = (D3DX_PI * 2.0f) / nNumAct;

	D3DXVECTOR3 posOrigin = m_pGimmick->GetVec3Position();
	posOrigin.x += sinf(fRate * 1 + HALF_PI) * 80.0f;
	posOrigin.z += cosf(fRate * 1 + HALF_PI) * 80.0f;

	// ギミックの位置に移動する
	SetVec3Position(posOrigin);

	// ギミック待機状態にする
	m_Action = ACTION_WAIT;
#endif

	// 移動モーションを返す
	return MOTION_DASH;
}

//==========================================
//  ギミック待機
//==========================================
CPlayerClone::EMotion CPlayerClone::UpdateWait(const float fDeltaTime)
{
#ifdef _DEBUG
	// マテリアルカラーを変えてわかりやすくする
	SetAllMaterial(material::Yellow());
#endif

	// ギミックがnullの場合関数を抜ける
	if (m_pGimmick == nullptr) { assert(false); return MOTION_IDOL; }

	// ギミックがアクティブ状態なら
	if (!m_pGimmick->IsActive()) { return MOTION_IDOL; }

	// ギミックに対応したステータスを適用する
	switch (m_pGimmick->GetType())
	{
	case CGimmick::TYPE_JUMPTABLE: // ジャンプ台

		// ジャンプ台状態に変更
		m_Action = ACTION_JUMPTABLE;

		break;

	default: // その他
		break;
	}

	return MOTION_IDOL;
}

//===========================================
//  落とし穴警戒時の更新処理
//===========================================
CPlayerClone::EMotion CPlayerClone::UpdateFall(const float fDeltaTime)
{
	// 位置の取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 移動
	pos += m_move * fDeltaTime;

	// 位置の適用
	SetVec3Position(pos);

	// アクティブ状態になったら落下して関数を抜ける
	if (m_pGimmick->IsActive())
	{
		// 移動量を減少
		m_move.y -= FALL;

		// 落下
		return MOTION_FALL;
	}

	// 着地判定
	UpdateLanding(pos);

	// 落ちる前の高さを保存
	m_fFallStart = pos.y;

	// 忍び足
	return MOTION_STEALTHWALK;
}

//============================================================
//	ジャンプ台行動時の更新処理
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateJumpTable(const float fDeltaTime)
{
	if (m_pGimmick->GetMoment()) { return MOTION_CATAPULT; }

	return MOTION_JUMP_IDOL;
}

//============================================================
//	過去位置の更新処理
//============================================================
void CPlayerClone::UpdateOldPosition(void)
{
	// 過去位置を更新
	m_oldPos = GetVec3Position();
}

//============================================================
//	重力の更新処理
//============================================================
void CPlayerClone::UpdateGravity(void)
{
	// 重力を加算
	m_move.y -= GRAVITY;
}

//============================================================
//	向きの更新処理
//============================================================
void CPlayerClone::UpdateRotation(D3DXVECTOR3& rRot)
{
	// 変数を宣言
	float fDiffRot = 0.0f;	// 差分向き

	// 目標向きの正規化
	useful::NormalizeRot(m_destRot.y);

	// 目標向きまでの差分を計算
	fDiffRot = m_destRot.y - rRot.y;

	// 差分向きの正規化
	useful::NormalizeRot(fDiffRot);

	// 向きの更新
	rRot.y += fDiffRot * REV_ROTA;

	// 向きの正規化
	useful::NormalizeRot(rRot.y);
}

//============================================================
//	着地状況の更新処理
//============================================================
void CPlayerClone::UpdateLanding(D3DXVECTOR3& rPos, EMotion* pCurMotion)
{
	CStage *pStage = CScene::GetStage();	// ステージ情報

	// 地面・制限位置の着地判定
	if (pStage->LandFieldPosition(rPos, m_move)
	||  pStage->LandLimitPosition(rPos, m_move, 0.0f))
	{ // プレイヤーが着地していた場合

		// ジャンプしていない状態にする
		m_bJump = false;
	}

	if (!m_bJump)
	{ // 空中にいない場合

		if (GetMotionType() == MOTION_FALL)
		{ // モーションが落下中の場合

			// 着地音の再生
			PLAY_SOUND(CSound::LABEL_SE_LAND_S);
		}
	}
	else
	{ // 空中にいる場合

		// 落下モーションを指定
		if (pCurMotion == nullptr) { return; }
		*pCurMotion = MOTION_FALL;
	}
}

//============================================================
//	モーション・オブジェクトキャラクターの更新処理
//============================================================
void CPlayerClone::UpdateMotion(int nMotion, const float fDeltaTime)
{
	// 死んでたら抜ける
	if (IsDeath()) { return; }

	// 変数を宣言
	int nAnimMotion = GetMotionType();	// 現在再生中のモーション

	if (nMotion != NONE_IDX)
	{ // モーションが設定されている場合

		if (IsMotionLoop())
		{ // ループするモーションだった場合

			if (nAnimMotion != nMotion)
			{ // 現在のモーションが再生中のモーションと一致しない場合

				// 現在のモーションの設定
				SetMotion(nMotion, BLEND_FRAME);
			}
		}
	}

	// オブジェクトキャラクターの更新
	CObjectChara::Update(fDeltaTime);

	switch (GetMotionType())
	{ // モーションの種類ごとの処理
	case MOTION_IDOL:	// 待機モーション

		if (GetMotionNumLoop() >= CAUTIOUS_TRANS_LOOP)
		{ // 待機モーションでしばらくいた場合

			// 警戒モーションの設定
			SetMotion(MOTION_CAUTIOUS, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_CAUTIOUS:	// 警戒モーション

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_LAND);
		}

		break;

#if 0
	case MOTION_DASH:	// 歩行モーション

		if (GetMotionPose() % 4 == 0 && GetMotionCounter() == 0)
		{ // 足がついたタイミングの場合

			switch (m_land)
			{ // 着地物ごとの処理
			case LAND_OBSTACLE:

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_WALK_OBS);	// 歩行音（障害物）

				break;

			default:

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_WALK_BUILD);	// 歩行音（ビル）

				break;
			}
		}

		break;

	case MOTION_STEALTHWALK:	// 忍び足モーション

		break;
#endif

	case MOTION_FALL:	// 落下モーション

		if (!m_bJump)
		{ // ジャンプ中ではない場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_LANDING:	// 着地モーション

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_LAND);
		}

		break;

	case MOTION_JUMP_IDOL:	// ジャンプ台モーション
		break;

	case MOTION_CATAPULT:	// 打ち上げモーション

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// ジャンプ台モーションに遷移
			SetMotion(MOTION_JUMP_IDOL, BLEND_FRAME_OTHER);
		}

		break;
	}
}

//============================================================
//	フェードアウト状態時の更新処理
//============================================================
bool CPlayerClone::UpdateFadeOut(const float fAdd)
{
	// 変数を宣言
	bool bAlpha = false;		// 透明状況
	float fAlpha = GetAlpha();	// 透明度

	// プレイヤー自身の描画を再開
	CObject::SetEnableDraw(true);

	// 透明度を上げる
	fAlpha += fAdd;

	if (fAlpha >= GetMaxAlpha())
	{ // 透明度が上がり切った場合

		// 透明度を補正
		fAlpha = GetMaxAlpha();

		// 不透明になり切った状態にする
		bAlpha = true;
	}

	// 透明度を設定
	SetAlpha(fAlpha);

	// 透明状況を返す
	return bAlpha;
}

//============================================================
//	フェードイン状態時の更新処理
//============================================================
bool CPlayerClone::UpdateFadeIn(const float fSub)
{
	// 変数を宣言
	bool bAlpha = false;		// 透明状況
	float fAlpha = GetAlpha();	// 透明度

	// 透明度を下げる
	fAlpha -= fSub;

	if (fAlpha <= 0.0f)
	{ // 透明度が下がり切った場合

		// 透明度を補正
		fAlpha = 0.0f;

		// 透明になり切った状態にする
		bAlpha = true;

		// プレイヤー自身の描画を停止
		CObject::SetEnableDraw(false);
	}

	// 透明度を設定
	SetAlpha(fAlpha);

	// 透明状況を返す
	return bAlpha;
}

//==========================================
//  前についていく処理
//==========================================
CPlayerClone::EMotion CPlayerClone::ChasePrev(D3DXVECTOR3* pPosThis, D3DXVECTOR3* pRotThis)
{
	// リストを取得する
	std::list<CPlayerClone*> list = m_pList->GetList();
	auto itrBegin = list.begin();
	auto itrEnd = list.end();

	// プレイヤー位置・向きを取得する
	CPlayer *pPlayer = GET_PLAYER;	// プレイヤー情報
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = pPlayer->GetVec3Rotation();	// プレイヤー向き

	// 一つ前のポインタを保存する変数
	CPlayerClone* prev = *itrBegin;

	// 自身のポインタを走査する
	for (auto itr = itrBegin; itr != itrEnd; ++itr)
	{
		// 自身ではない場合一つ前を保存して次に進む
		if (*itr != this) { prev = *itr; continue; }

		// 自身が先頭だった場合プレイヤーに追従し関数を抜ける
		if (this == *itrBegin) { return Chase(pPosThis, pRotThis, posPlayer, rotPlayer); }

		// 自身の追従する相手を選択する
		while (1)
		{
			// 一つ前のポインタを保存する
			--itr;
			prev = *itr;

			// 前が追従していない場合
			if (prev->GetAction() != ACTION_CHASE)
			{
				// 一つ前が先頭でない場合次に進む
				if (prev != *itrBegin) { continue; }

				// プレイヤーに追従し関数を抜ける
				return Chase(pPosThis, pRotThis, posPlayer, rotPlayer);
			}

			// 一つ前に追従し関数を抜ける
			return Chase(pPosThis, pRotThis, prev->GetVec3Position(), prev->GetVec3Rotation());
		}
	}

	// ここに来たらバグすぎきもすぎわろたんぬ
	assert(false);
	return MOTION_IDOL;
}

//==========================================
//  ついていく処理
//==========================================
CPlayerClone::EMotion CPlayerClone::Chase
(
	D3DXVECTOR3* pPosThis,			// 自身の位置
	D3DXVECTOR3* pRotThis,			// 自身の向き
	const D3DXVECTOR3& rPosPrev,	// ついていくやつの位置
	const D3DXVECTOR3& rRotPrev		// ついていくやつの向き
)
{
	// 一つ前に対して後ろ移動
	D3DXVECTOR3 posTarget = rPosPrev + D3DXVECTOR3
	(
		sinf(rRotPrev.y) * DISTANCE,
		0.0f,
		cosf(rRotPrev.y) * DISTANCE
	);

	// 目標地点へのベクトルを求める
	D3DXVECTOR3 vecTarget = posTarget - *pPosThis;

	// 目標へのベクトルに倍率をかけ現在地に加算する
	vecTarget.y = 0.0f;
	*pPosThis += vecTarget * 0.1f;

	// 目標の方向を向く処理
	ViewTarget(*pPosThis, rPosPrev);

	// 移動量のスカラー値を産出
	float fScalar = sqrtf(vecTarget.x * vecTarget.x + vecTarget.z * vecTarget.z);
	if (fScalar > DASH_SPEED)
	{
		return MOTION_DASH;
	}
	else if (fScalar > STEALTH_SPEED)
	{
		return MOTION_STEALTHWALK;
	}
	else
	{
		return MOTION_IDOL;
	}
}

//==========================================
//  目標の方向を向く処理
//==========================================
void CPlayerClone::ViewTarget(const D3DXVECTOR3& rPosThis, const D3DXVECTOR3& rPosPrev)
{
	// 目標方向との差分を求める
	D3DXVECTOR3 vecTarget = rPosPrev - rPosThis;

	// 差分ベクトルの向きを求める
	float fRot = -atan2f(vecTarget.x, -vecTarget.z);

	// 目標向きを設定
	m_destRot.y = fRot;
}
