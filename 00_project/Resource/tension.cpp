//===========================================
//
//  士気力ゲージ(tension.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "tension.h"
#include "player_clone.h"
#include "manager.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const int TENSION_MAX = 15; // 士気力の上限
	const D3DXVECTOR3 TENSION_SIZE = D3DXVECTOR3(65.0f, 75.0f, 0.0f); // ポリゴンの大きさ
	const float	TENSION_WIDTH = 460.0f; // 士気力ゲージの幅
	const char*	TENSION_TEXTURE = "data\\TEXTURE\\scroll.png"; // 士気力テクスチャ
}

//===========================================
//  静的メンバ変数宣言
//===========================================
CListManager<CTension>* CTension::m_pList = nullptr; // オブジェクトリスト
int CTension::m_nUseNum = 0; // 使用可能数

//===========================================
//  コンストラクタ
//===========================================
CTension::CTension() : CObject2D(CObject::LABEL_UI),
m_bUse(true)
{
}

//===========================================
//  デストラクタ
//===========================================
CTension::~CTension()
{
}

//===========================================
//  初期化処理
//===========================================
HRESULT CTension::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// リストが存在していない場合
	if (m_pList == nullptr)
	{
		// リストマネージャーの生成
		m_pList = CListManager<CTension>::Create();

		// 生成に失敗した場合警告を表示し関数を抜ける
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 大きさを設定
	SetVec3Sizing(TENSION_SIZE);

	// 位置の設定
	CalcPosition();

	// テクスチャ割り当て
	BindTexture(TENSION_TEXTURE);

	// 成功を返す
	return S_OK;
}

//===========================================
//  終了処理
//===========================================
void CTension::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 親クラスの終了
	CObject2D::Uninit();
}

//===========================================
//  更新処理
//===========================================
void CTension::Update(const float fDeltaTime)
{
	// 使用可能状態の変更
	SwitchUse();

	// 座標の計算
	CalcPosition();

	// 色の変更
	ChangeColor();

	// 親クラスの更新処理
	CObject2D::Update(fDeltaTime);
}

//===========================================
//  描画処理
//===========================================
void CTension::Draw(CShader* pShader)
{
	CObject2D::Draw(pShader);
}

//===========================================
//  生成処理
//===========================================
CTension* CTension::Create()
{
	// インスタンス生成
	CTension* pTension = new CTension;

	// メモリの確保に失敗した場合警告を表示しnullを返す
	if (pTension == nullptr) { assert(false); return nullptr; }

	// 初期化処理
	if (FAILED(pTension->Init())) { SAFE_DELETE(pTension); return nullptr; }

	// 最大数に達している場合nullを返す
	if (m_pList->GetNumAll() > TENSION_MAX) 
	{ SAFE_UNINIT(pTension); return nullptr; }

	return pTension;
}

//===========================================
//  リストの取得
//===========================================
CListManager<CTension>* CTension::GetList()
{
	// オブジェクトリストを返す
	return m_pList;
}

//===========================================
//  使用可能数の取得
//===========================================
int CTension::GetUseNum()
{
	// リストが存在していない場合-1を返す
	if (m_pList == nullptr) { return -1; }

	// 使用可能な士気力を確認する
	int nNum = 0;
	for (auto tension : m_pList->GetList())
	{
		// 使用可能な場合加算
		if (tension->IsUse()) { ++nNum; }
	}

	return nNum;
}

//===========================================
//  消滅
//===========================================
void CTension::Vanish()
{
	// リストが無ければ関数を抜ける
	if (m_pList == nullptr) { return; }

	// リストの先頭を取得
	CTension* begin = *m_pList->GetIndex(0);

	// 先頭を削除
	SAFE_UNINIT(begin);
}

//===========================================
//  座標の計算処理
//===========================================
void CTension::CalcPosition()
{
	// 隣との幅を算出
	float fWidth = TENSION_WIDTH / m_pList->GetNumAll();

	// 自身のインデックスを取得
	int nIdx = m_pList->GetIndex(this);

	// 自身のインデックス分x座標をずらす
	float fPosX = (TENSION_SIZE.x * 0.5f) + (fWidth * nIdx);

	// 座標を設定
	D3DXVECTOR3 pos = D3DXVECTOR3(fPosX + 10.0f, TENSION_SIZE.y * 0.5f + 10.0f, 0.0f);

	// 座標を適用
	SetVec3Position(pos);
}

//===========================================
//  色の変更
//===========================================
void CTension::ChangeColor()
{
	// 使用可能の場合は不透明,不可能の場合は半透明
	if (m_bUse)
	{
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
	}
}

//===========================================
//  使用可能状態の変更
//===========================================
void CTension::SwitchUse()
{
	// 一度使用可能にする
	m_bUse = true;

	// 分身のリストが無ければ関数を抜ける
	if (CPlayerClone::GetList() == nullptr) { return; }

	// 分身の総数を取得
	int nNumClone = CPlayerClone::GetList()->GetNumAll();

	// 士気力の総数を取得
	int nNumTension = m_pList->GetNumAll();

	// 分身と士気力の総数の差分を算出
	int nNumUse = nNumTension - nNumClone;

	// 自身のインデックスが差分よりも大きい場合は使用不可
	if (nNumUse <= m_pList->GetIndex(this))
	{
		m_bUse = false;
	}
}
