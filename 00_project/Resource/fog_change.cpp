//===========================================
//
//  フォグ変更クラス(fog_change.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "fog_change.h"
#include "manager.h"
#include "player.h"
#include "collision.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "fog.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const D3DXVECTOR3 CHANGE_POS = D3DXVECTOR3(5250.0f, 1400.0f, 75.0f); // 位置
	const D3DXVECTOR3 CHANGE_SIZE = D3DXVECTOR3(100.0f, 100.0f, 100.0f); // サイズ

	const float MOVE_SPEED = 0.01f; // 移動していく時間
	const float TARGET_START = 0.0f; // 始点の移動先
	const float TARGET_END = 0.0f; // 終点の移動先
}

//===========================================
//  静的メンバ変数宣言
//===========================================
CListManager<CFogChanger>* CFogChanger::m_pList = nullptr; // オブジェクトリスト

//===========================================
//  コンストラクタ
//===========================================
CFogChanger::CFogChanger() : CObjectMeshCube(CObject::LABEL_COLLISION, CObject::SCENE_MAIN, CObject::DIM_3D)
{
}

//===========================================
//  デストラクタ
//===========================================
CFogChanger::~CFogChanger()
{
}

//===========================================
//  初期化処理
//===========================================
HRESULT CFogChanger::Init()
{
	// 親クラスの初期化
	if (FAILED(CObjectMeshCube::Init())) { assert(false); return E_FAIL; }

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CFogChanger>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// 原点を設定
	SetOrigin(ORIGIN_DOWN);

	// 位置を設定
	SetVec3Position(CHANGE_POS);

	// サイズを設定
	SetVec3Sizing(CHANGE_SIZE);

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 成功を返す
	return S_OK;
}

//===========================================
//  終了処理
//===========================================
void CFogChanger::Uninit()
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 親クラスの終了
	CObjectMeshCube::Uninit();
}

//===========================================
//  更新処理
//===========================================
void CFogChanger::Update(const float fDeltaTime)
{
	// 遠距離判定
	bool bFar = useful::IsNearPosR(GetVec3Position());
	SetEnableDraw(bFar);
	if (!bFar) { return; }

	// ゲーム中じゃない場合関数を抜ける
	if (GET_GAMEMANAGER->GetState() != CGameManager::STATE_NORMAL) { return; }

	// 移動
	Move();

	// 親クラスの更新処理
	CObjectMeshCube::Update(fDeltaTime);
}

//===========================================
//  描画処理
//===========================================
void CFogChanger::Draw(CShader* pShader)
{
#ifndef _DEBUG
	// デバッグ中以外は描画したくない
	return;
#endif

	//親クラスの描画処理
	CObjectMeshCube::Draw(pShader);
}

//===========================================
//	ヒット判定の取得
//===========================================
bool CFogChanger::CollChange()
{
	// プレイヤー情報の取得
	CPlayer* pPlayer = GET_PLAYER;

	// プレイヤーがいない場合抜ける
	if (pPlayer == nullptr) { return false; }

	// プレイヤーの情報を取得
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();
	D3DXVECTOR3 sizePlayer = pPlayer->GetVec3Sizing();

	// 各種判定用情報の取得
	D3DXVECTOR3 posThis = GetVec3Position(); // 自身の座標
	D3DXVECTOR3 sizeThis = GetVec3Sizing(); // 自身のサイズ

	// 矩形判定の判定結果を返す
	return collision::Box3D(posThis, posPlayer, D3DXVECTOR3(sizeThis.x, sizeThis.y * 2.0f, sizeThis.z), D3DXVECTOR3(sizeThis.x, 0.0f, sizeThis.z), sizePlayer, sizePlayer);
}

//==========================================
//  移動
//==========================================
void CFogChanger::Move()
{
	// 目標値を保存する変数
	float fStart = 0.0f, fEnd = 0.0f;

	// 目標値を設定
	if (CollChange())
	{
		fStart = TARGET_START;
		fEnd = TARGET_END;
	}
	else
	{
		fStart = Fog::GetInitStart();
		fEnd = Fog::GetInitEnd();
	}

	// 移動量の設定
	float fDestStart = (fStart - Fog::GetStart()) * MOVE_SPEED;
	float fDestEnd = (fEnd - Fog::GetEnd()) * MOVE_SPEED;

	// フォグの距離を設定
	Fog::SetStart(fDestStart + Fog::GetStart());
	Fog::SetEnd(fDestEnd + Fog::GetEnd());
}

//===========================================
//  生成処理
//===========================================
CFogChanger* CFogChanger::Create()
{
	// メモリの確保
	CFogChanger* pChanger = new CFogChanger;

	// 確保に失敗した場合nullを返す
	if (pChanger == nullptr) { assert(false); return nullptr; }

	// 初期化処理
	if (FAILED(pChanger->Init())) { assert(false); return nullptr; }

	return pChanger;
}

//===========================================
//  リストの取得
//===========================================
CListManager<CFogChanger>* CFogChanger::GetList(void)
{
	return m_pList;
}
