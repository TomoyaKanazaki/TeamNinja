//============================================================
//
// �d�����̏��� [gimmick_heavydoor.cpp]
// Author�F����������
// Adder �F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gimmick_heavydoor.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"
#include "debugproc.h"
#include "MapModel.h"
#include "stage.h"
#include "scene.h"
#include "debug.h"
#include "debugproc.h"
#include "actor.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 MOVEUP	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// �����オ��ړ���
	const D3DXVECTOR3 MOVEDOWN	= D3DXVECTOR3(0.0f, 12.0f, 0.0f);	// ����������ړ���
	const float GRAVITY	= 360.0f;	// �d��
	const float CLONE_UP = 10.0f;	// ���g�̐g���ɉ��Z����l
}

//============================================================
//	�R���X�g���N�^
//============================================================
CGimmickHeavyDoor::CGimmickHeavyDoor() : CGimmickAction(),
m_pGateModel(nullptr),		// �I�u�W�F�N�g���f��(�g)�̏��
m_pDoorModel(nullptr),		// �I�u�W�F�N�g���f��(��)�̏��
m_oldPosDoor(VEC3_ZERO),	// �ߋ��ʒu
m_move(VEC3_ZERO),			// �ړ���
m_state(STATE_NONE)			// ���̏��
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGimmickHeavyDoor::~CGimmickHeavyDoor()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGimmickHeavyDoor::Init(void)
{
	// �ϐ�������
	m_pGateModel = nullptr;		// �g���f���̏��
	m_pDoorModel = nullptr;		// �����f���̏��
	m_oldPosDoor = VEC3_ZERO;	// �ߋ��ʒu
	m_move = VEC3_ZERO;			// �ړ���
	m_state = STATE_CLOSE;		// ���̏��

	// �M�~�b�N�A�N�V�����̏�����
	if (FAILED(CGimmickAction::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//-----------------------------------------------------------
	//	���f���̐���
	//-----------------------------------------------------------
	// �g�̐���
	m_pGateModel = CActor::Create(CActor::TYPE_GATE, GetVec3Position(), VEC3_ZERO);
	if (m_pGateModel == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����X�V�E�j������郉�x����K��
	m_pGateModel->SetLabel(CObject::LABEL_GIMMICK);

	// ���̐���
	m_pDoorModel = CActor::Create(CActor::TYPE_DOOR, GetVec3Position(), VEC3_ZERO);
	if (m_pDoorModel == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����X�V�E�j������郉�x����K��
	m_pDoorModel->SetLabel(CObject::LABEL_GIMMICK);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGimmickHeavyDoor::Uninit(void)
{
	// �g���f���̏I��
	SAFE_UNINIT(m_pGateModel);

	// �����f���̏I��
	SAFE_UNINIT(m_pDoorModel);

	// �M�~�b�N�A�N�V�����̏I��
	CGimmickAction::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGimmickHeavyDoor::Update(const float fDeltaTime)
{
	// �ߋ��ʒu���X�V
	m_oldPosDoor = m_pDoorModel->GetVec3Position();
	
	// �ړ��ʉ��Z
	D3DXVECTOR3 posDoor = m_pDoorModel->GetVec3Position();
	posDoor += m_move * fDeltaTime;

	// �ʒu�ݒ�
	m_pDoorModel->SetVec3Position(posDoor);

	switch (m_state)
	{
	case CGimmickHeavyDoor::STATE_NONE:		// �������ĂȂ�
		break;

	case CGimmickHeavyDoor::STATE_CLOSE:	// �����Ă�

		if (IsActive())
		{ // �K�v�Ȑl���������Ă�ꍇ

			m_state = STATE_OPEN;	// ���グ��
			m_move = MOVEUP;		// �ړ���
		}

		break;

	case CGimmickHeavyDoor::STATE_OPEN:		// ���グ�Ă�

		// ���J��
		OpenTheDoor();

		break;

	case CGimmickHeavyDoor::STATE_FULLY:	// ���S�J

		if (IsActive() == false)
		{ // �K�v�Ȑl���������ĂȂ��ꍇ

			// ���߂�
			CloseTheDoor();
		}

		break;

	default:
		assert(false);
		break;
	}

	// �M�~�b�N�A�N�V�����̍X�V
	CGimmickAction::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CGimmickHeavyDoor::Draw(CShader* pShader)
{
	// �M�~�b�N�A�N�V�����̕`��
	CGimmickAction::Draw(pShader);
}

//============================================================
// �ʒu�̐ݒ菈��
//============================================================
void CGimmickHeavyDoor::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �e�N���X�̈ʒu�ݒ�
	CGimmickAction::SetVec3Position(rPos);

	// �g�̈ʒu�ݒ�
	m_pGateModel->SetVec3Position(rPos + D3DXVECTOR3(0.0f, 0.0f, 100.0f));

	// ���̈ʒu�ݒ�
	m_pDoorModel->SetVec3Position(rPos + D3DXVECTOR3(0.0f, 0.0f, 100.0f));
}

//============================================================
// �傫���̐ݒ菈��
//============================================================
void CGimmickHeavyDoor::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �e�N���X�̑傫���ݒ�
	CGimmickAction::SetVec3Sizing(rSize);
}

//===========================================
//  �e���g���̑ҋ@�ʒu���Z�o
//===========================================
D3DXVECTOR3 CGimmickHeavyDoor::CalcWaitPoint(const int Idx)
{
	// �󂯎�����C���f�b�N�X���ő�l�𒴂��Ă���ꍇ�x��
	if (Idx > GetNumActive()) { assert(false); }

	// ���g�̈ʒu���擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �ő吔���擾
	int nNumActive = GetNumActive();

	// �ő吔��0�̏ꍇ�֐��𔲂���
	if (nNumActive == 0) { assert(false); return pos; }

	// ���g�̉������擾
	D3DXVECTOR3 size = GetVec3Sizing();

	// �ҋ@�ʒu�̑��Βl���Z�o
	float fValue = (size.x / (float)(nNumActive + 1)) * (Idx + 1);

	// �ҋ@�ʒu��ݒ�
	D3DXVECTOR3 posWait = pos;
	posWait.x += fValue - (size.x * 0.5f);

	return posWait;
}

//============================================================
// �����グ��
//============================================================
void CGimmickHeavyDoor::OpenTheDoor(void)
{
	D3DXVECTOR3 posDoor = m_pDoorModel->GetVec3Position();	// �ʒu

	// �ړ��ʉ��Z
	posDoor += m_move;

	// �ʒu�ݒ�
	m_pDoorModel->SetVec3Position(posDoor);

	if (posDoor.y >= GetVec3Position().y + CPlayerClone::GetHeight() + CLONE_UP || IsActive() == false)
	{ // ��莞�Ԍo������

		m_state = STATE_FULLY;	// ���S�J���
	}
}

//============================================================
// ����������
//============================================================
void CGimmickHeavyDoor::CloseTheDoor(void)
{
	// �ϐ��錾
	CStage *pStage = GET_MANAGER->GetScene()->GetStage();	// �X�e�[�W���
	D3DXVECTOR3 posDoor = m_pDoorModel->GetVec3Position();	// �ʒu

	// �d��
	m_move.y -= GRAVITY;

	// �͈͊O�̒��n����
	if (pStage->LandFieldPosition(posDoor, m_oldPosDoor, m_move))	// TODO�F�o�E���h�����悤
	{
		m_pDoorModel->SetVec3Position(posDoor);	// �ʒu
		m_state = STATE_CLOSE;	// �����Ă���
	}
}
