//==========================================
//
//  ��]���N���X(spin_wall.cpp)
//  Author�F��������
//
//==========================================
#include "spin_wall.h"
#include "collision.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float MOVE_SPEED = 120.0f; // �ړ����x
}

//==========================================
//  �R���X�g���N�^
//==========================================
CSpinWall::CSpinWall(const D3DXVECTOR3& rRot) :
	m_rotDefault(rRot),
	m_collMax(VEC3_ZERO),
	m_collMin(VEC3_ZERO),
	m_state(STATE_STOP),
	m_fRotMove(0.0f)
{
	// Do Nothing
}

//==========================================
//  �f�X�g���N�^
//==========================================
CSpinWall::~CSpinWall()
{

}

//==========================================
//  ����������
//==========================================
HRESULT CSpinWall::Init(void)
{
	// �l��������
	m_state = STATE_STOP;		// ���

	// �e�N���X�̏���������
	return CActor::Init();
}

//==========================================
//  �I������
//==========================================
void CSpinWall::Uninit(void)
{
	// �e�N���X�̏I������
	CActor::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CSpinWall::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V����
	CActor::Update(fDeltaTime);

	{
		D3DXVECTOR3 rot = GetVec3Rotation();

		rot.y += m_fRotMove;

		SetVec3Rotation(rot);
	}
}

//==========================================
//  �`�揈��
//==========================================
void CSpinWall::Draw(CShader* pShader)
{
	// �e�N���X�̕`�揈��
	CActor::Draw(pShader);
}

//==========================================
//  �g�嗦�ݒ�̃I�[�o�[���C�h
//==========================================
void CSpinWall::SetVec3Scaling(const D3DXVECTOR3& rScale)
{
	// �g�嗦�̐ݒ菈��
	CActor::SetVec3Scaling(rScale);

	// �����蔻��̃T�C�Y�ݒ�
	CollSizeSet(rScale);
}

#ifdef _DEBUG

//==========================================
// ���������̐ݒ菈��
//==========================================
void CSpinWall::SetDefaultRot(const D3DXVECTOR3& rRot)
{
	// ����������ݒ肷��
	m_rotDefault = rRot;
}

#endif // _DEBUG


//==========================================
// �����蔻�菈��
//==========================================
void CSpinWall::Collision
(
	D3DXVECTOR3& rPos,				// �ʒu
	const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
	const float fRadius,			// ���a
	const float fHeight,			// ����
	D3DXVECTOR3& rMove,				// �ړ���
	bool& bJump						// �W�����v��
)
{
	// �ʒu���擾
	D3DXVECTOR3 posWall = GetVec3Position();
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(fRadius, fHeight, fRadius);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(fRadius, 0.0f, fRadius);
	EAngle angle = useful::RotToFourDire(m_rotDefault.y);

	if (angle == EAngle::ANGLE_90 ||
		angle == EAngle::ANGLE_270)
	{ // 90�x�A270�x�̏ꍇ

		if (posWall.z + m_collMax.z >= rPos.z - fRadius &&
			posWall.z <= rPos.z + fRadius)
		{ // �����ɂ����ꍇ

			if (rPos.x + fRadius > posWall.x - m_collMin.x
				&& rPosOld.x + fRadius <= posWall.x - m_collMin.x)
			{ // ������̓����蔻��

				// ��]��Ԃɂ���
				m_state = STATE_SPIN;

				m_fRotMove = 0.02f;
			}
			else if (rPos.x - fRadius < posWall.x + m_collMax.x
				&& rPosOld.x - fRadius >= posWall.x + m_collMax.x)
			{ // �E����̓����蔻��

				// ��]��Ԃɂ���
				m_state = STATE_SPIN;

				m_fRotMove = -0.02f;
			}
		}
		else if(posWall.z - m_collMin.z <= rPos.z + fRadius &&
			posWall.z >= rPos.z - fRadius)
		{ // �E���ɂ����ꍇ

			if (rPos.x + fRadius > posWall.x - m_collMin.x
				&& rPosOld.x + fRadius <= posWall.x - m_collMin.x)
			{ // ������̓����蔻��

				// ��]��Ԃɂ���
				m_state = STATE_SPIN;

				m_fRotMove = -0.02f;
			}
			else if (rPos.x - fRadius < posWall.x + m_collMax.x
				&& rPosOld.x - fRadius >= posWall.x + m_collMax.x)
			{ // �E����̓����蔻��

				// ��]��Ԃɂ���
				m_state = STATE_SPIN;

				m_fRotMove = 0.02f;
			}
		}
	}
	else
	{ // ��L�ȊO

		if (posWall.x + m_collMax.x >= rPos.x - fRadius &&
			posWall.x <= rPos.x + fRadius)
		{ // �����ɂ����ꍇ

			if (rPos.z + fRadius > posWall.z - m_collMin.z
				&& rPosOld.z + fRadius <= posWall.z - m_collMin.z)
			{ // ������̓����蔻��

				// ��]��Ԃɂ���
				m_state = STATE_SPIN;

				m_fRotMove = -0.02f;
			}
			else if (rPos.z - fRadius < posWall.z + m_collMax.z
				&& rPosOld.z - fRadius >= posWall.z + m_collMax.z)
			{ // �E����̓����蔻��

				// ��]��Ԃɂ���
				m_state = STATE_SPIN;

				m_fRotMove = 0.02f;
			}
		}
		else if (posWall.x - m_collMin.x <= rPos.x + fRadius &&
			posWall.x >= rPos.x - fRadius)
		{ // �E���ɂ����ꍇ

			if (rPos.z + fRadius > posWall.z - m_collMin.z
				&& rPosOld.z + fRadius <= posWall.z - m_collMin.z)
			{ // ������̓����蔻��

				// ��]��Ԃɂ���
				m_state = STATE_SPIN;

				m_fRotMove = 0.02f;
			}
			else if (rPos.z - fRadius < posWall.z + m_collMax.z
				&& rPosOld.z - fRadius >= posWall.z + m_collMax.z)
			{ // �E����̓����蔻��

				// ��]��Ԃɂ���
				m_state = STATE_SPIN;

				m_fRotMove = -0.02f;
			}
		}
	}
}

//==========================================
// �����蔻�菈��(�����Ԃ��I�[�o�[���C�h)
//==========================================
void CSpinWall::Collision
(
	D3DXVECTOR3& rPos,				// �ʒu
	const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
	const float fRadius,			// ���a
	const float fHeight,			// ����
	D3DXVECTOR3& rMove,				// �ړ���
	bool& bJump,					// �W�����v��
	bool& bHit						// �Փ˔���
)
{
	// �ʒu���擾
	D3DXVECTOR3 posMash = GetVec3Position();
	D3DXVECTOR3 rotMash = GetVec3Rotation();
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(fRadius, fHeight, fRadius);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(fRadius, 0.0f, fRadius);
	D3DXVECTOR3 vtxMashMax = GetModelData().vtxMax;
	D3DXVECTOR3 vtxMashMin = -GetModelData().vtxMin;
	EAngle angle = useful::RotToFourDire(GetVec3Rotation().y);

	if (collision::Box3D
	(
		rPos,				// ����ʒu
		GetVec3Position(),	// ����ڕW�ʒu
		vtxMax,				// ����T�C�Y(�E�E��E��)
		vtxMin,				// ����T�C�Y(���E���E�O)
		vtxMashMax,			// ����ڕW�T�C�Y(�E�E��E��)
		vtxMashMin,			// ����ڕW�T�C�Y(���E���E�O)
		ANGLE_0,			// ���������
		angle				// ����ڕW������
	))
	{ // ��]���ɋ߂Â����ꍇ

		// ��]��Ԃɂ���
		m_state = STATE_SPIN;
	}
}

//==========================================
// ��ԏ���
//==========================================
void CSpinWall::State(void)
{
	switch (m_state)
	{
	case CSpinWall::STATE_STOP:



		break;

	case CSpinWall::STATE_SPIN:



		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//==========================================
// �����蔻��̃T�C�Y�̐ݒ菈��
//==========================================
void CSpinWall::CollSizeSet(const D3DXVECTOR3& rScale)
{
	EAngle angle = useful::RotToFourDire(GetVec3Rotation().y);
	D3DXVECTOR3 ModelMax = GetModelData().vtxMax;
	D3DXVECTOR3 ModelMin = GetModelData().vtxMin;

	if (angle == EAngle::ANGLE_90 ||
		angle == EAngle::ANGLE_270)
	{ // 90�x�A270�x�̏ꍇ

		m_collMax = D3DXVECTOR3
		(
			ModelMax.z * rScale.z,
			ModelMax.y * rScale.y,
			ModelMax.x * rScale.x
		);
		m_collMin = D3DXVECTOR3
		(
			-ModelMin.z * rScale.z,
			-ModelMin.y * rScale.y,
			-ModelMin.x * rScale.x
		);
	}
	else
	{ // ��L�ȊO

		m_collMax = D3DXVECTOR3
		(
			ModelMax.x * rScale.x,
			ModelMax.y * rScale.y,
			ModelMax.z * rScale.z
		);
		m_collMin = D3DXVECTOR3
		(
			-ModelMin.x * rScale.x,
			-ModelMin.y * rScale.y,
			-ModelMin.z * rScale.z
		);
	}
}