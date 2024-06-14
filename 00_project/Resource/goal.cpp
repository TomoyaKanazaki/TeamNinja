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
}

//------------------------------------------
// 静的メンバ変数宣言
//------------------------------------------
CListManager<CGoal>* CGoal::m_pList = nullptr;	// オブジェクトリスト

//==========================================
//  コンストラクタ
//==========================================
CGoal::CGoal():
	m_bClear(false),
	m_fRadius(0.0f),
	m_fRotSpeed(0.0f)
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
	m_fRadius = 0.0f;
	m_fRotSpeed = 0.0f;

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

	// 定数パラメータの読み込み
	Load();

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
	rot.y += m_fRotSpeed;
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
CGoal* CGoal::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
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
	pSavePoint->SetVec3Rotation(rRot);

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
	if (!collision::CirclePillar(pos, Player->GetVec3Position(), m_fRadius, Player->GetRadius()))
	{ return; }

	// ゴール時のUIを表示する
	CPopUpUI::Create(GOAL_TEXTURE);

	// クリアフラグをオンにする
	m_bClear = true;
}

//==========================================
//  外部情報の読み込み
//==========================================
void CGoal::Load()
{
	//ローカル変数宣言
	FILE* pFile; // ファイルポインタ

	//ファイルを読み取り専用で開く
	pFile = fopen(PARAM_FILE, "r");

	// ファイルが開けなかった場合
	if (pFile == NULL) { assert(false); return; }

	// 情報の読み込み
	while (1)
	{
		// 文字列の記録用
		char aStr[256];

		// 文字列読み込み
		fscanf(pFile, "%s", &aStr[0]);

		// 条件分岐
		if (strcmp(&aStr[0], "RADIUS") == 0) // 当たり判定の半径の設定
		{
			// データを格納
			fscanf(pFile, "%f", &m_fRadius);
		}
		if (strcmp(&aStr[0], "ROT_SPEED") == 0) // 回る速度
		{
			// データを格納
			fscanf(pFile, "%f", &m_fRotSpeed);
		}
		if (strcmp(&aStr[0], "END_OF_FILE") == 0) // 読み込み終了
		{
			break;
		}
	}
}
