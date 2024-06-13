//============================================================
//
// �d�����̏��� [gimmick_heavydoor.cpp]
// Author�F����������
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

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 MOVEUP = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// �����オ��ړ���
	const D3DXVECTOR3 MOVEDOWN = D3DXVECTOR3(0.0f, 20.0f, 0.0f);	// ����������ړ���
	const float GRAVITY = 60.0f;		// �d��

	const int OPEN_DOOR = 60 * 5;		// �h�A���J���܂ł̎���
	const int NUM_BOUND = 5;			// �������˂��
}
//============================================================
//	�R���X�g���N�^
//============================================================
CGimmickHeavyDoor::CGimmickHeavyDoor() : CGimmickAction(),
m_pRoofModel(nullptr),	// �I�u�W�F�N�g���f��(����)�̏��
m_pDoorModel(nullptr),	// �I�u�W�F�N�g���f��(��)�̏��
m_pStage(nullptr),			// �X�e�[�W�̏��
m_pos(VEC3_ZERO),			// ���̈ʒu
m_posSave(VEC3_ZERO),		// �ʒu�ۑ��p
m_move(VEC3_ZERO),			// �ړ���
m_nDoorCounter(0),			// ���̊J�J�E���^�[
m_nNumBound(0),				// �������˂���
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
	m_pRoofModel = nullptr;	// �������f���̏��
	m_pDoorModel = nullptr;	// �����f���̏��
	m_pStage = CManager::GetInstance()->GetScene()->GetStage();		// �X�e�[�W�̏��
	m_pos = D3DXVECTOR3(-1400.0f, 0.0f, -200.0f);	// ���̈ʒu
	m_posSave = VEC3_ZERO;	// �ʒu�ۑ��p
	m_move = VEC3_ZERO;		// �ړ���
	m_nDoorCounter = 0;		// ���̊J�J�E���^�[
	m_nNumBound = 1;		// �������˂���
	m_state = STATE_CLOSE;	// ���̏��


	//-----------------------------------------------------------
	//	���f���̐���
	//-----------------------------------------------------------

	// �����̐���
	m_pRoofModel = CMapModel::Create
	(
		m_pos,
		VEC3_ZERO,
		CMapModel::MODEL_TYPE_DOOR00
	);
	if (m_pRoofModel == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���̐���
	m_pDoorModel = CMapModel::Create
	(
		m_pos,
		VEC3_ZERO,
		CMapModel::MODEL_TYPE_DOOR01
	);
	if (m_pDoorModel == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �M�~�b�N�A�N�V�����̏�����
	if (FAILED(CGimmickAction::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGimmickHeavyDoor::Uninit(void)
{
	// �������f���̏I��
	SAFE_UNINIT(m_pRoofModel);

	// �����f���̏I��
	SAFE_UNINIT(m_pDoorModel);

	// �X�e�[�W�̏I��
	SAFE_UNINIT(m_pStage);

	// �M�~�b�N�A�N�V�����̏I��
	CGimmickAction::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGimmickHeavyDoor::Update(const float fDeltaTime)
{
	switch (m_state)
	{
	case CGimmickHeavyDoor::STATE_NONE:		// �������ĂȂ�
		break;
	case CGimmickHeavyDoor::STATE_CLOSE:	// �����Ă�

		if (IsActive())
		{ // �K�v�Ȑl���������Ă�ꍇ

			m_state = STATE_OPEN;		// ���グ��
			m_move = MOVEUP;			// �ړ���

			// ���グ�Ă�
			SetMoment(true);
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
// �����グ��
//============================================================
void CGimmickHeavyDoor::OpenTheDoor(void)
{
	D3DXVECTOR3 pos;		// �ʒu

	// �ʒu�擾
	pos = m_pDoorModel->GetVec3Position();

	// �ړ��ʉ��Z
	pos += m_move;
	m_posSave += m_move;	// �ʒu�ۑ�

	// �ʒu�ݒ�
	m_pDoorModel->SetVec3Position(pos);

	if (m_nDoorCounter >= OPEN_DOOR || IsActive() == false)
	{ // ��莞�Ԍo������

		m_state = STATE_FULLY;		// ���S�J���
		m_nDoorCounter = 0;			// �J�E���^�[������
		m_move = m_posSave;			// �ړ���

	}
	else
	{ // ���Ԍo���ĂȂ��ꍇ

		m_nDoorCounter++;
	}
}

//============================================================
// ����������
//============================================================
void CGimmickHeavyDoor::CloseTheDoor(void)
{
	D3DXVECTOR3 pos;		// �ʒu

	// �ʒu�擾
	pos = m_pDoorModel->GetVec3Position();

	// �d��
	m_move.y -= GRAVITY;

	// �ړ��ʉ��Z
	pos += m_move;

	// �͈͊O�̒��n����
	if (m_pStage->LandLimitPosition(pos, m_move, 0.0f))
	{
		if (m_nNumBound <= NUM_BOUND)
		{ // ���˂��񐔂��ڕW�̉񐔂���Ȃ��ꍇ

			m_nNumBound++;		// ���˂��񐔉��Z

			// ���˂鍂���ݒ�
			m_move.y = m_posSave.y / m_nNumBound;
		}
		else
		{ // ���ː؂��ĂȂ��ꍇ

			m_state = STATE_CLOSE;	// �����Ă���
			m_posSave = VEC3_ZERO;	// �ۑ��p�̈ʒu��������

			m_nNumBound = 1;

			// ���J���ĂȂ�
			SetMoment(false);
		}
	}

	// �ʒu�ݒ�
	m_pDoorModel->SetVec3Position(pos);
}
