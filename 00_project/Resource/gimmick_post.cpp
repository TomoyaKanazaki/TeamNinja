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
		VEC3_ZERO,
		VEC3_ZERO,
		VEC3_ZERO,
		XCOL_BLUE
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
		VEC3_ZERO,
		VEC3_ZERO,
		VEC3_ZERO,
		XCOL_WHITE
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
	// アクティブ状態ならボタンが押されてる
	if (IsActive())
	{ // ボタンが押されている場合

		// ボタンを押しこむ
		m_fButtonHeight -= BUTTON_MOVE * fDeltaTime;
	}
	else
	{ // ボタンが押されていない場合

		// ボタンを押し上げる
		m_fButtonHeight += BUTTON_MOVE * fDeltaTime;
	}

	// ボタンの大きさ設定
	SetButtonSizing();

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
	// 親クラスの大きさ設定
	CGimmickAction::SetVec3Sizing(rSize);

	// ボタンの大きさ設定
	SetButtonSizing();

	// 縁の大きさ設定
	m_pEdge->SetVec3Sizing(rSize * 0.5f + SCALE_EDGE);
}

//===========================================
//  各分身毎の待機位置を算出
//===========================================
D3DXVECTOR3 CGimmickPost::CalcWaitPoint(const int Idx) const
{
	return GetVec3Position();
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
