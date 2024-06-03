//============================================================
//
//	当たり判定エディター処理 [editorCollShape.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "editorCollShape.h"
#include "editManager.h"

#include "editCollCube.h"
#include "editCollCylinder.h"
#include "editCollSphere.h"
#include "actor.h"

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
#define KEY_CREATE	(DIK_0)	// 生成キー
#define NAME_CREATE	("0")	// 生成表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float MOVE = 1.0f;		// 移動量
}

//************************************************************
//	親クラス [CEditorCollShape] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditorCollShape::CEditorCollShape(const int nIdx)
{
#if _DEBUG

	// メンバ変数をクリア
	m_offset = VEC3_ZERO;		// オフセット座標

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditorCollShape::~CEditorCollShape()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditorCollShape::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_offset = VEC3_ZERO;		// オフセット座標

	// 成功を返す
	return S_OK;

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	終了処理
//============================================================
void CEditorCollShape::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditorCollShape::Update(void)
{
#if _DEBUG

	// オフセット座標の更新
	UpdateOffset();

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditorCollShape::DrawDebugControl(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "移動：[%s/%s/%s/%s/%s/%s+%s]\n", NAME_FAR, NAME_LEFT, NAME_NEAR, NAME_RIGHT, NAME_UP, NAME_DOWN, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "設置：[%s]\n", NAME_CREATE);

#endif	// _DEBUG
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditorCollShape::DrawDebugInfo(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f：[オフセット]\n", m_offset.x, m_offset.y, m_offset.z);

#endif	// _DEBUG
}

//============================================================
//	生成処理
//============================================================
CEditorCollShape* CEditorCollShape::Create(CEditCollision::EType type, const int nIdx)
{
#if _DEBUG

	// ポインタを宣言
	CEditorCollShape* pEditorObject = nullptr;	// エディターオブジェクト情報

	// エディターオブジェクトの生成
	switch (type)
	{
	case CEditCollision::TYPE_CUBE:
		pEditorObject = new CEditCollCube(nIdx);		// エディットコリジョンキューブ
		break;
	case CEditCollision::TYPE_CYLINDER:
		pEditorObject = new CEditCollCylinder(nIdx);	// エディットコリジョンシリンダー
		break;
	case CEditCollision::TYPE_SPHERE:
		pEditorObject = new CEditCollSphere(nIdx);		// エディットコリジョンスフィア
		break;
	default:		// 例外処理
		assert(false);
		break;
	}

	if (pEditorObject == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// エディターオブジェクトの初期化
		if (FAILED(pEditorObject->Init()))
		{ // 初期化に失敗した場合

			// エディターオブジェクトの破棄
			SAFE_DELETE(pEditorObject);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pEditorObject;
	}

#else	// NDEBUG

	// nullptrを返す
	return nullptr;

#endif	// _DEBUG
}

//============================================================
//	破棄処理
//============================================================
void CEditorCollShape::Release(CEditorCollShape*& prEditorObject)
{
#if _DEBUG

	// エディターオブジェクトの終了
	assert(prEditorObject != nullptr);
	prEditorObject->Uninit();

	// メモリ開放
	SAFE_DELETE(prEditorObject);

#endif	// _DEBUG
}

//============================================================
//	オフセット座標の更新処理
//============================================================
void CEditorCollShape::UpdateOffset(void)
{
	// 位置を変更
	CInputKeyboard* m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_FAR))
		{
			m_offset.z += MOVE;
		}
		if (m_pKeyboard->IsPress(KEY_NEAR))
		{
			m_offset.z -= MOVE;
		}
		if (m_pKeyboard->IsPress(KEY_RIGHT))
		{
			m_offset.x += MOVE;
		}
		if (m_pKeyboard->IsPress(KEY_LEFT))
		{
			m_offset.x -= MOVE;
		}
		if (m_pKeyboard->IsPress(KEY_UP))
		{
			m_offset.y += MOVE;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN))
		{
			m_offset.y -= MOVE;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_FAR))
		{
			m_offset.z += MOVE;
		}
		if (m_pKeyboard->IsTrigger(KEY_NEAR))
		{
			m_offset.z -= MOVE;
		}
		if (m_pKeyboard->IsTrigger(KEY_RIGHT))
		{
			m_offset.x += MOVE;
		}
		if (m_pKeyboard->IsTrigger(KEY_LEFT))
		{
			m_offset.x -= MOVE;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP))
		{
			m_offset.y += MOVE;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN))
		{
			m_offset.y -= MOVE;
		}
	}
}
