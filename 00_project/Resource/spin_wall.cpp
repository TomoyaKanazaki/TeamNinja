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
	const float ROT_MOVE = 0.04f;	// �����̈ړ���
}

//==========================================
//  �R���X�g���N�^
//==========================================
CSpinWall::CSpinWall(const D3DXVECTOR3& rRot) :
	m_rotDefault(rRot),
	m_collMax(VEC3_ZERO),
	m_collMin(VEC3_ZERO),
	m_state(STATE_STOP),
	m_fRotMove(0.0f),
	m_fTotalMove(0.0f),
	m_bFront(true)
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
	// ��ԏ���
	State();

	// �e�N���X�̍X�V����
	CActor::Update(fDeltaTime);
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
	// ��]���̏ꍇ�A������
	if (m_state == STATE_SPIN) { return; }

	// ��]�̓����蔻��
	SpinCollision
	(
		rPos,		// �ʒu
		rPosOld,	// �O��̈ʒu
		fRadius,	// ���a
		fHeight		// ����
	);

	// �����蔻�菈��
	CActor::Collision
	(
		rPos,		// �ʒu
		rPosOld,	// �O��̈ʒu
		fRadius,	// ���a
		fHeight,	// ����
		rMove,		// �ړ���
		bJump		// �W�����v��
	);
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
	// ��]���̏ꍇ�A������
	if (m_state == STATE_SPIN) { return; }

	// ��]�̓����蔻��
	SpinCollision
	(
		rPos,		// �ʒu
		rPosOld,	// �O��̈ʒu
		fRadius,	// ���a
		fHeight		// ����
	);

	// �����蔻�菈��
	CActor::Collision
	(
		rPos,		// �ʒu
		rPosOld,	// �O��̈ʒu
		fRadius,	// ���a
		fHeight,	// ����
		rMove,		// �ړ���
		bJump,		// �W�����v��
		bHit		// �Փ˔���
	);
}

//==========================================
// ��ԏ���
//==========================================
void CSpinWall::State(void)
{
	switch (m_state)
	{
	case CSpinWall::STATE_STOP:

		// ���ɖ���

		break;

	case CSpinWall::STATE_SPIN:

		// ��]����
		Spin();

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//==========================================
// ��]����
//==========================================
void CSpinWall::Spin(void)
{
	// �������擾����
	D3DXVECTOR3 rot = GetVec3Rotation();
	D3DXVECTOR3 rotOld = GetVec3Rotation();

	// ��]������
	rot.y += m_fRotMove;
	m_fTotalMove += fabsf(m_fRotMove);
	useful::NormalizeRot(rot.y);

	if (m_fTotalMove >= D3DX_PI)
	{ // ����]�����ꍇ

		// ��~��Ԃɂ���
		m_state = STATE_STOP;

		// �����̍��v�l�����Z�b�g����
		m_fTotalMove = 0.0f;

		// �\�����󋵂��t�]����
		m_bFront = !m_bFront;

		// ������␳����
		rot.y = m_bFront == true ? m_rotDefault.y : m_rotDefault.y + D3DX_PI;
	}

	// �����𐳋K������
	SetVec3Rotation(rot);
}

//==========================================
// ��]�ݒ菈��
//==========================================
void CSpinWall::SetSpin(const float fRotMove)
{
	// ��]��Ԃɂ���
	m_state = STATE_SPIN;

	// �����̈ړ��ʂ�ݒ肷��
	m_fRotMove = fRotMove;
}

//==========================================
// ��]�̓����蔻�菈��
//==========================================
void CSpinWall::SpinCollision
(
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
	const float fRadius,			// ���a
	const float fHeight				// ����
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

				// ��]�ݒ菈��
				SetSpin(ROT_MOVE);
			}
			else if (rPos.x - fRadius < posWall.x + m_collMax.x
				&& rPosOld.x - fRadius >= posWall.x + m_collMax.x)
			{ // �E����̓����蔻��

				// ��]�ݒ菈��
				SetSpin(-ROT_MOVE);
			}
		}
		else if (posWall.z - m_collMin.z <= rPos.z + fRadius &&
			posWall.z >= rPos.z - fRadius)
		{ // �E���ɂ����ꍇ

			if (rPos.x + fRadius > posWall.x - m_collMin.x
				&& rPosOld.x + fRadius <= posWall.x - m_collMin.x)
			{ // ������̓����蔻��

				// ��]�ݒ菈��
				SetSpin(-ROT_MOVE);
			}
			else if (rPos.x - fRadius < posWall.x + m_collMax.x
				&& rPosOld.x - fRadius >= posWall.x + m_collMax.x)
			{ // �E����̓����蔻��

				// ��]�ݒ菈��
				SetSpin(ROT_MOVE);
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

				// ��]�ݒ菈��
				SetSpin(-ROT_MOVE);
			}
			else if (rPos.z - fRadius < posWall.z + m_collMax.z
				&& rPosOld.z - fRadius >= posWall.z + m_collMax.z)
			{ // �E����̓����蔻��

				// ��]�ݒ菈��
				SetSpin(ROT_MOVE);
			}
		}
		else if (posWall.x - m_collMin.x <= rPos.x + fRadius &&
			posWall.x >= rPos.x - fRadius)
		{ // �E���ɂ����ꍇ

			if (rPos.z + fRadius > posWall.z - m_collMin.z
				&& rPosOld.z + fRadius <= posWall.z - m_collMin.z)
			{ // ������̓����蔻��

				// ��]�ݒ菈��
				SetSpin(ROT_MOVE);
			}
			else if (rPos.z - fRadius < posWall.z + m_collMax.z
				&& rPosOld.z - fRadius >= posWall.z + m_collMax.z)
			{ // �E����̓����蔻��

				// ��]�ݒ菈��
				SetSpin(-ROT_MOVE);
			}
		}
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