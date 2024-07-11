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
	const float DISTANCE = 30.0f; // �ҋ@�ʒu�Ƃ̋���
	const float ACTIVE_UP = 10.0f; // ����������ꂽ�ۂ�Y�ʒu�㏸��
	const float FIELD_SIZE = 60.0f; // ���̕�
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
D3DXVECTOR3 CGimmickBridge::CalcWaitPoint(const int Idx, const CPlayerClone* pClone)
{
	// �󂯎�����C���f�b�N�X���ő�l�𒴂��Ă���ꍇ�x��
	if (Idx > GetNumActive()) { assert(false); }
	
	// �����̎擾
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

		// xz�̕Ў��𕪐g�̈ʒu�ɂ���
		switch (angle)
		{
		case ANGLE_90:
		case ANGLE_270:
			m_ConectPoint[0].z = pClone->GetVec3Position().z;
			m_ConectPoint[1].z = pClone->GetVec3Position().z;
			break;

		case ANGLE_0:
		case ANGLE_180:
			m_ConectPoint[0].x = pClone->GetVec3Position().x;
			m_ConectPoint[1].x = pClone->GetVec3Position().x;
			break;

		default:
			assert(false);
			break;
		}

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

	// ���������߂�
	D3DXVECTOR3 rot = VEC3_ZERO;
	rot.y = atan2f(m_vecToWait.x, m_vecToWait.z);

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
	if (m_pField != nullptr) { return; }

	// ����̍��W��ݒ�
	D3DXVECTOR3 posField = (m_ConectPoint[0] + m_ConectPoint[1]) * 0.5f;

	// ����̃T�C�Y
	D3DXVECTOR2 sizeField = VEC2_ZERO;

	// ���g�̕������擾
	EAngle angle = GetAngle();

	// ���̕��������߂�
	switch (angle)
	{
	// x�������ɉ˂���
	case ANGLE_90:
	case ANGLE_270:

		sizeField.x = fabsf(m_ConectPoint[0].x - m_ConectPoint[1].x);
		sizeField.y = FIELD_SIZE;

		break;

	// z�������ɉ˂���
	case ANGLE_0:
	case ANGLE_180:

		sizeField.x = FIELD_SIZE;
		sizeField.y = fabsf(m_ConectPoint[0].z - m_ConectPoint[1].z);

		break;

	default:
		assert(false);
		break;
	}

	// ����
	m_pField = CField::Create(CField::TYPE_BRIDGE, posField, VEC3_ZERO, sizeField, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), POSGRID2(1, 1), POSGRID2(1, 1));
}
