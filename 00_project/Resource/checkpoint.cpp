//==========================================
//
//  チェックポイント(checkpoint.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "checkpoint.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "collision.h"

//==========================================
//  静的メンバ変数宣言
//==========================================
int CCheckPoint::m_nNumAll = 0;

//==========================================
//  コンストラクタ
//==========================================
CCheckPoint::CCheckPoint():
	m_bSave(false),
	m_fRadius(0.0f),
	m_nSaveTension(0)
{
	// 総数を加算
	++m_nNumAll;
}

//==========================================
//  デストラクタ
//==========================================
CCheckPoint::~CCheckPoint()
{
	// 総数を減算
	--m_nNumAll;
}

//==========================================
//  初期化処理
//==========================================
HRESULT CCheckPoint::Init(void)
{
	// 値の初期化
	m_bSave = false;
	m_fRadius = 50.0f;
	m_nSaveTension = 0;

	// 親クラスの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルを割り当て
	BindModel("data\\MODEL\\PLAYER\\02_head.x");

	// 自身のラベルを設定
	SetLabel(LABEL_CHECKPOINT);

	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void CCheckPoint::Uninit(void)
{
	// 親クラスの終了
	CObjectModel::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CCheckPoint::Update(const float fDeltaTime)
{
	// プレイヤーとの当たり判定
	CollisionPlayer();

	// 親クラスの更新
	CObjectModel::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CCheckPoint::Draw(CShader* pShader)
{
	// 親クラスの描画
	CObjectModel::Draw();
}

//==========================================
//  生成処理
//==========================================
CCheckPoint* CCheckPoint::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// ポインタを宣言
	CCheckPoint* pSavePoint = new CCheckPoint;	// セーブポイント生成用

	// メモリの確保に失敗していた場合nullを返す
	if (pSavePoint == nullptr) { assert(false); return nullptr; }

	// セーブポイントの初期化
	if (FAILED(pSavePoint->Init()))
	{
		// メモリ開放
		delete pSavePoint;

		// nullを返す
		return nullptr;
	}

	// 位置を設定
	pSavePoint->SetVec3Position(rPos);

	// 向きを設定
	pSavePoint->SetVec3Rotation(rRot);

	// 確保したアドレスを返す
	return pSavePoint;
}

//==========================================
//  プレイヤーとの当たり判定
//==========================================
void CCheckPoint::CollisionPlayer(void)
{
	// セーブフラグがオンなら関数を抜ける
	if (m_bSave) { return; }

	//　自身の位置を取得
	D3DXVECTOR3 pos = GetVec3Position();

	// プレイヤーの情報を取得
	CPlayer* Player = GET_PLAYER; // 座標

	// 当たっていない場合関数を抜ける
	if (!collision::CirclePillar(pos, Player->GetVec3Position(), m_fRadius, Player->GetRadius()))
	{ return; }

	// プレイヤーを回復する
	Player->RecoverCheckPoint();

	// 士気力を保存する
	m_nSaveTension = Player->GetTension();

	// セーブフラグをオンにする
	m_bSave = true;
}
