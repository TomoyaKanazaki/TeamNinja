//===========================================
//
//  植物管理クラス(multi_plant.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "multi_plant.h"
#include "plant.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	// TODO ちゃんとした植物のテクスチャ用意する
	const char* TEXTURE_FILE[] = // テクスチャファイル
	{
		"data\\TEXTURE\\test.png", // テストテクスチャ
	};

	const float PERMISSION = 0.02f; // 重なりの許容範囲
}

//===========================================
//  静的警告処理
//===========================================
static_assert(NUM_ARRAY(TEXTURE_FILE) == CMultiPlant::TYPE_MAX, "ERROR : Type Count Mismatch");

//===========================================
//  コンストラクタ
//===========================================
CMultiPlant::CMultiPlant() : CObject3D(),
m_Type(CMultiPlant::TYPE_MAX),
m_pos(VEC3_ZERO),
m_size(VEC3_ZERO),
m_nNum(0),
m_bGrow(false)
{
}

//===========================================
//  デストラクタ
//===========================================
CMultiPlant::~CMultiPlant()
{
}

//===========================================
//  初期化処理
//===========================================
HRESULT CMultiPlant::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CObject3D::Init())){ assert(false); return E_FAIL; }

	// 成功を返す
	return S_OK;
}

//===========================================
//  終了処理
//===========================================
void CMultiPlant::Uninit(void)
{
	// 親クラスの終了
	CObject3D::Uninit();
}

//===========================================
//  更新処理
//===========================================
void CMultiPlant::Update(const float fDeltaTime)
{
	// 植物の生成
	if (!m_bGrow) { Grow(); }

	// 親クラスの更新
	CObject3D::Update(fDeltaTime);
}

//===========================================
//  描画処理
//===========================================
void CMultiPlant::Draw(CShader* pShader)
{
	// 親クラスの描画
	CObject3D::Draw(pShader);
}

//===========================================
//  生成処理
//===========================================
CMultiPlant* CMultiPlant::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const EType type, int nNum)
{
	// ギミックの生成
	CMultiPlant* pPlant = new CMultiPlant;

	// メモリの確保に失敗した場合関数を抜ける
	if (pPlant == nullptr) { assert(false); return nullptr; }

	// 初期化
	if (FAILED(pPlant->Init())) { assert(false); SAFE_DELETE(pPlant); return nullptr; }

	// 位置を設定
	pPlant->SetVec3Position(rPos);

	// サイズを設定
	pPlant->SetVec3Sizing(rSize);

	// タイプの設定
	pPlant->m_Type = type;

	// 生成数の設定
	pPlant->m_nNum = nNum;

	// 確保したアドレスを返す
	return pPlant;
}

//===========================================
//  花の生成
//===========================================
void CMultiPlant::Grow()
{
	// 生成フラグを立てる
	m_bGrow = true;

	// オブジェクトの生成座標保存用変数
	D3DXVECTOR2* pos = new D3DXVECTOR2[m_nNum];
	for (int i = 0; i < m_nNum; ++i) { pos[i] = VEC2_ZERO; }

	// 必要な数生成する
	for (int i = 0; i < m_nNum; ++i)
	{
		// x座標を設定
		while (1)
		{
			// 乱数生成
			float fTemp = ((float)rand() / (RAND_MAX * 0.5f)) - 1.0f;

			// 重ならないようにする
			bool bHit = false;
			for (int j = 0; j < m_nNum; ++j)
			{
				// 重なりすぎた場合ループを抜ける
				if (pos[j].x - fTemp < PERMISSION)
				{
					bHit = true;
					break;
				}
			}

			// 重なっていた場合もう一度
			if (bHit) { continue; }

			// 乱数を保存する
			pos[i].x = fTemp;

			// ループを抜ける
			break;
		}

		// y座標を設定
		while (1)
		{
			// 乱数生成
			float fTemp = ((float)rand() / (RAND_MAX * 0.5f)) - 1.0f;

			// 重ならないようにする
			bool bHit = false;
			for (int j = 0; j < m_nNum; ++j)
			{
				// 重なりすぎた場合ループを抜ける
				if (pos[j].y - fTemp < PERMISSION)
				{
					bHit = true;
					break;
				}
			}

			// 重なっていた場合もう一度
			if (bHit) { continue; }

			// 乱数を保存する
			pos[i].y = fTemp;

			// ループを抜ける
			break;
		}

		// 生成した乱数から座標を算出
		D3DXVECTOR3 posPlant = D3DXVECTOR3
		(
			m_pos.x + (m_size.x * pos[i].x),
			m_pos.y,
			m_pos.z + (m_size.z * pos[i].y)
		);

		// 植物を生成
		CPlant::Create(posPlant, TEXTURE_FILE[m_Type]);
	}

	// メモリの解放
	delete[] pos;
}
