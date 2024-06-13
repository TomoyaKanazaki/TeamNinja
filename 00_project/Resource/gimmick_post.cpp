//=========================================
//
//  �ݒu�M�~�b�N (gimmick_post.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_post.h"
#include "manager.h"
#include "objectMeshCube.h"

#include "player.h"
#include "player_clone.h"

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickPost::CGimmickPost() : CGimmickAction(),
m_pButton(nullptr), // �{�^��
m_pEdge(nullptr), // ��
m_bPost(false) // �ݒu�t���O
{

}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickPost::~CGimmickPost()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickPost::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CGimmickAction::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �{�^���̐���
	m_pButton = CObjectMeshCube::Create
	( // ����
		VEC3_ZERO,
		VEC3_ZERO,
		VEC3_ZERO,
		XCOL_BLUE
	);
	if (m_pButton == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���x�����M�~�b�N�ɂ���
	m_pButton->SetLabel(CObject::LABEL_GIMMICK);

	// ���̐���
	m_pEdge = CObjectMeshCube::Create
	( // ����
		VEC3_ZERO,
		VEC3_ZERO,
		VEC3_ZERO,
		XCOL_WHITE
	);
	if (m_pEdge == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���x�����M�~�b�N�ɂ���
	m_pEdge->SetLabel(CObject::LABEL_GIMMICK);

	// ������Ԃ�
	return S_OK;
}

//=========================================
//  �I������
//=========================================
void CGimmickPost::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmickAction::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickPost::Update(const float fDeltaTime)
{
	// �A�N�e�B�u��ԂɂȂ��Ă�����ݒu�t���O��on
	m_bPost = IsActive();

	// �e�N���X�̍X�V
	CGimmickAction::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickPost::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmickAction::Draw(pShader);
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CGimmickPost::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �e�N���X�̈ʒu�ݒ�
	CGimmickAction::SetVec3Position(rPos);

	// �{�^���̈ʒu�ݒ�
	m_pButton->SetVec3Position(rPos);

	// ���̈ʒu�ݒ�
	m_pEdge->SetVec3Position(rPos);
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CGimmickPost::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �e�N���X�̑傫���ݒ�
	CGimmickAction::SetVec3Sizing(rSize);

	// �{�^���̑傫���ݒ�
	m_pButton->SetVec3Sizing(rSize * 0.5f + D3DXVECTOR3(-5.0f, 15.0f, -5.0f));

	// ���̑傫���ݒ�
	m_pEdge->SetVec3Sizing(rSize * 0.5f + D3DXVECTOR3(0.0f, 5.0f, 0.0f));
}
