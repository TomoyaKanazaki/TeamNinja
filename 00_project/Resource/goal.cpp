//==========================================
//
//  ゴール(goal.cpp)
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
	const char* MODEL = "data\\MODEL\\Scroll\\Scroll000.x";		// モデルのパス
	const D3DXVECTOR3 INIT_ROT = D3DXVECTOR3(0.7f, 0.0f, 0.0f);	// 初期向き
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
	BindModel(MODEL);

	// 自身のラベルを設定
	SetLabel(LABEL_GOALPOINT);

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

	// ゴールの初期化
	if (FAILED(pSavePoint->Init()))
	{
		// メモリ開放
		delete pSavePoint;

		// nullを返す
		return nullptr;
	}

	// 位置を設定
	pSavePoint->SetVec3Position(D3DXVECTOR3(rPos.x, rPos.y + RADIUS, rPos.z));

	// 向きを設定
	pSavePoint->SetVec3Rotation(INIT_ROT);

	// 確保したアドレスを返す
	return pSavePoint;
}

//==========================================
// リスト取得
//==========================================
CGoal* CGoal::GetGoal(void)
{
	if (m_pList == nullptr) { return nullptr; }		// リスト未使用の場合抜ける
	if (m_pList->GetNumAll() != 1) { assert(false); return nullptr; }	// ゴールが存在した場合関数を抜ける
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

	// クリアフラグをオンにする
	m_bClear = true;
}

//============================================================
//	セットアップ処理
//============================================================
HRESULT CGoal::LoadSetup(const char* pPass)
{
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用

	// ファイルを開く
	std::ifstream file(pPass);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ゴールポイントセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

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
		else if (str == "STAGE_GOALSET")
		{
			do
			{ // END_STAGE_GOALSETを読み込むまでループ

				// 文字列を読み込む
				file >> str;

				if (str == "POS")
				{
					file >> str;	// ＝を読込

					// 位置を読込
					file >> pos.x;
					file >> pos.y;
					file >> pos.z;
				}
			} while (str != "END_STAGE_GOALSET");	// END_STAGE_CHECKSETを読み込むまでループ

			// ゴールポイントの生成
			if (CGoal::Create(pos) == nullptr)
			{ // 確保に失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}
