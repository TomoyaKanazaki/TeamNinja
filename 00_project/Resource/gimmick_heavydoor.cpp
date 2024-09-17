//============================================================
//
// �d�����̏��� [gimmick_heavydoor.cpp]
// Author�F����������
// Adder �F���c�E��
// Adder �F�������
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
#include "multi_plant.h"
#include "sound.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 WAIT_OFFSET	= D3DXVECTOR3(65.0f, 0.0f, 65.0f);	// �ҋ@�ʒu�̃I�t�Z�b�g
	const D3DXVECTOR3 MOVEUP		= D3DXVECTOR3(0.0f, 60.0f, 0.0f);	// �����オ��ړ���
	const float GRAVITY	= 360.0f;	// �d��
	const float CLONE_UP = 2.0f;	// ���g�̐g���ɉ��Z����l

	const CCamera::SSwing OPEN_SWING = CCamera::SSwing(9.0f, 2.0f, 0.1f);		// �J�����̗h��̒l
}

//============================================================
//	�R���X�g���N�^
//============================================================
CGimmickHeavyDoor::CGimmickHeavyDoor() : CGimmickAction(),
m_pGateModel(nullptr),		// �I�u�W�F�N�g���f��(�g)�̏��
m_pDoorModel(nullptr),		// �I�u�W�F�N�g���f��(��)�̏��
m_oldPosDoor(VEC3_ZERO),	// �ߋ��ʒu
m_move(VEC3_ZERO),			// �ړ���
m_state(STATE_NONE),		// ���̏��
m_nIdxWait(0),				// �ҋ@���S�̃C���f�b�N�X
m_vecToWait(VEC3_ZERO)		// ���S���W����ҋ@���S�ւ̃x�N�g��(�P�ʃx�N�g��)
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

	// ���g�̍��W
	D3DXVECTOR3 pos = GetVec3Position();

	//-----------------------------------------------------------
	//	���f���̐���
	//-----------------------------------------------------------
	// �g�̐���
	m_pGateModel = CActor::Create(CActor::TYPE_GATE, pos);
	if (m_pGateModel == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����X�V�E�j������郉�x����K��
	m_pGateModel->SetLabel(CObject::LABEL_GIMMICK);

	// ���̐���
	m_pDoorModel = CActor::Create(CActor::TYPE_DOOR, pos);
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
	// ����������
	bool bFar = useful::IsNearPosR(GetVec3Position());
	if (!bFar) { return; }

	// �ߋ��ʒu���X�V
	m_oldPosDoor = m_pDoorModel->GetVec3Position();
	
	// �ړ��ʉ��Z
	D3DXVECTOR3 posDoor = m_pDoorModel->GetVec3Position();
	posDoor += m_move * fDeltaTime;

	switch (m_state)
	{
	case CGimmickHeavyDoor::STATE_NONE:		// �������ĂȂ�
		break;

	case CGimmickHeavyDoor::STATE_CLOSE:	// �����Ă�

		if (IsActive())
		{ // �K�v�Ȑl���������Ă�ꍇ

			m_move = MOVEUP;		// �ړ���
			m_state = STATE_OPEN;	// ���グ��

			// �J�����h���ݒ肷��
			CManager::GetInstance()->GetCamera()->SetSwing(CCamera::TYPE_MAIN, OPEN_SWING);

			// �h�A���J������炷
			PLAY_SOUND(CSound::LABEL_SE_DOOROPEN);
		}

		break;

	case CGimmickHeavyDoor::STATE_OPEN:		// ���グ�Ă�

		// ���J��
		OpenTheDoor(posDoor);

		break;

	case CGimmickHeavyDoor::STATE_FULLY:	// ���S�J

		if (IsActive() == false)
		{ // �K�v�Ȑl���������ĂȂ��ꍇ

			// ���߂�
			CloseTheDoor(posDoor);
		}

		break;

	default:
		assert(false);
		break;
	}

	// �ʒu�ݒ�
	m_pDoorModel->SetVec3Position(posDoor);

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

	// �����̐ݒ�
	D3DXVECTOR3 rot = VEC3_ZERO;
	switch (GetAngle())
	{
	case EAngle::ANGLE_0: // 0
		rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f);
		break;

	case EAngle::ANGLE_90: // 1.57
		rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
		break;

	case EAngle::ANGLE_180: // 3.14
		rot = D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f);
		break;

	case EAngle::ANGLE_270: // 4.71
		rot = D3DXVECTOR3(0.0f, D3DX_PI * 1.5f, 0.0f);
		break;

	default:
		assert(false);
		break;
	}

	// �����ڂ̕����ݒ�
	m_pGateModel->SetVec3Rotation(rot);
	m_pDoorModel->SetVec3Rotation(rot);

	// �����ڂ̈ʒu�ݒ�
	m_pGateModel->SetVec3Position(rPos);
	m_pDoorModel->SetVec3Position(rPos);

	// �����ڂ̓����蔻��I�t�Z�b�g�ݒ菈��
	m_pGateModel->CollisionOffset();
	m_pDoorModel->CollisionOffset();
}

//============================================================
// �傫���̐ݒ菈��
//============================================================
void CGimmickHeavyDoor::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �e�N���X�̑傫���ݒ�
	CObject3D::SetVec3Sizing(rSize);

	// ���W���擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �Ԃ̐�������ݒ肷��
	D3DXVECTOR3 posPlant[2] = {};
	D3DXVECTOR3 sizePlant = {};
	if ((int)GetAngle() % 2)
	{
		posPlant[0] = pos + D3DXVECTOR3(rSize.x * 0.25f, 0.0f, 0.0f);
		posPlant[1] = pos - D3DXVECTOR3(rSize.x * 0.25f, 0.0f, 0.0f);
		sizePlant = D3DXVECTOR3(rSize.x * 0.5f, 0.0f, rSize.z);
	}
	else
	{
		posPlant[0] = pos + D3DXVECTOR3(0.0f, 0.0f, rSize.z * 0.25f);
		posPlant[1] = pos - D3DXVECTOR3(0.0f, 0.0f, rSize.z * 0.25f);
		sizePlant = D3DXVECTOR3(rSize.x, 0.0f, rSize.z * 0.5f);
	}

	// �Ԃ𐶐�
	for (int i = 0; i < 2; ++i)
	{
		CMultiPlant::Create(posPlant[i], sizePlant, GetType(), GetNumActive());
	}
	
	// �ҋ@���S�̐ݒ�
	CalcConectPoint();
}

//===========================================
//  �e���g���̑ҋ@�ʒu���Z�o
//===========================================
D3DXVECTOR3 CGimmickHeavyDoor::CalcWaitPoint(const int Idx, const CPlayerClone* pClone)
{
	// �󂯎�����C���f�b�N�X���ő�l�𒴂��Ă���ꍇ�x��
	if (Idx > GetNumActive()) { assert(false); }

	// ���g�̈ʒu���擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �p�x���擾
	EAngle angle = GetAngle();

	// �C���f�b�N�X�ԍ���0�̏ꍇ2�_�̂����v���C���[�ɋ߂�����ҋ@���S�Ƃ���
	if (Idx == 0 && !IsActive())
	{
		// �v���C���[���W���擾
		D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();

		// �v���C���[��2�_�����ԃx�N�g�����Z�o
		D3DXVECTOR3 vecToPlayer[2] = { posPlayer - m_ConectPoint[0], posPlayer - m_ConectPoint[1] };

		// 2�_�����ԃx�N�g��
		D3DXVECTOR3 vecToWait = VEC3_ZERO;

		// ������2�悪���������̔z��ԍ���ۑ�
		if (vecToPlayer[0].x * vecToPlayer[0].x + vecToPlayer[0].z * vecToPlayer[0].z <=
			vecToPlayer[1].x * vecToPlayer[1].x + vecToPlayer[1].z * vecToPlayer[1].z)
		{
			m_nIdxWait = 0;
			vecToWait = m_ConectPoint[0] - m_ConectPoint[1];
		}
		else
		{
			m_nIdxWait = 1;
			vecToWait = m_ConectPoint[1] - m_ConectPoint[0];
		}

		// �ҋ@���S���m�����ԃx�N�g�����Z�o�����K������
		D3DXVec3Normalize(&m_vecToWait, &vecToWait);
	}

	// �ő吔���擾
	int nNumActive = GetNumActive();

	// �ő吔��0�̏ꍇ�֐��𔲂���
	if (nNumActive == 0) { assert(false); return pos; }

	// ���g�̉������擾
	D3DXVECTOR3 size = GetVec3Sizing();

	// �ҋ@�ʒu�̎Z�o
	D3DXVECTOR3 posWait = m_ConectPoint[m_nIdxWait];
	float fValue = 0.0f;
	switch (angle)
	{
	case EAngle::ANGLE_0: // 0
	case EAngle::ANGLE_180: // 3.14

		// �ҋ@�ʒu�̑��Βl���Z�o
		fValue = (size.x / (float)(nNumActive + 1)) * (Idx + 1);

		// �ҋ@�ʒu��ݒ�
		posWait.x += fValue - (size.x * 0.5f);
		break;

	case EAngle::ANGLE_90: // 1.57
	case EAngle::ANGLE_270: // 4.71

		// �ҋ@�ʒu�̑��Βl���Z�o
		fValue = (size.z / (float)(nNumActive + 1)) * (Idx + 1);

		// �ҋ@�ʒu��ݒ�
		posWait.z += fValue - (size.z * 0.5f);
		break;

	default:
		assert(false);
		break;
	}

	// �ҋ@�ʒu��Ԃ�
	return posWait;
}

//===========================================
//  �e���g���̑ҋ@�������Z�o
//===========================================
D3DXVECTOR3 CGimmickHeavyDoor::CalcWaitRotation(const int Idx, const CPlayerClone* pClone)
{
	// �󂯎�����C���f�b�N�X���ő�l�𒴂��Ă���ꍇ�x��
	if (Idx > GetNumActive()) { assert(false); }

	// ���������߂�
	D3DXVECTOR3 rot = VEC3_ZERO;
	rot.y = atan2f(m_vecToWait.x, m_vecToWait.z);

	// �Z�o����������Ԃ�
	return rot;
}

//===========================================
//  �ҋ@�ʒu�̌v�Z����
//===========================================
void CGimmickHeavyDoor::CalcConectPoint()
{
	// ���g�̈ʒu���擾
	D3DXVECTOR3 pos = GetVec3Position();

	// ���g�̃T�C�Y���擾
	D3DXVECTOR3 size = GetVec3Sizing();

	// ���g�̕������擾
	EAngle angle = GetAngle();

	// �v�Z���s��
	switch (angle)
	{
		// x�������ɉ˂���
	case ANGLE_90:
	case ANGLE_270:

		m_ConectPoint[0] = pos + D3DXVECTOR3(size.x * 0.5f, 0.0f, 0.0f);
		m_ConectPoint[1] = pos - D3DXVECTOR3(size.x * 0.5f, 0.0f, 0.0f);
		break;

		// z�������ɉ˂���
	case ANGLE_0:
	case ANGLE_180:

		m_ConectPoint[0] = pos + D3DXVECTOR3(0.0f, 0.0f, size.z * 0.5f);
		m_ConectPoint[1] = pos - D3DXVECTOR3(0.0f, 0.0f, size.z * 0.5f);
		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
// �����グ��
//============================================================
void CGimmickHeavyDoor::OpenTheDoor(D3DXVECTOR3& rPos)
{
	if (rPos.y >= GetVec3Position().y + CPlayerClone::GetHeight() + CLONE_UP || IsActive() == false)
	{ // ��莞�Ԍo������

		m_move = VEC3_ZERO;		// �ړ���
		m_state = STATE_FULLY;	// ���S�J���
	}
}

//============================================================
// ����������
//============================================================
void CGimmickHeavyDoor::CloseTheDoor(D3DXVECTOR3& rPos)
{
	// �ϐ��錾
	CStage *pStage = GET_STAGE;	// �X�e�[�W���

	// �d��
	m_move.y -= GRAVITY;

	// �͈͊O�̒��n����
	if (pStage->LandFieldPosition(rPos, m_oldPosDoor, m_move))
	{
		m_move = VEC3_ZERO;		// �ړ���
		m_state = STATE_CLOSE;	// �����Ă���

		// �h�A�����鉹��炷
		PLAY_SOUND(CSound::LABEL_SE_DOORCLOSE);
	}
}
