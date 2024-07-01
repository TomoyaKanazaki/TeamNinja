//===========================================
//
//  �J���������ύX����N���X(camera_change.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "camera_change.h"

//===========================================
//  �R���X�g���N�^
//===========================================
CCameraChanger::CCameraChanger() : CObjectMeshCube(),
m_bChange(false),
m_eDir(DIRECTION_FRONT),
m_eRot(ROTATION_DEFAULT)
{
}

//===========================================
//  �f�X�g���N�^
//===========================================
CCameraChanger::~CCameraChanger()
{
}

//===========================================
//  ����������
//===========================================
HRESULT CCameraChanger::Init()
{
	return E_NOTIMPL;
}

//===========================================
//  �I������
//===========================================
void CCameraChanger::Uninit()
{
}

//===========================================
//  �X�V����
//===========================================
void CCameraChanger::Update(const float fDeltaTime)
{
}

//===========================================
//  �`�揈��
//===========================================
void CCameraChanger::Draw(CShader* pShader)
{
}

//===========================================
//  ��������
//===========================================
CCameraChanger* CCameraChanger::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const EDirection Dir, const ERotation Rot)
{
	// �������̊m��
	CCameraChanger* pChanger = new CCameraChanger;

	// �m�ۂɎ��s�����ꍇnull��Ԃ�
	if (pChanger == nullptr) { assert(false); return nullptr; }

	// ����������
	if(FAILED(pChanger->Init())) {}

	return nullptr;
}
