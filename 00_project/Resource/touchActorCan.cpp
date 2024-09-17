//============================================================
//
//	�^�b�`�A�N�^�[(��)���� [touchActorCan.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "touchActorCan.h"

#include "collision.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float FLY_SPEED = 300.0f;		// ���ł������x
	const float FLY_HEIGHT = 100.0f;	// ���ł�������
	const float GRAVITY = 100.0f;		// �d��
	const float CYCLE_SPEED = 0.2f;		// ��]���x
}

//************************************************************
//	�q�N���X [CTouchCan] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTouchCan::CTouchCan() : CTouchActor(),
m_move(VEC3_ZERO)		// �ړ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTouchCan::~CTouchCan()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTouchCan::Init(void)
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
void CTouchCan::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CTouchActor::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CTouchCan::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g���f���̍X�V
	CTouchActor::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CTouchCan::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g���f���̕`��
	CTouchActor::Draw(pShader);
}

//============================================================
// �����蔻�菈��
//============================================================
bool CTouchCan::Collision
(
	const D3DXVECTOR3& rPos,
	const D3DXVECTOR3& rPosOld,
	const float fRadius,
	const float fHeight
)
{
	// �ʏ��ԈȊO�Afalse��Ԃ�
	if (GetState() != STATE_NONE) { return false; }

	D3DXVECTOR3 posCan = GetVec3Position();		// �ʒu
	float fRadiusCan = GetModelData().fRadius;	// ���a
	float fCanMax = GetModelData().vtxMax.y;	// ����(�ő�l)
	float fCanMin = GetModelData().vtxMin.y;	// ����(�ŏ��l)

	if (posCan.y + fCanMax > rPos.y &&
		posCan.y + fCanMin < rPos.y + fHeight &&
		collision::CirclePillar(posCan, rPos, fRadius, fRadiusCan))
	{ // �ʂ��R������

		D3DXVECTOR3 rot = GetVec3Rotation();

		// ���ł���������ݒ�
		float fRot = atan2f(posCan.x - rPos.x, posCan.z - rPos.z);

		// �A�N�V������Ԃɂ���
		SetState(STATE_ACT);

		// �ړ��ʂ�ݒ肷��
		m_move.x = sinf(fRot) * FLY_SPEED;
		m_move.y = FLY_HEIGHT;
		m_move.z = cosf(fRot) * FLY_SPEED;

		// ������ݒ肷��
		rot.y = fRot;

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
void CTouchCan::UpdateNone(const float /*fDeltaTime*/)
{
	// ���ɖ���
}

//============================================================
// �A�N�V������ԍX�V����
//============================================================
void CTouchCan::UpdateAct(const float fDeltaTime)
{
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu
	D3DXVECTOR3 rot = GetVec3Rotation();	// ����

	// �ړ�����
	pos += m_move * fDeltaTime;

	// �d�͂�������
	m_move.y -= GRAVITY * fDeltaTime;

	// ���������Z����
	rot.z += CYCLE_SPEED;

	// �ʒu�ƌ����𔽉f
	SetVec3Position(pos);
	SetVec3Rotation(rot);
}
