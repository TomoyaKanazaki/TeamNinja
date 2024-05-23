//============================================================
//
//	エディットステージ処理 [editStage.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editStage.h"
#include "manager.h"
#include "editManager.h"
#include "editField.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_FAR		(DIK_W)	// 奥移動キー
#define NAME_FAR	("W")	// 奥移動表示
#define KEY_NEAR	(DIK_S)	// 手前移動キー
#define NAME_NEAR	("S")	// 手前移動表示
#define KEY_RIGHT	(DIK_D)	// 右移動キー
#define NAME_RIGHT	("D")	// 右移動表示
#define KEY_LEFT	(DIK_A)	// 左移動キー
#define NAME_LEFT	("A")	// 左移動表示
#define KEY_UP		(DIK_E)	// 上移動キー
#define NAME_UP		("E")	// 上移動表示
#define KEY_DOWN	(DIK_Q)	// 下移動キー
#define NAME_DOWN	("Q")	// 下移動表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{

}

//************************************************************
//	親クラス [CEditStage] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditStage::CEditStage(CEditManager *pEditManager)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pEditManager = pEditManager;	// エディットマネージャー
	m_pos = VEC3_ZERO;	// 位置

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditStage::~CEditStage()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditStage::Init(void)
{
	// メンバ変数を初期化
	m_pos = VEC3_ZERO;	// 位置

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEditStage::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CEditStage::Update(void)
{
	// 位置の更新
	UpdatePosition();
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditStage::DrawDebugControl(void)
{
	DebugProc::Print(DebugProc::POINT_RIGHT, "移動：[%s/%s/%s/%s/%s/%s+%s]\n", NAME_FAR, NAME_LEFT, NAME_NEAR, NAME_RIGHT, NAME_UP, NAME_DOWN, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "--------------------------------------\n");
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditStage::DrawDebugInfo(void)
{
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f：[位置]\n", m_pos.x, m_pos.y, m_pos.z);
	DebugProc::Print(DebugProc::POINT_RIGHT, "--------------------------------------\n");
}

//============================================================
//	生成処理
//============================================================
CEditStage *CEditStage::Create(CEditManager *pEditManager, EType type)
{
#if _DEBUG

	// ポインタを宣言
	CEditStage *pEditStage = nullptr;	// エディットステージ情報

	// エディットステージの生成
	switch (type)
	{ // 種類ごとの処理
	case TYPE_FIELD:
		pEditStage = new CEditField(pEditManager);	// エディットフィールド
		break;

	case TYPE_WALL:
		//pEditStage = new CEditWall(pEditManager);	// エディットウォール
		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (pEditStage == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// エディットステージの初期化
		if (FAILED(pEditStage->Init()))
		{ // 初期化に失敗した場合

			// エディットステージの破棄
			SAFE_DELETE(pEditStage);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pEditStage;
	}

#else	// NDEBUG

	// nullptrを返す
	return nullptr;

#endif	// _DEBUG
}

//============================================================
//	破棄処理
//============================================================
void CEditStage::Release(CEditStage *&prEditStage)
{
#if _DEBUG

	// エディットステージの終了
	assert(prEditStage != nullptr);
	prEditStage->Uninit();

	// メモリ開放
	SAFE_DELETE(prEditStage);

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	エディットマネージャー取得処理
//============================================================
CEditManager *CEditStage::GetPtrEditManager(void) const
{
#if _DEBUG

	// エディットマネージャーを返す
	return m_pEditManager;

#else	// NDEBUG

	// nullptrを返す
	return nullptr;

#endif	// _DEBUG
}

//============================================================
//	位置の更新処理
//============================================================
void CEditStage::UpdatePosition(void)
{
	// 位置を変更
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_FAR))
		{
			m_pos.z += editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_NEAR))
		{
			m_pos.z -= editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_RIGHT))
		{
			m_pos.x += editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_LEFT))
		{
			m_pos.x -= editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_UP))
		{
			m_pos.y += editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN))
		{
			m_pos.y -= editstage::SIZE;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_FAR))
		{
			m_pos.z += editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_NEAR))
		{
			m_pos.z -= editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_RIGHT))
		{
			m_pos.x += editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_LEFT))
		{
			m_pos.x -= editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP))
		{
			m_pos.y += editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN))
		{
			m_pos.y -= editstage::SIZE;
		}
	}
}
