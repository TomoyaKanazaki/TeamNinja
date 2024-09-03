//============================================================
//
//	壁処理 [wall.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "wall.h"
#include "manager.h"
#include "collision.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"
#include "camera.h"
#include "player.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// テクスチャファイル
	{
		"data\\TEXTURE\\FIELD\\soil005.png",	// 土テクスチャ
		"data\\TEXTURE\\FIELD\\soil005.png",	// 草土テクスチャ
		"data\\TEXTURE\\FIELD\\soil002.png",	// 草テクスチャ
		"data\\TEXTURE\\FIELD\\water.png",		// 水テクスチャ
		"data\\TEXTURE\\FIELD\\plaster00.jpg",	// 城の漆喰テクスチャ
		"data\\TEXTURE\\FIELD\\OldWood001.jpg",	// 木テクスチャ
	};

	const int PRIORITY = 4;	// 壁の優先順位
	const float INVISIBLE_MIN = 300.0f; // 透明化最小範囲
	const float INVISIBLE_MAX = 500.0f; // 透明化最小範囲
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CWall::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CWall> *CWall::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CWall] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CWall::CWall() : CObjectMeshWall(CObject::LABEL_WALL, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
	m_type	(TYPE_SOIL),	// 種類
	m_posVtx(nullptr)
{

}

//============================================================
//	デストラクタ
//============================================================
CWall::~CWall()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CWall::Init(void)
{
	// メンバ変数を初期化
	m_type = TYPE_SOIL;	// 種類
	m_posVtx = nullptr;

	// オブジェクトメッシュウォールの初期化
	if (FAILED(CObjectMeshWall::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CWall>::Create();
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
void CWall::Uninit(void)
{
	// 頂点情報の破棄
	if (m_posVtx != nullptr)
	{
		delete[] m_posVtx;
	}

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトメッシュウォールの終了
	CObjectMeshWall::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CWall::Update(const float fDeltaTime)
{
	// 頂点情報の設定
	//Invisible();

	// オブジェクトメッシュウォールの更新
	CObjectMeshWall::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CWall::Draw(CShader *pShader)
{
	// オブジェクトメッシュウォールの描画
	CObjectMeshWall::Draw(pShader);
}

//============================================================
// 当たり判定処理
//============================================================
bool CWall::Collision
(
	D3DXVECTOR3& rPos,		// 位置
	D3DXVECTOR3& rPosOld,	// 前回の位置
	const float fRadius,	// 半径
	const float fHeight,	// 高さ
	D3DXVECTOR3& rMove,		// 移動量
	bool* pJump				// ジャンプ状況
)
{
	EAngle angle = useful::RotToFourDire(GetVec3Rotation().y);		// 向き
	D3DXVECTOR3 PlayerUp = D3DXVECTOR3(fRadius, fHeight, fRadius);	// プレイヤーのサイズ(上)
	D3DXVECTOR3 PlayerDown = D3DXVECTOR3(fRadius, 0.0f, fRadius);	// プレイヤーのサイズ(下)
	D3DXVECTOR3 sizeUp = VEC3_ZERO;			// プレイヤーのサイズ(上)
	D3DXVECTOR3 sizeDown = VEC3_ZERO;		// プレイヤーのサイズ(下)
	bool bUp = false;		// 上状況
	bool bHit = false;		// ヒット状況

	// サイズを設定
	sizeUp.x = GetVec2Sizing().x * 0.5f;
	sizeUp.y = GetVec2Sizing().y;
	sizeUp.z = 0.0f;
	sizeDown.x = GetVec2Sizing().x * 0.5f;
	sizeDown.y = 0.0f;
	sizeDown.z = 0.0f;

	// 当たり判定処理(向きの列挙判定入り)
	bHit = collision::ResponseBox3D
	(
		rPos,				// プレイヤーの位置
		rPosOld,			// プレイヤーの前回の位置
		GetVec3Position(),	// 壁の位置
		PlayerUp,			// プレイヤーのサイズ(上)
		PlayerDown,			// プレイヤーのサイズ(下)
		sizeUp,				// 壁のサイズ(上)
		sizeDown,			// 壁のサイズ(下)
		angle,				// 方向
		&rMove,				// 移動量
		&bUp				// 上状況
	);

	if (bUp &&
		pJump != nullptr)
	{ // 上に乗ったかつ、ジャンプ状況が NULL じゃない場合

		// ジャンプ状況を設定する
		*pJump = false;
	}

	// ヒット状況を返す
	return bHit;
}

//============================================================
//	生成処理
//============================================================
CWall *CWall::Create
(
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR2& rSize,	// 大きさ
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const POSGRID2& rTexPart	// テクスチャ分割数
)
{
	// 壁の生成
	CWall *pWall = new CWall;
	if (pWall == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 壁の初期化
		if (FAILED(pWall->Init()))
		{ // 初期化に失敗した場合

			// 壁の破棄
			SAFE_DELETE(pWall);
			return nullptr;
		}

		// 種類を設定
		pWall->SetType(type);

		// 位置を設定
		pWall->SetVec3Position(rPos);

		// 向きを設定
		pWall->SetVec3Rotation(rRot);

		// 大きさを設定
		pWall->SetVec2Sizing(rSize);

		// 色を設定
		pWall->SetColor(rCol);

		// 分割数を設定
		if (FAILED(pWall->SetPattern(rTexPart)))
		{ // 分割数の設定に失敗した場合

			// 壁の破棄
			SAFE_DELETE(pWall);
			return nullptr;
		}

		// テクスチャ分割数を設定
		pWall->SetTexPattern(rTexPart);

		// 頂点情報を計算を行う
		pWall->CalcVtx();

		// 確保したアドレスを返す
		return pWall;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CWall> *CWall::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	種類の設定処理
//============================================================
void CWall::SetType(const EType type)
{
	if (type > NONE_IDX && type < TYPE_MAX)
	{ // インデックスが範囲内の場合

		// 種類を保存
		m_type = type;

		// テクスチャを登録・割当
		BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[type]));
	}
	else { assert(false); }	// 範囲外
}

//==========================================
//  頂点情報の設定
//==========================================
void CWall::Invisible()
{
	// カメラ情報の取得
	CCamera* pCamera = GET_CAMERA;

	// 回り込み状態でなければ関数を抜ける
	if (pCamera->GetState() != CCamera::STATE_AROUND) { return; }

	// 自身の情報を取得する
	POSGRID2 part = GetPattern();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff == nullptr) { return; }
	SMeshWall meshWall = GetMeshWall();
	D3DXCOLOR col = meshWall.col;

	// カメラの情報を取得する
	D3DXVECTOR3 posV = pCamera->GetPositionV();
	float fRotCamera = pCamera->GetRotation().y;

	// カメラと壁の方向が一致しない場合関数を抜ける
	EAngle angleCamera = useful::RotToFourDire(fRotCamera);
	EAngle angleWall = useful::RotToFourDire(meshWall.rot.y);
	if ((int)angleCamera + (int)angleWall % 2)
	{
		return;
	}

	// プレイヤーの座標を取得
	D3DXVECTOR3 posPlayerScreen = pCamera->CalcPlayerPos();
	D3DXVECTOR3 posPlayerWorld = GET_PLAYER->GetCenterPos();

	// 頂点情報へのポインタ宣言
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeight = 0; nCntHeight < part.y + 1; nCntHeight++) // 縦
	{
		for (int nCntWidth = 0; nCntWidth < part.x + 1; nCntWidth++) // 横
		{
			// xz平面上における視点との距離を算出
			D3DXVECTOR3 vecPlayer = posPlayerWorld - posV;
			D3DXVECTOR3 vecVtx = m_posVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))] - posV;

			// 頂点座標がプレイヤーよりも遠い位置にある場合1.0を返す
			if (vecPlayer.x * vecPlayer.x + vecPlayer.z * vecPlayer.z < vecVtx.x * vecVtx.x + vecVtx.z * vecVtx.z)
			{
				col.a = 1.0f;
				pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].col = col;
				continue;
			}

			// 頂点のスクリーン座標を算出
			D3DXVECTOR3 pos = VEC3_ZERO;
			if (!pCamera->OnScreen(m_posVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))], pos))
			{
				// 画面外の場合1.0を返す
				col.a = 1.0f;
				pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].col = col;
				continue;
			}

			// プレイヤーよりも奥に描画される場合1.0を返す
			if (pos.z >= posPlayerScreen.z)
			{
				col.a = 1.0f;
				pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].col = col;
				continue;
			}

			// プレイヤーとの距離^2を算出
			D3DXVECTOR3 vec = posPlayerScreen - pos;
			float fLength = sqrtf(vec.x * vec.x + vec.y * vec.y);

			// プレイヤーとの距離が近い場合は0を返す
			if (fLength < INVISIBLE_MIN)
			{
				col.a = 0.0f;
				pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].col = col;
				continue;
			}

			// 透過範囲内の割合を算出し返す
			col.a = (fLength - INVISIBLE_MIN) / (INVISIBLE_MAX - INVISIBLE_MIN);
			pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].col = col;
		}
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//==========================================
//  頂点座標の計算処理
//==========================================
void CWall::CalcVtx()
{
	// 自身の情報を取得
	SMeshWall meshWall = GetMeshWall();
	POSGRID2 part = GetPattern();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff == nullptr) { return; }

	// 頂点数分のメモリを確保する
	m_posVtx = new D3DXVECTOR3[(part.x + 1) * (part.y + 1)];

	//  計算用マトリックスの宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	// ポインタを宣言
	VERTEX_3D* pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeight = 0; nCntHeight < part.y + 1; nCntHeight++) // 縦
	{
		for (int nCntWidth = 0; nCntWidth < part.x + 1; nCntWidth++) // 横
		{ 
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&meshWall.mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, meshWall.rot.y, meshWall.rot.x, meshWall.rot.z);
			D3DXMatrixMultiply(&meshWall.mtxWorld, &meshWall.mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation
			(
				&mtxTrans,
				pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].pos.x + meshWall.pos.x,
				pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].pos.y + meshWall.pos.y,
				pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].pos.z + meshWall.pos.z
			);
			D3DXMatrixMultiply(&meshWall.mtxWorld, &meshWall.mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &meshWall.mtxWorld);

			// マトリックスから頂点座標を抽出
			m_posVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))] = D3DXVECTOR3(meshWall.mtxWorld._41, meshWall.mtxWorld._42, meshWall.mtxWorld._43);
		}
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}
