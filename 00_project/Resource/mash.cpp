//==========================================
//
//  �ӂ��܃N���X(mash.cpp)
//  Author : Tomoya Kanazaki
//  Adder�FRitsuki Obara
//
//==========================================
#include "mash.h"
#include "manager.h"
#include "collision.h"
#include "sound.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float MOVE_SPEED = 120.0f;	// �ړ����x
	const float SENSOR_RANGE = 40.0f;	// �Z���T�[����������͈�
}

//************************************************************
// �ÓI�����o�ϐ��錾
//************************************************************
CListManager<CMash>* CMash::m_pList = nullptr;		// ���X�g�\��

//==========================================
//  �R���X�g���N�^
//==========================================
CMash::CMash(const D3DXVECTOR3& rPos) :
	m_posDefault(rPos),
	m_offsetMove(VEC3_ZERO),
	m_move(VEC3_ZERO),
	m_collMax(VEC3_ZERO),
	m_collMin(VEC3_ZERO),
	m_state(STATE_CLOSE),
	m_stateOld(m_state)
{
	// Do Nothing
}

//==========================================
//  �f�X�g���N�^
//==========================================
CMash::~CMash()
{

}

//==========================================
//  ����������
//==========================================
HRESULT CMash::Init(void)
{
	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CMash>::Create();

		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// �e�N���X�̏���������
	return CActor::Init();
}

//==========================================
//  �I������
//==========================================
void CMash::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �e�N���X�̏I������
	CActor::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CMash::Update(const float fDeltaTime)
{
	if (m_state != m_stateOld)
	{ // ��Ԃ��O��ƈႤ�ꍇ

		switch (m_state)
		{
		case CMash::STATE_CLOSE:

			// �ӂ��܂������Ƃ��̉���炷
			PLAY_SOUND(CSound::LABEL_SE_MASHCLOSE);

			break;

		case CMash::STATE_OPEN:

			// �ӂ��܂̉���炷
			PLAY_SOUND(CSound::LABEL_SE_MASH);

			break;

		default:

			// ��~
			assert(false);

			break;
		}
	}

	// ��Ԃ�ݒ肷��
	m_stateOld = m_state;

	// ��ԏ���
	State(fDeltaTime);

	// �e�N���X�̍X�V����
	CActor::Update(fDeltaTime);
}

//==========================================
//  �`�揈��
//==========================================
void CMash::Draw(CShader* pShader)
{
	// �e�N���X�̕`�揈��
	CActor::Draw(pShader);
}

//==========================================
//  �g�嗦�ݒ�̃I�[�o�[���C�h
//==========================================
void CMash::SetVec3Scaling(const D3DXVECTOR3& rScale)
{
	// �g�嗦�̐ݒ菈��
	CActor::SetVec3Scaling(rScale);

	// �ړ����鋗�����擾����
	float offsetMax = (GetModelData().vtxMax.x * rScale.x) * 2.0f;

	// ��������ړ���I�t�Z�b�g���Z�o����
	float fRot = GetVec3Rotation().y + (D3DX_PI * 0.5f);
	m_offsetMove = D3DXVECTOR3
	(
		m_posDefault.x + offsetMax * sinf(fRot),
		0.0f,
		m_posDefault.z + offsetMax * cosf(fRot)
	);

	// ��������ړ��ʂ��Z�o����
	m_move = D3DXVECTOR3
	(
		(MOVE_SPEED * rScale.x) * sinf(fRot),
		0.0f,
		(MOVE_SPEED * rScale.x) * cosf(fRot)
	);

	// �����蔻��̃T�C�Y�ݒ�
	CollSizeSet(rScale);
}

//==========================================
// ���X�g�\���̎擾����
//==========================================
CListManager<CMash>* CMash::GetList(void)
{
	// ���X�g�\����Ԃ�
	return m_pList;
}

#ifdef _DEBUG

//==========================================
// �����ʒu�̐ݒ菈��
//==========================================
void CMash::SetDefaultPos(const D3DXVECTOR3& rPos)
{
	// �����ʒu��ݒ肷��
	m_posDefault = rPos;
}

#endif // _DEBUG


//==========================================
// �����蔻�菈��
//==========================================
void CMash::Collision
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
	D3DXVECTOR3 posMash = GetVec3Position();
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(fRadius, fHeight, fRadius);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(fRadius, 0.0f, fRadius);

	if (collision::Box3D
	(
		rPos,			// ����ʒu
		m_posDefault,	// ����ڕW�ʒu
		vtxMax,			// ����T�C�Y(�E�E��E��)
		vtxMin,			// ����T�C�Y(���E���E�O)
		m_collMax,		// ����ڕW�T�C�Y(�E�E��E��)
		m_collMin		// ����ڕW�T�C�Y(���E���E�O)
	))
	{ // �ӂ��܂ɋ߂Â����ꍇ

		// �J����Ԃɂ���
		m_state = STATE_OPEN;
	}
	else
	{ // ��L�ȊO

		// ����Ԃɂ���
		m_state = STATE_CLOSE;
	}
}

//==========================================
// �����蔻�菈��(�����Ԃ��I�[�o�[���C�h)
//==========================================
bool CMash::Collision
(
	D3DXVECTOR3& rPos,				// �ʒu
	const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
	const float fRadius,			// ���a
	const float fHeight,			// ����
	D3DXVECTOR3& rMove,				// �ړ���
	bool& bJump,					// �W�����v��
	bool& bHit,						// �Փ˔���
	const bool bDelete				// �������
)
{
	// �ʒu���擾
	D3DXVECTOR3 posMash = GetVec3Position();
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(fRadius, fHeight, fRadius);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(fRadius, 0.0f, fRadius);

	if (collision::Box3D
	(
		rPos,			// ����ʒu
		m_posDefault,	// ����ڕW�ʒu
		vtxMax,			// ����T�C�Y(�E�E��E��)
		vtxMin,			// ����T�C�Y(���E���E�O)
		m_collMax,		// ����ڕW�T�C�Y(�E�E��E��)
		m_collMin		// ����ڕW�T�C�Y(���E���E�O)
	))
	{ // �ӂ��܂ɋ߂Â����ꍇ

		// �J����Ԃɂ���
		m_state = STATE_OPEN;
	}
	else
	{ // ��L�ȊO

		// ����Ԃɂ���
		m_state = STATE_CLOSE;
	}

	// �A�N�^�[�̓����蔻��
	return CActor::Collision
	(
		rPos,		// �ʒu
		rPosOld,	// �O��̈ʒu
		fRadius,	// ���a
		fHeight,	// ����
		rMove,		// �ړ���
		bJump,		// �W�����v��
		bHit,		// �Փ˔���
		bDelete		// �������
	);
}

//==========================================
// ��ԏ���
//==========================================
void CMash::State(const float fDeltaTime)
{
	switch (m_state)
	{
	case CMash::STATE_CLOSE:

		// ������
		Close(fDeltaTime);

		break;

	case CMash::STATE_OPEN:

		// �J������
		Open(fDeltaTime);

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//==========================================
// ������
//==========================================
void CMash::Close(const float fDeltaTime)
{
	// ���W���擾
	D3DXVECTOR3 pos = GetVec3Position();
	D3DXVECTOR3 posOld = GetVec3Position();

	// �ړ��ʂ����Z
	pos -= m_move * fDeltaTime;

	if ((pos.x >= m_posDefault.x && posOld.x <= m_posDefault.x) ||
		(pos.x <= m_posDefault.x && posOld.x >= m_posDefault.x))
	{ // �ڕW�ʒu�𒴂����ꍇ

		// �ʒu��ݒ肷��
		pos.x = m_posDefault.x;
	}

	if ((pos.z >= m_posDefault.z && posOld.z <= m_posDefault.z) ||
		(pos.z <= m_posDefault.z && posOld.z >= m_posDefault.z))
	{ // �ڕW�ʒu�𒴂����ꍇ

		// �ʒu��ݒ肷��
		pos.z = m_posDefault.z;
	}

	// �ʒu�𔽉f
	SetVec3Position(pos);
}

//==========================================
//  �ړ�����
//==========================================
void CMash::Open(const float fDeltaTime)
{
	// ���W���擾
	D3DXVECTOR3 pos = GetVec3Position();
	D3DXVECTOR3 posOld = GetVec3Position();

	// �ړ��ʂ����Z
	pos += m_move * fDeltaTime;

	if ((pos.x >= m_offsetMove.x && posOld.x <= m_offsetMove.x) ||
		(pos.x <= m_offsetMove.x && posOld.x >= m_offsetMove.x))
	{ // �ڕW�ʒu�𒴂����ꍇ

		// �ʒu��ݒ肷��
		pos.x = m_offsetMove.x;
	}

	if ((pos.z >= m_offsetMove.z && posOld.z <= m_offsetMove.z) ||
		(pos.z <= m_offsetMove.z && posOld.z >= m_offsetMove.z))
	{ // �ڕW�ʒu�𒴂����ꍇ

		// �ʒu��ݒ肷��
		pos.z = m_offsetMove.z;
	}

	// �ʒu�𔽉f
	SetVec3Position(pos);
}

//==========================================
// �����蔻��̃T�C�Y�̐ݒ菈��
//==========================================
void CMash::CollSizeSet(const D3DXVECTOR3& rScale)
{
	EAngle angle = useful::RotToFourDire(GetVec3Rotation().y);
	D3DXVECTOR3 ModelMax = GetModelData().vtxMax;
	D3DXVECTOR3 ModelMin = GetModelData().vtxMin;

	if (angle == EAngle::ANGLE_90 ||
		angle == EAngle::ANGLE_270)
	{ // 90�x�A270�x�̏ꍇ

		m_collMax = D3DXVECTOR3
		(
			ModelMax.z * rScale.z + SENSOR_RANGE,
			ModelMax.y * rScale.y,
			ModelMax.x * rScale.x
		);
		m_collMin = D3DXVECTOR3
		(
			-ModelMin.z * rScale.z + SENSOR_RANGE,
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
			ModelMax.z * rScale.z + SENSOR_RANGE
		);
		m_collMin = D3DXVECTOR3
		(
			-ModelMin.x * rScale.x,
			-ModelMin.y * rScale.y,
			-ModelMin.z * rScale.z + SENSOR_RANGE
		);
	}
}
