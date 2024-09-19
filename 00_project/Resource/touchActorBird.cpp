//============================================================
//
//	�^�b�`�A�N�^�[(��)���� [touchActorBird.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "touchActorBird.h"

#include "collision.h"
#include "scene.h"
#include "player.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int NONE_MOVE_COUNT = 90;				// �ʏ��Ԃ̈ړ��J�E���g��
	const float COLLISION_RADIUS = 150.0f;		// �����蔻��̔��a
	const float NONE_SPEED = 200.0f;			// �ʏ펞�̈ړ���
	const float NONE_HEIGHT = 80.0f;			// �ʏ펞�̍���
	const float NONE_GRAVITY = 500.0f;			// �ʏ펞�̏d��
	const float FLY_SPEED = 700.0f;				// ���ł������x
	const float FLY_HEIGHT = 300.0f;			// ���ł�������
	const float FLY_GRAVITY = 200.0f;			// ��s���̏d��
}

//************************************************************
//	�q�N���X [CTouchBird] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTouchBird::CTouchBird() : CTouchActor()
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
		D3DXVECTOR3 move = VEC3_ZERO;

		// ���ł���������ݒ�
		rot.y = atan2f(posBird.x - rPos.x, posBird.z - rPos.z);

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
void CTouchBird::UpdateNone(const float fDeltaTime)
{
	D3DXVECTOR3 pos = GetVec3Position();
	D3DXVECTOR3 rot = GetVec3Rotation();
	D3DXVECTOR3 move = GetVec3Move();

	// �d�͂�������
	move.y -= NONE_GRAVITY * fDeltaTime;

	if (GetStateCount() % NONE_MOVE_COUNT == 0)
	{ // ���J�E���g���Ƃ�

		// �v���C���[�̈ʒu���擾����
		D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();

		// �����������_���Őݒ�
		rot.y = atan2f(pos.x - posPlayer.x, pos.z - posPlayer.z) + (float)(rand() % 315 - 157) * 0.01f;

		// �ړ��ʂ�ݒ肷��
		move.x = sinf(rot.y) * NONE_SPEED;
		move.y = NONE_HEIGHT;
		move.z = cosf(rot.y) * NONE_SPEED;
	}

	// �ړ�����
	pos += move * fDeltaTime;

	// ���Ƃ̓����蔻��
	if (CollisionFieid(pos))
	{
		// �ړ��ʂ�0�ɂ���
		move = VEC3_ZERO;
	}

	// �ʒu�ƌ����ƈړ��ʂ𔽉f
	SetVec3Position(pos);
	SetVec3Rotation(rot);
	SetVec3Move(move);
}

//============================================================
// �A�N�V������ԍX�V����
//============================================================
void CTouchBird::UpdateAct(const float fDeltaTime)
{
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu
	D3DXVECTOR3 move = GetVec3Move();		// �ړ���

	// �ړ�����
	pos += move * fDeltaTime;

	// �d�͂�������
	move.y -= FLY_GRAVITY * fDeltaTime;

	// �ړ��ʂ� 0.0f �ȉ��ɂȂ����ꍇ�A0�ɕ␳����
	if (move.y <= 0.0f) { move.y = 0.0f; }

	// �ʒu�ƈړ��ʂ𔽉f
	SetVec3Position(pos);
	SetVec3Move(move);
}
