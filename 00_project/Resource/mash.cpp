//==========================================
//
//  �ӂ��܃N���X(mash.cpp)
//  Author : Tomoya Kanazaki
//  Adder�FRitsuki Obara
//
//==========================================
#include "mash.h"
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
CMash::CMash(const D3DXVECTOR3& rPos) :
	m_posDefault(rPos),
	m_offsetMove(VEC3_ZERO),
	m_move(VEC3_ZERO),
	m_state(STATE_CLOSE)
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
	// �e�N���X�̏���������
	return CActor::Init();
}

//==========================================
//  �I������
//==========================================
void CMash::Uninit(void)
{
	// �e�N���X�̏I������
	CActor::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CMash::Update(const float fDeltaTime)
{
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
//  �ʒu�ݒ�̃I�[�o�[���C�h
//==========================================
void CMash::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �ړ����鋗�����擾����
	float offsetMax = GetModelData().vtxMax.x * 2.0f;

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
		MOVE_SPEED * sinf(fRot),
		0.0f,
		MOVE_SPEED * cosf(fRot)
	);

	// �e�N���X�̈ʒu�ݒ�
	CActor::SetVec3Position(rPos);
}

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
	D3DXVECTOR3 rotMash = GetVec3Rotation();
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(fRadius, fHeight, fRadius);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(fRadius, 0.0f, fRadius);
	D3DXVECTOR3 vtxMashMax = D3DXVECTOR3
	(
		GetModelData().vtxMax.x + sinf(rotMash.y) * GetModelData().vtxMax.x,
		fHeight,
		GetModelData().vtxMax.z + cosf(rotMash.y) * GetModelData().vtxMax.x
	);
	D3DXVECTOR3 vtxMashMin = D3DXVECTOR3
	(
		-GetModelData().vtxMin.x + sinf(rotMash.y) * -GetModelData().vtxMin.x,
		0.0f,
		-GetModelData().vtxMin.z + cosf(rotMash.y) * -GetModelData().vtxMin.x
	);

	// �����̎l�����ϊ�
	useful::RotToFourDire(GetVec3Rotation().y);

	if (collision::Box3D
	(
		rPos,			// ����ʒu
		m_posDefault,	// ����ڕW�ʒu
		vtxMax,			// ����T�C�Y(�E�E��E��)
		vtxMin,			// ����T�C�Y(���E���E�O)
		vtxMashMax,		// ����ڕW�T�C�Y(�E�E��E��)
		vtxMashMin		// ����ڕW�T�C�Y(���E���E�O)
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
void CMash::Collision
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
	D3DXVECTOR3 vtxMashMax = D3DXVECTOR3
	(
		GetModelData().vtxMax.x + sinf(rotMash.y) * GetModelData().vtxMax.x,
		fHeight,
		GetModelData().vtxMax.z + cosf(rotMash.y) * GetModelData().vtxMax.x
	);
	D3DXVECTOR3 vtxMashMin = D3DXVECTOR3
	(
		-GetModelData().vtxMin.x + sinf(rotMash.y) * -GetModelData().vtxMin.x,
		0.0f,
		-GetModelData().vtxMin.z + cosf(rotMash.y) * -GetModelData().vtxMin.x
	);

	// �����̎l�����ϊ�
	useful::RotToFourDire(GetVec3Rotation().y);

	if (collision::Box3D
	(
		rPos,			// ����ʒu
		m_posDefault,	// ����ڕW�ʒu
		vtxMax,			// ����T�C�Y(�E�E��E��)
		vtxMin,			// ����T�C�Y(���E���E�O)
		vtxMashMax,		// ����ڕW�T�C�Y(�E�E��E��)
		vtxMashMin		// ����ڕW�T�C�Y(���E���E�O)
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
