//=========================================
//
//  設置ギミック (gimmick_post.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_post.h"
#include "manager.h"
#include "objectMeshCube.h"

#include "player.h"
#include "player_clone.h"

#include "directxmath.h"
#include "collision.h"

//=========================================
//  コンストラクタ
//=========================================
namespace
{
	const float BUTTON_OFF	= 15.0f;	// ボタンオフの時のオフセット
	const float BUTTON_ON	= 6.0f;		// ボタンオンの時のオフセット
	const float BUTTON_MOVE	= 60.0f;	// ボタン押込/戻し時の移動量

	const D3DXVECTOR3 SCALE_BUTTON	= D3DXVECTOR3(-5.0f, 0.0f, -5.0f);				// ボタン大きさのオフセット
	const D3DXVECTOR3 SCALE_EDGE	= D3DXVECTOR3(0.0f, BUTTON_ON - 1.0f, 0.0f);	// 縁取り大きさのオフセット

	const D3DXCOLOR COLOR_DEFAULT	= D3DXCOLOR(0.31372549f, 0.89803922f, 0.97647059f, 1.0f);	// 色
	const D3DXCOLOR COLOR_ON		= D3DXCOLOR(0.98823529f, 0.20392157f, 0.53333333f, 1.0f);	// 色
}

//=========================================
//  コンストラクタ
//=========================================
CGimmickPost::CGimmickPost() : CGimmickAction(),
m_pButton(nullptr), // ボタン
m_pEdge(nullptr), // 縁
m_fButtonHeight(BUTTON_OFF) // ボタン縦オフセット
{

}

//=========================================
//  デストラクタ
//=========================================
CGimmickPost::~CGimmickPost()
{

}

//=========================================
//  初期化処理
//=========================================
HRESULT CGimmickPost::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CGimmickAction::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ボタンの生成
	m_pButton = CObjectMeshCube::Create
	( // 引数
		VEC3_ZERO,	// 位置
		VEC3_ZERO,	// 向き
		VEC3_ZERO,	// 大きさ
		COLOR_DEFAULT,	// キューブ色
		XCOL_BLACK,	// 縁取り色
		CObjectMeshCube::BORDER_OFF,			// 縁取り状態
		0.0f,									// 縁取り太さ
		CObjectMeshCube::TEXSTATE_ONE,			// テクスチャ状態
		CObjectMeshCube::SFaceTex(NONE_IDX),	// テクスチャ種類
		CObjectMeshCube::ORIGIN_DOWN			// 原点
	);
	if (m_pButton == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ラベルをギミックにする
	m_pButton->SetLabel(CObject::LABEL_GIMMICK);

	// 縁の生成
	m_pEdge = CObjectMeshCube::Create
	( // 引数
		VEC3_ZERO,	// 位置
		VEC3_ZERO,	// 向き
		VEC3_ZERO,	// 大きさ
		XCOL_WHITE,	// キューブ色
		XCOL_BLACK,	// 縁取り色
		CObjectMeshCube::BORDER_OFF,			// 縁取り状態
		0.0f,									// 縁取り太さ
		CObjectMeshCube::TEXSTATE_ONE,			// テクスチャ状態
		CObjectMeshCube::SFaceTex(NONE_IDX),	// テクスチャ種類
		CObjectMeshCube::ORIGIN_DOWN			// 原点
	);
	if (m_pEdge == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ラベルをギミックにする
	m_pEdge->SetLabel(CObject::LABEL_GIMMICK);

	// 成功を返す
	return S_OK;
}

//=========================================
//  終了処理
//=========================================
void CGimmickPost::Uninit(void)
{
	// 親クラスの終了
	CGimmickAction::Uninit();
}

//=========================================
//  更新処理
//=========================================
void CGimmickPost::Update(const float fDeltaTime)
{
	// 遠距離判定
	bool bFar = useful::IsNearPosR(GetVec3Position());
	if (!bFar) { return; }

	// 色を変える
	m_pButton->SetCubeColor(COLOR_DEFAULT);

	// アクティブ状態ならボタンが押されてる
	if (IsActive())
	{ // ボタンが押されている場合

		// ボタンを押しこむ
		m_fButtonHeight -= BUTTON_MOVE * fDeltaTime;

		// 色を変える
		m_pButton->SetCubeColor(COLOR_ON);
	}
	else
	{ // ボタンが押されていない場合

		// ボタンを押し上げる
		m_fButtonHeight += BUTTON_MOVE * fDeltaTime;
	}

	// ボタンの大きさ設定
	SetButtonSizing();

	// プレイヤーとの当たり判定
	CollisionPlayer();

	// 親クラスの更新
	CGimmickAction::Update(fDeltaTime);
}

//=========================================
//  描画処理
//=========================================
void CGimmickPost::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmickAction::Draw(pShader);
}

//=========================================
//	位置の設定処理
//=========================================
void CGimmickPost::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 親クラスの位置設定
	CGimmickAction::SetVec3Position(rPos);

	// ボタンの位置設定
	m_pButton->SetVec3Position(rPos);

	// 縁の位置設定
	m_pEdge->SetVec3Position(rPos);
}

//=========================================
//	大きさの設定処理
//=========================================
void CGimmickPost::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// サイズの設定
	CObject3D::SetVec3Sizing(rSize);

	// ボタンの大きさ設定
	SetButtonSizing();

	// 縁の大きさ設定
	m_pEdge->SetVec3Sizing(rSize * 0.5f + SCALE_EDGE);
}

//===========================================
//  各分身毎の待機位置を算出
//===========================================
D3DXVECTOR3 CGimmickPost::CalcWaitPoint(const int Idx, const CPlayerClone* pClone)
{
	D3DXVECTOR3 posWait = GetVec3Position();	// 待機位置

	// ギミック原点位置に踏み込んでるボタンの高さを与える
	posWait.y += m_pButton->GetVec3Sizing().y * 2.0f;

	// 算出した位置を返す
	return posWait;
}

//===========================================
//  各分身毎の待機向きを算出
//===========================================
D3DXVECTOR3 CGimmickPost::CalcWaitRotation(const int Idx, const CPlayerClone* pClone)
{
	// 受け取ったインデックスが最大値を超えている場合警告
	if (Idx > GetNumActive()) { assert(false); }

	// プレイヤーの位置を取得
	D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();

	// 待機位置を取得
	D3DXVECTOR3 posThis = GetActionPoint();

	// 目標方向との差分を求める
	D3DXVECTOR3 vecTarget = posPlayer - posThis;

	// 待機向きを求める
	D3DXVECTOR3 rotWait = VEC3_ZERO;
	rotWait.y = atan2f(-vecTarget.x, -vecTarget.z);

	// 算出した向きを返す
	return rotWait;
}

//==========================================
//  プレイヤーとの当たり判定
//==========================================
bool CGimmickPost::CollisionPlayer()
{
	// プレイヤー座標を取得
	CPlayer* pPlayer = GET_PLAYER;
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();
	D3DXVECTOR3 oldPlayer = pPlayer->GetOldPosition();
	float fRadius = pPlayer->GetRadius();

	// 自身のサイズを取得
	D3DXVECTOR3 size = GetVec3Sizing() * 0.5f;

	//当たり判定
	bool bHit = collision::BoxPillar
	(
		posPlayer, oldPlayer,
		GetVec3Position(),
		D3DXVECTOR3(fRadius, fRadius, fRadius),
		D3DXVECTOR3(fRadius, fRadius, fRadius),
		size,
		size
	);

	// 当たっている場合プレイヤー座標を補正する
	if (bHit)
	{
		pPlayer->SetVec3Position(posPlayer);
	}

	return bHit;
}

//=========================================
//	ボタン大きさの設定処理
//=========================================
void CGimmickPost::SetButtonSizing(void)
{
	D3DXVECTOR3 size = GetVec3Sizing() * 0.5f + SCALE_BUTTON;	// ボタンの大きさ

	// ボタンの踏まれ具合を反映
	useful::LimitNum(m_fButtonHeight, BUTTON_ON, BUTTON_OFF);	// 踏まれ具合を補正
	size.y = m_fButtonHeight;

	// ボタンの大きさ設定
	m_pButton->SetVec3Sizing(size);
}

//===========================================
//  色の変更
//===========================================
void CGimmickPost::ChangeColor()
{
	// 色を取得
	D3DXCOLOR col = m_pButton->GetCubeColor();
	col *= 255.0f;

	// 変数宣言
	float H = 0.0f, S = 0.0f, V = 0.0f;
	float fCol[3] = { col.r, col.g, col.b };

	// RGB -> HSV 変換
	{
		float fMax = 0.0f, fMin = 1.0f;

		// 最大と最小の値を取得する
		for (int i = 0; i < 3; ++i)
		{
			if (fMax <= fCol[i]) { fMax = fCol[i]; }
			if (fMin >= fCol[i]) { fMin = fCol[i]; }
		}

		// 最大値と一致する色を判別する
		if (col.r == col.g && col.g == col.b && col.b == col.r)
		{
			H = 0.0f;
		}
		else if (col.r == fMax)
		{
			H = 60.0f * ((col.g - col.b) / (fMax - fMin));
		}
		else if (col.g == fMax)
		{
			H = 60.0f * ((col.b - col.r) / (fMax - fMin)) + 120.0f;
		}
		else if (col.b == fMax)
		{
			H = 60.0f * ((col.r - col.g) / (fMax - fMin)) + 240.0f;
		}

		// 値が負の場合360加算して正の値にする
		if (H < 0.0f)
		{
			H += 360.0f;
		}

		// 値を更新する
		H += 0.5f;

		// 360を超えたら0に戻す
		if (H > 360.0f)
		{
			H = 0.0f;
		}

		// Sを求める
		if (fMax == 0.0f)
		{
			S = 0.0f;
		}
		else
		{
			S = ((fMax - fMin) / fMax) * 255.0f;
		}

		// Vを求める
		V = fMax;
	}

	// HSV -> RGB 変換
	{
		float fMax = 0.0f, fMin = 0.0f;
		// 最大値を求める
		fMax = V;

		// 最小値を求める
		fMin = fMax - ((S / 255.0f) * fMax);

		// 場合分けをしてRGB値を決定する
		if (0.0f <= H && H <= 60.0f)
		{
			col.r = fMax;
			col.g = ((H / 60.0f) * (fMax - fMin) + fMin);
			col.b = fMin;
		}
		else if (60.0f <= H && H <= 120.0f)
		{
			col.r = (((120.0f - H) / 60.0f) * (fMax - fMin) + fMin);
			col.g = fMax;
			col.b = fMin;
		}
		else if (120.0f <= H && H <= 180.0f)
		{
			col.r = fMin;
			col.g = fMax;
			col.b = (((H - 120.0f) / 60.0f) * (fMax - fMin) + fMin);
		}
		else if (180.0f <= H && H <= 240.0f)
		{
			col.r = fMin;
			col.g = (((240.0f - H) / 60.0f) * (fMax - fMin) + fMin);
			col.b = fMax;
		}
		else if (240.0f <= H && H <= 300.0f)
		{
			col.r = (((H - 240.0f) / 60.0f) * (fMax - fMin) + fMin);
			col.g = fMin;
			col.b = fMax;
		}
		else if (300.0f <= H && H <= 360.0f)
		{
			col.r = fMax;
			col.g = fMin;
			col.b = (((360.0f - H) / 60.0f) * (fMax - fMin) + fMin);
		}
		col /= 255.0f;
		m_pButton->SetCubeColor(col);
	}
}
