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

#include "manager.h"
#include "collision.h"
#include "stage.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float FLY_SPEED = 900.0f;		// ���ł������x
	const float FLY_HEIGHT = 300.0f;	// ���ł�������
	const float GRAVITY = 900.0f;		// �d��
	const float CYCLE_Y = 8.8f;			// Y���̉�]���x
	const float CYCLE_Z = 11.1f;		// Z���̉�]���x
}

//************************************************************
//	�q�N���X [CTouchCan] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTouchCan::CTouchCan() : CTouchActor(),
m_posOld(VEC3_ZERO),	// �O��̈ʒu
m_move(VEC3_ZERO),		// �ړ���
m_bDelete(false)		// ������
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
	// �ʒu��ݒ肷��
	m_posOld = GetVec3Position();

	// �I�u�W�F�N�g���f���̍X�V
	CTouchActor::Update(fDeltaTime);

	if (m_bDelete)
	{ // �����󋵂� true �̏ꍇ

		// �I������
		Uninit();

		// �֐��𔲂���
		return;
	}
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
// ���̐ݒ菈��
//============================================================
void CTouchCan::SetData
(
	const EType type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR3& rScale	// �g�嗦
)
{
	// ����ݒ肷��
	CTouchActor::SetData(type, rPos, rRot, rScale);

	// �������Ă��镪�����グ��
	D3DXVECTOR3 pos = rPos;
	pos.y -= GetModelData().vtxMin.y;
	SetVec3Position(pos);
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
		collision::CirclePillar(posCan, rPos, fRadiusCan, fRadius))
	{ // �ʂ��R������

		D3DXVECTOR3 rot = GetVec3Rotation();

		// ���ł���������ݒ�
		rot.y = atan2f(posCan.x - rPos.x, posCan.z - rPos.z);

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
void CTouchCan::UpdateNone(const float /*fDeltaTime*/)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = GetVec3Position();
	pos.y -= GRAVITY;

	// �t�B�[���h�Ƃ̓����蔻��
	GET_STAGE->LandFieldPosition(pos, m_posOld, m_move);

	// ���̕��𑫂�
	pos.y -= GetModelData().vtxMin.y;

	// �ʒu�𔽉f
	SetVec3Position(pos);
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
	rot.y += CYCLE_Y * fDeltaTime;
	rot.z += CYCLE_Z * fDeltaTime;

	// �t�B�[���h�Ƃ̓����蔻��
	m_bDelete = GET_STAGE->LandFieldPosition(pos, m_posOld, m_move);

	// �ʒu�ƌ����𔽉f
	SetVec3Position(pos);
	SetVec3Rotation(rot);
}
