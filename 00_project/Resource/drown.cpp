//==========================================
//
//  溺死uiクラス(drown.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "drown.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const char* TEXTURE_PAS = "data\\TEXTURE\\wave.png"; // テクスチャパス
	const float MOVE_SPEED = -360.0f; // 移動量
}

//==========================================
//  コンストラクタ
//==========================================
CDrown::CDrown() :
	m_bMove(false),
	m_fMove(MOVE_SPEED)
{
}

//==========================================
//  デストラクタ
//==========================================
CDrown::~CDrown()
{
}

//==========================================
//  初期化処理
//==========================================
HRESULT CDrown::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャ割り当て
	BindTexture(TEXTURE_PAS);

	// ラベルを設定
	SetLabel(LABEL_UI);

	// 移動量を設定
	m_fMove = MOVE_SPEED;

	// 成功を返す
	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void CDrown::Uninit(void)
{
	// 親クラスの終了
	CObject2D::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CDrown::Update(const float fDeltaTime)
{
	// 移動
	Move(fDeltaTime);

	// 親クラスの更新
	CObject2D::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CDrown::Draw(CShader* pShader)
{
	// 親クラスの描画
	CObject2D::Draw(pShader);
}

//==========================================
//  移動処理
//==========================================
void CDrown::Move(const float fDeltaTime)
{
	// フラグがoffなら関数を抜ける
	if (!m_bMove) { return; }

	// 座標を取得
	D3DXVECTOR3 pos = GetVec3Position();
	
	// 移動
	pos.y += m_fMove * fDeltaTime;

	// 座標を補正する
	if (GetVec3Position().y < SCREEN_CENT.y)
	{
		pos = SCREEN_CENT;
		m_fMove *= -1.0f;
		m_bBrind = true;
	}

	// 座標を適用
	SetVec3Position(pos);
}

//==========================================
//  生成処理
//==========================================
CDrown* CDrown::Create(void)
{
	// インスタンス生成
	CDrown* pUI = new CDrown;

	// メモリの確保に失敗した場合警告を表示しnullを返す
	if (pUI == nullptr) { assert(false); return nullptr; }

	// 初期化処理
	if (FAILED(pUI->Init())) { SAFE_DELETE(pUI); return nullptr; }

	// サイズを設定
	pUI->SetVec3Sizing(SCREEN_SIZE * 1.4f);

	// 座標を設定
	pUI->SetVec3Position(D3DXVECTOR3(SCREEN_CENT.x, SCREEN_CENT.y * 3.0f, 0.0f));

	return pUI;
}
