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
#include "multi_plant.h"
#include "sound.h"

#include "camera.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const float DISTANCE = 30.0f; // �ҋ@�ʒu�Ƃ̋���
	const float ACTIVE_UP = 10.0f; // ����������ꂽ�ۂ�Y�ʒu�㏸��
	const float FIELD_SIZE = 55.0f; // ���̕�
	const float PLANT_RANGE = 50.0f; // �Ԃ̍炭�͈�
	const CCamera::SSwing SWING = CCamera::SSwing(10.0f, 2.0f, 0.6f);		// �J�����h��̒l
	const float ROTATE_SPEED = 0.05f; // ����������܂łɂ����鎞��
}

//===========================================
//  �R���X�g���N�^
//===========================================
CGimmickBridge::CGimmickBridge() : CGimmickAction(),
m_ConectPoint(),
m_vecToWait(VEC3_ZERO),
m_nIdxWait(0),
m_pField(nullptr),
m_fRot(0.0f),
m_fMoveTime(0.0f),
m_bMove(false)
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
	// ����������
	bool bFar = useful::IsNearPosR(GetVec3Position());
	if (!bFar) { return; }

	// �����˂���
	if (IsActive()) { Active(fDeltaTime); }
	else { SAFE_UNINIT(m_pField); m_fRot = 0.0f; }

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

	// �ҋ@�ʒu���Z�o����
	D3DXVECTOR3 pos = m_ConectPoint[m_nIdxWait] + D3DXVECTOR3(0.0f, ACTIVE_UP * (float)IsActive(), 0.0f);

	// �ҋ@�ʒu�ɃG�t�F�N�g���ړ�����
	CEffekseer::CEffectData* pEffect = GetEffect();
	if (pEffect != nullptr) { pEffect->m_pos = pos; }

	// ���g�̃C���f�b�N�X�ɍ��킹�č��W�����炷
	pos += m_vecToWait * DISTANCE * (float)Idx;

	return pos;	// �M�~�b�N�������Ȃ班����ɂ��炷
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

		// ���g�̌������擾
		D3DXVECTOR3 rot = pClone->GetVec3Rotation();

		// �ړ����������Ă���ꍇ�␳�����ɒl��Ԃ�
		if (m_bMove)
		{
			return rot;
		}

		// ������␳����
		rot.x += -HALF_PI * m_fMoveTime;
		if (rot.x <= -HALF_PI)
		{
			rot.x = -HALF_PI;
			m_bMove = true;

			// �J�������h���
			CManager::GetInstance()->GetCamera()->SetSwing(CCamera::TYPE_MAIN, SWING);

			// �R���g���[���̃o�C�u���[�V����
			GET_INPUTPAD->SetVibration(CInputPad::TYPE_BRIDGE);

			// ����������炷
			PLAY_SOUND(CSound::LABEL_SE_GIMMICKBRIDGE);
		}

		// ������ۑ�����
		m_fRot = rot.x;

		// ������Ԃ�
		return D3DXVECTOR3(rot.x, fTemp + (D3DX_PI * (float)m_nIdxWait), 0.0f);
	}

	// ���������߂�
	D3DXVECTOR3 rot = VEC3_ZERO;
	rot.y = atan2f(m_vecToWait.x, m_vecToWait.z);

	// �Z�o����������Ԃ�
	return rot;
}

//===========================================
//   �T�C�Y�̐ݒ�
//===========================================
void CGimmickBridge::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �T�C�Y�̐ݒ�
	CObject3D::SetVec3Sizing(rSize);
	
	// �Ԃ̐����ʒu��ݒ肷��
	D3DXVECTOR3 posPlant[2] = { GetVec3Position(), GetVec3Position() };

	// �����̎擾
	EAngle angle = GetAngle();
	float fAngle = ANGLE_PI(angle) + D3DX_PI * 0.5f;

	// �����ɍ��킹�Đ����ʒu�����炷
	posPlant[0] += D3DXVECTOR3((rSize.x + PLANT_RANGE) * 0.5f * cosf(fAngle), 0.0f, (rSize.z + PLANT_RANGE) * 0.5f * sinf(fAngle));
	posPlant[1] += D3DXVECTOR3((rSize.x + PLANT_RANGE) * 0.5f * cosf(fAngle + D3DX_PI), 0.0f, (rSize.z + PLANT_RANGE) * 0.5f * sinf(fAngle + D3DX_PI));

	// �T�C�Y�̐ݒ�
	D3DXVECTOR3 size = GetVec3Sizing();
	switch (angle)
	{
	case ANGLE_0:
	case ANGLE_180:
		size.z = PLANT_RANGE;
		break;

	case ANGLE_90:
	case ANGLE_270:
		size.x = PLANT_RANGE;
		break;

	default:
		assert(false);
		break;
	}

	// �Ԃ𐶐�
	for (int i = 0; i < 2; ++i)
	{
		CMultiPlant::Create(posPlant[i], size, GetType(), GetNumActive());
	}

	// ���̒[��ݒ�
	CalcConectPoint();
}

//===========================================
//  ���̒[�̌v�Z����
//===========================================
void CGimmickBridge::CalcConectPoint()
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

//===========================================
//  �A�N�e�B�u��Ԃ̏���
//===========================================
void CGimmickBridge::Active(const float fDeltaTime)
{
	// �ړ����Ԃ̉��Z
	m_fMoveTime += fDeltaTime * ROTATE_SPEED;

	// �A�N�e�B�u�̏u�Ԃ̏���
	Moment();

	// �A�N�e�B�u���̏���
	Movement();
}

//==========================================
//  �A�N�e�B�u��ԂɂȂ����u�Ԃ̏���
//==========================================
void CGimmickBridge::Moment()
{
	// ���ꂪ���łɑ��݂��Ă���ꍇ�֐��𔲂���
	if (m_pField != nullptr) { return; }

	// �ړ������t���O��false�ɐݒ�
	m_bMove = false;

	// �ړ����Ԃ����Z�b�g
	m_fMoveTime = 0.0f;

	// ���g�̌������擾
	EAngle angle = GetAngle();

	// ����̍��W��ݒ�
	D3DXVECTOR3 posField = m_ConectPoint[m_nIdxWait];

	// ����̃T�C�Y
	D3DXVECTOR2 sizeField = VEC2_ZERO;

	// ����̌���
	D3DXVECTOR3 rotField = VEC3_ZERO;
	float fRot = m_fRot + HALF_PI - (m_nIdxWait + 1.0f) * -1.0f * HALF_PI;

	// ����̌��������߂�
	switch (angle)
	{
	// x�������ɉ˂���
	case ANGLE_90:
	case ANGLE_270:

		// �傫����ݒ�
		sizeField.x = fabsf(m_ConectPoint[0].x - m_ConectPoint[1].x);
		sizeField.y = FIELD_SIZE;

		// �ʒu��ݒ�
		posField.x += sinf(fRot) * sizeField.x * 0.5f;
		posField.y += sinf(m_fRot + HALF_PI) * sizeField.x * 0.5f;

		// ������ݒ�
		rotField.z = -fRot + m_nIdxWait * -1.0f * D3DX_PI;

		// ����
		m_pField = CField::Create(CField::TYPE_XBRIDGE, posField, rotField, sizeField, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), POSGRID2(1, 1), POSGRID2(1, 1));

		break;

		// z�������ɉ˂���
	case ANGLE_0:
	case ANGLE_180:

		// �傫����ݒ�
		sizeField.x = FIELD_SIZE;
		sizeField.y = fabsf(m_ConectPoint[0].z - m_ConectPoint[1].z);

		// �ʒu��ݒ�
		posField.z += sinf(fRot) * sizeField.y * 0.5f;
		posField.y += sinf(m_fRot + HALF_PI) * sizeField.y * 0.5f;

		// ������ݒ�
		rotField.x = fRot - m_nIdxWait * -1.0f * D3DX_PI;

		// ����
		m_pField = CField::Create(CField::TYPE_ZBRIDGE, posField, rotField, sizeField, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), POSGRID2(1, 1), POSGRID2(1, 1));

		break;

	default:
		assert(false);
		break;
	}

	m_pField->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
}

//==========================================
//  �A�N�e�B�u��Ԓ��̏���
//==========================================
void CGimmickBridge::Movement()
{
	// ���ꂪ���݂��Ă��Ȃ��ꍇ�֐��𔲂���
	if (m_pField == nullptr) { return; }

	// �ړ����������Ă����ꍇ�֐��𔲂���
	if (m_bMove)
	{
		m_pField->SetVec3Rotation(VEC3_ZERO);
		return;
	}

	// ���g�̌������擾
	EAngle angle = GetAngle();

	// ����̍��W��ݒ�
	D3DXVECTOR3 posField = m_ConectPoint[m_nIdxWait];

	// ����̑傫�����擾
	float fSizeField = 0.5f;
	switch (angle)
	{
	case ANGLE_90:
	case ANGLE_270:
		fSizeField *= m_pField->GetVec2Sizing().x;
		break;

	case ANGLE_0:
	case ANGLE_180:
		fSizeField *= m_pField->GetVec2Sizing().y;
		break;

	default:
		assert(false);
		break;
	}

	// ����̌���
	D3DXVECTOR3 rotField = VEC3_ZERO;

	// ����̌�������y���W���Z�o
	posField.y += sinf(m_fRot + HALF_PI) * fSizeField;

	// x,z���W���Z�o
	switch (angle)
	{
	case ANGLE_0:
	case ANGLE_180:
		posField.z += sinf(m_fRot) * fSizeField * (1.0f - m_nIdxWait * 2.0f);
		rotField.x = HALF_PI + m_fRot * (1.0f - m_nIdxWait * 2.0f);
		if (m_nIdxWait) { rotField.z -= D3DX_PI; }
		break;

	case ANGLE_90:
	case ANGLE_270:
		posField.x += sinf(m_fRot) * fSizeField * (1.0f - m_nIdxWait * 2.0f);
		rotField.z = HALF_PI - m_fRot * (1.0f - m_nIdxWait * 2.0f);
		if (!m_nIdxWait) { rotField.z -= D3DX_PI; }
		break;

	default:
		assert(false);
		break;
	}
	
	// �����ƍ��W��K�p
	m_pField->SetVec3Position(posField);
	m_pField->SetVec3Rotation(rotField);
}
