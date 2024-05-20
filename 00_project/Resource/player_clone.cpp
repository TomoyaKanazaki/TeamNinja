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
#include "manager.h"
#include "useful.h"
#include "player.h"
#include "orbit.h"
#include "multiModel.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\CHARACTER\\player.txt";	// セットアップテキスト相対パス

	const int	PRIORITY	= 3;		// プレイヤーの優先順位
	const float	MOVE		= 150.0f;	// 移動量
	const float	JUMP		= 21.0f;	// ジャンプ上昇量
	const float	GRAVITY		= 1.0f;		// 重力
	const float	RADIUS		= 20.0f;	// 半径
	const float	HEIGHT		= 100.0f;	// 縦幅
	const float	REV_ROTA	= 0.15f;	// 向き変更の補正係数
	const float	ADD_MOVE	= 0.08f;	// 非アクション時の速度加算量
	const float	JUMP_REV	= 0.16f;	// 通常状態時の空中の移動量の減衰係数
	const float	LAND_REV	= 0.16f;	// 通常状態時の地上の移動量の減衰係数
	const float	SPAWN_ADD_ALPHA	= 0.03f;	// スポーン状態時の透明度の加算量

	const D3DXVECTOR3 DMG_ADDROT	= D3DXVECTOR3(0.04f, 0.0f, -0.02f);	// ダメージ状態時のプレイヤー回転量
	const D3DXVECTOR3 SHADOW_SIZE	= D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// 影の大きさ

	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), XCOL_GREEN);	// オフセット情報
	const int ORBIT_PART = 10;	// 分割数

	const float DISTANCE = 75.0f; // プレイヤーとの距離
	const float TIMER = 10.0f; // 自動消滅タイマー
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
m_pShadow(nullptr),			// 影の情報
m_pOrbit(nullptr),			// 軌跡の情報
m_move(0.0f, 0.0f, 0.0f),	// 移動量
m_Action(ACTION_NONE),		// 行動
m_fTimer(0.0f)				// 自動消滅タイマー
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
	m_pShadow = nullptr;		// 影の情報
	m_pOrbit = nullptr;		// 軌跡の情報
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 移動量
	m_Action = ACTION_NONE; // 行動
	m_fTimer = 0.0f; // 自動消滅タイマー

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

	// マテリアルを変更
	SetAllMaterial(material::Green());

	// サイズを調整
	SetVec3Scaling(D3DXVECTOR3(0.8f, 0.8f, 0.8f));

	// プレイヤー位置に出現
	SetVec3Position(GET_PLAYER->GetVec3Position());

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
	// 変数を宣言
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション

	// 各種行動を起こす
	switch (m_Action)
	{
	case ACTION_MOVE:

		// 移動
		SetVec3Position(GetVec3Position() + (m_move * fDeltaTime));

		// 消滅
		m_fTimer -= fDeltaTime;
		if (m_fTimer <= 0.0f)
		{
			Uninit();
			return;
		}

		break;

	case ACTION_NONE:

		// 一つ前を追いかける
		ChasePrev();

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

//============================================================
//	生成処理
//============================================================
CPlayerClone* CPlayerClone::Create(void)
{
	// ポインタを宣言
	CPlayerClone* pPlayer = nullptr;	// プレイヤー情報
	pPlayer = new CPlayerClone;

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

	// 移動量を設定
	pPlayer->m_move = move;

	// 行動を設定
	pPlayer->m_Action = ACTION_MOVE;

	// 自動消滅タイマーを設定
	pPlayer->m_fTimer = TIMER;

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

	// 分身の終了
	pAvatar->Uninit();
}

//============================================================
//  全消去処理 (金崎追加)
//============================================================
void CPlayerClone::Delete(void)
{
	// リスト情報がない場合停止する
	if (m_pList == nullptr) { assert(false); return; }

	// 総数を取得
	int nNum = m_pList->GetNumAll();

	// 全ての分身を削除する
	for (int i = 0; i < nNum; ++i)
	{
		// 分身を取得
		CPlayerClone* pAvatar = *m_pList->GetIndex(0);

		// 分身の終了
		pAvatar->Uninit();
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CPlayerClone>* CPlayerClone::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	通常状態時の更新処理
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateNormal(void)
{
	// 変数を宣言
	EMotion currentMotion = MOTION_IDOL;		// 現在のモーション
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	// 現在のモーションを返す
	return currentMotion;
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
				SetMotion(nMotion);
			}
		}
	}

	// オブジェクトキャラクターの更新
	CObjectChara::Update(fDeltaTime);
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
void CPlayerClone::ChasePrev()
{
	// リストの先頭を取得する
	std::list<CPlayerClone*> list = m_pList->GetList();
	auto itrBegin = list.begin();

	// 先頭と自身を比較する
	if (*itrBegin == this)
	{
		// ついていく
		Chase(GET_PLAYER->GetVec3Position(), GET_PLAYER->GetVec3Rotation());

		return;
	}

	// リストの最後尾を取得する
	auto itrEnd = list.end();

	// 次のポインタを取得する変数
	CPlayerClone* prev = *itrBegin;

	// 自身に一致するポインタの一つ前に追従する
	for (auto itr = itrBegin; itr != itrEnd; itr++)
	{
		// 現在のポインタと自身を比較する
		if (*itr == this)
		{
			// ついていく
			Chase(prev->GetVec3Position(), prev->GetVec3Rotation());

			return;
		}

		// 現在のポインタを保存する
		prev = *itr;
	}
}

//==========================================
//  ついていく処理
//==========================================
void CPlayerClone::Chase(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// 一つ前に対して後ろ移動
	D3DXVECTOR3 posTarget = rPos + D3DXVECTOR3
	(
		sinf(rRot.y) * DISTANCE,
		0.0f,
		cosf(rRot.y) * DISTANCE
	);

	// 現在の座標を取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 目標地点へのベクトルを求める
	D3DXVECTOR3 vecTarget = posTarget - pos;

	// 目標へのベクトルに倍率をかけ現在地に加算する
	pos += vecTarget * 0.1f;

	// 位置を適用
	SetVec3Position(pos);

	// 目標の方向を向く処理
	ViewTarget(rPos);
}

//==========================================
//  目標の方向を向く処理
//==========================================
void CPlayerClone::ViewTarget(const D3DXVECTOR3& rPos)
{
	// 目標方向との差分を求める
	D3DXVECTOR3 vecTarget = rPos - GetVec3Position();

	// 差分ベクトルの向きを求める
	float fRot = -atan2f(vecTarget.x, -vecTarget.z);

	// 向きを適用する
	D3DXVECTOR3 rot = GetVec3Rotation();
	rot.y = fRot;
	SetVec3Rotation(rot);
}
