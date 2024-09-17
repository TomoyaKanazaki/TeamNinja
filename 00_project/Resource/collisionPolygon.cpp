//============================================================
//
//	ポリゴンの当たり判定処理 [collisionPolygon.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "collisionPolygon.h"
#include "collision.h"

//************************************************************
// 定数定義
//************************************************************
namespace
{
	const int POLYGON_PRIORITY = 5;			// ポリゴンの優先順位
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// 色
	const float GRAVITY = -250.0f;			// 立った時の重力
	const float ROT_TOLERANCE = 0.02f;		// 向きの許容範囲
}

//============================================================
// コンストラクタ
//============================================================
CCollisionPolygon::CCollisionPolygon() : CCollision(),
m_rot(VEC3_ZERO),		// 幅
m_size(VEC3_ZERO),		// 高さ
m_pPolygon(nullptr)		// 当たり判定ポリゴン
{

}

//============================================================
// デストラクタ
//============================================================
CCollisionPolygon::~CCollisionPolygon()
{

}

//============================================================
// 終了処理
//============================================================
void CCollisionPolygon::Uninit(void)
{
	// メッシュポリゴンの終了
	SAFE_UNINIT(m_pPolygon);

	// 終了処理
	CCollision::Uninit();
}

//============================================================
// ヒット処理
//============================================================
bool CCollisionPolygon::Hit
(
	D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rPosOld,		// 前回の位置
	const float fRadius,			// 半径
	const float fHeight,			// 高さ
	D3DXVECTOR3& rMove,				// 移動量
	bool& rJump						// ジャンプ状況
)
{
	// 高さを取得する
	float fPosY = m_pPolygon->GetPositionHeight(rPos);

	if (m_pPolygon->GetInside(rPos))
	{ // 内側にいる場合

		if (fPosY > rPos.y &&
			fPosY < rPos.y + fHeight)
		{ // 現在地がポリゴンより高かった場合

			// Y軸の位置を適用する
			rPos.y = fPosY;

			// 重力を下にかけ続ける
			rMove.y = GRAVITY;

			// ジャンプ状況を false にする
			rJump = false;

			// true を返す
			return true;
		}
	}
	else
	{ // 上記以外

	}

	// false を返す
	return false;
}

//============================================================
// オフセット設定処理
//============================================================
void CCollisionPolygon::OffSet(const D3DXMATRIX& mtx)
{
	// オフセット処理
	CCollision::OffSet(mtx);

	// ポリゴンの位置を設定
	m_pPolygon->SetVec3Position(GetPos());
}

//============================================================
// 生成処理
//============================================================
CCollisionPolygon* CCollisionPolygon::Create
(
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rOffset,		// オフセット
	const D3DXVECTOR3& rRot,		// 向き
	const D3DXVECTOR3& rSize		// サイズ
)
{
	// 当たり判定の生成
	CCollisionPolygon* pColl = new CCollisionPolygon();

	// 生成出来ていない場合 nullptr を返す
	if (pColl == nullptr) { return nullptr; }

	// 位置を設定
	pColl->SetPos(rPos);

	// オフセットの設定
	pColl->SetOffset(rOffset);

	// 向きを設定
	pColl->m_rot = rRot;

	// サイズを設定
	pColl->m_size = rSize;

	// ポリゴンを生成
	pColl->m_pPolygon = CObject3D::Create
	(
		rPos,
		pColl->m_size,
		pColl->m_rot,
		COL
	);
	pColl->m_pPolygon->SetPriority(POLYGON_PRIORITY);
	pColl->m_pPolygon->SetLabel(CObject::LABEL_COLLISION);

	// 向きによる変換処理
	if (pColl->Convert(rSize.x, rSize.z, rRot.y))
	{
		std::string str;

		str += std::to_string(rPos.x);
		str += "　　";
		str += std::to_string(rPos.y);
		str += "　　";
		str += std::to_string(rPos.z);

		// エラーメッセージボックス
		MessageBox(nullptr, "ポリゴンの当たり判定あるのに角度おかしいよ。", "警告！", MB_ICONWARNING);
		MessageBox(nullptr, str.c_str(), "警告！", MB_ICONWARNING);
	}

#ifndef _DEBUG

	// 描画を切る
	pColl->m_pPolygon->SetEnableDraw(false);

#endif // !_DEBUG

	// 当たり判定を返す
	return pColl;
}

//============================================================
// 向きによる変換処理
//============================================================
bool CCollisionPolygon::Convert(const float fWidth, const float fDepth, const float fRot)
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
		m_size.x = fDepth;

		// 奥行を設定
		m_size.z = fWidth;
	}
	else
	{ // 上記以外

		// 半径を設定
		m_size.x = fWidth;

		// 奥行を設定
		m_size.z = fDepth;
	}

	return b;
}


#ifdef _DEBUG
//============================================================
// 向きの警告処理
//============================================================
bool CCollisionPolygon::RotWarning(const float fRot)
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