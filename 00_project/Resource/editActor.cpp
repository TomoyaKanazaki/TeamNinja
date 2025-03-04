#if 1
//============================================================
//
//	エディットアクター処理 [editActor.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editActor.h"
#include "editManager.h"
#include "manager.h"
#include "collision.h"
#include "useful.h"
#include "stage.h"

#include "collisionCube.h"
#include "collisionCylinder.h"
#include "collisionSphere.h"
#include "collisionPolygon.h"
#include "collManager.h"
#include "mash.h"
#include "spin_wall.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_CREATE		(DIK_0)	// 生成キー
#define NAME_CREATE		("0")	// 生成表示
#define KEY_RELEASE		(DIK_9)	// 破棄キー
#define NAME_RELEASE	("9")	// 破棄表示
#define KEY_UP_TYPE		(DIK_4)	// 種類前進変更キー
#define NAME_UP_TYPE	("4")	// 種類前進変更表示
#define KEY_DOWN_TYPE	(DIK_5)	// 種類後進変更キー
#define NAME_DOWN_TYPE	("5")	// 種類後進変更表示

#define KEY_UP_SIZE		(DIK_T)	// 拡大キー
#define NAME_UP_SIZE	("T")	// 拡大表示
#define KEY_DOWN_SIZE	(DIK_G)	// 縮小キー
#define NAME_DOWN_SIZE	("G")	// 縮小表示
#define KEY_ROTA_RIGHT	(DIK_Z)	// 右回転キー
#define NAME_ROTA_RIGHT	("Z")	// 右回転表示
#define KEY_ROTA_LEFT	(DIK_C)	// 左回転キー
#define NAME_ROTA_LEFT	("C")	// 左回転表示

#define KEY_FAR			(DIK_W)	// 奥移動キー
#define KEY_NEAR		(DIK_S)	// 手前移動キー
#define KEY_RIGHT		(DIK_D)	// 右移動キー
#define KEY_LEFT		(DIK_A)	// 左移動キー
#define KEY_UP			(DIK_E)	// 上移動キー
#define KEY_DOWN		(DIK_Q)	// 下移動キー

#define KEY_UP_MOVE		(DIK_UP)	// 移動量上キー
#define NAME_UP_MOVE	("↑")		// 移動量上表示
#define KEY_DOWN_MOVE	(DIK_DOWN)	// 移動量下キー
#define NAME_DOWN_MOVE	("↓")		// 移動量下表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_actor.txt";	// セーブテキストパス

	const D3DXVECTOR3 SCALING = D3DXVECTOR3(0.05f, 0.05f, 0.05f);	// 拡縮率
	const float INIT_MOVE = 25.0f;	// 初期移動量
	const float CALC_MOVE = 0.5f;	// 計算移動量
	const float	INIT_ALPHA = 0.5f;	// 配置前のα値
	const int DIGIT_FLOAT = 2;		// 小数点以下の桁数

	const float ROT_MOVE = 0.02f;	// 向きの移動量
}

//************************************************************
//	親クラス [CEditActor] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditActor::CEditActor(CEditStage* pEditor) : CEditorObject(pEditor)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pActor = nullptr;		// アクター情報
	m_fMove = INIT_MOVE;	// 移動量
	m_bSave = false;		// 保存状況
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// アクター配置情報

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditActor::~CEditActor()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditActor::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pActor = nullptr;						// 情報
	m_infoCreate.type = (CActor::EType)0;	// 種類
	m_infoCreate.scale = VEC3_ONE;			// 大きさ
	m_bSave = false;						// 保存状況

	// 親クラスの初期化
	if (FAILED(CEditorObject::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// アクターの生成
	D3DXCOLOR colActor = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// 地面色
	m_pActor = CActor::Create
	( // 引数
		m_infoCreate.type,		// 種類
		GetVec3Position(),		// 位置
		GetVec3Rotation(),		// 向き
		m_infoCreate.scale		// 大きさ
	);
	if (m_pActor == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

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
void CEditActor::Uninit(void)
{
#if _DEBUG

	// 親クラスの終了
	CEditorObject::Uninit();

	// アクターの色の全初期化
	InitAllColorActor();

	// アクターの終了
	SAFE_UNINIT(m_pActor);

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditActor::Update(void)
{
#if _DEBUG

	// 親クラスの更新
	CEditorObject::Update();

	// 拡大率の更新
	UpdateScaling();

	// 種類の変更
	ChangeType();

	// 移動量の設定
	MoveSet();

	// アクターの生成
	CreateActor();

	// アクターの破棄
	ReleaseActor();

	// 位置を反映
	m_pActor->SetVec3Position(GetVec3Position());

	// ふすまの設定処理
	SetMash();

	// 回転扉の設定処理
	SetSpinWall();

#endif	// _DEBUG
}

//============================================================
//	保存状況取得処理
//============================================================
bool CEditActor::IsSave(void)
{
#if _DEBUG

	// 保存状況を返す
	return m_bSave;

#else	// NDEBUG

	// falseを返す
	return false;

#endif	// _DEBUG
}

//============================================================
//	情報保存処理
//============================================================
void CEditActor::SaveInfo(void)
{
#if _DEBUG

	// 現在の情報を保存
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	情報読込処理
//============================================================
void CEditActor::LoadInfo(void)
{
#if _DEBUG

	// 保存情報を設定
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditActor::DrawDebugControl(void)
{
#if _DEBUG

	// 操作表示の描画
	CEditorObject::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "大きさ：[%s/%s+%s]\n", NAME_UP_SIZE, NAME_DOWN_SIZE, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "種類変更：[%s/%s]\n", NAME_UP_TYPE, NAME_DOWN_TYPE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "移動量変更：[%s/%s]\n", NAME_UP_MOVE, NAME_DOWN_MOVE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "削除：[%s]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "設置：[%s]\n", NAME_CREATE);

#endif	// _DEBUG
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditActor::DrawDebugInfo(void)
{
#if _DEBUG

	// 情報表示の描画
	CEditorObject::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%d：[種類]\n", m_infoCreate.type);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f：[移動量]\n", m_fMove);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f：[大きさ]\n", m_infoCreate.scale.x, m_infoCreate.scale.y, m_infoCreate.scale.z);

#endif	// _DEBUG
}

//============================================================
// 位置更新
//============================================================
void CEditActor::UpdatePosition(void)
{
	CInputKeyboard* m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報
	D3DXVECTOR3 pos = GetVec3Position();		// 位置

	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_FAR))
		{
			pos.z += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_NEAR))
		{
			pos.z -= m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_RIGHT))
		{
			pos.x += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_LEFT))
		{
			pos.x -= m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_UP))
		{
			pos.y += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN))
		{
			pos.y -= m_fMove;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_FAR))
		{
			pos.z += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_NEAR))
		{
			pos.z -= m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_RIGHT))
		{
			pos.x += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_LEFT))
		{
			pos.x -= m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP))
		{
			pos.y += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN))
		{
			pos.y -= m_fMove;
		}
	}

	// 位置を反映
	SetVec3Position(pos);

	// 位置を反映
	m_pActor->SetVec3Position(GetVec3Position());
}

//============================================================
//	向き更新処理
//============================================================
void CEditActor::UpdateRotation(void)
{
	if (m_pActor->GetCube().empty() &&
		m_pActor->GetPolygon().empty())
	{ // 空白の場合

		// 向きを変更
		D3DXVECTOR3 rot = GetVec3Rotation();			// 向きを取得する
		CInputKeyboard* m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報
		if (!m_pKeyboard->IsPress(KEY_TRIGGER))
		{
			if (m_pKeyboard->IsPress(KEY_ROTA_RIGHT))
			{
				rot.y -= ROT_MOVE;
			}
			if (m_pKeyboard->IsPress(KEY_ROTA_LEFT))
			{
				rot.y += ROT_MOVE;
			}
		}
		else
		{
			if (m_pKeyboard->IsTrigger(KEY_ROTA_RIGHT))
			{
				rot.y -= ROT_MOVE;
			}
			if (m_pKeyboard->IsTrigger(KEY_ROTA_LEFT))
			{
				rot.y += ROT_MOVE;
			}
		}

		// 向きを設定する
		SetVec3Rotation(rot);
	}
	else
	{ // 上記以外

		// 向きの更新
		CEditorObject::UpdateRotation();
	}

#ifdef _DEBUG

	for (auto& rCube : m_pActor->GetCube())
	{ // コリジョンキューブ数分繰り返す

		// 向きを適用する
		rCube->GetCube()->SetVec3Rotation(GetVec3Rotation());
	}

	for (auto& rCylinder : m_pActor->GetCylinder())
	{ // コリジョンシリンダー数分繰り返す

		// 向きを適用する
		rCylinder->GetTube()->SetVec3Rotation(GetVec3Rotation());
	}

	for (auto& rSphere : m_pActor->GetSphere())
	{ // コリジョンスフィア数分繰り返す

		// 向きを適用する
		rSphere->GetSphere()->SetVec3Rotation(GetVec3Rotation());
	}

	for (auto& rPolygon : m_pActor->GetPolygon())
	{ // コリジョンポリゴン数分繰り返す

		// 向きを適用する
		rPolygon->GetPolygon()->SetVec3Rotation(D3DXVECTOR3
		(
			rPolygon->GetRot().x,
			rPolygon->GetRot().y + GetVec3Rotation().y,
			rPolygon->GetRot().z
		));
	}

#endif // _DEBUG

	// 向きを反映
	m_pActor->SetVec3Rotation(GetVec3Rotation());
}

//============================================================
//	拡大率の更新処理
//============================================================
void CEditActor::UpdateScaling(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 拡大率を変更
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_SIZE))
		{
			m_infoCreate.scale += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE))
		{
			m_infoCreate.scale -= SCALING;
		}
	}
	else
	{
		if (pKeyboard->IsTrigger(KEY_UP_SIZE))
		{
			m_infoCreate.scale += SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE))
		{
			m_infoCreate.scale -= SCALING;
		}
	}

	// 拡大率を補正
	useful::LimitMinNum(m_infoCreate.scale.x, SCALING.x);
	useful::LimitMinNum(m_infoCreate.scale.y, SCALING.y);
	useful::LimitMinNum(m_infoCreate.scale.z, SCALING.z);

#ifdef _DEBUG

	for (auto& rCube : m_pActor->GetCube())
	{ // コリジョンキューブ数分繰り返す

		// 拡大率を適用する
		rCube->GetCube()->SetVec3Sizing(D3DXVECTOR3
		(
			rCube->GetWidth() * m_infoCreate.scale.x,
			rCube->GetHeight() * m_infoCreate.scale.y,
			rCube->GetDepth() * m_infoCreate.scale.z
		));
	}

	for (auto& rCylinder : m_pActor->GetCylinder())
	{ // コリジョンシリンダー数分繰り返す

		// 拡大率を適用する
		rCylinder->GetTube()->SetRadius(rCylinder->GetRadius() * m_infoCreate.scale.x);
		rCylinder->GetTube()->SetHeight(rCylinder->GetHeight() * m_infoCreate.scale.y);
	}

	for (auto& rSphere : m_pActor->GetSphere())
	{ // コリジョンスフィア数分繰り返す

		// 拡大率を適用する
		rSphere->GetSphere()->SetRadius(rSphere->GetRadius() * m_infoCreate.scale.x);
	}

	for (auto& rPolygon : m_pActor->GetPolygon())
	{ // コリジョンポリゴン数分繰り返す

		// 拡大率を適用する
		rPolygon->GetPolygon()->SetVec3Sizing(D3DXVECTOR3
		(
			rPolygon->GetSize().x * m_infoCreate.scale.x,
			rPolygon->GetSize().y * m_infoCreate.scale.y,
			rPolygon->GetSize().z * m_infoCreate.scale.z
		));
	}

#endif // _DEBUG

	// 拡大率を反映
	m_pActor->SetVec3Scaling(m_infoCreate.scale);
}

//============================================================
//	種類変更の更新処理
//============================================================
void CEditActor::ChangeType(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 種類を変更
	if (pKeyboard->IsTrigger(KEY_UP_TYPE))
	{
		m_infoCreate.type = (CActor::EType)((m_infoCreate.type + 1) % CActor::TYPE_MAX);

		if (!CScene::GetCollManager()->GetCollInfo(m_infoCreate.type).m_cube.empty())
		{ // キューブの情報が入っている場合

			// 向きの補正処理
			RotCorrect();
		}

		// モデルを生成し直す
		m_pActor->Uninit();
		m_pActor = CActor::Create(m_infoCreate.type, GetVec3Position(), GetVec3Rotation(), m_infoCreate.scale);

		for (auto& rCube : m_pActor->GetCube())
		{ // コリジョンキューブ数分繰り返す

			// 大きさを設定する
			rCube->SetWidth(rCube->GetWidth() / m_infoCreate.scale.x);
			rCube->SetHeight(rCube->GetHeight() / m_infoCreate.scale.y);
			rCube->SetDepth(rCube->GetDepth() / m_infoCreate.scale.z);
		}

		for (auto& rCylinder : m_pActor->GetCylinder())
		{ // コリジョンシリンダー数分繰り返す

			// 大きさを設定する
			rCylinder->SetRadius(rCylinder->GetRadius() / m_infoCreate.scale.x);
			rCylinder->SetHeight(rCylinder->GetHeight() / m_infoCreate.scale.y);
		}

		for (auto& rSphere : m_pActor->GetSphere())
		{ // コリジョンスフィア数分繰り返す

			// 大きさを設定する
			rSphere->SetRadius(rSphere->GetRadius() / m_infoCreate.scale.x);
		}

		for (auto& rPolygon : m_pActor->GetPolygon())
		{ // コリジョンポリゴン数分繰り返す

			// 大きさを設定する
			rPolygon->SetSize(D3DXVECTOR3
			(
				rPolygon->GetSize().x / m_infoCreate.scale.x,
				rPolygon->GetSize().y / m_infoCreate.scale.y,
				rPolygon->GetSize().z / m_infoCreate.scale.z
			));
		}

		if (!m_pActor->GetCube().empty() &&
			!m_pActor->GetPolygon().empty())
		{ // キューブの当たり判定が存在していた場合
			
			// 方向の設定処理
			SetAngle(useful::RotToFourDire(GetVec3Rotation().y));
		}
	}
	else if (pKeyboard->IsTrigger(KEY_DOWN_TYPE))
	{
		m_infoCreate.type = (CActor::EType)((m_infoCreate.type + (CActor::TYPE_MAX - 1)) % CActor::TYPE_MAX);

		if (!CScene::GetCollManager()->GetCollInfo(m_infoCreate.type).m_cube.empty())
		{ // キューブの情報が入っている場合

			// 向きの補正処理
			RotCorrect();
		}

		// モデルを生成し直す
		m_pActor->Uninit();
		m_pActor = CActor::Create(m_infoCreate.type, GetVec3Position(), GetVec3Rotation(), m_infoCreate.scale);

		for (auto& rCube : m_pActor->GetCube())
		{ // コリジョンキューブ数分繰り返す

			// 大きさを設定する
			rCube->SetWidth(rCube->GetWidth() / m_infoCreate.scale.x);
			rCube->SetHeight(rCube->GetHeight() / m_infoCreate.scale.y);
			rCube->SetDepth(rCube->GetDepth() / m_infoCreate.scale.z);
		}

		for (auto& rCylinder : m_pActor->GetCylinder())
		{ // コリジョンシリンダー数分繰り返す

			// 大きさを設定する
			rCylinder->SetRadius(rCylinder->GetRadius() / m_infoCreate.scale.x);
			rCylinder->SetHeight(rCylinder->GetHeight() / m_infoCreate.scale.y);
		}

		for (auto& rSphere : m_pActor->GetSphere())
		{ // コリジョンスフィア数分繰り返す

			// 大きさを設定する
			rSphere->SetRadius(rSphere->GetRadius() / m_infoCreate.scale.x);
		}

		for (auto& rPolygon : m_pActor->GetPolygon())
		{ // コリジョンポリゴン数分繰り返す

			// 大きさを設定する
			rPolygon->SetSize(D3DXVECTOR3
			(
				rPolygon->GetSize().x / m_infoCreate.scale.x,
				rPolygon->GetSize().y / m_infoCreate.scale.y,
				rPolygon->GetSize().z / m_infoCreate.scale.z
			));
		}

		if (!m_pActor->GetCube().empty() &&
			!m_pActor->GetPolygon().empty())
		{ // キューブの当たり判定が存在していた場合

			// 方向の設定処理
			SetAngle(useful::RotToFourDire(GetVec3Rotation().y));
		}
	}

	// 種類を反映
	m_pActor->SetType(m_infoCreate.type);
}

//============================================================
// 移動量の設定
//============================================================
void CEditActor::MoveSet(void)
{
	// キーボード情報
	CInputKeyboard* m_pKeyboard = CManager::GetInstance()->GetKeyboard();

	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_UP_MOVE))
		{
			m_fMove += CALC_MOVE;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN_MOVE))
		{
			m_fMove -= CALC_MOVE;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_UP_MOVE))
		{
			m_fMove += CALC_MOVE;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_MOVE))
		{
			m_fMove -= CALC_MOVE;
		}
	}

	// 移動量を補正
	useful::LimitMinNum(m_fMove, 0.0f);
}

//============================================================
// 向きの補正処理
//============================================================
void CEditActor::RotCorrect(void)
{
	// 向きを取得
	D3DXVECTOR3 rot = GetVec3Rotation();

	if (rot.y >= -D3DX_PI * 0.25f &&
		rot.y <= D3DX_PI * 0.25f)
	{ // 0度補正前提で誤差が大きすぎる場合

		// 向きを設定する
		rot.y = 0.0f;
	}
	else if (rot.y >= D3DX_PI * 0.25f &&
		rot.y <= D3DX_PI * 0.75f)
	{ // 90度補正前提で誤差が大きすぎる場合

		// 向きを設定する
		rot.y = D3DX_PI * 0.5f;
	}
	else if ((rot.y >= D3DX_PI * -0.75f &&
		rot.y <= D3DX_PI * -0.25f))
	{ // 270度補正前提で誤差が大きすぎる場合

		// 向きを設定する
		rot.y = D3DX_PI * -0.5f;
	}
	else
	{ // 上記以外

		// 向きを設定する
		rot.y = -D3DX_PI;
	}

	// 向きを適用する
	SetVec3Rotation(rot);
}

//============================================================
//	アクターの生成処理
//============================================================
void CEditActor::CreateActor(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// アクターを配置
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	アクターの情報を配置用に変更
		//----------------------------------------------------
		// 自動更新・自動描画をONにする
		m_pActor->SetEnableUpdate(true);
		m_pActor->SetEnableDraw(true);

		// ふすまの設定処理
		SetMash();

		// 回転扉の設定処理
		SetSpinWall();

		// 未保存を設定
		m_bSave = false;

		//----------------------------------------------------
		//	新しいアクターの生成
		//----------------------------------------------------
		// アクターの生成
		m_pActor = CActor::Create
		( // 引数
			m_infoCreate.type,		// 種類
			GetVec3Position(),		// 位置
			VEC3_ZERO,				// 向き
			m_infoCreate.scale		// 拡大率
		);
		assert(m_pActor != nullptr);

		// 向きを設定する
		m_pActor->SetVec3Rotation(GetVec3Rotation());

		for (auto& rCube : m_pActor->GetCube())
		{ // コリジョンキューブ数分繰り返す

			// 大きさを設定する
			rCube->SetWidth(rCube->GetWidth() / m_infoCreate.scale.x);
			rCube->SetHeight(rCube->GetHeight() / m_infoCreate.scale.y);
			rCube->SetDepth(rCube->GetDepth() / m_infoCreate.scale.z);
		}

		for (auto& rCylinder : m_pActor->GetCylinder())
		{ // コリジョンシリンダー数分繰り返す

			// 大きさを設定する
			rCylinder->SetRadius(rCylinder->GetRadius() / m_infoCreate.scale.x);
			rCylinder->SetHeight(rCylinder->GetHeight() / m_infoCreate.scale.y);
		}

		for (auto& rSphere : m_pActor->GetSphere())
		{ // コリジョンスフィア数分繰り返す

			// 大きさを設定する
			rSphere->SetRadius(rSphere->GetRadius() / m_infoCreate.scale.x);
		}

		for (auto& rPolygon : m_pActor->GetPolygon())
		{ // コリジョンポリゴン数分繰り返す

			// 大きさを設定する
			rPolygon->SetSize(D3DXVECTOR3
			(
				rPolygon->GetSize().x / m_infoCreate.scale.x,
				rPolygon->GetSize().y / m_infoCreate.scale.y,
				rPolygon->GetSize().z / m_infoCreate.scale.z
			));
		}
	}
}

//============================================================
// ふすまの設定処理
//============================================================
void CEditActor::SetMash(void)
{
	// ふすまにダイナミックキャスト
	CMash* pMash = dynamic_cast<CMash*>(m_pActor);

	// 動くふすまが NULL の場合、抜ける
	if (pMash == nullptr) { return; }

#ifdef _DEBUG
	// 初期位置を反映する
	pMash->SetDefaultPos(m_pActor->GetVec3Position());
#endif
}

//============================================================
// 回転扉の設定処理
//============================================================
void CEditActor::SetSpinWall(void)
{
	// 回転扉にダイナミックキャスト
	CSpinWall* pWall = dynamic_cast<CSpinWall*>(m_pActor);

	// 回転扉が NULL の場合、抜ける
	if (pWall == nullptr) { return; }

#ifdef _DEBUG
	// 初期位置を反映する
	pWall->SetDefaultRot(m_pActor->GetVec3Rotation());
#endif
}

//============================================================
//	アクターの破棄処理
//============================================================
void CEditActor::ReleaseActor(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
	bool bRelease = false;	// 破棄状況

	// アクターを削除
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// 破棄する状態を設定
		bRelease = true;
	}

	// アクターの削除判定
	DeleteCollisionActor(bRelease);
}

//============================================================
//	アクターの削除判定
//============================================================
void CEditActor::DeleteCollisionActor(const bool bRelase)
{
	CListManager<CActor>* pListManager = CActor::GetList();	// アクターリストマネージャー
	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
	std::list<CActor*> listActor = pListManager->GetList();	// アクターリスト情報

	D3DXVECTOR3 posEdit = GetVec3Position();	// エディットの位置
	for (auto& rList : listActor)
	{ // アクター数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pActor) { continue; }

		D3DXVECTOR3 posOther = rList->GetVec3Position();	// 対象の地面位置
		D3DXVECTOR3 thisMax = VEC3_ZERO;	// 自身の大きさ
		D3DXVECTOR3 thisMin = VEC3_ZERO;	// 自身の大きさ
		D3DXVECTOR3 otherMax = VEC3_ZERO;	// 対象の大きさ
		D3DXVECTOR3 otherMin = VEC3_ZERO;	// 対象の大きさ

		// 自身の大きさを設定
		D3DXVECTOR3 scaleThisActor = m_pActor->GetVec3Scaling();	// 自身の地面の大きさ
		D3DXVECTOR3 MaxThisActor = m_pActor->GetModelData().vtxMax;	// 自身の地面の大きさ
		D3DXVECTOR3 MinThisActor = m_pActor->GetModelData().vtxMin;	// 自身の地面の大きさ
		thisMax.x = scaleThisActor.x * MaxThisActor.x;
		thisMax.y = scaleThisActor.y * MaxThisActor.y;
		thisMax.z = scaleThisActor.z * MaxThisActor.z;
		thisMin.x = scaleThisActor.x * -MinThisActor.x;
		thisMin.y = scaleThisActor.y * -MinThisActor.y;
		thisMin.z = scaleThisActor.z * -MinThisActor.z;

		// 対象の大きさを設定
		D3DXVECTOR3 scaleOtherActor = rList->GetVec3Scaling();		// 対象の地面の大きさ
		D3DXVECTOR3 MaxOtherActor = rList->GetModelData().vtxMax;	// 対象の地面の大きさ
		D3DXVECTOR3 MinOtherActor = rList->GetModelData().vtxMin;	// 対象の地面の大きさ
		otherMax.x = scaleOtherActor.x * MaxOtherActor.x;
		otherMax.y = scaleOtherActor.y * MaxOtherActor.y;
		otherMax.z = scaleOtherActor.z * MaxOtherActor.z;
		otherMin.x = scaleOtherActor.x * -MinOtherActor.x;
		otherMin.y = scaleOtherActor.y * -MinOtherActor.y;
		otherMin.z = scaleOtherActor.z * -MinOtherActor.z;

		// 矩形の当たり判定
		if (collision::Box3D
		( // 引数
			posEdit,	// 判定位置
			posOther,	// 判定目標位置
			thisMax,	// 判定サイズ(右・上・後)
			thisMin,	// 判定サイズ(左・下・前)
			otherMax,	// 判定目標サイズ(右・上・後)
			otherMin	// 判定目標サイズ(左・下・前)
		))
		{ // 判定内だった場合

			if (bRelase)
			{ // 破棄する場合

				// 終了処理
				rList->Uninit();

				// 未保存を設定
				m_bSave = false;
			}
			else
			{ // 破棄しない場合

				// 赤を設定
				rList->SetAllMaterial(material::Red());
			}
		}
		else
		{ // 判定外だった場合

			// 通常色を設定
			rList->ResetMaterial();
		}
	}
}

//============================================================
//	アクターの色の全初期化処理
//============================================================
void CEditActor::InitAllColorActor(void)
{
	CListManager<CActor>* pListManager = CActor::GetList();	// アクターリストマネージャー
	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
	std::list<CActor*> listActor = pListManager->GetList();	// アクターリスト情報

	for (auto& rList : listActor)
	{ // アクター数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pActor) { continue; }

		// 通常色を設定
		rList->ResetMaterial();
	}
}

//============================================================
//	保存処理
//============================================================
HRESULT CEditActor::Save(void)
{
#if _DEBUG

	// 地面のリストを取得
	CListManager<CActor>* pListManager = CActor::GetList();	// リストマネージャー
	std::list<CActor*> listActor;	// 地面リスト
	if (pListManager != nullptr)
	{ // リストマネージャーが生成されている場合

		// リストを取得
		listActor = pListManager->GetList();
	}

	// ファイルを開く
	std::ofstream  file(SAVE_PASS);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "アクター配置の書き出しに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 見出しを書き出し
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	アクター配置のセーブデータ [save_actor.txt]" << std::endl;
	file << "#	Author : 藤田 勇一" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# この行から下をコピーし [actor.txt] に張り付け\n" << std::endl;

	// アクターの色の全初期化
	InitAllColorActor();

	// 小数点書き出しの方法を指定
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// 読み込み開始文字列を書き出し
	file << "STAGE_ACTORSET\n" << std::endl;

	for (const auto& rList : listActor)
	{ // 地面の総数分繰り返す

		// 同じアドレスだった場合または、ドア・竹の格子の場合次へ
		if (rList == m_pActor ||
			rList->GetType() == CActor::TYPE_GATE ||
			rList->GetType() == CActor::TYPE_DOOR || 
			rList->GetType() == CActor::TYPE_B_LATTICE)
		{
			continue;
		}

		// 書き出す情報を取得
		CActor::EType type = rList->GetType();			// 種類
		D3DXVECTOR3 pos = rList->GetVec3Position();		// 位置
		D3DXVECTOR3 rot = rList->GetVec3Rotation();		// 向き
		D3DXVECTOR3 scale = rList->GetVec3Scaling();	// 大きさ

		if (typeid(*rList) == typeid(CMash))
		{ // ふすまの場合

			// ふすまにダイナミックキャスト
			CMash* pMash = dynamic_cast<CMash*>(rList);
			if (pMash == nullptr) { assert(false); }

			// 初期位置を取得する
			pos = pMash->GetDefaultPos();
		}
		else if (typeid(*rList) == typeid(CSpinWall))
		{ // 回転扉の場合

			// ふすまにダイナミックキャスト
			CSpinWall* pWall = dynamic_cast<CSpinWall*>(rList);
			if (pWall == nullptr) { assert(false); }

			// 初期向きを取得する
			rot = pWall->GetDefaultRot();
		}

		// 向きを360度に変換
		rot = D3DXToDegree(rot);

		// 情報を書き出し
		file << "	ACTORSET" << std::endl;
		file << "		TYPE	= " << type << std::endl;
		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		file << "		ROT		= " << rot.x << " " << rot.y << " " << rot.z << std::endl;
		file << "		SCALE	= " << scale.x << " " << scale.y << " " << scale.z << std::endl;
		file << "	END_ACTORSET\n" << std::endl;
	}

	// 読み込み終了文字列を書き出し
	file << "END_STAGE_ACTORSET" << std::endl;

	// アクターの削除判定
	DeleteCollisionActor(false);

	// 保存済みにする
	m_bSave = true;

	// 成功を返す
	return S_OK;

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}
#endif
