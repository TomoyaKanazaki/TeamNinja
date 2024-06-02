//============================================================
//
//	エディターオブジェクト処理 [editorObject.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editorObject.h"
#include "manager.h"
#include "editManager.h"

#include "editField.h"
#include "editActor.h"
#include "editGimmick.h"

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

#define KEY_ROTA_RIGHT	(DIK_Z)	// 右回転キー
#define NAME_ROTA_RIGHT	("Z")	// 右回転表示
#define KEY_ROTA_LEFT	(DIK_C)	// 左回転キー
#define NAME_ROTA_LEFT	("C")	// 左回転表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{

}

//************************************************************
//	親クラス [CEditorObject] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditorObject::CEditorObject()
{
#if _DEBUG

	// メンバ変数をクリア
	m_pos = VEC3_ZERO;			// 位置
	m_rot = VEC3_ZERO;			// 向き
	m_angle = CEditor::ANGLE_0;	// 角度

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditorObject::~CEditorObject()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditorObject::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pos = VEC3_ZERO;			// 位置
	m_rot = VEC3_ZERO;			// 向き
	m_angle = CEditor::ANGLE_0;	// 角度

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
void CEditorObject::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditorObject::Update(void)
{
#if _DEBUG

	// 位置の更新
	UpdatePosition();

	// 向きの更新
	UpdateRotation();

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditorObject::DrawDebugControl(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "移動：[%s/%s/%s/%s/%s/%s+%s]\n", NAME_FAR, NAME_LEFT, NAME_NEAR, NAME_RIGHT, NAME_UP, NAME_DOWN, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "回転：[%s/%s]\n", NAME_ROTA_RIGHT, NAME_ROTA_LEFT);

#endif	// _DEBUG
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditorObject::DrawDebugInfo(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f：[位置]\n", m_pos.x, m_pos.y, m_pos.z);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f：[向き]\n", m_rot.x, m_rot.y, m_rot.z);

#endif	// _DEBUG
}

//============================================================
//	生成処理
//============================================================
CEditorObject *CEditorObject::Create(CEditStage::EType type)
{
#if _DEBUG

	// ポインタを宣言
	CEditorObject *pEditorObject = nullptr;	// エディターオブジェクト情報

	// エディターオブジェクトの生成
	switch (type)
	{ // 種類ごとの処理
	case CEditStage::TYPE_FIELD:
		pEditorObject = new CEditField;		// エディットフィールド
		break;

	case CEditStage::TYPE_WALL:

		// TODO：エディットウォールできたら置き換え
		pEditorObject = new CEditField;	// エディットフィールド
		//pEditorObject = new CEditWall;	// エディットウォール

		break;

	case CEditStage::TYPE_ACTOR:
		pEditorObject = new CEditActor;		// エディットアクター
		break;

	case CEditStage::TYPE_GIMMICK:
		pEditorObject = new CEditGimmick;	// エディットギミック
		break;

	default:	// 例外処理
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
void CEditorObject::Release(CEditorObject *&prEditorObject)
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
//	位置の更新処理
//============================================================
void CEditorObject::UpdatePosition(void)
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

//============================================================
//	向き更新処理
//============================================================
void CEditorObject::UpdateRotation(void)
{
	// 向きを変更
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報
	if (m_pKeyboard->IsTrigger(KEY_ROTA_RIGHT))
	{
		// 角度を左回転
		m_angle = (CEditor::EAngle)((m_angle + (CEditor::ANGLE_MAX - 1)) % CEditor::ANGLE_MAX);

		// 現在の角度から向きを計算
		int nTemp = ((m_angle - 1) + (CEditor::ANGLE_MAX - 1)) % CEditor::ANGLE_MAX;
		m_rot.y = ((float)nTemp * HALF_PI) - D3DX_PI;
	}
	if (m_pKeyboard->IsTrigger(KEY_ROTA_LEFT))
	{
		// 角度を右回転
		m_angle = (CEditor::EAngle)((m_angle + 1) % CEditor::ANGLE_MAX);

		// 現在の角度から向きを計算
		int nTemp = ((m_angle - 1) + (CEditor::ANGLE_MAX - 1)) % CEditor::ANGLE_MAX;
		m_rot.y = ((float)nTemp * HALF_PI) - D3DX_PI;
	}

	// 向きを正規化
	useful::NormalizeRot(m_rot);
}
