//=========================================
//
//  �����Ǘ��M�~�b�N (gimmick_mulch.cpp)
//  Author : Tomoya kanazaki
//  Adder  : Yuichi Fujita
//
//=========================================
#include "gimmick_mulch.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"
#include "actor.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const int NUM_CLONE = 1;	// �{�^���������݂ɕK�v�Ȑl��
}

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickMulch::CGimmickMulch() : CGimmick(),
m_bActive (true), // �A�N�e�B�u�t���O
m_pModel (nullptr) // ���f�����
{
	// �{�^�����I�z����N���A
	m_vecButton.clear();
}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickMulch::~CGimmickMulch()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickMulch::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CGimmick::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f���̐���
	m_pModel = CActor::Create(CActor::TYPE_B_LATTICE, GetVec3Position(), VEC3_ZERO);
	if (m_pModel == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f���̃��x����ύX
	m_pModel->SetLabel(CObject::LABEL_GIMMICK);

	// ������Ԃ�
	return S_OK;
}

//=========================================
//  �I������
//=========================================
void CGimmickMulch::Uninit(void)
{
	// �g���f���̏I��
	SAFE_UNINIT(m_pModel);

	// �{�^�����I�z����N���A
	m_vecButton.clear();

	// �e�N���X�̏I��
	CGimmick::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickMulch::Update(const float fDeltaTime)
{
	// �t���O�����Z�b�g
	m_bActive = true;

	// �Ǘ����Ă���M�~�b�N�̐ݒu�t���O���擾
	for (const auto& rGimmick : m_vecButton)
	{ // �{�^���z�񕪌J��Ԃ�

		// �M�~�b�N�̃t���O�������Ă����玟�ɐi��
		if (rGimmick->IsSet()) { continue; }

		// 1�ł��t���O��off�Ȃ�t���O��false�ɂ���
		m_bActive = false;
		break;
	}

	// �e�N���X�̍X�V
	CGimmick::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickMulch::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmick::Draw(pShader);
}

//===========================================
//  �ʒu�̐ݒ�
//===========================================
void CGimmickMulch::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �e�N���X�̐ݒ菈�����Ă�
	CGimmick::SetVec3Position(rPos);

	// �����ڂ̈ʒu��ݒ�
	m_pModel->SetVec3Position(rPos);

	// �p�x���擾
	EAngle angle = GetAngle();
	D3DXVECTOR3 rot = VEC3_ZERO;
	switch (angle)
	{
	case EAngle::ANGLE_0: // 0
		rot.y = D3DX_PI * 0.0f;
		break;

	case EAngle::ANGLE_90: // 1.57
		rot.y = D3DX_PI * 0.5f;
		break;

	case EAngle::ANGLE_180: // 3.14
		rot.y = D3DX_PI * 1.0f;
		break;

	case EAngle::ANGLE_270: // 4.71
		rot.y = D3DX_PI * 1.5f;
		break;

	default:
		assert(false);
		break;
	}

	// �����ڂ̊p�x��ݒ�
	m_pModel->SetVec3Rotation(rot);
}

//===========================================
//  �ݒu�M�~�b�N�̐���
//===========================================
CGimmickMulch* CGimmickMulch::Create(const D3DXVECTOR3& rPos, const EAngle angle, std::vector<SButton> vecButton)
{
	// �����Ǘ��M�~�b�N�̐���
	CGimmickMulch *pGimmick = new CGimmickMulch;
	if (pGimmick == nullptr) { return nullptr; }

	// �����Ǘ��M�~�b�N�̏�����
	if (FAILED(pGimmick->Init()))
	{ // �������Ɏ��s�����ꍇ

		// �����Ǘ��M�~�b�N�̔j��
		SAFE_DELETE(pGimmick);
		return nullptr;
	}

	// �{�^�����̐���
	if (FAILED(pGimmick->CreateButton(vecButton)))
	{ // �����Ɏ��s�����ꍇ

		// �����Ǘ��M�~�b�N�̔j��
		SAFE_DELETE(pGimmick);
		return nullptr;
	}

	// �����̐ݒ�
	pGimmick->SetAngle(angle);

	// ���W�̐ݒ�
	pGimmick->SetVec3Position(rPos);

	// �m�ۂ����A�h���X��Ԃ�
	return pGimmick;
}

//===========================================
//  �{�^�����̐���
//===========================================
HRESULT CGimmickMulch::CreateButton(std::vector<SButton> vecButton)
{
	for (const auto& rButton : vecButton)
	{ // �{�^���z�񕪌J��Ԃ�

		// �{�^���̐���
		CGimmick* pButton = CGimmick::Create
		( // ����
			rButton.pos,			// �����ʒu
			EAngle::ANGLE_0,		// �����p�x
			rButton.size,			// �����傫��
			CGimmick::TYPE_POST,	// �M�~�b�N���x�� (�{�^��)
			NUM_CLONE				// �{�^�������ɕK�v�Ȑl��
		);
		if (pButton == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// �Ō���ɐ��������{�^����ۑ�
		m_vecButton.push_back(pButton);
	}

	return S_OK;
}
