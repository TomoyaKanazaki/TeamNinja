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
#include "popupUI.h"
#include "goal.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const char *SETUP_TXT	= "data\\TXT\\Point.txt";	// セットアップテキスト相対パス
	const float RADIUS		= 50.0f;	// 半径
	const float ROT_SPEED	= 0.01f;	// 回る速度
}

//==========================================
//  静的メンバ変数宣言
//==========================================
int CCheckPoint::m_nNumAll = 0;
CListManager<CCheckPoint>* CCheckPoint::m_pList = nullptr;	// オブジェクトリスト

//==========================================
//  コンストラクタ
//==========================================
CCheckPoint::CCheckPoint():
	m_bSave(false),
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
	m_nSaveTension = 0;

	// 親クラスの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルを割り当て
	BindModel("data\\MODEL\\FONT\\name_boss000.x");

	// 自身のラベルを設定
	SetLabel(LABEL_CHECKPOINT);

	// サイズを調整
	SetVec3Scaling(D3DXVECTOR3(0.2f, 0.2f, 0.2f));

	// マテリアルを変更
	SetAllMaterial(material::GlowCyan());

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CCheckPoint>::Create();
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
void CCheckPoint::Uninit(void)
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
void CCheckPoint::Update(const float fDeltaTime)
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
void CCheckPoint::Draw(CShader* pShader)
{
	// 親クラスの描画
	CObjectModel::Draw(pShader);
}

//==========================================
//  生成処理
//==========================================
CCheckPoint* CCheckPoint::Create(const D3DXVECTOR3& rPos)
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

	// 確保したアドレスを返す
	return pSavePoint;
}

//==========================================
// リスト取得
//==========================================
CListManager<CCheckPoint>* CCheckPoint::GetList(void)
{
	// リスト構造を返す
	return m_pList;
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
	if (!collision::CirclePillar(pos, Player->GetVec3Position(), RADIUS, Player->GetRadius()))
	{ return; }

	// プレイヤーを回復する
	Player->RecoverCheckPoint();

	// プレイヤーに自身の情報を与える
	Player->SetCheckPoint(this);

	// 士気力を保存する
	m_nSaveTension = Player->GetTension();

	// マテリアルを変更
	SetAllMaterial(material::Red());

	// セーブフラグをオンにする
	m_bSave = true;
	
	// UIを表示
	CPopUpUI::Create();
}

//============================================================
//	セットアップ処理
//============================================================
HRESULT CCheckPoint::LoadSetup(void)
{
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用

	// ファイルを開く
	std::ifstream file(SETUP_TXT);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "チェックポイントセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

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
		else if (str == "STAGE_CHECKSET")
		{
			do
			{ // END_STAGE_CHECKSETを読み込むまでループ

				// 文字列を読み込む
				file >> str;

				if (str.front() == '#')
				{ // コメントアウトされている場合

					// 一行全て読み込む
					std::getline(file, str);
				}
				else if (str == "CHECKSET")
				{
					do
					{ // END_CHECKSETを読み込むまでループ

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
					} while (str != "END_CHECKSET");	// END_CHECKSETを読み込むまでループ

					// チェックポイントの生成
					if (CCheckPoint::Create(pos) == nullptr)
					{ // 確保に失敗した場合

						// 失敗を返す
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_CHECKSET");	// END_STAGE_CHECKSETを読み込むまでループ
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

			// チェックポイントの生成
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
