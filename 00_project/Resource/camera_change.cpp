//===========================================
//
//  カメラ方向変更判定クラス(camera_change.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "camera_change.h"

//===========================================
//  コンストラクタ
//===========================================
CCameraChanger::CCameraChanger() : CObjectMeshCube(),
m_bChange(false),
m_eDir(DIRECTION_FRONT),
m_eRot(ROTATION_DEFAULT)
{
}

//===========================================
//  デストラクタ
//===========================================
CCameraChanger::~CCameraChanger()
{
}

//===========================================
//  初期化処理
//===========================================
HRESULT CCameraChanger::Init()
{
	return E_NOTIMPL;
}

//===========================================
//  終了処理
//===========================================
void CCameraChanger::Uninit()
{
}

//===========================================
//  更新処理
//===========================================
void CCameraChanger::Update(const float fDeltaTime)
{
}

//===========================================
//  描画処理
//===========================================
void CCameraChanger::Draw(CShader* pShader)
{
}

//===========================================
//  生成処理
//===========================================
CCameraChanger* CCameraChanger::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const EDirection Dir, const ERotation Rot)
{
	// メモリの確保
	CCameraChanger* pChanger = new CCameraChanger;

	// 確保に失敗した場合nullを返す
	if (pChanger == nullptr) { assert(false); return nullptr; }

	// 初期化処理
	if(FAILED(pChanger->Init())) {}

	return nullptr;
}
