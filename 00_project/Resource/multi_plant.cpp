//===========================================
//
//  植物管理クラス(multi_plant.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "multi_plant.h"
#include "plant.h"
#include "manager.h"
#include "camera.h"
//===========================================
//  定数定義
//===========================================
namespace
{
	// TODO ちゃんとした植物のテクスチャ用意する
	const char* TEXTURE_FILE[] = // テクスチャファイル
	{
		"data\\TEXTURE\\Flower001.png",	// チューリップ
		"data\\TEXTURE\\Flower001.png",	// テストテクスチャ
		"data\\TEXTURE\\Flower001.png",	// テストテクスチャ
		"data\\TEXTURE\\Flower001.png",	// テストテクスチャ
		"data\\TEXTURE\\Flower001.png",	// テストテクスチャ
		"data\\TEXTURE\\Flower001.png",	// テストテクスチャ
	};

	const float PERMISSION = 0.05f; // 重なりの許容範囲
	const float SIZE_SCALE = 0.3f; // 生成範囲の倍率

	const float FRAME_SIZE = 2.0f; // 縁取り
}

//===========================================
//  静的警告処理
//===========================================
static_assert(NUM_ARRAY(TEXTURE_FILE) == CGimmick::TYPE_MAX, "ERROR : Type Count Mismatch");

//===========================================
//  コンストラクタ
//===========================================
CMultiPlant::CMultiPlant() : CObject3D(),
m_Type(CGimmick::TYPE_MAX),
m_nNum(0),
m_bGrow(false)
{
	memset(&m_pFlame[0], 0, sizeof(m_pFlame));
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

	// ラベルの変更
	SetLabel(LABEL_GIMMICK);

	// 描画設定の変更
	SetEnableDraw(false);

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
	// 縁取りの色を更新
	FrameAlpha();

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
CMultiPlant* CMultiPlant::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const CGimmick::EType type, int nNum)
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

	// 縁取りの生成
	pPlant->FrameCreate();

	// 花の生成
	pPlant->Grow();

	// 確保したアドレスを返す
	return pPlant;
}

//===========================================
//  縁取りの透明化
//===========================================
void CMultiPlant::FrameAlpha()
{
	// 自身のスクリーン座標を取得
	D3DXVECTOR3 posScreen = VEC3_ZERO;
	GET_CAMERA->OnScreen(GetVec3Position(), posScreen);

	// スクリーンの中心と結ぶベクトルの大きさ ^ 2を算出
	D3DXVECTOR3 vecCenter = SCREEN_CENT - posScreen;
	float fScalar = vecCenter.x * vecCenter.x + vecCenter.y * vecCenter.y;

	// スクリーンの高さ * 0.5に対する割合の算出
	float fScale = fScalar / (SCREEN_CENT.y * SCREEN_CENT.y);

	// 縁取りの透明度を更新
	for (int i = 0; i < 4; ++i)
	{
		// 生成されていない場合次に進む
		if (m_pFlame[i] == nullptr) { continue; }

		// Scaleが0以下の場合描画しない
		if (fScale <= 0.0) { m_pFlame[i]->SetEnableDraw(false); continue; }
		else { m_pFlame[i]->SetEnableDraw(true); }

		// 透明度を更新する
		m_pFlame[i]->SetAlpha(1.0f - fScale);
	}
}

//===========================================
//  縁取りの表示
//===========================================
void CMultiPlant::FrameCreate()
{
	// 各頂点の座標を取得
	D3DXVECTOR3 pos = GetVec3Position();
	D3DXVECTOR3 posVtx[4] =
	{
		GetVertexPosition(0) + pos,
		GetVertexPosition(1) + pos,
		GetVertexPosition(2) + pos,
		GetVertexPosition(3) + pos
	};

	// 各頂点を結ぶベクトルを算出
	D3DXVECTOR3 vecVtx[4] =
	{
		(posVtx[1] - posVtx[0]),
		(posVtx[3] - posVtx[1]),
		(posVtx[0] - posVtx[2]),
		(posVtx[2] - posVtx[3])
	};

	// 縁を生成
	for (int i = 0; i < 4; ++i)
	{
		// 辺の情報を設定
		D3DXVECTOR3 size = vecVtx[i];
		if (size.x == 0.0f) { size.x = FRAME_SIZE; size.z = fabsf(size.z); }
		if (size.z == 0.0f) { size.z = FRAME_SIZE; size.x = fabsf(size.x); }

		// 辺を生成
		m_pFlame[i] = CObject3D::Create(posVtx[i] + (vecVtx[i] * 0.5f), size);
		m_pFlame[i]->SetColor(XCOL_CYAN);
		m_pFlame[i]->SetLabel(CObject::LABEL_GIMMICK);
	}
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
				if (fabsf(pos[j].x - fTemp) < PERMISSION)
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
				if (fabsf(pos[j].y - fTemp) < PERMISSION)
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
		D3DXVECTOR3 posThis = GetVec3Position();
		D3DXVECTOR3 sizeThis = GetVec3Sizing() * SIZE_SCALE;
		D3DXVECTOR3 posPlant = D3DXVECTOR3
		(
			posThis.x + (sizeThis.x * pos[i].x),
			posThis.y,
			posThis.z + (sizeThis.z * pos[i].y)
		);

		// 植物を生成
		CPlant::Create(posPlant, TEXTURE_FILE[m_Type]);
	}

	// メモリの解放
	delete[] pos;
}
