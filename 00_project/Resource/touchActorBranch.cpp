//============================================================
//
//	�^�b�`�A�N�^�[(�})���� [touchActorBranch.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "touchActorBranch.h"
#include "manager.h"
#include "sound.h"

#include "collision.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float FLY_SPEED = 900.0f;		// ���ł������x
	const float FLY_HEIGHT = 300.0f;	// ���ł�������
	const float GRAVITY = 900.0f;		// �d��
	const float CYCLE_Y = 9.0f;			// Y���̉�]���x
	const float CYCLE_Z = 12.1f;		// Z���̉�]���x
}

//************************************************************
//	�q�N���X [CTouchBranch] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTouchBranch::CTouchBranch() : CTouchActor(),
m_bSound(false)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTouchBranch::~CTouchBranch()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTouchBranch::Init(void)
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
void CTouchBranch::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CTouchActor::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CTouchBranch::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g���f���̍X�V
	CTouchActor::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CTouchBranch::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g���f���̕`��
	CTouchActor::Draw(pShader);
}

//============================================================
// ���̐ݒ菈��
//============================================================
void CTouchBranch::SetData
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
bool CTouchBranch::Collision
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
		D3DXVECTOR3 move = VEC3_ZERO;

		// ���ł���������ݒ�
		rot.y = atan2f(posCan.x - rPos.x, posCan.z - rPos.z);

		// �A�N�V������Ԃɂ���
		SetState(STATE_ACT);

		// �ړ��ʂ�ݒ肷��
		move.x = sinf(rot.y) * FLY_SPEED;
		move.y = FLY_HEIGHT;
		move.z = cosf(rot.y) * FLY_SPEED;

		// �����ƈړ��ʂ𔽉f
		SetVec3Rotation(rot);
		SetVec3Move(move);

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
	return false;
}

//============================================================
// �ʏ��ԍX�V����
//============================================================
void CTouchBranch::UpdateNone(const float /*fDeltaTime*/)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = GetVec3Position();
	pos.y -= GRAVITY;

	// ���Ƃ̓����蔻��
	CollisionFieid(pos);

	// ���̕��𑫂�
	pos.y -= GetModelData().vtxMin.y;

	// �ʒu�𔽉f
	SetVec3Position(pos);
}

//============================================================
// �A�N�V������ԍX�V����
//============================================================
void CTouchBranch::UpdateAct(const float fDeltaTime)
{
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu
	D3DXVECTOR3 rot = GetVec3Rotation();	// ����
	D3DXVECTOR3 move = GetVec3Move();		// �ړ���

	// �ړ�����
	pos += move * fDeltaTime;

	// �d�͂�������
	move.y -= GRAVITY * fDeltaTime;

	// ���������Z����
	rot.y += CYCLE_Y * fDeltaTime;
	rot.z += CYCLE_Z * fDeltaTime;

	// �t�B�[���h�Ƃ̓����蔻��
	if (CollisionFieid(pos))
	{ // �t�B�[���h�ɓ��������ꍇ

		if (!m_bSound)
		{ // �������Ă��Ȃ������ꍇ

			// �����鉹��炷
			PLAY_SOUND(CSound::LABEL_SE_TOUCHFLY);

			// �T�E���h�󋵂� true �ɂ���
			m_bSound = true;
		}

		// ���a���グ��
		pos.y += GetModelData().fRadius;

		// �ړ��ʂ��d�͈ȊO0�ɂ���
		move.x = 0.0f;
		move.z = 0.0f;

		// ���������ɂ���
		rot.z = 0.0f;
	}

	// �ʒu�ƌ����ƈړ��ʂ𔽉f
	SetVec3Position(pos);
	SetVec3Rotation(rot);
	SetVec3Move(move);
}
