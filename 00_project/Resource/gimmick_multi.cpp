//=========================================
//
//  複数管理ギミック (gimmick_multi.cpp)
//  Author : Tomoya kanazaki
//  Adder  : Yuichi Fujita
//
//=========================================
#include "gimmick_multi.h"
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
	const float MOVE_SPEED = 150.0f; // 移動速度
	const D3DXVECTOR3 MOVE_POS = D3DXVECTOR3(0.0f, -250.0f, 0.0f); // 移動後の位置(オフセット)
}

//=========================================
//  コンストラクタ
//=========================================
CGimmickMulti::CGimmickMulti() : CGimmick(),
m_bActive (true), // アクティブフラグ
m_pModel (nullptr) // モデル情報
{
	// ボタン動的配列をクリア
	m_vecButton.clear();
}

//=========================================
//  デストラクタ
//=========================================
CGimmickMulti::~CGimmickMulti()
{

}

//=========================================
//  初期化処理
//=========================================
HRESULT CGimmickMulti::Init(void)
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
void CGimmickMulti::Uninit(void)
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
void CGimmickMulti::Update(const float fDeltaTime)
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

	// モデルの移動
	MoveModel(fDeltaTime);

	// 親クラスの更新
	CGimmick::Update(fDeltaTime);
}

//=========================================
//  描画処理
//=========================================
void CGimmickMulti::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmick::Draw(pShader);
}

//===========================================
//  位置の設定
//===========================================
void CGimmickMulti::SetVec3Position(const D3DXVECTOR3& rPos)
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
CGimmickMulti* CGimmickMulti::Create(const D3DXVECTOR3& rPos, const EAngle angle, std::vector<SButton> vecButton)
{
	// 複数管理ギミックの生成
	CGimmickMulti *pGimmick = new CGimmickMulti;
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
HRESULT CGimmickMulti::CreateButton(std::vector<SButton> vecButton)
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

//===========================================
//  モデルの移動
//===========================================
void CGimmickMulti::MoveModel(const float fDeltaTime)
{
	// モデルの位置を取得
	D3DXVECTOR3 posModel = m_pModel->GetVec3Position();

	// 移動量
	float fMove = 0.0f;

	// アクティブフラグがオンの場合移動先に向かって動く
	if (m_bActive)
	{
		// モデルの位置が移動先を下回った場合関数を抜ける
		if (posModel.y <= GetVec3Position().y + MOVE_POS.y)
		{
			posModel.y = GetVec3Position().y + MOVE_POS.y;
			return;
		}

		// 移動の設定
		fMove -= MOVE_SPEED;
	}
	else // オフの場合基準位置に戻る
	{
		// モデルの位置が基準点を上回った場合関数を抜ける
		if (posModel.y >= GetVec3Position().y)
		{
			posModel.y = GetVec3Position().y;
			return;
		}

		// 移動の設定
		fMove += MOVE_SPEED;
	}

	// 移動量の適用
	posModel.y += fMove * fDeltaTime;

	// モデルの位置を設定
	m_pModel->SetVec3Position(posModel);
}
