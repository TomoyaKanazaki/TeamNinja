//============================================================
//
//	ステージ処理 [stage.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "stage.h"
#include "manager.h"
#include "texture.h"
#include "collision.h"
#include "retentionManager.h"

#include "field.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "liquid.h"
#include "camera_change.h"
#include "enemyAttack.h"
#include "coin.h"
#include "godItem.h"
#include "actor.h"
#include "gimmick.h"
#include "touchActor.h"
#include "player.h"
#include "checkpoint.h"
#include "goal.h"
#include "transpoint.h"
#include "weed.h"
#include "tutorial.h"
#include "fog_change.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* INIT_MAPPASS = "data\\MAP\\LOGO\\map.txt";	// 初期マップパス
}

//************************************************************
//	定数宣言
//************************************************************
//	親クラス [CStage] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CStage::CStage()
{
	// メンバ変数をクリア
	memset(&m_limit, 0, sizeof(m_limit));	// 範囲情報

	// マップパス連想配列をクリア
	m_mapPass.clear();
}

//============================================================
//	デストラクタ
//============================================================
CStage::~CStage()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CStage::Init(void)
{
	// メンバ変数を初期化
	memset(&m_limit, 0, sizeof(m_limit));	// 範囲情報
	m_sInitMapPass = INIT_MAPPASS;			// 初期化時に生成するマップパス

	// マップパス連想配列を初期化
	m_mapPass.clear();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CStage::Uninit(void)
{
	// マップパス連想配列をクリア
	m_mapPass.clear();
}

//============================================================
//	マップパスの登録処理
//============================================================
CStage::SPass CStage::Regist(const char* pMapPass)
{
	// 既に生成済みかを検索
	auto itr = m_mapPass.find(pMapPass);	// 引数のフォントを検索
	if (itr != m_mapPass.end())
	{ // 生成済みの場合

		// 読込済みのフォント情報を返す
		return itr->second;
	}

	// マップパス情報を読込
	SPass tempPass;	// マップパス情報
	if (FAILED(LoadPass(pMapPass, &tempPass)))
	{ // 読込に失敗した場合

		// 初期値を返す
		assert(false);
		return {};
	}

	// フォント情報を保存
	m_mapPass.insert(std::make_pair(pMapPass, tempPass));

	// 生成したマップパス情報を返す
	return tempPass;
}

//============================================================
//	ステージの割当処理 (パス)
//============================================================
HRESULT CStage::BindStage(const char* pPass)
{
	// ステージパスがない場合抜ける
	if (pPass == nullptr) { return S_OK; }

	// ステージの割当
	return BindStage(Regist(pPass));
}

//============================================================
//	ステージの割当処理 (情報)
//============================================================
HRESULT CStage::BindStage(const SPass& rPass)
{
	// 割り当てるマップのディレクトリを保存
	std::filesystem::path path = rPass.sMap;
	m_sCurMapFolder = path.parent_path().string();

	// ステージのセットアップの読込
	if (FAILED(LoadSetup(rPass.sStage.c_str())))
	{ // 読み込みに失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// アクターのセットアップの読込
	if (!rPass.sActor.empty())		// パスが指定されている場合
	if (FAILED(CActor::LoadSetup(rPass.sActor.c_str())))
	{ // セットアップに失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ギミックのセットアップの読込
	if (!rPass.sGimmick.empty())	// パスが指定されている場合
	if (FAILED(CGimmick::LoadSetup(rPass.sGimmick.c_str())))
	{ // セットアップに失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// タッチアクターのセットアップの読込
	if(!rPass.sTouchActor.empty())	// パスが指定されている場合
	if (FAILED(CTouchActor::LoadSetup(rPass.sTouchActor.c_str())))
	{ // セットアップに失敗した場合

		//失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 敵のセットアップの読込
	if (!rPass.sEnemyAtc.empty())	// パスが指定されている場合
	if (FAILED(CEnemyAttack::LoadSetup(rPass.sEnemyAtc.c_str())))
	{ // セットアップに失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// コインのセットアップの読込
	if(!rPass.sCoin.empty())	// パスが指定されている場合
	if (FAILED(CCoin::LoadSetup(rPass.sCoin.c_str())))
	{ // セットアップに失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 神器のセットアップの読込
	if (!rPass.sGodItem.empty())	// パスが指定されている場合
	if (FAILED(CGodItem::LoadSetup(rPass.sGodItem.c_str())))
	{ // セットアップに失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (!rPass.sPoint.empty())
	{ // パスが指定されている場合

		// チェックポイントのセットアップの読込
		if (FAILED(CCheckPoint::LoadSetup(rPass.sPoint.c_str())))
		{ // セットアップに失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// ゴールポイントのセットアップの読込
		if (FAILED(CGoal::LoadSetup(rPass.sPoint.c_str())))
		{ // セットアップに失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 遷移ポイントのセットアップの読込
		if (FAILED(CTransPoint::LoadSetup(rPass.sPoint.c_str())))
		{ // セットアップに失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// プレイヤーのセットアップの読込
		if (FAILED(CPlayer::LoadSetup(rPass.sPoint.c_str())))
		{ // セットアップに失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// チュートリアルの生成
		if (rPass.sMap.find("FOREST00") != std::string::npos)
		{
			CTutorial::Create(D3DXVECTOR3(-400.0f, 0.0f, -300.0f), CTutorial::TYPE_MOVE);
			CTutorial::Create(D3DXVECTOR3(2500.0f, 0.0f, -300.0f), CTutorial::TYPE_CLONE);
			CTutorial::Create(D3DXVECTOR3(4000.0f, 0.0f, -300.0f), CTutorial::TYPE_GIMMICK);
			CTutorial::Create(D3DXVECTOR3(12300.0f, 650.0f, -150.0f), CTutorial::TYPE_GUARD);
			//CTutorial::Create(D3DXVECTOR3(12900.0f, 650.0f, -150.0f), CTutorial::TYPE_DOGDE);
		}

		// フォグ変更点の生成
		if (rPass.sMap.find("FOREST02") != std::string::npos)
		{
			CFogChanger::Create();
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	現在マップのステージパス取得処理
//============================================================
std::string CStage::GetCurMapStagePass(void) const
{
	// マップディレクトリにステージパスを追加し返す
	std::filesystem::path path = m_sCurMapFolder;	// マップディレクトリ
	path.append("stage.txt");	// ステージパスを追加
	return path.string();		// 文字列化して返す
}

//============================================================
//	現在マップの神器パス取得処理
//============================================================
std::string CStage::GetCurMapGodItemPass(void) const
{
	// マップディレクトリに神器パスを追加し返す
	std::filesystem::path path = m_sCurMapFolder;	// マップディレクトリ
	path.append("goditem.txt");	// 神器パスを追加
	return path.string();		// 文字列化して返す
}

//============================================================
//	現在マップの神器保存パス取得処理
//============================================================
std::string CStage::GetCurMapSaveGodItemPass(void) const
{
	// マップディレクトリに神器保存パスを追加し返す
	std::filesystem::path path = m_sCurMapFolder;	// マップディレクトリ
	path.append("goditem.bin");	// 神器保存パスを追加
	return path.string();		// 文字列化して返す
}

//============================================================
//	現在マップのランキング保存パス取得処理
//============================================================
std::string CStage::GetCurMapSaveRankPass(void) const
{
	// マップディレクトリにランキング保存パスを追加し返す
	std::filesystem::path path = m_sCurMapFolder;	// マップディレクトリ
	path.append("ranking.bin");	// ランキング保存パスを追加
	return path.string();		// 文字列化して返す
}

//============================================================
//	範囲外の着地処理
//============================================================
bool CStage::LandLimitPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight)
{
	if (rPos.y - fHeight < m_limit.fField)
	{ // 位置が地面より下の場合

		// 位置を補正
		rPos.y = m_limit.fField + fHeight;

		// 移動量を初期化
		rMove.y = 0.0f;

		// 着地している状況を返す
		return true;
	}

	// 着地していない状況を返す
	return false;
}

//============================================================
//	地面の着地処理
//============================================================
bool CStage::LandFieldPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rOldPos, D3DXVECTOR3& rMove, CField** ppCollField)
{
	CListManager<CField> *pListManager = CField::GetList();	// フィールドリストマネージャー
	if (pListManager == nullptr) { return false; }			// リスト未使用の場合抜ける
	std::list<CField*> listField = pListManager->GetList();	// フィールドリスト情報
	CField *pCurField = nullptr;	// 着地予定の地面
	float fCurPos = m_limit.fField;	// 着地予定のY座標
	bool bLand = false;				// 地面の着地判定

	for (auto& rList : listField)
	{ // 地面の総数分繰り返す

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // 地面の範囲内の場合

			float fPosHeight = rList->GetPositionHeight(rPos);	// 着地Y座標
			if (fPosHeight <= rOldPos.y && fPosHeight > rPos.y)
			{
				if (fCurPos <= fPosHeight)
				{ // 現在の着地予定Y座標より高い位置にある場合

					// 着地予定の地面を更新
					pCurField = rList;

					// 着地予定のY座標を更新
					fCurPos = fPosHeight;
				}
			}
		}
	}

	if (pCurField == nullptr)
	{ // 着地予定の地面が存在しない場合

		if (ppCollField != nullptr)
		{ // 地面の保存アドレスがある場合

			// 地面無しを保存
			*ppCollField = nullptr;
		}
	}
	else
	{ // 着地予定の地面が存在する場合

		// メッシュの着地状況を保存
		bLand = pCurField->LandPosition(rPos, rMove);
		if (ppCollField != nullptr)
		{ // 地面の保存アドレスがある場合

			if (bLand)
			{ // 着地している場合

				// 着地した地面を保存
				*ppCollField = pCurField;
			}
		}
	}

	// 着地判定を返す
	return bLand;
}

//============================================================
//	一番上の地面着地処理
//============================================================
bool CStage::LandFieldPositionTop(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, CField** ppCollField)
{
	CListManager<CField> *pListManager = CField::GetList();	// フィールドリストマネージャー
	if (pListManager == nullptr) { return false; }			// リスト未使用の場合抜ける
	std::list<CField*> listField = pListManager->GetList();	// フィールドリスト情報
	CField *pCurField = nullptr;	// 着地予定の地面
	float fCurPos = m_limit.fField;	// 着地予定のY座標
	bool bLand = false;				// 地面の着地判定

	for (auto& rList : listField)
	{ // 地面の総数分繰り返す

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // 地面の範囲内の場合

			float fPosHeight = rList->GetPositionHeight(rPos);	// 着地Y座標
			if (fCurPos <= fPosHeight)
			{ // 現在の着地予定Y座標より高い位置にある場合

				// 着地予定の地面を更新
				pCurField = rList;

				// 着地予定のY座標を更新
				fCurPos = fPosHeight;
			}
		}
	}

	if (pCurField == nullptr)
	{ // 着地予定の地面が存在しない場合抜ける

		if (ppCollField != nullptr)
		{ // 地面の保存アドレスがある場合

			// 地面無しを保存
			*ppCollField = pCurField;
		}
	}
	else
	{ // 着地予定の地面が存在しない場合抜ける

		// メッシュの着地状況を保存
		bLand = pCurField->LandPosition(rPos, rMove);
		if (ppCollField != nullptr)
		{ // 地面の保存アドレスがある場合

			if (bLand)
			{ // 着地している場合

				// 着地した地面を保存
				*ppCollField = pCurField;
			}
		}
	}

	// 着地判定を返す
	return bLand;
}

//============================================================
//	壁との当たり判定
//============================================================
bool CStage::CollisionWall(D3DXVECTOR3& rPos, D3DXVECTOR3& rPosOld, const float fRadius, const float fHeight, D3DXVECTOR3& rMove, bool* pJump)
{
	CListManager<CWall>* pListManager = CWall::GetList();	// フィールドリストマネージャー
	if (pListManager == nullptr) { return false; }			// リスト未使用の場合抜ける
	std::list<CWall*> listWall = pListManager->GetList();	// フィールドリスト情報
	bool bHit = false;			// ヒット状況

	for (auto& rList : listWall)
	{ // 地面の総数分繰り返す

		assert(rList != nullptr);

		// 当たり判定処理
		if (!rList->Collision(rPos, rPosOld, fRadius, fHeight, rMove, pJump)) { continue; }

		// ヒット状況を true にする
		bHit = true;
	}

	// ヒット状況を返す
	return bHit;
}

//============================================================
//	位置の補正処理
//============================================================
void CStage::LimitPosition(D3DXVECTOR3& rPos, const float fRadius)
{
	switch (m_limit.mode)
	{ // 制限モードごとの処理
	case LIMIT_NONE:	// 制限なし

		// 制限がない場合関数を呼び出す必要がない
		//assert(false);

		break;

	case LIMIT_BOX:		// 矩形範囲

		// 角柱の内側制限
		collision::InBoxPillar
		( // 引数
			rPos,				// 判定位置
			VEC3_ZERO,			// 判定原点位置
			VEC3_ALL(fRadius),	// 判定サイズ(右・上・後)
			VEC3_ALL(fRadius),	// 判定サイズ(左・下・前)
			D3DXVECTOR3(fabsf(m_limit.fRight), 0.0f, fabsf(m_limit.fFar)),	// 判定原点サイズ(右・上・後)
			D3DXVECTOR3(fabsf(m_limit.fLeft), 0.0f, fabsf(m_limit.fNear))	// 判定原点サイズ(左・下・前)
		);

		break;

	case LIMIT_CIRCLE:	// 円範囲

		// 円柱の内側制限
		collision::InCirclePillar
		( // 引数
			rPos,			// 判定位置
			VEC3_ZERO,		// 判定原点位置
			fRadius,		// 判定半径
			m_limit.fRadius	// 判定原点半径
		);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	キルY座標との当たり判定
//============================================================
bool CStage::CollisionKillY(const D3DXVECTOR3& rPos)
{
	if (rPos.y < m_limit.fKillY)
	{ // 位置が削除の制限位置より下の場合

		// 死亡している状況を返す
		return true;
	}

	// 死亡していない状況を返す
	return false;
}

//============================================================
//	地面の範囲内の取得処理
//============================================================
bool CStage::IsFieldPositionRange(const D3DXVECTOR3& rPos)
{
	CListManager<CField> *pListManager = CField::GetList();	// フィールドリストマネージャー
	if (pListManager == nullptr) { return false; }			// リスト未使用の場合抜ける
	std::list<CField*> listField = pListManager->GetList();	// フィールドリスト情報

	for (auto& rList : listField)
	{ // 地面の総数分繰り返す

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // 地面の範囲内の場合

			// 範囲内にいる状態を返す
			return true;
		}
	}

	// 範囲内にいない状態を返す
	return false;
}

//============================================================
//	地面の着地位置の取得処理
//============================================================
float CStage::GetFieldPositionHeight(const D3DXVECTOR3& rPos)
{
	CListManager<CField> *pListManager = CField::GetList();	// フィールドリストマネージャー
	if (pListManager == nullptr) { return false; }			// リスト未使用の場合抜ける
	std::list<CField*> listField = pListManager->GetList();	// フィールドリスト情報
	CField *pCurField = nullptr;	// 着地予定の地面
	float fCurPos = m_limit.fField;	// 着地予定のY座標

	for (auto& rList : listField)
	{ // 地面の総数分繰り返す

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // 地面の範囲内の場合

			float fPosHeight = rList->GetPositionHeight(rPos);	// 着地Y座標
			if (fCurPos <= fPosHeight)
			{ // 現在の着地予定Y座標より高い位置にある場合

				// 着地予定の地面を更新
				pCurField = rList;

				// 着地予定のY座標を更新
				fCurPos = fPosHeight;
			}
		}
	}

	if (pCurField != nullptr)
	{ // 着地予定の地面が存在する場合

		// 着地位置を返す
		return fCurPos;
	}
	else
	{ // 着地予定の地面が存在しない場合

		// 引数位置を返す
		return rPos.y;
	}
}

//============================================================
//	メッシュの着地位置の取得処理 (現在位置よりは下)
//============================================================
float CStage::GetFieldDownPositionHeight(const D3DXVECTOR3& rPos)
{
	CListManager<CField> *pListManager = CField::GetList();	// フィールドリストマネージャー
	if (pListManager == nullptr) { return false; }			// リスト未使用の場合抜ける
	std::list<CField*> listField = pListManager->GetList();	// フィールドリスト情報
	CField *pCurField = nullptr;	// 着地予定の地面
	float fCurPos = m_limit.fField;	// 着地予定のY座標

	for (auto& rList : listField)
	{ // 地面の総数分繰り返す

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // 地面の範囲内の場合

			float fPosHeight = rList->GetPositionHeight(rPos);	// 着地Y座標
			if (fCurPos <= fPosHeight && rPos.y >= fPosHeight)
			{ // 現在の着地予定Y座標より高い位置にある場合

				// 着地予定の地面を更新
				pCurField = rList;

				// 着地予定のY座標を更新
				fCurPos = fPosHeight;
			}
		}
	}

	if (pCurField != nullptr)
	{ // 着地予定の地面が存在する場合

		// 着地位置を返す
		return fCurPos;
	}
	else
	{ // 着地予定の地面が存在しない場合

		// 引数位置を返す
		return rPos.y;
	}
}

//============================================================
//	生成処理
//============================================================
CStage *CStage::Create(void)
{
	// ステージの生成
	CStage *pStage = new CStage;
	if (pStage == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ステージの初期化
		if (FAILED(pStage->Init()))
		{ // 初期化に失敗した場合

			// ステージの破棄
			SAFE_DELETE(pStage);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pStage;
	}
}

//============================================================
//	破棄処理
//============================================================
void CStage::Release(CStage *&prStage)
{
	// ステージの終了
	assert(prStage != nullptr);
	prStage->Uninit();

	// メモリ開放
	SAFE_DELETE(prStage);
}

//============================================================
//	パス情報の読込処理
//============================================================
HRESULT CStage::LoadPass(const char* pMapPass, SPass* pPassInfo)
{
	// ファイルを開く
	std::ifstream file(pMapPass);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "マップパスセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// マップパスを保存
	pPassInfo->sMap = pMapPass;

	// ファイルを読込
	std::string str;	// 読込文字列
	while (file >> str)
	{ // ファイルの終端ではない場合ループ

		if (str.front() == '#')
		{ // コメントアウトされている場合

			// 一行全て読み込む
			std::getline(file, str);
		}
		else if (str == "STAGE_PASS")
		{
			file >> str;	// ＝を読込
			file >> str;	// ステージ読込パスを読込

			// ステージ読込パスを保存
			pPassInfo->sStage = str;
		}
		else if (str == "ENEMYATTACK_PASS")
		{
			file >> str;	// ＝を読込
			file >> str;	// ステージ読込パスを読込

			// 敵情報パスを保存
			pPassInfo->sEnemyAtc = str;
		}
		else if (str == "COIN_PASS")
		{
			file >> str;	// ＝を読込
			file >> str;	// コイン読込パスを読込

			// コイン情報パスを保存
			pPassInfo->sCoin = str;
		}
		else if (str == "GODITEM_PASS")
		{
			file >> str;	// ＝を読込
			file >> str;	// 神器読込パスを読込

			// 神器情報パスを保存
			pPassInfo->sGodItem = str;
		}
		else if (str == "ACTOR_PASS")
		{
			file >> str;	// ＝を読込
			file >> str;	// アクター読込パスを読込

			// アクター読込パスを保存
			pPassInfo->sActor = str;
		}
		else if (str == "GIMMICK_PASS")
		{
			file >> str;	// ＝を読込
			file >> str;	// ギミック読込パスを読込

			// ギミック読込パスを保存
			pPassInfo->sGimmick = str;
		}
		else if (str == "TOUCH_ACTOR_PASS")
		{
			file >> str;	// ＝を読込
			file >> str;	// タッチアクター読込パスを読込

			// タッチアクター読込パスを保存
			pPassInfo->sTouchActor = str;
		}
		else if (str == "POINT_PASS")
		{
			file >> str;	// ＝を読込
			file >> str;	// ポイント読込パスを読込

			// ポイント読込パスを保存
			pPassInfo->sPoint = str;
		}
		else if (str == "WEED_PASS")
		{
			file >> str;	// ＝を読込
			file >> str;	// ポイント読込パスを読込

			// ポイント読込パスを保存
			pPassInfo->sPoint = str;
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}

//============================================================
//	セットアップ処理
//============================================================
HRESULT CStage::LoadSetup(const char* pPass)
{
	// 変数を宣言
	int nEnd = 0;	// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(pPass, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		while (1)
		{ // EOFまで無限ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// 終端の場合 EOFを返す
			if (nEnd == EOF)
			{ // ファイルが終端の場合

				// 処理を抜ける
				break;
			}

			// 解放マップディレクトリの読込
			if (FAILED(LoadOpen(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 範囲情報の読込
			if (FAILED(LoadLimit(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 地面の読込
			else if (FAILED(LoadField(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 壁の読込
			else if (FAILED(LoadWall(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 景色の読込
			else if (FAILED(LoadScenery(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 空の読込
			else if (FAILED(LoadSky(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 液体の読込
			else if (FAILED(LoadLiquid(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}
			
			// カメラ変更地点の読込
			else if (FAILED(LoadChanger(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 草の読込
			else if (FAILED(LoadWeed(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}
		}
		
		// ファイルを閉じる
		fclose(pFile);

		// 成功を返す
		return S_OK;
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ステージセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}
}

//============================================================
//	解放マップディレクトリの読込処理
//============================================================
HRESULT CStage::LoadOpen(const char* pString, FILE *pFile)
{
	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == nullptr || pFile == nullptr)
	{ // 文字列・ファイルが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ステージ範囲の設定
	if (strcmp(pString, "OPEN_PATH") == 0)
	{ // 読み込んだ文字列が OPEN_PATH の場合

		fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
		fscanf(pFile, "%s", &aString[0]);	// 解放マップディレクトリを読み込む

		// 解放マップディレクトリを保存
		m_sOpenMapFolder = &aString[0];

		// 解放マップの解放フラグパスを作成
		std::filesystem::path fsPath = m_sOpenMapFolder;
		fsPath.append("open.txt");

		// 解放フラグの取得
		bool bOpen = false;
		CTransPoint::LoadOpen(fsPath.string().c_str(), &bOpen);

		if (bOpen)
		{ // 既に解放済みの場合

			// パスをクリア
			m_sOpenMapFolder.clear();
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	範囲情報の読込処理
//============================================================
HRESULT CStage::LoadLimit(const char* pString, FILE *pFile)
{
	// 変数を宣言
	SLimit limit;	// ステージ範囲の代入用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == nullptr || pFile == nullptr)
	{ // 文字列・ファイルが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ステージ範囲の設定
	if (strcmp(pString, "LIMITSET") == 0)
	{ // 読み込んだ文字列が LIMITSET の場合

		// 制限モードを無しに設定
		limit.mode = LIMIT_NONE;

		do
		{ // 読み込んだ文字列が END_LIMITSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "CENTER") == 0)
			{ // 読み込んだ文字列が CENTER の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.center.x);	// 中心座標Xを読み込む
				fscanf(pFile, "%f", &limit.center.y);	// 中心座標Yを読み込む
				fscanf(pFile, "%f", &limit.center.z);	// 中心座標Zを読み込む
			}
			else if (strcmp(&aString[0], "NEAR") == 0)
			{ // 読み込んだ文字列が NEAR の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.fNear);		// 前位置を読み込む

				// 制限モードを矩形範囲に設定
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "FAR") == 0)
			{ // 読み込んだ文字列が FAR の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.fFar);		// 後位置を読み込む

				// 制限モードを矩形範囲に設定
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "RIGHT") == 0)
			{ // 読み込んだ文字列が RIGHT の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.fRight);		// 右位置を読み込む

				// 制限モードを矩形範囲に設定
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "LEFT") == 0)
			{ // 読み込んだ文字列が LEFT の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.fLeft);		// 左位置を読み込む

				// 制限モードを矩形範囲に設定
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "RADIUS") == 0)
			{ // 読み込んだ文字列が RADIUS の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.fRadius);	// 半径を読み込む

				// 制限モードを円範囲に設定
				limit.mode = LIMIT_CIRCLE;
			}
			else if (strcmp(&aString[0], "FIELD") == 0)
			{ // 読み込んだ文字列が FIELD の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.fField);		// 地面位置を読み込む
			}
			else if (strcmp(&aString[0], "KILLY") == 0)
			{ // 読み込んだ文字列が KILLY の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.fKillY);		// 削除位置を読み込む
			}
		} while (strcmp(&aString[0], "END_LIMITSET") != 0);	// 読み込んだ文字列が END_LIMITSET ではない場合ループ

		// ステージ範囲の設定
		SetLimit(limit);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	地面情報の読込処理
//============================================================
HRESULT CStage::LoadField(const char* pString, FILE *pFile)
{
	// 変数を宣言
	int nType = 0;					// 種類の代入用
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	D3DXVECTOR2 size = VEC2_ZERO;	// 大きさの代入用
	D3DXCOLOR col = XCOL_WHITE;		// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用
	POSGRID2 texPart = GRID2_ZERO;	// テクスチャ分割数の代入用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == nullptr || pFile == nullptr)
	{ // 文字列・ファイルが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 地面の設定
	if (strcmp(pString, "STAGE_FIELDSET") == 0)
	{ // 読み込んだ文字列が STAGE_FIELDSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_FIELDSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "FIELDSET") == 0)
			{ // 読み込んだ文字列が FIELDSET の場合
	
				do
				{ // 読み込んだ文字列が END_FIELDSET ではない場合ループ
	
					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合
	
						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nType);		// 種類を読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &size.x);		// 大きさXを読み込む
						fscanf(pFile, "%f", &size.y);		// 大きさYを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // 読み込んだ文字列が TEXPART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &texPart.x);	// テクスチャ横分割数を読み込む
						fscanf(pFile, "%d", &texPart.y);	// テクスチャ縦分割数を読み込む
					}
				} while (strcmp(&aString[0], "END_FIELDSET") != 0);	// 読み込んだ文字列が END_FIELDSET ではない場合ループ

				// 地面オブジェクトの生成
				if (CField::Create((CField::EType)nType, pos, D3DXToRadian(rot), size, col, part, texPart) == nullptr)
				{ // 確保に失敗した場合
	
					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_FIELDSET") != 0);	// 読み込んだ文字列が END_STAGE_FIELDSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	壁情報の読込処理
//============================================================
HRESULT CStage::LoadWall(const char* pString, FILE *pFile)
{
	// 変数を宣言
	int nType = 0;					// 種類の代入用
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	D3DXVECTOR2 size = VEC2_ZERO;	// 大きさの代入用
	D3DXCOLOR col = XCOL_WHITE;		// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用
	POSGRID2 texPart = GRID2_ZERO;	// テクスチャ分割数の代入用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == nullptr || pFile == nullptr)
	{ // 文字列・ファイルが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 壁の設定
	if (strcmp(pString, "STAGE_WALLSET") == 0)
	{ // 読み込んだ文字列が STAGE_WALLSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_SCENERYSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "WALLSET") == 0)
			{ // 読み込んだ文字列が WALLSET の場合
	
				do
				{ // 読み込んだ文字列が END_WALLSET ではない場合ループ
	
					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合
	
						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nType);		// 種類を読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &size.x);		// 大きさXを読み込む
						fscanf(pFile, "%f", &size.y);		// 大きさYを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // 読み込んだ文字列が TEXPART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &texPart.x);	// テクスチャ横分割数を読み込む
						fscanf(pFile, "%d", &texPart.y);	// テクスチャ縦分割数を読み込む
					}
				} while (strcmp(&aString[0], "END_WALLSET") != 0);	// 読み込んだ文字列が END_WALLSET ではない場合ループ

				// 壁オブジェクトの生成
				if (CWall::Create((CWall::EType)nType, pos, D3DXToRadian(rot), size, col, part, texPart) == nullptr)
				{ // 確保に失敗した場合

					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_WALLSET") != 0);	// 読み込んだ文字列が END_STAGE_WALLSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	景色情報の読込処理
//============================================================
HRESULT CStage::LoadScenery(const char* pString, FILE *pFile)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	D3DXCOLOR col = XCOL_WHITE;		// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用
	POSGRID2 texPart = GRID2_ZERO;	// テクスチャ分割数の代入用

	int nType = 0;			// 種類の代入用
	float fRadius = 0.0f;	// 半径の代入用
	float fHeight = 0.0f;	// 縦幅の代入用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == nullptr || pFile == nullptr)
	{ // 文字列・ファイルが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 景色の設定
	if (strcmp(pString, "STAGE_SCENERYSET") == 0)
	{ // 読み込んだ文字列が STAGE_SCENERYSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_SCENERYSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SCENERYSET") == 0)
			{ // 読み込んだ文字列が SCENERYSET の場合
	
				do
				{ // 読み込んだ文字列が END_SCENERYSET ではない場合ループ
	
					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合
	
						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nType);		// 種類を読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // 読み込んだ文字列が TEXPART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &texPart.x);	// テクスチャ横分割数を読み込む
						fscanf(pFile, "%d", &texPart.y);	// テクスチャ縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // 読み込んだ文字列が RADIUS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fRadius);		// 半径を読み込む
					}
					else if (strcmp(&aString[0], "HEIGHT") == 0)
					{ // 読み込んだ文字列が HEIGHT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fHeight);		// 縦幅を読み込む
					}
				} while (strcmp(&aString[0], "END_SCENERYSET") != 0);	// 読み込んだ文字列が END_SCENERYSET ではない場合ループ

				// 景色オブジェクトの生成
				if (CScenery::Create((CScenery::EType)nType, pos, D3DXToRadian(rot), col, part, texPart, fRadius, fHeight) == nullptr)
				{ // 確保に失敗した場合

					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_SCENERYSET") != 0);	// 読み込んだ文字列が END_STAGE_SCENERYSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	空情報の読込処理
//============================================================
HRESULT CStage::LoadSky(const char* pString, FILE *pFile)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	D3DXCOLOR col = XCOL_WHITE;		// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用
	POSGRID2 texPart = GRID2_ZERO;	// テクスチャ分割数の代入用

	int nType = 0;			// 種類の代入用
	float fRadius = 0.0f;	// 半径の代入用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == nullptr || pFile == nullptr)
	{ // 文字列・ファイルが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 空の設定
	if (strcmp(pString, "STAGE_SKYSET") == 0)
	{ // 読み込んだ文字列が STAGE_SKYSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_SKYSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SKYSET") == 0)
			{ // 読み込んだ文字列が SKYSET の場合

				do
				{ // 読み込んだ文字列が END_SKYSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nType);		// 種類を読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // 読み込んだ文字列が TEXPART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &texPart.x);	// テクスチャ横分割数を読み込む
						fscanf(pFile, "%d", &texPart.y);	// テクスチャ縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // 読み込んだ文字列が RADIUS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fRadius);		// 半径を読み込む
					}
				} while (strcmp(&aString[0], "END_SKYSET") != 0);	// 読み込んだ文字列が END_SKYSET ではない場合ループ

				// 空オブジェクトの生成
				if (CSky::Create((CSky::EType)nType, pos, D3DXToRadian(rot), col, part, texPart, fRadius) == nullptr)
				{ // 確保に失敗した場合

					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_SKYSET") != 0);	// 読み込んだ文字列が END_STAGE_SKYSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	液体情報の読込処理
//============================================================
HRESULT CStage::LoadLiquid(const char* pString, FILE *pFile)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	D3DXVECTOR2 size = VEC2_ZERO;	// 大きさの代入用
	D3DXCOLOR col = XCOL_WHITE;		// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用
	CLiquid::STexMove texMove;		// テクスチャ移動量の代入用

	int nType = 0;				// 種類の代入用
	float fMaxUp = 0.0f;		// 波の最高上昇量
	float fAddSinRot = 0.0f;	// 波打ち向き加算量
	float fAddVtxRot = 0.0f;	// 隣波の向き加算量

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == nullptr || pFile == nullptr)
	{ // 文字列・ファイルが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 液体の設定
	if (strcmp(pString, "STAGE_LIQUIDSET") == 0)
	{ // 読み込んだ文字列が STAGE_LIQUIDSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_LIQUIDSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "LIQUIDSET") == 0)
			{ // 読み込んだ文字列が LIQUIDSET の場合

				do
				{ // 読み込んだ文字列が END_LIQUIDSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nType);		// 種類を読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &size.x);		// 大きさXを読み込む
						fscanf(pFile, "%f", &size.y);		// 大きさYを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXMOVE_LOW") == 0)
					{ // 読み込んだ文字列が TEXMOVE_LOW の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &texMove.texMoveLow.x);	// 下液体のテクスチャ移動量を読み込む
						fscanf(pFile, "%f", &texMove.texMoveLow.y);	// 下液体のテクスチャ移動量を読み込む
					}
					else if (strcmp(&aString[0], "TEXMOVE_HIGH") == 0)
					{ // 読み込んだ文字列が TEXMOVE_HIGH の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f", &texMove.texMoveHigh.x);	// 上液体のテクスチャ移動量を読み込む
						fscanf(pFile, "%f", &texMove.texMoveHigh.y);	// 上液体のテクスチャ移動量を読み込む
					}
					else if (strcmp(&aString[0], "MAX_UP") == 0)
					{ // 読み込んだ文字列が MAX_UP の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fMaxUp);		// 波の最高上昇量を読み込む
					}
					else if (strcmp(&aString[0], "ADD_SINROT") == 0)
					{ // 読み込んだ文字列が ADD_SINROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fAddSinRot);	// 波打ち向き加算量を読み込む
					}
					else if (strcmp(&aString[0], "ADD_VTXROT") == 0)
					{ // 読み込んだ文字列が ADD_VTXROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fAddVtxRot);	// 隣波の向き加算量を読み込む
					}
				} while (strcmp(&aString[0], "END_LIQUIDSET") != 0);	// 読み込んだ文字列が END_LIQUIDSET ではない場合ループ

				// 液体オブジェクトの生成
				CLiquid *pLiquid = CLiquid::Create
				( // 引数
					(CLiquid::EType)nType,		// 種類
					pos,						// 位置
					D3DXToRadian(rot),			// 向き
					size,						// 大きさ
					col,						// 色
					part,						// 分割数
					texMove,					// テクスチャ移動量
					fMaxUp,						// 波の最高上昇量
					D3DXToRadian(fAddSinRot),	// 波打ち向き加算量
					D3DXToRadian(fAddVtxRot)	// 隣波の向き加算量
				);
				if (pLiquid == nullptr)
				{ // 確保に失敗した場合

					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_LIQUIDSET") != 0);	// 読み込んだ文字列が END_STAGE_LIQUIDSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}

//===========================================
//  カメラ変更地点情報の読込
//===========================================
HRESULT CStage::LoadChanger(const char* pString, FILE* pFile)
{
	// 読込に失敗した場合関数を抜ける
	if (pString == nullptr || pFile == nullptr) { assert(false); return E_FAIL; }

	// オブジェクトの設定
	if (strcmp(pString, "STAGE_CHANGERSET") == 0) // 読込開始フラグ
	{
		// 一時格納用変数
		char aTemp[MAX_STRING];

		do // 読込が完了するまでループする
		{
			// 文字列の読み込み
			fscanf(pFile, "%s", &aTemp[0]);

			// オブジェクトの配置
			if (strcmp(&aTemp[0], "CHANGERSET") == 0)
			{
				// 変数を宣言
				int nDir = 0; // 方向の格納用
				int nRot = 0; // 角度の格納用
				D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の格納用
				D3DXVECTOR3 size = VEC3_ZERO;	// 大きさの格納用

				do // 読込が完了するまでループする
				{
					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aTemp[0]);

					if (strcmp(&aTemp[0], "DIR") == 0)
					{
						fscanf(pFile, "%s", &aTemp[0]); // =
						fscanf(pFile, "%d", &nDir); // 方向の読込
					}
					if (strcmp(&aTemp[0], "ROT") == 0)
					{
						fscanf(pFile, "%s", &aTemp[0]); // =
						fscanf(pFile, "%d", &nRot); // 角度の読み込み
					}
					if (strcmp(&aTemp[0], "POS") == 0) // 位置の読み込み 
					{
						fscanf(pFile, "%s", &aTemp[0]); // =
						fscanf(pFile, "%f", &pos.x); // X
						fscanf(pFile, "%f", &pos.y); // Y
						fscanf(pFile, "%f", &pos.z); // Z
					}
					if (strcmp(&aTemp[0], "SIZE") == 0) // 大きさの読み込み
					{
						fscanf(pFile, "%s", &aTemp[0]); // =
						fscanf(pFile, "%f", &size.x); // X
						fscanf(pFile, "%f", &size.y); // Y
						fscanf(pFile, "%f", &size.z); // Z
					}

				} while (strcmp(&aTemp[0], "END_CHANGERSET") != 0);

				// オブジェクトの生成
				if (CCameraChanger::Create(pos, size, (CCameraChanger::EDirection)nDir, (CCameraChanger::ERotation)nRot) == nullptr)
				{
					assert(false); return E_FAIL;
				}
			}
		} while (strcmp(&aTemp[0], "END_STAGE_CHANGERSET") != 0);
	}

	// 成功を返す
	return S_OK;
}

//==========================================
//  草の読み込み
//==========================================
HRESULT CStage::LoadWeed(const char* pString, FILE* pFile)
{
	// 読込に失敗した場合関数を抜ける
	if (pString == nullptr || pFile == nullptr) { assert(false); return E_FAIL; }

	// オブジェクトの設定
	if (strcmp(pString, "STAGE_WEEDSET") == 0) // 読込開始フラグ
	{
		// 一時格納用変数
		char aTemp[MAX_STRING];

		do // 読込が完了するまでループする
		{
			// 文字列の読み込み
			fscanf(pFile, "%s", &aTemp[0]);

			// オブジェクトの配置
			if (strcmp(&aTemp[0], "WEEDSET") == 0)
			{
				// 変数を宣言
				D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の格納用
				D3DXVECTOR3 rot = VEC3_ZERO;	// 大きさの格納用

				do // 読込が完了するまでループする
				{
					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aTemp[0]);

					if (strcmp(&aTemp[0], "POS") == 0) // 位置の読み込み 
					{
						fscanf(pFile, "%s", &aTemp[0]); // =
						fscanf(pFile, "%f", &pos.x); // X
						fscanf(pFile, "%f", &pos.y); // Y
						fscanf(pFile, "%f", &pos.z); // Z
					}
					if (strcmp(&aTemp[0], "ROT") == 0)
					{
						fscanf(pFile, "%s", &aTemp[0]); // =
						fscanf(pFile, "%f", &rot.x); // X
						fscanf(pFile, "%f", &rot.y); // Y
						fscanf(pFile, "%f", &rot.z); // Z
					}

				} while (strcmp(&aTemp[0], "END_WEEDSET") != 0);

				// オブジェクトの生成
				if (CWeed::Create(pos, rot) == nullptr)
				{
					assert(false); return E_FAIL;
				}
			}
		} while (strcmp(&aTemp[0], "END_STAGE_WEEDSET") != 0);
	}

	// 成功を返す
	return S_OK;
}
