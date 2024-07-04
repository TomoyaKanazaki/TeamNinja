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
namespace
{
	const float BUTTON_OFF	= 15.0f;	// �{�^���I�t�̎��̃I�t�Z�b�g
	const float BUTTON_ON	= 6.0f;		// �{�^���I���̎��̃I�t�Z�b�g
	const float BUTTON_MOVE	= 60.0f;	// �{�^������/�߂����̈ړ���

	const D3DXVECTOR3 SCALE_BUTTON	= D3DXVECTOR3(-5.0f, 0.0f, -5.0f);				// �{�^���傫���̃I�t�Z�b�g
	const D3DXVECTOR3 SCALE_EDGE	= D3DXVECTOR3(0.0f, BUTTON_ON - 1.0f, 0.0f);	// �����傫���̃I�t�Z�b�g
}

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickPost::CGimmickPost() : CGimmickAction(),
m_pButton(nullptr), // �{�^��
m_pEdge(nullptr), // ��
m_fButtonHeight(BUTTON_OFF) // �{�^���c�I�t�Z�b�g
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
		VEC3_ZERO,	// �ʒu
		VEC3_ZERO,	// ����
		VEC3_ZERO,	// �傫��
		XCOL_BLUE,	// �L���[�u�F
		XCOL_BLACK,	// �����F
		CObjectMeshCube::BORDER_OFF,			// �������
		0.0f,									// ����葾��
		CObjectMeshCube::TEXSTATE_ONE,			// �e�N�X�`�����
		CObjectMeshCube::SFaceTex(NONE_IDX),	// �e�N�X�`�����
		CObjectMeshCube::ORIGIN_DOWN			// ���_
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
		VEC3_ZERO,	// �ʒu
		VEC3_ZERO,	// ����
		VEC3_ZERO,	// �傫��
		XCOL_WHITE,	// �L���[�u�F
		XCOL_BLACK,	// �����F
		CObjectMeshCube::BORDER_OFF,			// �������
		0.0f,									// ����葾��
		CObjectMeshCube::TEXSTATE_ONE,			// �e�N�X�`�����
		CObjectMeshCube::SFaceTex(NONE_IDX),	// �e�N�X�`�����
		CObjectMeshCube::ORIGIN_DOWN			// ���_
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
	// �A�N�e�B�u��ԂȂ�{�^����������Ă�
	if (IsActive())
	{ // �{�^����������Ă���ꍇ

		// �{�^������������
		m_fButtonHeight -= BUTTON_MOVE * fDeltaTime;
	}
	else
	{ // �{�^����������Ă��Ȃ��ꍇ

		// �{�^���������グ��
		m_fButtonHeight += BUTTON_MOVE * fDeltaTime;
	}

	// �{�^���̑傫���ݒ�
	SetButtonSizing();

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

//=========================================
//	�ʒu�̐ݒ菈��
//=========================================
void CGimmickPost::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �e�N���X�̈ʒu�ݒ�
	CGimmickAction::SetVec3Position(rPos);

	// �{�^���̈ʒu�ݒ�
	m_pButton->SetVec3Position(rPos);

	// ���̈ʒu�ݒ�
	m_pEdge->SetVec3Position(rPos);
}

//=========================================
//	�傫���̐ݒ菈��
//=========================================
void CGimmickPost::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �e�N���X�̑傫���ݒ�
	CGimmickAction::SetVec3Sizing(rSize);

	// �{�^���̑傫���ݒ�
	SetButtonSizing();

	// ���̑傫���ݒ�
	m_pEdge->SetVec3Sizing(rSize * 0.5f + SCALE_EDGE);
}

//===========================================
//  �e���g���̑ҋ@�ʒu���Z�o
//===========================================
D3DXVECTOR3 CGimmickPost::CalcWaitPoint(const int Idx)
{
	D3DXVECTOR3 posWait = GetVec3Position();	// �ҋ@�ʒu

	// �M�~�b�N���_�ʒu�ɓ��ݍ���ł�{�^���̍�����^����
	posWait.y += m_pButton->GetVec3Sizing().y * 2.0f;

	// �Z�o�����ʒu��Ԃ�
	return posWait;
}

//===========================================
//  �e���g���̑ҋ@�������Z�o
//===========================================
D3DXVECTOR3 CGimmickPost::CalcWaitRotation(const int Idx, const CPlayerClone* pClone)
{
	// �󂯎�����C���f�b�N�X���ő�l�𒴂��Ă���ꍇ�x��
	if (Idx > GetNumActive()) { assert(false); }

	// TODO�F�����v�Z�ǂ�����

	// �v���C���[�̈ʒu���擾
	D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();

	// �ҋ@�ʒu���擾
	D3DXVECTOR3 posThis = GetActionPoint();

	// �ڕW�����Ƃ̍��������߂�
	D3DXVECTOR3 vecTarget = posPlayer - posThis;

	// �ҋ@���������߂�
	D3DXVECTOR3 rotWait = VEC3_ZERO;
	rotWait.y = atan2f(-vecTarget.x, -vecTarget.z);

	// �Z�o����������Ԃ�
	return rotWait;
}

//=========================================
//	�{�^���傫���̐ݒ菈��
//=========================================
void CGimmickPost::SetButtonSizing(void)
{
	D3DXVECTOR3 size = GetVec3Sizing() * 0.5f + SCALE_BUTTON;	// �{�^���̑傫��

	// �{�^���̓��܂��𔽉f
	useful::LimitNum(m_fButtonHeight, BUTTON_ON, BUTTON_OFF);	// ���܂���␳
	size.y = m_fButtonHeight;

	// �{�^���̑傫���ݒ�
	m_pButton->SetVec3Sizing(size);
}
