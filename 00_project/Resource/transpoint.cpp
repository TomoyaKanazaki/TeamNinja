//============================================================
//
//	遷移ポイント処理 [transpoint.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "transpoint.h"
#include "collision.h"
#include "manager.h"
#include "stage.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char*	MODEL_PASS	= "data\\MODEL\\GATE\\gate001.x";	// モデルファイル
	const int	PRIORITY	= 3;		// 遷移ポイントの優先順位
	const float	RADIUS		= 120.0f;	// 遷移ポイントに触れられる半径
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CTransPoint>* CTransPoint::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CTransPoint] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTransPoint::CTransPoint(const char* pPass) : CObjectModel(CObject::LABEL_TRANSPOINT, CObject::DIM_3D, PRIORITY),
	m_sTransMapPass	(pPass)	// 遷移先マップパス
{

}

//============================================================
//	デストラクタ
//============================================================
CTransPoint::~CTransPoint()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTransPoint::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 遷移ポイントモデルを割当
	BindModel(MODEL_PASS);

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CTransPoint>::Create();
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
void CTransPoint::Uninit(void)
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
void CTransPoint::Update(const float fDeltaTime)
{
	// オブジェクトモデルの更新
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CTransPoint::Draw(CShader *pShader)
{
	CToonShader	*pToonShader = CToonShader::GetInstance();	// トゥーンシェーダー情報
	if (pToonShader->IsEffectOK())
	{ // エフェクトが使用可能な場合

		// オブジェクトモデルの描画
		CObjectModel::Draw(pToonShader);
	}
	else
	{ // エフェクトが使用不可能な場合

		// エフェクトエラー
		assert(false);

		// オブジェクトモデルの描画
		CObjectModel::Draw(pShader);
	}
}

//============================================================
//	生成処理
//============================================================
CTransPoint *CTransPoint::Create(const char* pPass, const D3DXVECTOR3& rPos)
{
	// 遷移ポイントの生成
	CTransPoint *pTransPoint = new CTransPoint(pPass);
	if (pTransPoint == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 遷移ポイントの初期化
		if (FAILED(pTransPoint->Init()))
		{ // 初期化に失敗した場合

			// 遷移ポイントの破棄
			SAFE_DELETE(pTransPoint);
			return nullptr;
		}

		// 位置を設定
		pTransPoint->SetVec3Position(rPos);

		// 確保したアドレスを返す
		return pTransPoint;
	}
}

//============================================================
//	遷移ポイントとの当たり判定
//============================================================
CTransPoint* CTransPoint::Collision(const D3DXVECTOR3& rPos, const float fRadius)
{
	// 遷移ポイントがない場合抜ける
	if (m_pList == nullptr) { return nullptr; }

	std::list<CTransPoint*> list = m_pList->GetList();	// 内部リスト
	for (const auto& rList : list)
	{ // 要素数分繰り返す

		D3DXVECTOR3 posTrans = rList->GetVec3Position();	// 遷移ポイント位置

		// XZ平面の円の当たり判定
		bool bHit = collision::Circle2D
		(
			rPos,		// 判定位置
			posTrans,	// 判定目標位置
			fRadius,	// 判定半径
			RADIUS		// 判定目標半径
		);
		if (bHit)
		{ // 当たっている場合

			// 当たっている遷移ポイントを返す
			return rList;
		}
	}

	// 何一つ当たっていない場合nullptrを返す
	return nullptr;
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CTransPoint> *CTransPoint::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	セットアップ処理
//============================================================
HRESULT CTransPoint::LoadSetup(const char* pPass)
{
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	std::string sTrans;	// 遷移先マップパスの代入用

	// ファイルを開く
	std::ifstream file(pPass);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "遷移ポイントセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

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
		else if (str == "STAGE_TRANSSET")
		{
			do
			{ // END_STAGE_TRANSSETを読み込むまでループ

				// 文字列を読み込む
				file >> str;

				if (str.front() == '#')
				{ // コメントアウトされている場合

					// 一行全て読み込む
					std::getline(file, str);
				}
				else if (str == "TRANSSET")
				{
					do
					{ // END_TRANSSETを読み込むまでループ

						// 文字列を読み込む
						file >> str;

						if (str == "TRANS_PASS")
						{
							file >> str;	// ＝を読込
							file >> sTrans;	// 遷移先マップパスを読込
						}
						else if (str == "POS")
						{
							file >> str;	// ＝を読込

							// 位置を読込
							file >> pos.x;
							file >> pos.y;
							file >> pos.z;
						}
					} while (str != "END_TRANSSET");	// END_TRANSSETを読み込むまでループ

					// 遷移ポイントの生成
					if (CTransPoint::Create(sTrans.c_str(), pos) == nullptr)
					{ // 確保に失敗した場合

						// 失敗を返す
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_TRANSSET");	// END_STAGE_TRANSSETを読み込むまでループ
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}
