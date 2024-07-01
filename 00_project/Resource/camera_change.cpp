//===========================================
//
//  �J���������ύX����N���X(camera_change.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "camera_change.h"

//===========================================
//  �ÓI�����o�ϐ��錾
//===========================================
CListManager<CCameraChanger>* CCameraChanger::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//===========================================
//  �R���X�g���N�^
//===========================================
CCameraChanger::CCameraChanger() : CObjectMeshCube(CObject::LABEL_COLLISION),
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
	// �e�N���X�̏�����
	if (FAILED(CObjectMeshCube::Init())) { assert(false); return E_FAIL; }

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CCameraChanger>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// �F��ύX
	D3DXCOLOR col = XCOL_ACYAN;
	col.a = 0.5f;
	SetCubeColor(col);

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// ������Ԃ�
	return S_OK;
}

//===========================================
//  �I������
//===========================================
void CCameraChanger::Uninit()
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �e�N���X�̏I��
	CObjectMeshCube::Uninit();
}

//===========================================
//  �X�V����
//===========================================
void CCameraChanger::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V����
	CObjectMeshCube::Update(fDeltaTime);
}

//===========================================
//  �`�揈��
//===========================================
void CCameraChanger::Draw(CShader* pShader)
{
#ifndef _DEBUG
	// �f�o�b�O���ȊO�͕`�悵�����Ȃ�
	return;
#endif

	//�e�N���X�̕`�揈��
	CObjectMeshCube::Draw(pShader);
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
	if (FAILED(pChanger->Init())) { assert(false); return nullptr; }

	// �ʒu�̐ݒ�
	pChanger->SetVec3Position(rPos);

	// �T�C�Y�̐ݒ�
	pChanger->SetVec3Sizing(rSize);

	// �J�������̐ݒ�
	pChanger->m_eDir = Dir;
	pChanger->m_eRot = Rot;

	return pChanger;
}

//===========================================
//  ���X�g�̎擾
//===========================================
CListManager<CCameraChanger>* CCameraChanger::GetList(void)
{
	return m_pList;
}
