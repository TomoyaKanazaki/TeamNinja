//=========================================
//
//  複数管理ギミック (gimmick_mulch.cpp)
//  Author : Tomoya kanazaki
//  Adder  : Yuichi Fujita
//
//=========================================
#include "gimmick_mulch.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"
#include "actor.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const int NUM_CLONE = 1;	// ボタン押し込みに必要な人数
}

//=========================================
//  コンストラクタ
//=========================================
CGimmickMulch::CGimmickMulch() : CGimmick(),
m_bActive (true), // アクティブフラグ
m_pModel (nullptr) // モデル情報
{
	// ボタン動的配列をクリア
	m_vecButton.clear();
}

//=========================================
//  デストラクタ
//=========================================
CGimmickMulch::~CGimmickMulch()
{

}

//=========================================
//  初期化処理
//=========================================
HRESULT CGimmickMulch::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CGimmick::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルの生成
	m_pModel = CActor::Create(CActor::TYPE_B_LATTICE, GetVec3Position(), VEC3_ZERO);
	if (m_pModel == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルのラベルを変更
	m_pModel->SetLabel(CObject::LABEL_GIMMICK);

	// 成功を返す
	return S_OK;
}

//=========================================
//  終了処理
//=========================================
void CGimmickMulch::Uninit(void)
{
	// 枠モデルの終了
	SAFE_UNINIT(m_pModel);

	// ボタン動的配列をクリア
	m_vecButton.clear();

	// 親クラスの終了
	CGimmick::Uninit();
}

//=========================================
//  更新処理
//=========================================
void CGimmickMulch::Update(const float fDeltaTime)
{
	// フラグをリセット
	m_bActive = true;

	// 管理しているギミックの設置フラグを取得
	for (const auto& rGimmick : m_vecButton)
	{ // ボタン配列分繰り返す

		// ギミックのフラグが立っていたら次に進む
		if (rGimmick->IsSet()) { continue; }

		// 1つでもフラグがoffならフラグをfalseにする
		m_bActive = false;
		break;
	}

	// 親クラスの更新
	CGimmick::Update(fDeltaTime);
}

//=========================================
//  描画処理
//=========================================
void CGimmickMulch::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmick::Draw(pShader);
}

//===========================================
//  位置の設定
//===========================================
void CGimmickMulch::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 親クラスの設定処理を呼ぶ
	CGimmick::SetVec3Position(rPos);

	// 見た目の位置を設定
	m_pModel->SetVec3Position(rPos);

	// 角度を取得
	EAngle angle = GetAngle();
	D3DXVECTOR3 rot = VEC3_ZERO;
	switch (angle)
	{
	case EAngle::ANGLE_0: // 0
		rot.y = D3DX_PI * 0.0f;
		break;

	case EAngle::ANGLE_90: // 1.57
		rot.y = D3DX_PI * 0.5f;
		break;

	case EAngle::ANGLE_180: // 3.14
		rot.y = D3DX_PI * 1.0f;
		break;

	case EAngle::ANGLE_270: // 4.71
		rot.y = D3DX_PI * 1.5f;
		break;

	default:
		assert(false);
		break;
	}

	// 見た目の角度を設定
	m_pModel->SetVec3Rotation(rot);
}

//===========================================
//  設置ギミックの生成
//===========================================
CGimmickMulch* CGimmickMulch::Create(const D3DXVECTOR3& rPos, const EAngle angle, std::vector<SButton> vecButton)
{
	// 複数管理ギミックの生成
	CGimmickMulch *pGimmick = new CGimmickMulch;
	if (pGimmick == nullptr) { return nullptr; }

	// 複数管理ギミックの初期化
	if (FAILED(pGimmick->Init()))
	{ // 初期化に失敗した場合

		// 複数管理ギミックの破棄
		SAFE_DELETE(pGimmick);
		return nullptr;
	}

	// ボタン情報の生成
	if (FAILED(pGimmick->CreateButton(vecButton)))
	{ // 生成に失敗した場合

		// 複数管理ギミックの破棄
		SAFE_DELETE(pGimmick);
		return nullptr;
	}

	// 向きの設定
	pGimmick->SetAngle(angle);

	// 座標の設定
	pGimmick->SetVec3Position(rPos);

	// 確保したアドレスを返す
	return pGimmick;
}

//===========================================
//  ボタン情報の生成
//===========================================
HRESULT CGimmickMulch::CreateButton(std::vector<SButton> vecButton)
{
	for (const auto& rButton : vecButton)
	{ // ボタン配列分繰り返す

		// ボタンの生成
		CGimmick* pButton = CGimmick::Create
		( // 引数
			rButton.pos,			// 生成位置
			EAngle::ANGLE_0,		// 生成角度
			rButton.size,			// 生成大きさ
			CGimmick::TYPE_POST,	// ギミックラベル (ボタン)
			NUM_CLONE				// ボタン押込に必要な人数
		);
		if (pButton == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// 最後尾に生成したボタンを保存
		m_vecButton.push_back(pButton);
	}

	return S_OK;
}
