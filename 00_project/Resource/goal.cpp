//==========================================
//
//  チェックポイント(checkpoint.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "goal.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "collision.h"
#include "popupUI.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const char* PARAM_FILE = "data\\TXT\\Goal.txt"; // パラメータが保存されたパス
	const char* GOAL_TEXTURE = "data\\TEXTURE\\end.png";	// ゴールのテクスチャ

	const float ROT_SPEED = 0.01f;		// 向きの速度
	const float RADIUS = 50.0f;			// 半径
}

//------------------------------------------
// 静的メンバ変数宣言
//------------------------------------------
CListManager<CGoal>* CGoal::m_pList = nullptr;	// オブジェクトリスト

//==========================================
//  コンストラクタ
//==========================================
CGoal::CGoal():
	m_bClear(false)
{

}

//==========================================
//  デストラクタ
//==========================================
CGoal::~CGoal()
{
}

//==========================================
//  初期化処理
//==========================================
HRESULT CGoal::Init(void)
{
	// 値の初期化
	m_bClear = false;

	// 親クラスの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルを割り当て
	BindModel("data\\MODEL\\ENEMY\\BOSS_DRAGON\\03_head.x");

	// 自身のラベルを設定
	SetLabel(LABEL_GOAL);

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CGoal>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void CGoal::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 親クラスの終了
	CObjectModel::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CGoal::Update(const float fDeltaTime)
{
	// プレイヤーとの当たり判定
	CollisionPlayer();

	// くるくるしてみる
	D3DXVECTOR3 rot = GetVec3Rotation();
	rot.y += ROT_SPEED;
	SetVec3Rotation(rot);

	// 親クラスの更新
	CObjectModel::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CGoal::Draw(CShader* pShader)
{
	// 親クラスの描画
	CObjectModel::Draw(pShader);
}

//==========================================
//  生成処理
//==========================================
CGoal* CGoal::Create(const D3DXVECTOR3& rPos)
{
	// ポインタを宣言
	CGoal* pSavePoint = new CGoal;	// セーブポイント生成用

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
	pSavePoint->SetVec3Rotation(VEC3_ZERO);

	// 確保したアドレスを返す
	return pSavePoint;
}

//==========================================
// リスト取得
//==========================================
CGoal* CGoal::GetGoal(void)
{
	if (m_pList == nullptr) { return nullptr; }		// リスト未使用の場合抜ける
	if (m_pList->GetNumAll() != 1) { assert(false); return nullptr; }	// ゴールが1人ではない場合抜ける
	CGoal* pGoal = m_pList->GetList().front();		// ゴールの情報

	// ゴールのポインタを返す
	return pGoal;
}

//==========================================
//  プレイヤーとの当たり判定
//==========================================
void CGoal::CollisionPlayer(void)
{
	// クリアフラグがオンなら関数を抜ける
	if (m_bClear) { return; }

	//　自身の位置を取得
	D3DXVECTOR3 pos = GetVec3Position();

	// プレイヤーの情報を取得
	CPlayer* Player = GET_PLAYER; // 座標

	// プレイヤーがいない場合抜ける
	if (Player == nullptr) { return; }

	// 当たっていない場合関数を抜ける
	if (!collision::CirclePillar(pos, Player->GetVec3Position(), RADIUS, Player->GetRadius()))
	{ return; }

	// ゴール時のUIを表示する
	CPopUpUI::Create(GOAL_TEXTURE);

	// クリアフラグをオンにする
	m_bClear = true;
}
