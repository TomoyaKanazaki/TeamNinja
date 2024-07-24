//============================================================
//
//	コイン処理 [coin.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "coin.h"
#include "renderer.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* MODEL = "data\\MODEL\\PLAYER\\02_head.x";	// モデルのパス
	const char* SETUP_TXT = "data\\TXT\\coin.txt";			// セットアップテキスト相対パス
	const int PRIORITY = 4;	// コインの優先順位
}

//************************************************************
// 静的メンバ変数宣言
//************************************************************
CListManager<CCoin>* CCoin::m_pList = nullptr;		// リスト構造

//************************************************************
//	子クラス [CCoin] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CCoin::CCoin() : CObjectModel(CObject::LABEL_COIN, CObject::DIM_3D, PRIORITY)
{

}

//============================================================
//	デストラクタ
//============================================================
CCoin::~CCoin()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CCoin::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CCoin>::Create();

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
void CCoin::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CCoin::Update(const float fDeltaTime)
{
	// オブジェクトモデルの更新
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CCoin::Draw(CShader* pShader)
{
	// オブジェクトモデルの描画
	CObjectModel::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CCoin* CCoin::Create(const D3DXVECTOR3& rPos)
{
	// モデルUIの生成
	CCoin* pCoin = new CCoin;

	if (pCoin == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// コインの初期化
		if (FAILED(pCoin->Init()))
		{ // 初期化に失敗した場合

			// コインの破棄
			SAFE_DELETE(pCoin);
			return nullptr;
		}

		// 位置を設定
		pCoin->SetVec3Position(rPos);

		// 向きを設定
		pCoin->SetVec3Rotation(VEC3_ZERO);

		// 拡大率を設定
		pCoin->SetVec3Scaling(VEC3_ONE);

		// モデルの割り当て処理
		pCoin->BindModel(MODEL);

		// 確保したアドレスを返す
		return pCoin;
	}
}

//============================================================
// リスト構造の取得処理
//============================================================
CListManager<CCoin>* CCoin::GetList(void)
{
	// リスト構造を返す
	return m_pList;
}

//============================================================
// 当たり判定処理
//============================================================
void CCoin::Collision
(
	D3DXVECTOR3& rPos,				// 位置
	const float fRadius,			// 半径
	const float fHeight				// 高さ
)
{

}

//============================================================
//	セットアップ処理
//============================================================
HRESULT CCoin::LoadSetup(const char* pPass)
{
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用

	// ファイルを開く
	std::ifstream file(pPass);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "コインセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

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
		else if (str == "STAGE_COINSET")
		{
			do
			{ // END_STAGE_COINSETを読み込むまでループ

				// 文字列を読み込む
				file >> str;

				if (str.front() == '#')
				{ // コメントアウトされている場合

					// 一行全て読み込む
					std::getline(file, str);
				}
				else if (str == "COINSET")
				{
					do
					{ // END_COINSETを読み込むまでループ

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
					} while (str != "END_COINSET");	// END_COINSETを読み込むまでループ

					// コインオブジェクトの生成
					if (CCoin::Create(pos) == nullptr)
					{ // 確保に失敗した場合

						// 失敗を返す
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_COINSET");	// END_STAGE_COINSETを読み込むまでループ
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}
