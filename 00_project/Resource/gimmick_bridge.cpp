//===========================================
//
//  ���̃M�~�b�N(gimmick_bridge.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "gimmick_bridge.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"
#include "field.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const float DISTANCE = 30.0f;	// �ҋ@�ʒu�Ƃ̋���
	const float ACTIVE_UP = 10.0f;	// ����������ꂽ�ۂ�Y�ʒu�㏸��
}

//===========================================
//  �R���X�g���N�^
//===========================================
CGimmickBridge::CGimmickBridge() : CGimmickAction(),
m_bSet(false),
m_ConectPoint(),
m_vecToWait(VEC3_ZERO),
m_nIdxWait(0),
m_pField(nullptr)
{
}

//===========================================
//  �f�X�g���N�^
//===========================================
CGimmickBridge::~CGimmickBridge()
{
}

//===========================================
//  ����������
//===========================================
HRESULT CGimmickBridge::Init(void)
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
void CGimmickBridge::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmickAction::Uninit();
}

//===========================================
//  �X�V����
//===========================================
void CGimmickBridge::Update(const float fDeltaTime)
{
	// ���̒[�̐ݒ�
	if (!m_bSet) { CalcConectPoint(); }

	// �����˂���
	if (IsActive())
	{
		Active();
		SetEnableDraw(false);
	}
	else
	{
		SAFE_UNINIT(m_pField);
		SetEnableDraw(true);
	}

	// �e�N���X�̍X�V
	CGimmickAction::Update(fDeltaTime);
}

//===========================================
//  �`�揈��
//===========================================
void CGimmickBridge::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmickAction::Draw(pShader);
}

//===========================================
//  �e���g���̑ҋ@�ʒu���Z�o
//===========================================
D3DXVECTOR3 CGimmickBridge::CalcWaitPoint(const int Idx)
{
	// �󂯎�����C���f�b�N�X���ő�l�𒴂��Ă���ꍇ�x��
	if (Idx > GetNumActive()) { assert(false); }

	// �C���f�b�N�X�ԍ���0�̏ꍇ2�_�̂����v���C���[�ɋ߂�����ҋ@���S�Ƃ���
	if (Idx == 0)
	{
		// �v���C���[���W���擾
		D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();

		// �v���C���[��2�_�����ԃx�N�g�����Z�o
		D3DXVECTOR3 vecToPlayer[2] = { posPlayer - m_ConectPoint[0], posPlayer - m_ConectPoint[1] };

		// ������2�悪���������̔z��ԍ���ۑ�
		if (vecToPlayer[0].x * vecToPlayer[0].x + vecToPlayer[0].z * vecToPlayer[0].z <=
			vecToPlayer[1].x * vecToPlayer[1].x + vecToPlayer[1].z * vecToPlayer[1].z)
		{
			m_nIdxWait = 0;
		}
		else
		{
			m_nIdxWait = 1;
		}

		// ���S����ҋ@���S�ւ̃x�N�g�����Z�o�����K������
		D3DXVECTOR3 vecToWait = m_ConectPoint[m_nIdxWait] - GetVec3Position();
		D3DXVec3Normalize(&m_vecToWait, &vecToWait);
	}

	// �ҋ@�ʒu��Ԃ�
	return m_ConectPoint[m_nIdxWait] + (m_vecToWait * DISTANCE * (float)Idx) + D3DXVECTOR3(0.0f, ACTIVE_UP * (float)IsActive(), 0.0f);	// �M�~�b�N�������Ȃ班����ɂ��炷
}

//===========================================
//  �e���g���̑ҋ@�������Z�o
//===========================================
D3DXVECTOR3 CGimmickBridge::CalcWaitRotation(const int Idx, const CPlayerClone* pClone)
{
	// �󂯎�����C���f�b�N�X���ő�l�𒴂��Ă���ꍇ�x��
	if (Idx > GetNumActive()) { assert(false); }

	if (IsActive())
	{ // �M�~�b�N�������̏ꍇ
		// �������擾
		EAngle angle = GetAngle();
		float fTemp = 0.0f;

		// y����ݒ�
		switch (angle)
		{
		case ANGLE_90:
		case ANGLE_270:

			fTemp = D3DX_PI * 0.5f;
			break;

		case ANGLE_0:
		case ANGLE_180:

			fTemp = 0.0f;
			break;

		default:
			assert(false);
			break;
		}

		// ������Q���ׂ�`�ɂ���
		return D3DXVECTOR3(-HALF_PI, fTemp + (D3DX_PI * (float)m_nIdxWait), 0.0f);
	}

	// �ҋ@���S�Ƃ̍��������߂�
	D3DXVECTOR3 vecCenter = GetActionPoint() - pClone->GetVec3Position();

	// ���������߂�
	D3DXVECTOR3 rot = VEC3_ZERO;
	rot.y = -atan2f(vecCenter.x, -vecCenter.z);

	// �Z�o����������Ԃ�
	return rot;
}

//===========================================
//  ���̒[�̌v�Z����
//===========================================
void CGimmickBridge::CalcConectPoint()
{
	// �ݒ�ς݃t���O�𗧂Ă�
	m_bSet = true;

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

//===========================================
//  �A�N�e�B�u��Ԃ̏���
//===========================================
void CGimmickBridge::Active()
{
	// ����𐶐�����
	if (m_pField == nullptr)
	{
		// size��2�����ɕϊ�
		D3DXVECTOR2 size = D3DXVECTOR2(GetVec3Sizing().x, GetVec3Sizing().z);

		// TODO�F�ʒu��ɂ�����
		m_pField = CField::Create(CField::TYPE_BRIDGE, GetVec3Position() + D3DXVECTOR3(0.0f, 25.0f, 0.0f), GetVec3Rotation(), size, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), POSGRID2(10, 10), POSGRID2(10, 10));
		m_pField->SetEnableDraw(false);
	}

	// TODO : ���g�̔z�u��ύX�H
}
