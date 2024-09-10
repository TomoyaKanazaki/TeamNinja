//============================================================
//
//	プレイヤーの分身処理 [player_clone.cpp]
//	Author：小原立暉
//	Adder : 藤田勇一
//	Adder : 金崎朋弥
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
#include "field.h"
#include "actor.h"
#include "wall.h"
#include "gauge2D.h"

#include "collision.h"
#include "gimmick_action.h"
#include "effekseerControl.h"
#include "effekseerManager.h"

#include "camera.h"
#include "tension.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\CHARACTER\\player_clone.txt";	// セットアップテキスト相対パス

	const int	PRIORITY	= 3;		// プレイヤーの優先順位
	const int	BLEND_FRAME	= 5;		// モーションのブレンドフレーム
	const float	GRAVITY		= 60.0f;	// 重力
	const float	RADIUS		= 20.0f;	// 半径
	const float HEIGHT		= 80.0f;	// 身長
	const float	REV_ROTA	= 0.15f;	// 向き変更の補正係数
	const float	ADD_MOVE	= 0.08f;	// 非アクション時の速度加算量
	const float	JUMP_REV	= 0.16f;	// 通常状態時の空中の移動量の減衰係数
	const float	LAND_REV	= 0.16f;	// 通常状態時の地上の移動量の減衰係数
	const int	BLEND_FRAME_OTHER	= 5;		// モーションの基本的なブレンドフレーム
	const int	BLEND_FRAME_LAND	= 15;		// モーション着地のブレンドフレーム
	const float	SPAWN_ADD_ALPHA		= 0.03f;	// スポーン状態時の透明度の加算量

	const D3DXVECTOR3 DMG_ADDROT	= D3DXVECTOR3(0.04f, 0.0f, -0.02f);	// ダメージ状態時のプレイヤー回転量
	const D3DXVECTOR3 SHADOW_SIZE	= D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// 影の大きさ

	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), XCOL_GREEN);	// オフセット情報
	const int ORBIT_PART = 10;	// 分割数

	const float DISTANCE = 45.0f; // プレイヤーとの距離
	const float WALK_TIMER = 10.0f; // 自動消滅タイマー
	const float GIMMICK_TIMER = 20.0f; // 自動消滅タイマー
	const float DELETE_DISTNCE = 1000.0f; // ギミックになってから消えるまでの距離

	const float DASH_SPEED = 30.0f; // ダッシュモーションになる速度
	const float STEALTH_SPEED = 1.0f; // 忍び足モーションになる速度
	const float FALL_SPEED = 0.2f; // 落とし穴待機時の移動速度倍率
	const float FALL_RETURN_SPEED = 5.0f; // 落とし穴からもとに戻る移動速度倍率
	const float FALL = 100.0f; // 落とし穴による落下
	const float FALL_DELETE = 500.0f; // 落とし穴に落ちて消えるまでの距離
	const float GIMMICK_HEIGHT = 30.0f; // ギミックに反応する高さ
	const D3DXVECTOR3 BRIDGE_SCALE = D3DXVECTOR3(1.5f, 1.5f, 1.25f); // 橋状態での大きさの倍率
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
CPlayerClone::CPlayerClone() : CObjectChara(CObject::LABEL_CLONE, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
	
	m_pOrbit		(nullptr),			// 軌跡の情報
	m_move			(VEC3_ZERO),		// 移動量
	m_Action		(ACTION_MOVE),		// 現在行動
	m_OldAction		(ACTION_MOVE),		// 過去行動
	m_fDeleteTimer	(0.0f),				// 自動消滅タイマー
	m_fGimmickTimer	(0.0f),				// ギミック受付時間タイマー
	m_pGimmick		(nullptr),			// ギミックのポインタ
	m_sFrags		({}),				// ギミックフラグの文字列
	m_nIdxGimmick	(-1),				// ギミック内の管理番号
	m_pCurField		(nullptr),			// 現在の地面
	m_pOldField		(nullptr),			// 過去の地面
	m_oldPos		(VEC3_ZERO),		// 過去位置
	m_destRot		(VEC3_ZERO),		// 目標向き
	m_bJump			(false),			// ジャンプ状況
	m_fFallStart	(0.0f),				// 落とし穴の落ちる前の高さ
	m_size			(VEC3_ZERO),		// サイズ
	m_pField		(nullptr)			// フィールドギミック
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

	m_pOrbit		= nullptr;			// 軌跡の情報
	m_move			= VEC3_ZERO;		// 移動量
	m_Action		= ACTION_MOVE;		// 現在行動
	m_OldAction		= ACTION_MOVE;		// 過去行動
	m_fDeleteTimer	= 0.0f;				// 自動消滅タイマー
	m_fGimmickTimer = 0.0f;				// ギミック受付時間タイマー
	m_pGimmick		= nullptr;			// ギミックのポインタ
	m_sFrags		= {};				// ギミックフラグの文字列
	m_nIdxGimmick	= -1;				// ギミック内の管理番号
	m_pCurField		= nullptr;			// 現在の地面
	m_pOldField		= nullptr;			// 過去の地面
	m_oldPos		= VEC3_ZERO;		// 過去位置
	m_destRot		= VEC3_ZERO;		// 目標向き
	m_bJump			= true;				// ジャンプ状況
	m_size			= D3DXVECTOR3(RADIUS, HEIGHT, RADIUS);
	m_pField		= nullptr;			// フィールドフラグ

	// オブジェクトキャラクターの初期化
	if (FAILED(CObjectChara::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キャラクター情報の割当
	BindCharaData(SETUP_TXT);


	// 軌跡の生成
	//m_pOrbit = COrbit::Create
	//( // 引数
	//	GetParts(MODEL_BODY)->GetPtrMtxWorld(),	// 親マトリックス
	//	ORBIT_OFFSET,	// オフセット情報
	//	ORBIT_PART		// 分割数
	//);
	//if (m_pOrbit == nullptr)
	//{ // 非使用中の場合

	//	// 失敗を返す
	//	assert(false);
	//	return E_FAIL;
	//}

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

	if (m_OldAction == ACTION_BRIDGE
	&&  m_Action != ACTION_BRIDGE)
	{
		// 腰の親モデルを初期化
		GetParts(MODEL_WAIST)->SetParentModel(nullptr);

		// スケールをもとに戻す
		SetVec3Scaling(VEC3_ONE);

		// 寝そべってる向きを修正
		D3DXVECTOR3 rotClone = GetVec3Rotation();	// 分身向きを取得
		rotClone.x = rotClone.z = 0.0f;	// Y向き以外は初期化
		SetVec3Rotation(rotClone);		// 向きを反映
	}

	// 過去行動の更新
	m_OldAction = m_Action;

	// 壁の当たり判定
	CollisionWall();

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

		// 現在座標と前回座標が一致した場合消滅して関数を抜ける
		if (m_oldPos == GetVec3Position())
		{
			Delete(this); return;
		}

		break;

	case ACTION_FALL_TO_WAIT: // 落とし穴警戒

		// ギミック待機状態の更新
		currentMotion = UpdateFallToWait(fDeltaTime);
		break;

	case ACTION_FALL: // 落とし穴落下

		// ギミック待機状態の更新
		currentMotion = UpdateFall(fDeltaTime);

		// 自身の削除
		if (m_fFallStart - FALL_DELETE >= GetVec3Position().y)
		{
			Uninit();
			return;
		}
		break;

	case ACTION_JUMPTABLE: // ジャンプ台状態

		// ギミック台状態の更新
		currentMotion = UpdateJumpTable(fDeltaTime);
		break;

	case ACTION_HEAVYDOOR: // 重い扉

		// 重い扉状態の更新
		currentMotion = UpdateHeavyDoor(fDeltaTime);
		break;

	case ACTION_STEP: // 梯子

		// 梯子状態の更新
		currentMotion = UpdateStep(fDeltaTime);
		break;

	case ACTION_BRIDGE: // 橋

		// 橋状態の更新
		currentMotion = UpdateBridge(fDeltaTime);
		break;

	case ACTION_BUTTON: // ボタン

		// ボタン状態の更新
		currentMotion = UpdateButton(fDeltaTime);
		break;

	case ACTION_VANISH: // 消滅

		// 分身の消滅
		Vanish();
		return;

	default:
		assert(false);
		break;
	}

	// アクティブ状態の更新
	if (UpdateActive(fDeltaTime))
	{
		Delete(m_Action);
		return;
	}

	// 軌跡の更新
	if (m_pOrbit != nullptr) { m_pOrbit->Update(fDeltaTime); }

	// モーション・オブジェクトキャラクターの更新
	UpdateMotion(currentMotion, fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CPlayerClone::Draw(CShader* pShader)
{
	// オブジェクトキャラクターの描画
	CObjectChara::Draw(pShader);
}

//============================================================
//	更新状況の設定処理
//============================================================
void CPlayerClone::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);		// 自身
}

//============================================================
//	描画状況の設定処理
//============================================================
void CPlayerClone::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);		// 自身
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

	// 士気力が減少する
	CTension::Vanish();

	// 終了処理
	Uninit();

	return true;
}

//==========================================
//  ギミックのポインタを取得する
//==========================================
void CPlayerClone::SetGimmick(CGimmickAction* gimmick)
{
	// 受け取ったギミックがnullの場合関数を抜ける
	if (gimmick == nullptr) { return; }

	// ギミックで所持する最大数を超えていた場合関数を抜ける
	if (gimmick->GetNumActive() <= gimmick->GetNumClone()) { return; }

	// 引数をポインタに設定する
	m_pGimmick = gimmick;

	// ギミックの待機位置を設定する
	m_pGimmick->SetActionPoint(GetVec3Position());

	// ギミックの人数を加算
	m_pGimmick->AddNumClone();

	// 分身セット音を鳴らす
	PLAY_SOUND(CSound::LABEL_SE_GIMMICKSET_000);

	// ギミック内での管理番号を取得する
	m_nIdxGimmick = m_pGimmick->GetNumClone() - 1;

	// ギミックに対応したステータスを適用する
	switch (m_pGimmick->GetType())
	{
	case CGimmick::TYPE_JUMPTABLE: // ジャンプ台

		// ジャンプ台状態に変更
		m_Action = ACTION_JUMPTABLE;
		break;

	case CGimmick::TYPE_HEAVYDOOR:	// 重い扉

		// 重い扉状態に変更
		m_Action = ACTION_HEAVYDOOR;
		break;

	case CGimmick::TYPE_STEP:		// 踏み台

		// 梯子状態に変更
		m_Action = ACTION_STEP;
		break;

	case CGimmick::TYPE_BRIDGE:		// 橋

		// 橋状態に変更
		m_Action = ACTION_BRIDGE;
		break;

	case CGimmick::TYPE_POST:		// 設置

		// ボタン状態に変更
		m_Action = ACTION_BUTTON;
		break;

	case CGimmick::TYPE_JUMPOFF:	// 飛び降り

		// TODO：飛び降り関数の作成
		break;

	default: // 例外処理
		assert(false);
		break;
	}

	// ギミックタイマーを初期値に設定する
	m_fGimmickTimer = GIMMICK_TIMER;

	// 移動量を0にする
	m_move = VEC3_ZERO;
}

//===========================================
//  ギミックの削除
//===========================================
void CPlayerClone::DeleteGimmick()
{
	// ギミックが元々nullの場合関数を抜ける
	if (m_pGimmick == nullptr) { return; }

	// ギミックの総数を減らす
	m_pGimmick->SabNumClone();

	// ギミックのポインタをnullにする
	m_pGimmick = nullptr;
}

//===========================================
//  フィールドのポインタを取得する
//===========================================
void CPlayerClone::SetField(CField* field)
{
	// 既に別のフィールドを所持している場合関数を抜ける
	if (m_pField != nullptr) { return; }

	// 引数をポインタに設定する
	m_pField = field;

	// 落下系フラグの場合警戒状態に変更
	const char flag = m_pField->GetFlag();
	if (
		flag == CField::GetFlag(CField::TYPE_FALL) ||
		flag == CField::GetFlag(CField::TYPE_DECAYED)
		)
	{
		m_move *= FALL_SPEED;
		m_Action = ACTION_FALL_TO_WAIT;
	}
}

//===========================================
//  フィールドの削除
//===========================================
void CPlayerClone::DeleteField(CField* field)
{
	// 引数と現在所持しているフィールドが一致した場合のみ削除する
	if (m_pField == field) { m_pField = nullptr; }
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
//  文字列(フラグ)の取得
//===========================================
bool CPlayerClone::GetFrags(const char cFrag)
{
	// 文字列内を検索した結果を返す
	return m_sFrags.find(cFrag) != std::string::npos;
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
	pPlayer->m_fDeleteTimer = WALK_TIMER;

	// コントローラのバイブレーション
	GET_INPUTPAD->SetVibration(CInputPad::TYPE_CLONE);

	// 確保したアドレスを返す
	return pPlayer->Block();
}

//===========================================
//  生成(直接ギミック)
//=========================================
CPlayerClone* CPlayerClone::Create(CGimmickAction* gimmick)
{
	// ギミックがnullの場合nullを返す
	if (gimmick == nullptr) { return nullptr; }

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

	// 生成座標をプレイヤーの座標に設定する
	pPlayer->SetVec3Position(GET_PLAYER->GetVec3Position());
	pPlayer->m_oldPos = GET_PLAYER->GetVec3Position();

	// 受け取ったギミックを割り当てる
	pPlayer->SetGimmick(gimmick);

	// 確保したアドレスを返す
	return pPlayer;
}

//===========================================
//  全消去処理
//===========================================
void CPlayerClone::Delete()
{
	// リスト情報がない場合停止する
	if (m_pList == nullptr) { return; }

	// 分身の総数を取得
	int nAll = m_pList->GetNumAll();

	// 分身を全て終了する
	for (int i = nAll - 1; i >= 0; --i)
	{
		// 分身を取得
		CPlayerClone* pClone = *m_pList->GetIndex(i);

		// 分身が所持しているギミックを削除
		pClone->DeleteGimmick();

		// 中心座標を取得
		D3DXVECTOR3 pos = useful::GetMatrixPosition(pClone->GetParts(MODEL_WAIST)->GetMtxWorld());

		// 消去のエフェクトを生成する
		GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", pos, pClone->GetVec3Rotation(), VEC3_ZERO, 25.0f);

		// 分身の終了
		pClone->Uninit();
	}
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

	// ０未満の場合停止する
	if (0 > nNum) { assert(false); return; }

	// 分身を取得
	CPlayerClone* pClone = *m_pList->GetIndex(nNum);

	// 分身が所持しているギミックを削除
	pClone->DeleteGimmick();

	// 中心座標を取得
	D3DXVECTOR3 pos = useful::GetMatrixPosition(pClone->GetParts(MODEL_WAIST)->GetMtxWorld());

	// 消去のエフェクトを生成する
	GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", pos, pClone->GetVec3Rotation(), VEC3_ZERO, 25.0f);

	// 分身の終了
	SAFE_UNINIT(pClone)
}

//============================================================
//  選択消去処理 (金崎追加)
//============================================================
void CPlayerClone::Delete(const EAction act)
{
	// リスト情報がない場合停止する
	if (m_pList == nullptr) { return; }

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
		CPlayerClone* pClone = *m_pList->GetIndex(i);

		// 分身の削除フラグをオン
		if (pClone->GetAction() == act)
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

//===========================================
//  選択消去処理
//===========================================
void CPlayerClone::Delete(CPlayerClone* pClone)
{
	// リスト情報がない場合停止する
	if (pClone == nullptr) { assert(false); return; }

	// 分身所持しているギミックを削除
	pClone->DeleteGimmick();

	// 中心座標を取得
	D3DXVECTOR3 pos = useful::GetMatrixPosition(pClone->GetParts(MODEL_WAIST)->GetMtxWorld());

	// 消去のエフェクトを生成する
	GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", pos, pClone->GetVec3Rotation(), VEC3_ZERO, 25.0f);

	// 分身の終了
	pClone->Uninit();
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CPlayerClone>* CPlayerClone::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//===========================================
// 半径の取得
//===========================================
float CPlayerClone::GetRadius()
{
	return RADIUS;
}

//===========================================
//  身長の取得
//===========================================
float CPlayerClone::GetHeight()
{
	return HEIGHT;
}

//============================================================
//	全消滅処理
//============================================================
void CPlayerClone::VanishAll(void)
{
	// 分身リストがない場合抜ける
	if (m_pList == nullptr) { return; }

	std::list<CPlayerClone*> list = m_pList->GetList();	// 内部リスト
	for (auto& rList : list)
	{ // 要素数分繰り返す

		// 分身の消滅行動にする
		rList->m_Action = ACTION_VANISH;
	}
}

//============================================================
//	分身の消滅処理
//============================================================
void CPlayerClone::Vanish(void)
{
	// 中心座標を取得
	D3DXVECTOR3 pos = useful::GetMatrixPosition(GetParts(MODEL_WAIST)->GetMtxWorld());

	// 消去のエフェクトを生成する
	GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", pos, GetVec3Rotation(), VEC3_ZERO, 25.0f);

	// 分身の終了
	Uninit();
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

	// 移動
	posClone += m_move * fDeltaTime;

	// 着地判定
	UpdateLanding(posClone, &currentMotion);

	// 消滅
	m_fDeleteTimer -= fDeltaTime;
	if (m_fDeleteTimer <= 0.0f)
	{
		// 消去のエフェクトを生成する
		GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", GetVec3Position(), GetVec3Rotation(), VEC3_ZERO, 25.0f);
		Uninit();
		return MOTION_IDOL;
	}

	// ギミックの接触判定
	CheckGimmick();

	// 位置を反映
	SetVec3Position(posClone);

	// 現在のモーションを返す
	return currentMotion;
}

//===========================================
//  落とし穴警戒状態の更新処理
//===========================================
CPlayerClone::EMotion CPlayerClone::UpdateFallToWait(const float fDeltaTime)
{
	// 位置の取得
	D3DXVECTOR3 pos = GetVec3Position();
	
	// 重力
	UpdateGravity();

	// 移動
	pos += m_move * fDeltaTime;

	// 着地判定
	UpdateLanding(pos);

	// 位置の適用
	SetVec3Position(pos);

	// ギミックを持っていなかった場合関数を抜ける
	if (m_pField == nullptr)
	{
		m_move.x *= FALL_RETURN_SPEED;
		m_move.z *= FALL_RETURN_SPEED;
		m_Action = ACTION_MOVE;
		return MOTION_DASH;
	}

	// アクティブ状態になったら落下して関数を抜ける
	if (m_pField->IsFall())
	{
		// 落とし穴落下に変更
		m_Action = ACTION_FALL;

		// 落下
		return MOTION_FALL;
	}

	// 落ちる前の高さを保存
	m_fFallStart = pos.y;

	// 忍び足
	return MOTION_STEALTHWALK;
}

//===========================================
//  落とし穴落下の更新処理
//===========================================
CPlayerClone::EMotion CPlayerClone::UpdateFall(const float fDeltaTime)
{
	// 落下の移動量を加算
	m_move.y -= FALL;

	// 位置の取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 移動
	pos += m_move * fDeltaTime;

	// 位置の適用
	SetVec3Position(pos);

	// 忍び足
	return MOTION_FALL;
}

//============================================================
//	ジャンプ台行動時の更新処理
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateJumpTable(const float fDeltaTime)
{
	// 移動
	Approach();

	// 重力
	UpdateGravity();

	// 位置の取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 着地判定
	UpdateLanding(pos);

	// 作動した瞬間の場合ジャンプ台打ち上げモーションを返す
	if (m_pGimmick->GetMoment()) { return MOTION_CATAPULT; }

	// ジャンプ台待機モーションを返す
	return MOTION_JUMP_IDOL;
}

//============================================================
//	重い扉行動時の更新処理
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateHeavyDoor(const float fDeltaTime)
{
	// 移動
	Approach();

	// 重力
	UpdateGravity();

	// 位置の取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 着地判定
	UpdateLanding(pos);

	// ギミック作動中の扉上げモーションを返す
	if (m_pGimmick->IsActive()) { return MOTION_OPEN; }

	// ジャンプ台待機モーションを返す
	return MOTION_JUMP_IDOL;
}

//============================================================
//	梯子行動時の更新処理
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateStep(const float fDeltaTime)
{
	// 移動
	Approach();

	// 梯子モーションを返す
	return MOTION_LADDER;
}

//============================================================
//	橋行動時の更新処理
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateBridge(const float fDeltaTime)
{
	// 移動
	Approach();

	// ギミック作動中の梯子モーションを返す
	if (m_pGimmick->IsActive())
	{
		// 自分の次のギミック内分身を取得
		CPlayerClone *pNextClone = GetGimmickNextClone();
		if (pNextClone != nullptr)
		{ // 次の分身がいた場合

			// 腰の親モデルを初期化
			pNextClone->GetParts(MODEL_WAIST)->SetParentModel(GetParts(MODEL_WAIST));
		}

		// 少しキャラクターを大きくする
		SetVec3Scaling(BRIDGE_SCALE);

		// 橋になる為のモーションを返す
		if (m_nIdxGimmick == 0)	{ SetMotion(MOTION_LADDER); return MOTION_LADDER; }	// 先頭は梯子モーション
		else					{ SetMotion(MOTION_BRIDGE); return MOTION_BRIDGE; }	// それ以降は橋モーション
	}

	// 重力
	UpdateGravity();

	// 位置の取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 着地判定
	UpdateLanding(pos);

	// 待機モーションを返す
	return MOTION_IDOL;
}

//============================================================
//	ボタン行動時の更新処理
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateButton(const float fDeltaTime)
{
	// 移動
	Approach();

	// 待機モーションを返す
	return MOTION_IDOL;
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
	CStage *pStage = GET_STAGE;	// ステージ情報

	// TODO
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d\n", m_pOldField == m_pCurField);

	// アクターの当たり判定
	(void)CollisionActor(rPos);

	// 前回の着地地面を保存
	m_pOldField = m_pCurField;

	// 地面・制限位置の着地判定
	if (pStage->LandFieldPosition(rPos, m_oldPos, m_move, &m_pCurField)
	||  pStage->LandLimitPosition(rPos, m_move, 0.0f))
	{ // プレイヤーが着地していた場合

		// ジャンプしていない状態にする
		m_bJump = false;
	}

	if (m_pCurField != nullptr)
	{ // 現在地面に着地している場合

		// 当たっている状態にする
		m_pCurField->Hit(this);
	}

	if (m_pCurField != m_pOldField)
	{ // 前回と違う地面の場合

		if (m_pOldField != nullptr)
		{ // 前回地面に着地している場合

			// 当たっていない状態にする
			m_pOldField->Miss(this);
		}

		// 床が水の場合殺す
		if (m_pCurField != nullptr && m_pCurField->GetFlag() == m_pCurField->GetFlag(CField::TYPE_WATER))
		{
			// 死ね
			Delete(this);

			// 落水音の再生
			PLAY_SOUND(CSound::LABEL_SE_WATERDEATH_000);

			return;
		}
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

	case MOTION_JUMP_IDOL:	// ジャンプ台待機モーション
		break;

	case MOTION_JUMP_WALK:	// ジャンプ台移動モーション
		break;

	case MOTION_CATAPULT:	// ジャンプ台打ち上げモーション

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 削除する
			Delete(this);
		}
		break;

	case MOTION_LADDER:	// 梯子/橋モーション
		break;

	case MOTION_OPEN:	// 扉上げモーション

		if (IsMotionFinish() && GetGimmick() == nullptr)
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_LAND);
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

//===========================================
//  アクティブ状態での処理
//===========================================
bool CPlayerClone::UpdateActive(const float fDeltaTime)
{
	// ギミックがnullの場合falseを返す
	if (m_pGimmick == nullptr) { return false; }

	// アクティブ状態でない場合falseを返す
	if (!m_pGimmick->IsActive()) { return false; }

	// ギミックタイマーを減少
	m_fGimmickTimer -= fDeltaTime;

	// ギミックタイマーが0を下回ってる場合trueを返す
	if (m_fGimmickTimer <= 0.0f) { return true; }

	return false;
}

//===========================================
//  ギミックの接触判定
//===========================================
void CPlayerClone::CheckGimmick()
{
	// ギミックのリスト構造が無ければ抜ける
	if (CGimmickAction::GetList() == nullptr) { return; }

	std::list<CGimmickAction*> list = CGimmickAction::GetList()->GetList();	// リストを取得
	D3DXVECTOR3 pos = GetVec3Position();		// 位置
	D3DXVECTOR3 size = m_size * 0.5f;			// サイズ
	D3DXVECTOR3 posGimmick = VEC3_ZERO;			// ギミックの位置
	D3DXVECTOR3 sizeGimmick = VEC3_ZERO;		// ギミックのサイズ

	// 全てのギミックを確認する
	for (CGimmickAction* gimmick : list)
	{
		// ギミックが使える状態なら次に進む
		if (gimmick->IsActive()) { continue; }

		// 位置を取得
		posGimmick = gimmick->GetVec3Position();

		// y軸の距離が離れていた場合次に進む
		if (fabsf(posGimmick.y - pos.y) >= GIMMICK_HEIGHT) { continue; }

		// サイズを取得
		sizeGimmick = gimmick->GetVec3Sizing() * 0.5f;

		// 矩形の外の場合次に進む
		if (!collision::Box3D
		(
			pos,			// 判定位置
			posGimmick,		// 判定目標位置
			size,			// 判定サイズ(右・上・後)
			size,			// 判定サイズ(左・下・前)
			sizeGimmick,	// 判定目標サイズ(右・上・後)
			sizeGimmick		// 判定目標サイズ(左・下・前)
		))
		{
			continue;
		}

		// ギミックを設定
		SetGimmick(gimmick);

		// ループを抜ける
		break;
	}
}

//===========================================
//  目標位置に向かう処理
//===========================================
void CPlayerClone::Approach(void)
{
#if 0
	// 自身の位置を取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 目標方向との差分を求める
	D3DXVECTOR3 vecTarget = posTarget - pos;

	// 目標へのベクトルに倍率をかけ現在地に加算する
	pos += vecTarget * 0.1f;
#endif
	
	// 移動量を初期化する
	m_move = VEC3_ZERO;

	// ギミック待機位置を適用する
	SetVec3Position(m_pGimmick->CalcWaitPoint(m_nIdxGimmick, this));

	// ギミック待機向きを適用する
	SetVec3Rotation(m_pGimmick->CalcWaitRotation(m_nIdxGimmick, this));

#if 0
	// 移動量のスカラー値を算出
	float fScalar = vecTarget.x * vecTarget.x + vecTarget.z * vecTarget.z;

	// 移動量が極端に小さい場合trueを返す
	if (fScalar < DASH_SPEED * DASH_SPEED)
	{
		return true;
	}

	return false;
#endif
}

//===========================================
//  生成をブロックする処理
//===========================================
CPlayerClone* CPlayerClone::Block()
{
	// 自身の座標を取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 前回座標をプレイヤー座標に設定する
	m_oldPos = GET_PLAYER->GetVec3Position();

	// 何かに衝突した場合生成したものを削除する
	if (CollisionActor(pos) || CollisionWall())
	{
		GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", pos, GetVec3Rotation(), VEC3_ZERO, 25.0f);
		Uninit();
		return nullptr;
	}

	// エフェクトを生成
	GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_zitu_2.efkefc", pos, GetVec3Rotation(), VEC3_ZERO, 15.0f);

	// TODO：士気力の減る量考えて！
#if 0
#ifndef _DEBUG
	// 士気力を減少
	GET_PLAYER->GetTensionGauge()->AddNum(-500);
#endif
#endif

	// ヒットしていなければ生成できる
	return this;
}

//==========================================
// アクターの当たり判定
//==========================================
bool CPlayerClone::CollisionActor(D3DXVECTOR3& pos)
{
	// 判定用変数
	bool bJump = true;
	bool bHit = false;

	// アクターのリスト構造が無ければ抜ける
	if (CActor::GetList() == nullptr) { return bHit; }

	std::list<CActor*> list = CActor::GetList()->GetList();	// リストを取得

	for (CActor* actor : list)
	{
		// 当たり判定処理
		actor->Collision
		(
			pos,		// 位置
			m_oldPos,	// 前回の位置
			RADIUS,		// 半径
			HEIGHT,		// 高さ
			m_move,		// 移動量
			bJump,		// ジャンプ状況
			bHit
		);
	}

	// 位置を適用
	SetVec3Position(pos);

	if (bJump == false)
	{ // 着地状況が false かつ、ジャンプ状況が false の場合

		// ジャンプ状況を false にする
		m_bJump = false;
	}

	// 衝突判定を返す
	return bHit;
}

//===========================================
//  壁との当たり判定
//===========================================
bool CPlayerClone::CollisionWall()
{
	// 座標を取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 壁の判定
	bool bHit = GET_STAGE->CollisionWall(pos, m_oldPos, RADIUS, HEIGHT, m_move, nullptr);

	// 大人の壁の判定
	GET_STAGE->LimitPosition(pos, RADIUS);

	// 位置を適用
	SetVec3Position(pos);

	// 衝突判定を返す
	return bHit;
}

//===========================================
//	ギミックの次の分身取得
//===========================================
CPlayerClone *CPlayerClone::GetGimmickNextClone()
{
	// 分身リストがない場合抜ける
	if (m_pList == nullptr) { return nullptr; }

	// ギミックがない場合抜ける
	if (m_pGimmick == nullptr) { return nullptr; }

	// 発動人数以上の次の分身が求められている場合
	if (m_nIdxGimmick + 1 >= m_pGimmick->GetNumActive()) { return nullptr; }

	std::list<CPlayerClone*> list = m_pList->GetList();	// 内部リスト
	for (auto& rList : list)
	{ // 要素数分繰り返す

		// 違うギミックを持っている場合抜ける
		if (rList->m_pGimmick != m_pGimmick) { continue; }

		if (rList->m_nIdxGimmick == m_nIdxGimmick + 1)
		{ // 次の分身を見つけた場合

			// 分身を返す
			return rList;
		}
	}

	// ここにきたらおかしい
	assert(false);
	return nullptr;
}
