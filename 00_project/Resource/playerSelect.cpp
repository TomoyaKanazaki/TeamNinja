//============================================================
//
//	�Z���N�g��ʃv���C���[���� [playerSelect.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "playerSelect.h"
#include "manager.h"
#include "stage.h"
#include "camera.h"
#include "transpoint.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{

}

//************************************************************
//	�q�N���X [CPlayerSelect] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerSelect::CPlayerSelect()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayerSelect::~CPlayerSelect()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayerSelect::Init(void)
{
	// �v���C���[�̏�����
	if (FAILED(CPlayer::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�|�[���̐ݒ�
	SetSpawn();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayerSelect::Uninit(void)
{
	// �v���C���[�̏I��
	CPlayer::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayerSelect::Update(const float fDeltaTime)
{
	// �v���C���[�̍X�V
	CPlayer::Update(fDeltaTime);
}

//============================================================
//	��Ԃ̍X�V����
//============================================================
CPlayer::EMotion CPlayerSelect::UpdateState(const float fDeltaTime)
{
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����
	switch (GetState())
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:

		// �������Ȃ���Ԏ��̍X�V
		currentMotion = UpdateNone(fDeltaTime);
		break;

	case STATE_SELECT_SPAWN:

		// �X�|�[����Ԏ��̍X�V
		currentMotion = UpdateSpawn(fDeltaTime);
		break;

	case STATE_SELECT_NORMAL:

		// �ʏ��Ԏ��̍X�V
		currentMotion = UpdateNormal(fDeltaTime);
		break;

	case STATE_SELECT_ENTER:

		// �����Ԏ��̍X�V
		currentMotion = UpdateEnter(fDeltaTime);
		break;

	default:
		assert(false);
		break;
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	�X�|�[����Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayerSelect::UpdateSpawn(const float fDeltaTime)
{
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����
	if (IsMotionFinish())
	{ // �I�����[�V�������I�������ꍇ

		// ��荞�݃J�����ɑJ��
		GET_CAMERA->SetState(CCamera::STATE_AROUND);

		// �ҋ@���[�V������ݒ�
		SetMotion(MOTION_IDOL);

		// �ʏ��Ԃ�ݒ�
		SetState(STATE_SELECT_NORMAL);
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayerSelect::UpdateNormal(const float fDeltaTime)
{
	EMotion currentMotion = MOTION_IDOL;		// ���݂̃��[�V����
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �ړ�����
	currentMotion = UpdateMove();

	// �d�͂̍X�V
	UpdateGravity(fDeltaTime);

	// �ʒu�X�V
	UpdatePosition(posPlayer, fDeltaTime);

	// ���n����
	UpdateLanding(posPlayer, fDeltaTime);

	// �����X�V
	UpdateRotation(rotPlayer, fDeltaTime);

	// �ǂ̓����蔻��
	CollisionWall(posPlayer);

	// ��l�̕ǂ̔���
	GET_STAGE->LimitPosition(posPlayer, GetRadius());

	// �X�e�[�W�J�ڂ̍X�V
	UpdateTrans(posPlayer);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// ���g�̏���
	if(ControlClone(posPlayer, rotPlayer, fDeltaTime))
	{ currentMotion = MOTION_DODGE; }

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//==========================================
//	�����Ԏ��̍X�V����
//==========================================
CPlayer::EMotion CPlayerSelect::UpdateEnter(const float fDeltaTime)
{
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����
	if (IsMotionFinish())
	{ // �I�����[�V�������I�������ꍇ

		// �ݒ�ς݃}�b�v�p�X�ɑJ��
		GET_MANAGER->SetLoadScene(CScene::MODE_GAME);
	}

	// �����𔽉f
	SetVec3Rotation(GetDestRotation());

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//==========================================
//	�X�e�[�W�J�ڂ̍X�V����
//==========================================
void CPlayerSelect::UpdateTrans(D3DXVECTOR3& rPos)
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CInputPad*		pPad = GET_INPUTPAD;	// �p�b�h���

	// �G��Ă���J�ڃ|�C���g���擾
	CTransPoint *pHitTrans = CTransPoint::Collision(rPos, GetRadius());

	// �J�ڃ|�C���g�ɐG��Ă��Ȃ��ꍇ������
	if (pHitTrans == nullptr) { return; }

	if (pKey->IsTrigger(DIK_SPACE)
	||  pKey->IsTrigger(DIK_RETURN)
	||  pPad->IsTrigger(CInputPad::KEY_A)
	||  pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X)
	||  pPad->IsTrigger(CInputPad::KEY_Y))
	{
		// �J�ڃ|�C���g�C���f�b�N�X��ۑ�
		GET_RETENTION->SetTransIdx(CTransPoint::GetList()->GetIndex(pHitTrans));

		// ����̐ݒ�
		SetEnter(pHitTrans->GetTransMapPass().c_str());
	}
}

//==========================================
//	�X�|�[���̐ݒ菈��
//==========================================
void CPlayerSelect::SetSpawn(void)
{
	// ����������
	SetState(STATE_SELECT_SPAWN);	// �X�|�[����Ԃ̐ݒ�
	SetMotion(MOTION_SELECT);		// �I�����[�V������ݒ�

	// �ړ��ʂ�������
	SetMove(VEC3_ZERO);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �����x��s�����ɐݒ�
	SetAlpha(1.0f);

	// �`����ĊJ
	SetEnableDraw(true);

	// ��荞�݃J�����̐ݒ�
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_SELECT);
	GET_MANAGER->GetCamera()->SetDestSelect();

	// �v���C���[�̌������J���������ɐݒ�
	D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, GET_CAMERA->GetDestRotation().y, 0.0f);	// �J��������
	SetVec3Rotation(rotCamera);
	SetDestRotation(rotCamera);
}

//==========================================
//	����̐ݒ菈��
//==========================================
void CPlayerSelect::SetEnter(const char* pTransMapPath)
{
	// �J�ڃ|�C���g�̃}�b�v�p�X��ۑ�
	GET_STAGE->SetInitMapPass(pTransMapPath);

	// �I�����[�V�����ɂ���
	SetMotion(MOTION_SELECT);

	// �����Ԃɂ���
	SetState(STATE_SELECT_ENTER);

	// �I���J�����ɂ���
	GET_CAMERA->SetState(CCamera::STATE_SELECT);

	// �v���C���[�̌������J���������ɐݒ�
	D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, GET_CAMERA->GetDestRotation().y, 0.0f);	// �J��������
	SetVec3Rotation(rotCamera);
	SetDestRotation(rotCamera);

	// TODO�F�J�n�G�t�F�N�g�𐶐�
	GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_zitu_2.efkefc", GetVec3Position(), VEC3_ZERO, VEC3_ZERO, 45.0f);
}
