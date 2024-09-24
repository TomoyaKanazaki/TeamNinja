//============================================================
//
//	キューブの当たり判定処理 [collisionCube.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "collisionCube.h"
#include "collision.h"

//************************************************************
// 定数定義
//************************************************************
namespace
{
	const int CUBE_PRIORITY = 5;			// キューブの優先順位
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// 色
	const float ROT_TOLERANCE = 0.02f;		// 向きの許容範囲
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CCollisionCube>* CCollisionCube::m_pList = nullptr;	// オブジェクトリスト

//============================================================
// コンストラクタ
//============================================================
CCollisionCube::CCollisionCube() : CCollision(),
m_fWidth(0.0f),		// 幅
m_fHeight(0.0f),	// 高さ
m_fDepth(0.0f)		// 奥行
#ifdef _DEBUG
, m_pCube(nullptr)	// メッシュキューブ
#endif // _DEBUG

{

}

//============================================================
// デストラクタ
//============================================================
CCollisionCube::~CCollisionCube()
{

}

//============================================================
// 初期化処理
//============================================================
void CCollisionCube::Init(void)
{
	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CCollisionCube>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);
}

//============================================================
// 終了処理
//============================================================
void CCollisionCube::Uninit(void)
{
#ifdef _DEBUG

	// メッシュキューブの終了
	SAFE_UNINIT(m_pCube);

#endif // _DEBUG

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 終了処理
	CCollision::Uninit();
}

//============================================================
// ヒット処理
//============================================================
bool CCollisionCube::Hit
(
	D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rPosOld,		// 前回の位置
	const float fRadius,			// 半径
	const float fHeight,			// 高さ
	D3DXVECTOR3& rMove,				// 移動量
	bool& rJump						// ジャンプ状況
)
{
	// サイズ
	D3DXVECTOR3 centerSizeUp = D3DXVECTOR3(fRadius, fHeight, fRadius);
	D3DXVECTOR3 centerSizeDown = D3DXVECTOR3(fRadius, 0.0f, fRadius);
	D3DXVECTOR3 targetSizeUp = D3DXVECTOR3(m_fWidth, m_fHeight * 2.0f, m_fDepth);
	D3DXVECTOR3 targetSizeDown = D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth);
	D3DXVECTOR3 posOld = rPosOld;
	D3DXVECTOR3 move = rMove;
	bool bUp = false;

	if (collision::ResponseBox3D(rPos, posOld, GetPos(), centerSizeUp, centerSizeDown, targetSizeUp, targetSizeDown, &move, &bUp))
	{ // 当たった場合

		// 移動量とジャンプ状況を設定する
		rMove = move;

		if (bUp == true)
		{ // 上に乗っている場合

			// ジャンプ状況を false にする
			rJump = false;
		}

		// true を返す
		return true;
	}

	// false を返す
	return false;
}

//============================================================
// オフセット設定処理
//============================================================
void CCollisionCube::OffSet(const D3DXMATRIX& mtx)
{
	// オフセット処理
	CCollision::OffSet(mtx);

#ifdef _DEBUG

	// キューブの位置を設定
	m_pCube->SetVec3Position(GetPos());

#endif // _DEBUG
}

//============================================================
// 生成処理
//============================================================
CCollisionCube* CCollisionCube::Create
(
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rOffset,		// オフセット
	const float fWidth,				// 幅
	const float fHeight,			// 高さ
	const float fDepth,				// 奥行
	const float fRot				// 向き
)
{
	// 当たり判定の生成
	CCollisionCube* pColl = new CCollisionCube();

	// 生成出来ていない場合 nullptr を返す
	if (pColl == nullptr) { return nullptr; }

	// 初期化処理
	pColl->Init();

	// 位置を設定
	pColl->SetPos(rPos);

	// オフセットの設定
	pColl->SetOffset(rOffset);

	// 向きによる変換処理
	if (pColl->Convert(fWidth, fDepth, fRot))
	{
		std::string str;

		str += std::to_string(rPos.x);
		str += "　　";
		str += std::to_string(rPos.y);
		str += "　　";
		str += std::to_string(rPos.z);

		// エラーメッセージボックス
		MessageBox(nullptr, "キューブの当たり判定あるのに角度おかしいよ。", "警告！", MB_ICONWARNING);
		MessageBox(nullptr, str.c_str(), "警告！", MB_ICONWARNING);
	}

	// 高さを設定
	pColl->m_fHeight = fHeight;

#ifdef _DEBUG

	// キューブを生成
	pColl->m_pCube = CObjectMeshCube::Create
	(
		rPos,
		VEC3_ZERO,
		D3DXVECTOR3(pColl->m_fWidth, pColl->m_fHeight, pColl->m_fDepth),
		COL,
		XCOL_BLACK,
		CObjectMeshCube::BORDER_OFF,
		0.0f,
		CObjectMeshCube::TEXSTATE_ONE,
		CObjectMeshCube::SFaceTex(NONE_IDX),
		CObjectMeshCube::ORIGIN_DOWN
	);
	pColl->m_pCube->SetPriority(CUBE_PRIORITY);
	pColl->m_pCube->SetLabel(CObject::LABEL_COLLISION);

#endif // _DEBUG

	// 当たり判定を返す
	return pColl;
}

//============================================================
// リスト取得
//============================================================
CListManager<CCollisionCube>* CCollisionCube::GetList(void)
{
	// リスト構造を返す
	return m_pList;
}

//============================================================
// 向きによる変換処理
//============================================================
bool CCollisionCube::Convert(const float fWidth, const float fDepth, const float fRot)
{	
	bool b = false;

#ifdef _DEBUG

	// 向きの警告処理
	b = RotWarning(fRot);

#endif // _DEBUG

	if ((fRot >= D3DX_PI * -0.75f && fRot <= D3DX_PI * -0.25f) ||
		(fRot >= D3DX_PI * 0.25f && fRot <= D3DX_PI * 0.75f))
	{ // 90度、270度の場合

		// 半径を設定
		m_fWidth = fDepth;

		// 奥行を設定
		m_fDepth = fWidth;
	}
	else
	{ // 上記以外

		// 半径を設定
		m_fWidth = fWidth;

		// 奥行を設定
		m_fDepth = fDepth;
	}

	return b;
}

#ifdef _DEBUG
//============================================================
// 向きの警告処理
//============================================================
bool CCollisionCube::RotWarning(const float fRot)
{
	bool bOver = false;		// 範囲外の向き

	if (fRot >= -D3DX_PI * 0.25f &&
		fRot <= D3DX_PI * 0.25f)
	{ // 0度補正前提で誤差が大きすぎる場合

		if (fabsf(fRot) >= ROT_TOLERANCE)
		{ // 向きの許容範囲を超えた場合

			// 範囲外判定を出す
			bOver = true;
		}
	}
	else if (fRot >= D3DX_PI * 0.25f &&
		fRot <= D3DX_PI * 0.75f)
	{ // 90度補正前提で誤差が大きすぎる場合

		if (fabsf(fRot - (D3DX_PI * 0.5f)) >= ROT_TOLERANCE)
		{ // 向きの許容範囲を超えた場合

			// 範囲外判定を出す
			bOver = true;
		}
	}
	else if ((fRot >= D3DX_PI * -0.75f &&
		fRot <= D3DX_PI * -0.25f))
	{ // 270度補正前提で誤差が大きすぎる場合

		if (fabsf(fRot + (D3DX_PI * 0.5f)) >= ROT_TOLERANCE)
		{ // 向きの許容範囲を超えた場合

			// 範囲外判定を出す
			bOver = true;
		}
	}
	else
	{
		if (fabsf(fRot - D3DX_PI) >= ROT_TOLERANCE &&
			fabsf(fRot + D3DX_PI) >= ROT_TOLERANCE)
		{  // 向きの許容範囲を超えた場合

			// 範囲外判定を出す
			bOver = true;
		}
	}

	// 範囲外判定を出す
	return bOver;
}

#endif