//============================================================
//
//	�^�b�`�A�N�^�[(��)���� [touchActorBird.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "touchActorBird.h"

#include "collision.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int NONE_MOVE_COUNT = 50;				// �ʏ��Ԃ̈ړ��J�E���g��
	const float COLLISION_RADIUS = 150.0f;		// �����蔻��̔��a
	const float FLY_SPEED = 600.0f;				// ���ł������x
	const float FLY_HEIGHT = 180.0f;			// ���ł�������
	const float FLY_GRAVITY = 50.0f;			// ��s���̏d��
}

//************************************************************
//	�q�N���X [CTouchBird] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTouchBird::CTouchBird() : CTouchActor(),
m_move(VEC3_ZERO),		// �ړ���
m_nStateCount(0)		// ��ԃJ�E���g
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTouchBird::~CTouchBird()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTouchBird::Init(void)
{
	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CTouchActor::Init()))
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
void CTouchBird::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CTouchActor::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CTouchBird::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g���f���̍X�V
	CTouchActor::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CTouchBird::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g���f���̕`��
	CTouchActor::Draw(pShader);
}

//============================================================
// �����蔻�菈��
//============================================================
bool CTouchBird::Collision
(
	const D3DXVECTOR3& rPos,
	const D3DXVECTOR3& rPosOld,
	const float fRadius,
	const float fHeight
)
{
	// �ʏ��ԈȊO�Afalse��Ԃ�
	if (GetState() != STATE_NONE) { return false; }

	D3DXVECTOR3 posBird = GetVec3Position();		// �ʒu
	float fBirdMax = GetModelData().vtxMax.y;	// ����(�ő�l)
	float fBirdMin = GetModelData().vtxMin.y;	// ����(�ŏ��l)

	if (posBird.y + fBirdMax > rPos.y &&
		posBird.y + fBirdMin < rPos.y + fHeight &&
		collision::Circle2D(posBird, rPos, COLLISION_RADIUS, fRadius))
	{ // ���͈̔͂ɓ������ꍇ

		D3DXVECTOR3 rot = GetVec3Rotation();

		// ���ł���������ݒ�
		rot.y = atan2f(posBird.x - rPos.x, posBird.z - rPos.z);

		// �A�N�V������Ԃɂ���
		SetState(STATE_ACT);

		// �ړ��ʂ�ݒ肷��
		m_move.x = sinf(rot.y) * FLY_SPEED;
		m_move.y = FLY_HEIGHT;
		m_move.z = cosf(rot.y) * FLY_SPEED;

		// �����𔽉f
		SetVec3Rotation(rot);

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
	return false;
}

//============================================================
// �ʏ��ԍX�V����
//============================================================
void CTouchBird::UpdateNone(const float /*fDeltaTime*/)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	if (m_nStateCount % NONE_MOVE_COUNT == 0)
	{ // ���J�E���g���Ƃ�

		// �҂��҂�񒵂˂鏈����ǉ�
	}
}

//============================================================
// �A�N�V������ԍX�V����
//============================================================
void CTouchBird::UpdateAct(const float fDeltaTime)
{
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu

	// �ړ�����
	pos += m_move * fDeltaTime;

	// �d�͂�������
	m_move.y -= FLY_GRAVITY * fDeltaTime;

	// �ʒu�ƌ����𔽉f
	SetVec3Position(pos);
}
