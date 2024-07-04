//===========================================
//
//  ��C�̃M�~�b�N(gimmick_canon.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "gimmick_canon.h"
#include "manager.h"
#include "player.h"
#include "field.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const float FEILD_LINE = 2600.0f; // �t�B�[���h�̊Z��
	const int ACTIVE_NUM = 5; // �����ɕK�v�Ȑl��
}

//===========================================
//  �R���X�g���N�^
//===========================================
CGimmickCanon::CGimmickCanon() : CGimmickAction(),
m_fTarget(0.0f),
m_bShoot(false)
{
}

//===========================================
//  �f�X�g���N�^
//===========================================
CGimmickCanon::~CGimmickCanon()
{
}

//===========================================
//  ����������
//===========================================
HRESULT CGimmickCanon::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CGimmickAction::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
//  �I������
//===========================================
void CGimmickCanon::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmickAction::Uninit();
}

//===========================================
//  �X�V����
//===========================================
void CGimmickCanon::Update(const float fDeltaTime)
{
	// ���ˏ���
	if (IsActive() && DistancePlayer() && !m_bShoot)
	{
		GET_PLAYER->SetShoot(m_fTarget);
		m_bShoot = true;
	}

	// �e�N���X�̍X�V
	CGimmickAction::Update(fDeltaTime);
}

//===========================================
//  �`�揈��
//===========================================
void CGimmickCanon::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmickAction::Draw(pShader);
}

//===========================================
//  �ҋ@�ʒu�̌v�Z����
//===========================================
D3DXVECTOR3 CGimmickCanon::CalcWaitPoint(const int Idx)
{
	// TODO ���[�V�����ɍ��킹�Ă��������̏��������
	return GetVec3Position();
}

//===========================================
//  �ҋ@�ʒu�̌����̌v�Z
//===========================================
D3DXVECTOR3 CGimmickCanon::CalcWaitRotation(const int Idx, const CPlayerClone* pClone)
{
	// TODO ���[�V�����ɍ��킹�Ă��������̏��������
	return GetVec3Rotation();
}

//===========================================
//  ��������
//===========================================
CGimmickCanon* CGimmickCanon::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, ETarget eTarget)
{
	// �M�~�b�N�̐���
	CGimmickCanon* pGimmick = new CGimmickCanon;

	// �������̊m�ۂɎ��s�����ꍇ�֐��𔲂���
	if (pGimmick == nullptr) { assert(false); return nullptr; }

	// ������
	if (FAILED(pGimmick->Init())) { assert(false); SAFE_DELETE(pGimmick); return nullptr; }

	// �ʒu��ݒ�
	pGimmick->SetVec3Position(rPos);

	// �T�C�Y�̐ݒ�
	pGimmick->SetVec3Sizing(rRot);

	// �ڕW�n�_�̐ݒ�
	pGimmick->SetTarget(FEILD_LINE * eTarget);

	// �K�v�l���̐ݒ�
	pGimmick->SetNumActive(ACTIVE_NUM);

	// �m�ۂ����A�h���X��Ԃ�
	return pGimmick;
}
