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
	const float MOVE_SPEED = 100.0f; // �ړ����x
}

//==========================================
//  �R���X�g���N�^
//==========================================
CMash::CMash() :
	m_posDefault(VEC3_ZERO),
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
	// �����ʒu��ۑ�����
	m_posDefault = rPos;

	// �ړ����鋗�����擾����
	float offsetMax = GetModelData().vtxMax.z * 2.0f;

	// ��������ړ���I�t�Z�b�g���Z�o����
	float fRot = GetVec3Rotation().y + (D3DX_PI * 0.5f);
	m_offsetMove = D3DXVECTOR3
	(
		offsetMax * cosf(fRot),
		0.0f,
		offsetMax * sinf(fRot)
	);

	// ��������ړ��ʂ��Z�o����
	m_move = D3DXVECTOR3
	(
		MOVE_SPEED * cosf(fRot),
		0.0f,
		MOVE_SPEED * sinf(fRot)
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
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(fRadius, fHeight, fRadius);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-fRadius, 0.0f, -fRadius);

	useful::RotToFourDire(GetVec3Rotation().y);
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

}

//==========================================
//  �ړ�����
//==========================================
void CMash::Move(const float fDeltaTime)
{
	// ���W���擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �ړ��ʂ����Z
	pos += m_move * fDeltaTime;
}
