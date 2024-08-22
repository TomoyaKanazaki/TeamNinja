//============================================================
//
//	草処理 [weed.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "weed.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "collision.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY		= 6;	// 草表示の優先順位
	const int ALPHA_NUMREF	= 120;	// αテストの参照値
}

//************************************************************
//	親クラス [CWeed] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CWeed::CWeed() : CObject3D(CObject::LABEL_WEED, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
	m_fCurLength	(0.0f),	// 現在の距離
	m_fDestLength	(0.0f),	// 目標の距離
	m_fCurAngle		(0.0f),	// 現在の角度
	m_fDestAngle	(0.0f)	// 目標の角度
{

}

//============================================================
//	デストラクタ
//============================================================
CWeed::~CWeed()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CWeed::Init(void)
{
	// メンバ変数を初期化
	m_fCurLength	= 0.0f;	// 現在の距離
	m_fDestLength	= 0.0f;	// 目標の距離
	m_fCurAngle		= 0.0f;	// 現在の角度
	m_fDestAngle	= 0.0f;	// 目標の角度

	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 草のテクスチャを割当
	BindTexture("data\\TEXTURE\\grass000.png");

	// 原点を下にする
	SetOrigin(CObject3D::ORIGIN_DOWN);

	// レンダーステートの情報を取得
	CRenderState *pRenderState = GetRenderState();

	// αテストの設定
	pRenderState->SetAlphaTest(true);			// αテストの有効 / 無効の設定
	pRenderState->SetAlphaFunc(D3DCMP_GREATER);	// αテストの設定
	pRenderState->SetAlphaNumRef(ALPHA_NUMREF);	// αテストの参照値設定

	// カリングモードを両面表示にする
	pRenderState->SetCulling(D3DCULL_NONE);

	// ライティングの影響を受けないようにする
	pRenderState->SetLighting(false);

	// 大きさを設定
	SetVec3Sizing(D3DXVECTOR3(100.0f, 100.0f, 0.0f));	// TODO

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CWeed::Uninit(void)
{
	// オブジェクト3Dの終了
	CObject3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CWeed::Update(const float fDeltaTime)
{
	// プレイヤーとの当たり判定
	CollisionPlayer();

	// オブジェクト3Dの更新
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CWeed::Draw(CShader *pShader)
{
	// オブジェクト3Dの描画
	CObject3D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CWeed *CWeed::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot		// 向き
)
{
	// 草の生成
	CWeed *pWeed = new CWeed;
	if (pWeed == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 草の初期化
		if (FAILED(pWeed->Init()))
		{ // 初期化に失敗した場合

			// 草の破棄
			SAFE_DELETE(pWeed);
			return nullptr;
		}

		// 位置を設定
		pWeed->SetVec3Position(rPos);

		// 向きを設定
		pWeed->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pWeed;
	}
}

//============================================================
//	プレイヤーとの当たり判定
//============================================================
void CWeed::CollisionPlayer(void)
{
	CPlayer* pPlayer = GET_PLAYER;	// プレイヤー情報

	// プレイヤーがいない場合抜ける
	if (pPlayer == nullptr) { return; }

	// プレイヤーとの接触判定
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 posWeed = GetVec3Position();			// 草位置
	float fRadPlayer = pPlayer->GetRadius();			// プレイヤー半径
	float fRadWeed = GetVec3Sizing().x;					// 草半径
	if (collision::Circle3D(posPlayer, posWeed, fRadPlayer, fRadWeed))
	{ // 踏んでいた場合

		// プレイヤー間の距離を求める
		float fDistance = sqrtf((posPlayer.x - posWeed.x) * (posPlayer.x - posWeed.x) + (posPlayer.z - posWeed.z) * (posPlayer.z - posWeed.z));

		// 草の先端をずらす距離を求める
		m_fDestLength = fabsf(fDistance - (fRadPlayer + fRadWeed));
		useful::LimitMaxNum(m_fDestLength, (fRadPlayer + fRadWeed) - 60.0f);

		// プレイヤー方向を求める
		m_fDestAngle = atan2f(posWeed.x - posPlayer.x, posWeed.z - posPlayer.z);
	}
	else
	{ // 踏んでいない場合

		// 踏まれていないので初期値に戻す
		m_fDestLength = 0.0f;	// 目標の距離
		m_fDestAngle = 0.0f;	// 目標の角度
	}

	// 現在の距離を求める
	float fDiffLength = m_fDestLength - m_fCurLength;	// 差分の距離
	m_fCurLength += fDiffLength * 0.5f;					// 距離を目標に近づける

	// 現在の角度を求める
	float fDiffAngle = m_fDestAngle - m_fCurAngle;		// 差分の角度
	m_fCurAngle += fDiffAngle * 0.5f;					// 距離を目標に近づける

	// 上頂点移動オフセットを求める
	D3DXVECTOR3 offset = D3DXVECTOR3(m_fCurLength * sinf(m_fCurAngle), 0.0f, m_fCurLength * cosf(m_fCurAngle));

	// 座標のずらす量を設定
	SetGapPosition(0, offset);
	SetGapPosition(1, offset);
}
