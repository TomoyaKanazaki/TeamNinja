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
#include "actor.h"

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

	const int WARNING_COUNT[CEnemyAttack::TYPE_MAX] =	// 警告状態の遷移カウント
	{
		59,			// しつこい敵
		36,			// 狼敵
		59,			// 待ち伏せ敵
	};
	const int ATTACK_COUNT[CEnemyAttack::TYPE_MAX] =	// 攻撃状態の遷移カウント
	{
		44,			// しつこい敵
		34,			// 狼敵
		44,			// 待ち伏せ敵
	};
	const int BLANKATTACK_COUNT[CEnemyAttack::TYPE_MAX] =		// 空白攻撃状態の遷移カウント
	{
		340,		// しつこい敵
		340,		// 狼敵
		340,		// 待ち伏せ敵
	};
	const int BLANKATTACK_CYCLE_COUNT[CEnemyAttack::TYPE_MAX] =		// 空白攻撃状態の回転カウント
	{
		18,		// しつこい敵
		18,		// 狼敵
		18,		// 待ち伏せ敵
	};
}

// 音関係定数
namespace sound
{
	const int WARNING_COUNT[CEnemyAttack::TYPE_MAX] =		// 警告音の鳴るカウント
	{
		37,			// しつこい敵
		2,			// 狼敵
		37,			// 待ち伏せ敵
	};
	const CSound::ELabel WARNING_LABEL[CEnemyAttack::TYPE_MAX] =	// 警告音のラベル
	{
		CSound::LABEL_SE_STALKFOUND_000,	// しつこい敵
		CSound::LABEL_SE_WOLFFOUND_000,		// 狼敵
		CSound::LABEL_SE_STALKFOUND_000,	// 待ち伏せ敵
	};
	const CSound::ELabel ATTACK_LABEL[CEnemyAttack::TYPE_MAX] =		// 攻撃音のラベル
	{
		CSound::LABEL_SE_STALKATTACK_000,	// しつこい敵
		CSound::LABEL_SE_WOLFATTACK_000,	// 狼敵
		CSound::LABEL_SE_STALKATTACK_000,	// 待ち伏せ敵
	};
	const CSound::ELabel BLANK_ATTACK_LABEL[CEnemyAttack::TYPE_MAX] =	// 空白攻撃音のラベル
	{
		CSound::LABEL_SE_STALKATTACK_001,	// しつこい敵
		CSound::LABEL_SE_WOLFATTACK_001,	// 狼敵
		CSound::LABEL_SE_STALKATTACK_001,	// 待ち伏せ敵
	};
	const int UPSET_COUNT[CEnemyAttack::TYPE_MAX] =			// 動揺音の鳴るカウント
	{
		200,		// しつこい敵
		80,			// 狼敵
		200,		// 待ち伏せ敵
	};
	const CSound::ELabel UPSET_LABEL[CEnemyAttack::TYPE_MAX] =		// 動揺音のラベル
	{
		CSound::LABEL_SE_STALKUPSET_000,	// しつこい敵
		CSound::LABEL_SE_WOLFUPSET_000,		// 狼敵
		CSound::LABEL_SE_STALKUPSET_000,	// 待ち伏せ敵
	};
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
m_state(STATE_ORIGIN),		// 状態
m_nStateCount(0),			// 状態カウント
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

	// 消去処理
	m_actor.clear();

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
	// アクターの登録処理
	ActorRegist();
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
// プレイヤー進入処理
//============================================================
bool CEnemyAttack::PlayerIngress(void)
{
	D3DXVECTOR3 pos = VEC3_ZERO;				// 位置

	// プレイヤー情報の取得
	CPlayer* pPlayer = GET_PLAYER;

	// プレイヤーがいない場合、抜ける
	if (pPlayer == nullptr) { return false; }

	// 位置を取得する
	pos = pPlayer->GetVec3Position();

	// 範囲内にいなかった場合、false を返す
	if (!m_pChaseRange->InsideTargetPos(GetPosInit(), pos)) { return false; }

	// 目的をプレイヤーにする
	m_target = TARGET_PLAYER;

	// 目的の位置を設定する
	m_posTarget = pos;

	// true を返す
	return true;
}

//============================================================
// 分身進入処理
//============================================================
bool CEnemyAttack::CloneIngress(void)
{
	D3DXVECTOR3 pos = VEC3_ZERO;					// 位置

	if (CPlayerClone::GetList() == nullptr ||
		*CPlayerClone::GetList()->GetBegin() == nullptr)
	{ // 分身のリストが無い場合

		// falseを返す
		return false;
	}

	for (auto& clone : CPlayerClone::GetList()->GetList())
	{
		// 歩行状態じゃない場合、次に進む
		if (clone->GetAction() != CPlayerClone::ACTION_MOVE) { continue; }

		// 分身の位置を取得する
		pos = clone->GetVec3Position();

		// 範囲内進入状況を返す
		if (!m_pChaseRange->InsideTargetPos(GetPosInit(), pos)) { continue; }

		// 分身の情報を設定する
		m_pClone = clone;

		// 目的を分身にする
		m_target = TARGET_CLONE;

		// 目的の位置を設定する
		m_posTarget = pos;

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
	float fRadius = CScene::GetPlayer()->GetRadius();
	D3DXVECTOR3 sizeUpPlayer =				// プレイヤーの判定(右・上・後)
	{
		fRadius,
		CScene::GetPlayer()->GetHeight(),
		fRadius
	};
	D3DXVECTOR3 sizeDownPlayer =			// プレイヤーの判定(左・下・前)
	{
		fRadius,
		0.0f,
		fRadius
	};

	// 回避カウントを加算する
	m_nAttackCount++;

	if (m_nAttackCount > DODGE_COUNT)
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
	if (pClone == nullptr || pClone->GetAction() != CPlayerClone::ACTION_MOVE) { return false; }

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
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	if (pCamera->OnScreen(*pPos) ||
		pCamera->OnScreen(D3DXVECTOR3(pPos->x, pPos->y + fHeight, pPos->z)) ||
		pCamera->OnScreen(GetPosInit()))
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

//===========================================
// アクターの当たり判定処理
//===========================================
void CEnemyAttack::CollisionActor(D3DXVECTOR3& rPos, bool& bHit)
{
	// アクターのリスト構造が無ければ抜ける
	if (m_actor.empty()) { return; }

	D3DXVECTOR3 move = GetMovePosition();
	bool bJump = IsJump();

	for (auto& actor : m_actor)
	{
		// 当たり判定処理
		actor->Collision
		(
			rPos,				// 位置
			GetOldPosition(),	// 前回の位置
			GetRadius(),		// 半径
			GetHeight(),		// 高さ
			move,				// 移動量
			bJump,				// ジャンプ状況
			bHit				// ヒット状況
		);
	}

	// 移動量とジャンプ状況を反映
	SetMovePosition(move);
	SetEnableJump(bJump);
}

//===========================================
// 警告処理
//===========================================
int CEnemyAttack::Warning
(
	D3DXVECTOR3* pPos,		// 位置
	D3DXVECTOR3* pRot,		// 向き
	const float fDeltaTime,	// デルタタイム
	const float fRotRev		// 向きの補正数
)
{
	switch (m_target)
	{
	case CEnemyAttack::TARGET_PLAYER:

		// プレイヤーの探索処理
		JudgePlayer();

		break;

	case CEnemyAttack::TARGET_CLONE:

		// クローンの探索処理
		JudgeClone();

		break;

	default:
		// 停止
		assert(false);
		break;
	}

	// 状態カウントを加算する
	m_nStateCount++;

	if (m_nStateCount == sound::WARNING_COUNT[m_type])
	{ // 一定時間経過した場合

		// 発見音を鳴らす
		PLAY_SOUND(sound::WARNING_LABEL[m_type]);
	}

	// 目標位置の視認処理
	LookTarget(*pPos);

	// 向きの移動処理
	RotMove(*pRot, fRotRev, fDeltaTime);

	if (m_nStateCount % WARNING_COUNT[m_type] == 0)
	{ // 一定時間経過した場合

		// 追跡状態にする
		SetState(STATE_STALK);
	}

	// 状態を返す(意味なし)
	return m_state;
}

//===========================================
// 追跡処理
//===========================================
int CEnemyAttack::Stalk
(
	D3DXVECTOR3* pPos,		// 位置
	D3DXVECTOR3* pRot, 		// 向き
	const float fDeltaTime,	// デルタタイム
	const float fRotRev		// 向きの補正数
)
{
	// 歩行カウントを加算する
	m_nStateCount++;

	if (!CloneIngress() &&
		!PlayerIngress())
	{ // 分身もプレイヤーも範囲内にいない場合

		// 独自状態にする
		SetState(STATE_ORIGIN);

		// 標的を未設定にする
		SetTarget(TARGET_NONE);

		// 独自状態を返す
		return m_state;
	}

	// 目標位置の視認処理
	LookTarget(*pPos);

	// 回避受付フラグを false にする
	SetEnableDodge(false);

	// 攻撃カウントをリセットする
	SetAttackCount(0);

	// 向きの移動処理
	RotMove(*pRot, fRotRev, fDeltaTime);

	// 移動処理
	Move(pPos, *pRot, GetSpeed(), fDeltaTime);

	if (Approach(*pPos))
	{ // 接近した場合

		if (GetTarget() == CEnemyAttack::TARGET_PLAYER)
		{ // 目標がプレイヤーの場合

			// 回避受付フラグを true にする
			SetEnableDodge(true);

			// 攻撃カウントをリセットする
			SetAttackCount(0);
		}

		// 攻撃状態にする
		SetState(STATE_ATTACK);

		// 攻撃状態を返す
		return m_state;
	}

	if (GetChaseRange() != nullptr &&
		GetChaseRange()->ChaseRange(GetPosInit(), pPos))
	{ // 追跡範囲から出た場合

		// 構え状態にする
		SetState(STATE_STANCE);

		// 構え状態を返す
		return m_state;
	}

	// 追跡状態を返す
	return m_state;
}

//===========================================
// 攻撃処理
//===========================================
int CEnemyAttack::Attack(const D3DXVECTOR3& rPos)
{
	switch (GetTarget())
	{
	case CEnemyAttack::TARGET_PLAYER:

		if (HitPlayer(rPos))
		{ // プレイヤーに当たった場合

			// 攻撃音を鳴らす
			PLAY_SOUND(sound::ATTACK_LABEL[m_type]);
		}

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount % ATTACK_COUNT[m_type] == 0)
		{ // 一定カウント経過した場合

			// 独自状態にする
			SetState(STATE_ORIGIN);
		}

		// 独自状態を返す
		return m_state;

	case CEnemyAttack::TARGET_CLONE:

		if (HitClone(rPos))
		{ // 分身に当たった場合

			// 空白攻撃状態にする
			SetState(STATE_BLANKATTACK);

			// 分身攻撃音を鳴らす
			PLAY_SOUND(sound::BLANK_ATTACK_LABEL[m_type]);

			// 空白攻撃状態を返す
			return m_state;
		}
		else
		{ // 上記以外

			// 独自状態にする
			SetState(STATE_ORIGIN);
		}

		// 独自状態を返す
		return m_state;

	default:		// 例外処理
		assert(false);
		break;
	}

	// 独自状態を返す
	return m_state;
}

//===========================================
// 空白攻撃処理
//===========================================
int CEnemyAttack::BlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime, const float fRotRev)
{
	// 状態カウントを加算する
	m_nStateCount++;

	// 向きの移動処理
	RotMove(*pRot, fRotRev, fDeltaTime);

	if (m_nStateCount <= BLANKATTACK_COUNT[m_type])
	{ // 一定カウント以下の場合

		if (m_nStateCount % BLANKATTACK_CYCLE_COUNT[m_type] == 0)
		{ // 一定カウントごとに

			// 目的の向きを取得
			D3DXVECTOR3 rotDest = GetDestRotation();

			// 目的の向きを設定する
			rotDest.y = useful::RandomRot();

			// 目的の向きを適用
			SetDestRotation(rotDest);
		}
	}
	else
	{ // 上記以外

		// 動揺状態にする
		SetState(STATE_UPSET);

		// 動揺状態を返す
		return m_state;
	}

	// 空白攻撃状態にする
	return m_state;
}

//===========================================
// 動揺処理
//===========================================
int CEnemyAttack::Upset(void)
{
	// 状態カウントを加算する
	m_nStateCount++;

	if (m_nStateCount == sound::UPSET_COUNT[m_type])
	{ // 状態カウントが一定数になったとき

		// 動揺音を鳴らす
		PLAY_SOUND(sound::UPSET_LABEL[m_type]);
	}

	// 動揺状態を返す
	return m_state;
}

//===========================================
// 構え処理
//===========================================
int CEnemyAttack::Stance(void)
{
	// 分身の発見処理
	JudgeClone();

	if (PlayerIngress())
	{ // 範囲内に入った場合

		// 警告状態にする
		SetState(STATE_WARNING);

		// 警告モーションを返す
		return m_state;
	}

	// 構え状態を返す
	return m_state;
}

//===========================================
// 状態の設定処理
//===========================================
void CEnemyAttack::SetState(const EState state)
{
	// 状態を設定する
	m_state = state;

	// 状態カウントを0にする
	m_nStateCount = 0;
}

//===========================================
// アクターの登録処理
//===========================================
void CEnemyAttack::ActorRegist(void)
{
	// アクターのリスト構造が無ければ抜ける
	if (CActor::GetList() == nullptr) { return; }

	// リストを取得
	std::list<CActor*> list = CActor::GetList()->GetList();
	D3DXVECTOR3 pos = GetPosInit();
	D3DXVECTOR3 vtxChase = D3DXVECTOR3(m_pChaseRange->GetWidth(), 0.0f, m_pChaseRange->GetDepth());

	for (auto actor : list)
	{
		assert(actor != nullptr);

		// モデルが追跡範囲内にない場合、次に進む
		if (!collision::Box2D(pos, actor->GetVec3Position(), vtxChase, vtxChase, actor->GetModelData().vtxMax, -actor->GetModelData().vtxMin)) { continue; }

		// アクターを登録する
		m_actor.push_back(actor);
	}
}
