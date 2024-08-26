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
#include "balloon.h"
#include "balloonManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char*	HIT_EFFECT_PASS		= "data\\EFFEKSEER\\checkpoint_blue.efkefc";	// 触れている際のエフェクトファイル
	const char*	UNHIT_EFFECT_PASS	= "data\\EFFEKSEER\\checkpoint_red.efkefc";		// 触れていない際のエフェクトファイル

	const D3DXVECTOR3 OFFSET = D3DXVECTOR3(0.0f, 5.0f, 0.0f);	// エフェクト用オフセット
	const int	PRIORITY	 = 2;		// 遷移ポイントの優先順位
	const float	RADIUS		 = 120.0f;	// 遷移ポイントに触れられる半径
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CTransPoint>* CTransPoint::m_pList = nullptr;	// オブジェクトリスト
CBalloonManager* CTransPoint::m_pBalloonManager = nullptr;	// 吹き出しマネージャー情報

//************************************************************
//	子クラス [CTransPoint] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTransPoint::CTransPoint(const char* pPass) : CObjectModel(CObject::LABEL_TRANSPOINT, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
	m_sTransMapPass	(pPass),	// 遷移先マップパス
	m_pEffectData	(nullptr),	// 保持するエフェクト情報
	m_pBalloon		(nullptr),	// 吹き出し情報
	m_bOpen			(false)		// ステージ解放フラグ
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
	// メンバ変数を初期化
	m_pEffectData = nullptr;	// 保持するエフェクト情報
	m_pBalloon = nullptr;		// 吹き出し情報
	m_bOpen = true;			// ステージ解放フラグ	// TODO：ここfalseね

	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 吹き出しの生成
	m_pBalloon = CBalloon::Create(GetVec3Position());
	if (m_pBalloon == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

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
	// エフェクトの破棄
	SAFE_DELETE(m_pEffectData);

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// 吹き出しマネージャーの終了
		SAFE_UNINIT(m_pBalloonManager);

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

	// 触れている遷移ポイントがない場合抜ける
	if (m_pBalloonManager == nullptr) { return; }

	if (m_pBalloonManager->IsNone()
	&&  m_pBalloon->IsSizeDisp())
	{ // 演出の開始タイミングの場合

		// 演出を開始する
		m_pBalloonManager->SetStag();
	}

	// 遷移ポイントの更新
	m_pBalloonManager->Update(fDeltaTime);
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
//	位置の設定処理
//============================================================
void CTransPoint::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 自身の位置を設定
	CObjectModel::SetVec3Position(rPos);

	// 吹き出しの位置を設定
	m_pBalloon->SetVec3Position(rPos);
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

		// エフェクトを設定
		pTransPoint->m_pEffectData = GET_EFFECT->Create(UNHIT_EFFECT_PASS, rPos + OFFSET, VEC3_ZERO, VEC3_ZERO, 50.0f, true,false);

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

	CTransPoint* pHitTransPoint = nullptr;	// ヒットした遷移ポイント
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
		if (bHit && pHitTransPoint == nullptr)
		{ // 当たっている且つまだどの遷移ポイントとも当たっていない場合

			// 当たっている遷移ポイントを保存
			pHitTransPoint = rList;

			// 吹き出し表示をONにする
			rList->m_pBalloon->SetDisp(true);

			if (rList->m_pEffectData->Path != HIT_EFFECT_PASS)
			{ // ヒット時のエフェクトではない場合

				// ヒット時のエフェクトを設定
				SAFE_DELETE(rList->m_pEffectData);
				rList->m_pEffectData = GET_EFFECT->Create(HIT_EFFECT_PASS, posTrans + OFFSET, VEC3_ZERO, VEC3_ZERO, 75.0f, true,false);

				// ステージ情報テクスチャの作成
				if (FAILED(rList->CreateStageTexture()))
				{ // 生成に失敗した場合

					// 失敗を返す
					assert(false);
					return nullptr;
				}
			}

			if (!rList->m_bOpen)
			{ // 解放されていない場合

				// 入っちゃダメなのでnullに戻す
				pHitTransPoint = nullptr;
			}
		}
		else
		{ // 当たっていない場合

			// 吹き出し表示をOFFにする
			rList->m_pBalloon->SetDisp(false);

			if (rList->m_pEffectData->Path != UNHIT_EFFECT_PASS)
			{ // 未ヒット時のエフェクトではない場合

				// 未ヒット時のエフェクトを設定
				SAFE_DELETE(rList->m_pEffectData);
				rList->m_pEffectData = GET_EFFECT->Create(UNHIT_EFFECT_PASS, posTrans + OFFSET, VEC3_ZERO, VEC3_ZERO, 50.0f, true,false);
			}
		}
	}

	// 最終的に当たっている遷移ポイントを返す
	return pHitTransPoint;
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
//	ステージ情報テクスチャの作成処理
//============================================================
HRESULT CTransPoint::CreateStageTexture(void)
{
	// 吹き出しマネージャーの終了
	SAFE_UNINIT(m_pBalloonManager);

	// 吹き出しマネージャーの生成
	m_pBalloonManager = CBalloonManager::Create(this, m_bOpen);
	if (m_pBalloonManager == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
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
