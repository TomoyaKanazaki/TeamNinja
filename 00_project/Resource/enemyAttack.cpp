//============================================================
//
//	攻撃系敵処理 [enemyAttack.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "enemyAttack.h"
#include "renderer.h"
#include "deltaTime.h"
#include "sound.h"

#include "player.h"
#include "player_clone.h"
#include "multiModel.h"
#include "collision.h"
#include "enemyChaseRange.h"

#include "enemyStalk.h"
#include "enemyWolf.h"
#include "enemyAmbush.h"
#include "effekseerControl.h"
#include "enemyNavigation.h"
#include "camera.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 ATTACK_COLLUP = D3DXVECTOR3(30.0f, 100.0f, 30.0f);	// 攻撃判定(上)
	const D3DXVECTOR3 ATTACK_COLLDOWN = D3DXVECTOR3(30.0f, 0.0f, 30.0f);	// 攻撃判定(下)
	const int DODGE_COUNT = 20;					// 回避カウント数
	const float SHAKEOFF_RANGE = 1000.0f;		// 振り切れる距離
	const float DIVERSION_EFFECT_SCALE = 18.0f;	// 分身との戦闘エフェクトの大きさ
	const int REGRESSION_COUNT = 120;			// 回帰するカウント数
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CEnemyAttack>* CEnemyAttack::m_pList = nullptr;			// オブジェクトリスト

//************************************************************
//	子クラス [CEnemyAttack] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyAttack::CEnemyAttack() : CEnemy(),
m_pChaseRange(nullptr),		// 追跡範囲の情報
m_pClone(nullptr),			// 分身の情報
m_posTarget(VEC3_ZERO),		// 目標の位置
m_target(TARGET_NONE),		// 標的
m_nAttackCount(0),			// 攻撃カウント
m_nRegressionCount(0),		// 回帰カウント
m_type(TYPE_STALK),			// 種類
m_bDodge(false)				// 回避受付フラグ
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyAttack::~CEnemyAttack()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyAttack::Init(void)
{
	// 敵の初期化
	if (FAILED(CEnemy::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CEnemyAttack>::Create();
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
void CEnemyAttack::Uninit(void)
{
	// 追跡範囲の終了処理
	SAFE_UNINIT(m_pChaseRange);

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 敵の終了
	CEnemy::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyAttack::Update(const float fDeltaTime)
{
	// 敵の更新
	CEnemy::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CEnemyAttack::Draw(CShader* pShader)
{
	// 敵の描画
	CEnemy::Draw(pShader);
}

//============================================================
// 情報の設定処理
//============================================================
void CEnemyAttack::SetData(void)
{

}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CEnemyAttack>* CEnemyAttack::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	セットアップ処理
//============================================================
HRESULT CEnemyAttack::LoadSetup(const char* pPass)
{
	int nType = 0;					// 種類の代入用
	int nNavType = 0;				// ナビの種類の代入用
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	float fMoveWidth = 0.0f;		// 移動幅
	float fMoveDepth = 0.0f;		// 移動奥行
	std::vector<D3DXVECTOR3> route;	// 順路
	float fChaseWidth = 0.0f;		// 追跡幅
	float fChaseDepth = 0.0f;		// 追跡奥行

	// ファイルを開く
	std::ifstream file(pPass);	// ファイルストリーム

	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "敵セットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// ファイルを読込
	std::string str;	// 読込文字列
	while (file >> str)
	{ // ファイルの終端ではない場合ループ

		if (str.front() == '#')
		{ // コメントアウトされている場合

			// 一行全て読み込む
			std::getline(file, str);
		}
		else if (str == "STAGE_ENEMYSET")
		{
			do
			{ // END_STAGE_ENEMYSETを読み込むまでループ

				// 文字列を読み込む
				file >> str;

				if (str.front() == '#')
				{ // コメントアウトされている場合

					// 一行全て読み込む
					std::getline(file, str);
				}
				else if (str == "ENEMYSET")
				{
					do
					{ // END_ENEMYSETを読み込むまでループ

						// 文字列を読み込む
						file >> str;

						if (str == "TYPE")
						{
							file >> str;	// ＝を読込

							// 種類を読込
							file >> nType;
						}
						else if (str == "NAVTYPE")
						{
							file >> str;	// ＝を読込

							// ナビの種類を読込
							file >> nNavType;
						}
						else if (str == "POS")
						{
							file >> str;	// ＝を読込

							// 位置を読込
							file >> pos.x;
							file >> pos.y;
							file >> pos.z;
						}
						else if (str == "ROT")
						{
							file >> str;	// ＝を読込

							// 向きを読込
							file >> rot.x;
							file >> rot.y;
							file >> rot.z;
						}
						else if (str == "MOVESIZE")
						{
							// ランダムじゃなかった場合、停止
							assert(nNavType == CEnemyNav::TYPE_RANDOM);

							file >> str;	// ＝を読込

							// 移動範囲を読込
							file >> fMoveWidth;
							file >> fMoveDepth;
						}
						else if (str == "ROUTE")
						{
							// ルート巡回じゃなかった場合、停止
							assert(nNavType == CEnemyNav::TYPE_STREET);

							int nNumRoute = 0;
							D3DXVECTOR3 posRoute = VEC3_ZERO;

							// 順路の総数を読込
							file >> nNumRoute;

							for (int nCnt = 0; nCnt < nNumRoute; nCnt++)
							{
								// 順路を読込
								file >> posRoute.x;
								file >> posRoute.y;
								file >> posRoute.z;

								// 順路を追加する
								route.push_back(posRoute);
							}
						}
						else if (str == "CHASESIZE")
						{
							file >> str;	// ＝を読込

							// 追跡範囲を読込
							file >> fChaseWidth;
							file >> fChaseDepth;
						}
					} while (str != "END_ENEMYSET");	// END_ENEMYSETを読み込むまでループ

					switch (nType)
					{
					case CEnemyAttack::TYPE_STALK:

						switch (nNavType)
						{
						case CEnemyNav::TYPE_RANDOM:

							// 追跡敵の生成
							CEnemyStalk::Create
							(
								pos,			// 位置
								rot,			// 向き
								fMoveWidth,		// 移動幅
								fMoveDepth,		// 移動奥行
								fChaseWidth,	// 追跡幅
								fChaseDepth		// 追跡奥行
							);

							break;

						case CEnemyNav::TYPE_STREET:

							// 追跡敵の生成
							CEnemyStalk::Create
							(
								pos,			// 位置
								rot,			// 向き
								route,			// ルート
								fChaseWidth,	// 追跡幅
								fChaseDepth		// 追跡奥行
							);

							break;

						default:

							// 停止
							assert(false);

							break;
						}

						break;

					case CEnemyAttack::TYPE_WOLF:

						switch (nNavType)
						{
						case CEnemyNav::TYPE_RANDOM:

							// 狼敵の生成
							CEnemyWolf::Create
							(
								pos,			// 位置
								rot,			// 向き
								fMoveWidth,		// 移動幅
								fMoveDepth,		// 移動奥行
								fChaseWidth,	// 追跡幅
								fChaseDepth		// 追跡奥行
							);

							break;

						case CEnemyNav::TYPE_STREET:

							// 狼敵の生成
							CEnemyWolf::Create
							(
								pos,			// 位置
								rot,			// 向き
								route,			// ルート
								fChaseWidth,	// 追跡幅
								fChaseDepth		// 追跡奥行
							);

							break;

						default:

							// 停止
							assert(false);

							break;
						}

						break;

					case CEnemyAttack::TYPE_AMBUSH:

						// 待ち伏せ敵の生成
						CEnemyAmbush::Create
						(
							pos,			// 位置
							rot,			// 向き
							fChaseWidth,	// 追跡幅
							fChaseDepth		// 追跡奥行
						);

						break;

					default:

						// 停止
						assert(false);

						break;
					}
				}
			} while (str != "END_STAGE_ENEMYSET");	// END_STAGE_ENEMYSETを読み込むまでループ
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}

//============================================================
// 移動処理
//============================================================
void CEnemyAttack::Move(D3DXVECTOR3* pPos, const D3DXVECTOR3& rRot, const float fSpeed, const float fDeltaTime)
{
	D3DXVECTOR3 move = GetMovePosition();		// 移動量

	// 移動量を設定する
	move.x = sinf(rRot.y) * fSpeed * fDeltaTime;
	move.z = cosf(rRot.y) * fSpeed * fDeltaTime;

	// 位置を移動する
	*pPos += move;

	// 情報を適用
	SetMovePosition(move);
}

//============================================================
// 向きの移動処理
//============================================================
void CEnemyAttack::RotMove(D3DXVECTOR3& rRot, const float fRevRota, const float fDeltaTime)
{
	D3DXVECTOR3 destRot = GetDestRotation();	// 目標向き
	float fDiffRot = 0.0f;	// 差分向き

	// 目標向きまでの差分を計算
	fDiffRot = destRot.y - rRot.y;
	useful::NormalizeRot(fDiffRot);	// 差分向きの正規化

	// 向きの更新
	rRot.y += fDiffRot * fDeltaTime * fRevRota;
	useful::NormalizeRot(rRot.y);	// 向きの正規化
}

//============================================================
//	目標位置の視認処理
//============================================================
void CEnemyAttack::LookTarget(const D3DXVECTOR3& rPos)
{
	D3DXVECTOR3 destRot = GetDestRotation();	// 目標向き

	// 目標向きを求める
	destRot.y = atan2f(rPos.x - GetTargetPos().x, rPos.z - GetTargetPos().z);

	SetDestRotation(destRot);	// 目標向きを反映
}

//============================================================
// 接近処理
//============================================================
bool CEnemyAttack::Approach(const D3DXVECTOR3& rPos, const float fDis)
{
	float fDistance = 0.0f;					// 距離

	// 距離を測る
	fDistance = sqrtf((rPos.x - m_posTarget.x) * (rPos.x - m_posTarget.x) + (rPos.z - m_posTarget.z) * (rPos.z - m_posTarget.z));

	if (fDistance <= fDis)
	{ // 一定の距離に入った場合

		// 接近した
		return true;
	}

	// 接近してない
	return false;
}

//============================================================
// プレイヤーの探索処理
//============================================================
bool CEnemyAttack::JudgePlayer(void)
{
	// プレイヤーが見つからなかった場合 false を返す
	if (!SearchPlayer(&m_posTarget)) { return false; }

	// 標的をプレイヤーにする
	m_target = TARGET_PLAYER;

	// true を返す
	return true;
}

//============================================================
// 分身の探索処理
//============================================================
bool CEnemyAttack::JudgeClone(void)
{
	// 分身が見つからなかった場合 false を返す
	if (!SearchClone(&m_posTarget, &m_pClone)) { return false; }

	// 標的を分身にする
	m_target = TARGET_CLONE;

	// true を返す
	return true;
}

//============================================================
// プレイヤーの振り切り処理
//============================================================
bool CEnemyAttack::ShakeOffPlayer(void)
{
	// 位置を取得する
	m_posTarget = CScene::GetPlayer()->GetVec3Position();

	if (collision::Circle2D(GetVec3Position(), m_posTarget, GetRadius(), SHAKEOFF_RANGE) == true)
	{ // 範囲内に入っている場合

		// プレイヤーを標的にする
		m_target = TARGET_PLAYER;

		// true を返す
		return true;
	}

	// false を返す
	return false;
}

//============================================================
// 分身の振り切り処理
//============================================================
bool CEnemyAttack::ShakeOffClone(void)
{
	D3DXVECTOR3 pos = VEC3_ZERO;					// 位置
	D3DXVECTOR3 posEnemy = GetVec3Position();		// 敵の位置

	if (CPlayerClone::GetList() == nullptr ||
		*CPlayerClone::GetList()->GetBegin() == nullptr)
	{ // 分身のリストが無い場合

		// falseを返す
		return false;
	}

	for (int nCnt = 0; nCnt < CPlayerClone::GetList()->GetNumAll(); nCnt++)
	{
		// 分身の位置を取得する
		pos = (*CPlayerClone::GetList()->GetIndex(nCnt))->GetVec3Position();

		if (!collision::Circle2D(GetVec3Position(), pos, GetRadius(), SHAKEOFF_RANGE)) { continue; }

		// 位置を設定する
		m_posTarget = (*CPlayerClone::GetList()->GetIndex(nCnt))->GetVec3Position();

		// 分身の情報を設定する
		m_pClone = *CPlayerClone::GetList()->GetIndex(nCnt);

		// 分身を標的にする
		m_target = TARGET_CLONE;

		// true を返す
		return true;
	}

	// false を返す
	return false;
}

//====================================================================================================================================================================================
// TODO：ここから下はう〇ちカス判定だから後で修正
//====================================================================================================================================================================================

//============================================================
// プレイヤーのヒット処理
//============================================================
bool CEnemyAttack::HitPlayer(const D3DXVECTOR3& rPos)
{
	// 回避受付フラグが false の場合、抜ける
	if (m_bDodge == false) { return false; }

	// ヒット処理
	D3DXVECTOR3 posPlayer = CScene::GetPlayer()->GetVec3Position();
	D3DXVECTOR3 sizeUpPlayer =				// プレイヤーの判定(右・上・後)
	{
		CScene::GetPlayer()->GetRadius(),
		CScene::GetPlayer()->GetHeight(),
		CScene::GetPlayer()->GetRadius()
	};
	D3DXVECTOR3 sizeDownPlayer =			// プレイヤーの判定(左・下・前)
	{
		CScene::GetPlayer()->GetRadius(),
		0.0f,
		CScene::GetPlayer()->GetRadius()
	};

	// 回避カウントを加算する
	m_nAttackCount++;

	if (m_nAttackCount >= DODGE_COUNT)
	{ // 回避カウントを過ぎた場合

		bool bHit = false;	// ヒット状況

		// ボックスの当たり判定
		if (collision::Box3D
		(
			rPos,				// 判定位置
			posPlayer,			// 判定目標位置
			ATTACK_COLLUP,		// 判定サイズ(右・上・後)
			ATTACK_COLLDOWN,	// 判定サイズ(左・下・前)
			sizeUpPlayer,		// 判定目標サイズ(右・上・後)
			sizeDownPlayer		// 判定目標サイズ(左・下・前)
		))
		{ // 当たってなかった場合

			// 回避カウントを初期化する
			m_nAttackCount = 0;

			// 自身とプレイヤーを結ぶベクトルを算出
			D3DXVECTOR3 vec = posPlayer - rPos;

			// ヒット処理
			CScene::GetPlayer()->HitKnockBack(500, vec);

			// ヒット状況を true にする
			bHit = true;
		}

		// 回避受付フラグを false にする
		m_bDodge = false;

		// ヒット状況 を返す
		return bHit;
	}

	// false を返す
	return false;
}

//============================================================
// 分身のヒット処理
//============================================================
bool CEnemyAttack::HitClone(const D3DXVECTOR3& rPos)
{
	// 分身の情報が存在しない場合抜ける
	if (CPlayerClone::GetList() == nullptr ||
		*CPlayerClone::GetList()->GetBegin() == nullptr ||
		m_pClone == nullptr)
	{
		return false;
	}

	CPlayerClone* pClone = nullptr;	// 分身の情報

	for (auto& rClone : CPlayerClone::GetList()->GetList())
	{
		// 分身の情報じゃ無かった場合次へ
		if (m_pClone != rClone) { continue; }

		// 分身の情報を設定
		pClone = rClone;

		// for文を抜ける
		break;
	}

	// 分身が NULL の場合抜ける
	if (pClone == nullptr) { return false; }

	// ヒット処理
	D3DXVECTOR3 sizeUpClone =
	{
		pClone->GetRadius(),
		pClone->GetHeight(),
		pClone->GetRadius()
	};
	D3DXVECTOR3 sizeDownClone =
	{
		pClone->GetRadius(),
		0.0f,
		pClone->GetRadius()
	};

	// ボックスの当たり判定
	if (!collision::Box3D
	(
		rPos,						// 判定位置
		pClone->GetVec3Position(),	// 判定目標位置
		ATTACK_COLLUP,				// 判定サイズ(右・上・後)
		ATTACK_COLLDOWN,			// 判定サイズ(左・下・前)
		sizeUpClone,				// 判定目標サイズ(右・上・後)
		sizeDownClone				// 判定目標サイズ(左・下・前)
	))
	{ // 当たってなかったら抜ける

		return false;
	}

	// ヒット処理
	pClone->Hit(20);

	// 分身との戦闘エフェクトを出す
	GET_EFFECT->Create("data\\EFFEKSEER\\diversion.efkefc", GetVec3Position(), GetVec3Rotation(), VEC3_ZERO, DIVERSION_EFFECT_SCALE);

	// 回避受付フラグを false にする
	m_bDodge = false;

	// true を返す
	return true;
}

//===========================================
//  当たり判定の取得
//===========================================
D3DXVECTOR3 CEnemyAttack::GetAttackUp()
{
	return ATTACK_COLLUP;
}

//===========================================
//  当たり判定の取得
//===========================================
D3DXVECTOR3 CEnemyAttack::GetAttackDown()
{
	return ATTACK_COLLDOWN;
}

//===========================================
// 元の位置に戻る処理
//===========================================
bool CEnemyAttack::BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight)
{
	if (CManager::GetInstance()->GetCamera()->OnScreen(*pPos) ||
		CManager::GetInstance()->GetCamera()->OnScreen(D3DXVECTOR3(pPos->x, pPos->y + fHeight, pPos->z)) ||
		CManager::GetInstance()->GetCamera()->OnScreen(GetPosInit()))
	{ // 画面内にいる場合

		// 回帰カウントをリセットする
		m_nRegressionCount = 0;

		// false を返す
		return false;
	}

	// 回帰カウントを加算する
	m_nRegressionCount++;

	// 回帰カウントが一定数以下の場合、抜ける
	if (m_nRegressionCount < REGRESSION_COUNT) { return false; }

	// 位置を設定する
	*pPos = GetPosInit();

	// 向きを設定する
	*pRot = GetRotInit();

	// 過去の位置を適用する(こうしないと当たり判定に引っかかってしまう)
	SetOldPosition(*pPos);

	// 目的の向きを設定する(復活後に無意味に向いてしまうため)
	SetDestRotation(*pRot);

	// 透明度を1.0fにする
	SetAlpha(1.0f);

	// 移動量をリセットする
	SetMovePosition(VEC3_ZERO);

	// ターゲットを無対象にする
	SetTarget(TARGET_NONE);

	// true を返す
	return true;
}
