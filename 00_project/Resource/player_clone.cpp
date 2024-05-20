//============================================================
//
//	�v���C���[�̕��g���� [player_clone.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "player_clone.h"
#include "shadow.h"
#include "input.h"
#include "manager.h"
#include "useful.h"
#include "player.h"
#include "orbit.h"
#include "multiModel.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\CHARACTER\\player.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

	const int	PRIORITY	= 3;		// �v���C���[�̗D�揇��
	const float	MOVE		= 150.0f;	// �ړ���
	const float	JUMP		= 21.0f;	// �W�����v�㏸��
	const float	GRAVITY		= 1.0f;		// �d��
	const float	RADIUS		= 20.0f;	// ���a
	const float	HEIGHT		= 100.0f;	// �c��
	const float	REV_ROTA	= 0.15f;	// �����ύX�̕␳�W��
	const float	ADD_MOVE	= 0.08f;	// ��A�N�V�������̑��x���Z��
	const float	JUMP_REV	= 0.16f;	// �ʏ��Ԏ��̋󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV	= 0.16f;	// �ʏ��Ԏ��̒n��̈ړ��ʂ̌����W��
	const float	SPAWN_ADD_ALPHA	= 0.03f;	// �X�|�[����Ԏ��̓����x�̉��Z��

	const D3DXVECTOR3 DMG_ADDROT	= D3DXVECTOR3(0.04f, 0.0f, -0.02f);	// �_���[�W��Ԏ��̃v���C���[��]��
	const D3DXVECTOR3 SHADOW_SIZE	= D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// �e�̑傫��

	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), XCOL_GREEN);	// �I�t�Z�b�g���
	const int ORBIT_PART = 10;	// ������

	const float DISTANCE = 75.0f; // �v���C���[�Ƃ̋���
	const float TIMER = 10.0f; // �������Ń^�C�}�[
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CPlayerClone>* CPlayerClone::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CPlayerClone] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerClone::CPlayerClone() : CObjectChara(CObject::LABEL_AVATAR, CObject::DIM_3D, PRIORITY),
m_pShadow(nullptr),			// �e�̏��
m_pOrbit(nullptr),			// �O�Ղ̏��
m_move(0.0f, 0.0f, 0.0f),	// �ړ���
m_Action(ACTION_NONE),		// �s��
m_fTimer(0.0f)				// �������Ń^�C�}�[
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayerClone::~CPlayerClone()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayerClone::Init(void)
{
	// �����o�ϐ���������
	m_pShadow = nullptr;		// �e�̏��
	m_pOrbit = nullptr;		// �O�Ղ̏��
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړ���
	m_Action = ACTION_NONE; // �s��
	m_fTimer = 0.0f; // �������Ń^�C�}�[

	// �I�u�W�F�N�g�L�����N�^�[�̏�����
	if (FAILED(CObjectChara::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �L�����N�^�[���̊���
	BindCharaData(SETUP_TXT);

	// �e�̐���
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, SHADOW_SIZE, this);
	if (m_pShadow == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �O�Ղ̐���
	m_pOrbit = COrbit::Create
	( // ����
		GetParts(MODEL_BODY)->GetPtrMtxWorld(),	// �e�}�g���b�N�X
		ORBIT_OFFSET,	// �I�t�Z�b�g���
		ORBIT_PART		// ������
	);
	if (m_pOrbit == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CPlayerClone>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// �}�e���A����ύX
	SetAllMaterial(material::Green());

	// �T�C�Y�𒲐�
	SetVec3Scaling(D3DXVECTOR3(0.8f, 0.8f, 0.8f));

	// �v���C���[�ʒu�ɏo��
	SetVec3Position(GET_PLAYER->GetVec3Position());

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayerClone::Uninit(void)
{
	// �e�̏I��
	m_pShadow->DeleteObjectParent();	// �e�I�u�W�F�N�g���폜
	SAFE_UNINIT(m_pShadow);

	// �O�Ղ̏I��
	SAFE_UNINIT(m_pOrbit);

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	CObjectChara::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayerClone::Update(const float fDeltaTime)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// �e��s�����N����
	switch (m_Action)
	{
	case ACTION_MOVE:

		// �ړ�
		SetVec3Position(GetVec3Position() + (m_move * fDeltaTime));

		// ����
		m_fTimer -= fDeltaTime;
		if (m_fTimer <= 0.0f)
		{
			Uninit();
			return;
		}

		break;

	case ACTION_NONE:

		// ��O��ǂ�������
		ChasePrev();

		break;
	}

	// �e�̍X�V
	m_pShadow->Update(fDeltaTime);

	// �O�Ղ̍X�V
	m_pOrbit->Update(fDeltaTime);

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	UpdateMotion(currentMotion, fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CPlayerClone::Draw(CShader* pShader)
{
	CToonShader* pToonShader = CToonShader::GetInstance();	// �g�D�[���V�F�[�_�[���
	if (pToonShader->IsEffectOK())
	{ // �G�t�F�N�g���g�p�\�ȏꍇ

		// �I�u�W�F�N�g�L�����N�^�[�̕`��
		CObjectChara::Draw(pToonShader);
	}
	else
	{ // �G�t�F�N�g���g�p�s�\�ȏꍇ

		// �G�t�F�N�g�G���[
		assert(false);

		// �I�u�W�F�N�g�L�����N�^�[�̕`��
		CObjectChara::Draw(pShader);
	}
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CPlayerClone::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);		// ���g
	m_pShadow->SetEnableUpdate(bUpdate);	// �e
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPlayerClone::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);		// ���g
	m_pShadow->SetEnableDraw(bDraw);	// �e
}

//============================================================
//	�m�b�N�o�b�N�q�b�g����
//============================================================
bool CPlayerClone::HitKnockBack(const int /*nDamage*/, const D3DXVECTOR3& /*rVecKnock*/)
{
	if (IsDeath()) { return false; }	// ���S�ς�

	return true;
}

//============================================================
//	�q�b�g����
//============================================================
bool CPlayerClone::Hit(const int nDamage)
{
	if (IsDeath()) { return false; }	// ���S�ς�

	return true;
}

//============================================================
//	��������
//============================================================
CPlayerClone* CPlayerClone::Create(void)
{
	// �|�C���^��錾
	CPlayerClone* pPlayer = nullptr;	// �v���C���[���
	pPlayer = new CPlayerClone;

	if (pPlayer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �v���C���[�̏�����
		if (FAILED(pPlayer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �v���C���[�̔j��
			SAFE_DELETE(pPlayer);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pPlayer;
	}
}

//==========================================
//  ��������(���s)
//==========================================
CPlayerClone* CPlayerClone::Create(const D3DXVECTOR3& move)
{
	// �|�C���^��錾
	CPlayerClone* pPlayer = new CPlayerClone;	// �v���C���[���

	// �����Ɏ��s�����ꍇnull��Ԃ�
	if (pPlayer == nullptr) { return nullptr; }

	// �v���C���[�̏�����
	if (FAILED(pPlayer->Init()))
	{ // �������Ɏ��s�����ꍇ

		// �v���C���[�̔j��
		SAFE_DELETE(pPlayer);
		return nullptr;
	}

	// �ړ��ʂ�ݒ�
	pPlayer->m_move = move;

	// �s����ݒ�
	pPlayer->m_Action = ACTION_MOVE;

	// �������Ń^�C�}�[��ݒ�
	pPlayer->m_fTimer = TIMER;

	// �m�ۂ����A�h���X��Ԃ�
	return pPlayer;
}

//============================================================
// ��������
//============================================================
void CPlayerClone::Delete(const int nNum)
{
	// ���X�g��񂪂Ȃ��ꍇ��~����
	if (m_pList == nullptr) { assert(false); return; }

	// ���݂̑���������̐������w�肳��Ă����ꍇ��~����
	if (m_pList->GetNumAll() <= nNum) { assert(false); return; }

	// ���g���擾
	CPlayerClone* pAvatar = *m_pList->GetIndex(nNum);

	// ���g�̏I��
	pAvatar->Uninit();
}

//============================================================
//  �S�������� (����ǉ�)
//============================================================
void CPlayerClone::Delete(void)
{
	// ���X�g��񂪂Ȃ��ꍇ��~����
	if (m_pList == nullptr) { assert(false); return; }

	// �������擾
	int nNum = m_pList->GetNumAll();

	// �S�Ă̕��g���폜����
	for (int i = 0; i < nNum; ++i)
	{
		// ���g���擾
		CPlayerClone* pAvatar = *m_pList->GetIndex(0);

		// ���g�̏I��
		pAvatar->Uninit();
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CPlayerClone>* CPlayerClone::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateNormal(void)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_IDOL;		// ���݂̃��[�V����
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V����
//============================================================
void CPlayerClone::UpdateMotion(int nMotion, const float fDeltaTime)
{
	// ����ł��甲����
	if (IsDeath()) { return; }

	// �ϐ���錾
	int nAnimMotion = GetMotionType();	// ���ݍĐ����̃��[�V����

	if (nMotion != NONE_IDX)
	{ // ���[�V�������ݒ肳��Ă���ꍇ

		if (IsMotionLoop())
		{ // ���[�v���郂�[�V�����������ꍇ

			if (nAnimMotion != nMotion)
			{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(nMotion);
			}
		}
	}

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update(fDeltaTime);
}

//============================================================
//	�t�F�[�h�A�E�g��Ԏ��̍X�V����
//============================================================
bool CPlayerClone::UpdateFadeOut(const float fAdd)
{
	// �ϐ���錾
	bool bAlpha = false;		// ������
	float fAlpha = GetAlpha();	// �����x

	// �v���C���[���g�̕`����ĊJ
	CObject::SetEnableDraw(true);

	// �����x���グ��
	fAlpha += fAdd;

	if (fAlpha >= GetMaxAlpha())
	{ // �����x���オ��؂����ꍇ

		// �����x��␳
		fAlpha = GetMaxAlpha();

		// �s�����ɂȂ�؂�����Ԃɂ���
		bAlpha = true;
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bAlpha;
}

//============================================================
//	�t�F�[�h�C����Ԏ��̍X�V����
//============================================================
bool CPlayerClone::UpdateFadeIn(const float fSub)
{
	// �ϐ���錾
	bool bAlpha = false;		// ������
	float fAlpha = GetAlpha();	// �����x

	// �����x��������
	fAlpha -= fSub;

	if (fAlpha <= 0.0f)
	{ // �����x��������؂����ꍇ

		// �����x��␳
		fAlpha = 0.0f;

		// �����ɂȂ�؂�����Ԃɂ���
		bAlpha = true;

		// �v���C���[���g�̕`����~
		CObject::SetEnableDraw(false);
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bAlpha;
}

//==========================================
//  �O�ɂ��Ă�������
//==========================================
void CPlayerClone::ChasePrev()
{
	// ���X�g�̐擪���擾����
	std::list<CPlayerClone*> list = m_pList->GetList();
	auto itrBegin = list.begin();

	// �擪�Ǝ��g���r����
	if (*itrBegin == this)
	{
		// ���Ă���
		Chase(GET_PLAYER->GetVec3Position(), GET_PLAYER->GetVec3Rotation());

		return;
	}

	// ���X�g�̍Ō�����擾����
	auto itrEnd = list.end();

	// ���̃|�C���^���擾����ϐ�
	CPlayerClone* prev = *itrBegin;

	// ���g�Ɉ�v����|�C���^�̈�O�ɒǏ]����
	for (auto itr = itrBegin; itr != itrEnd; itr++)
	{
		// ���݂̃|�C���^�Ǝ��g���r����
		if (*itr == this)
		{
			// ���Ă���
			Chase(prev->GetVec3Position(), prev->GetVec3Rotation());

			return;
		}

		// ���݂̃|�C���^��ۑ�����
		prev = *itr;
	}
}

//==========================================
//  ���Ă�������
//==========================================
void CPlayerClone::Chase(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// ��O�ɑ΂��Č��ړ�
	D3DXVECTOR3 posTarget = rPos + D3DXVECTOR3
	(
		sinf(rRot.y) * DISTANCE,
		0.0f,
		cosf(rRot.y) * DISTANCE
	);

	// ���݂̍��W���擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �ڕW�n�_�ւ̃x�N�g�������߂�
	D3DXVECTOR3 vecTarget = posTarget - pos;

	// �ڕW�ւ̃x�N�g���ɔ{�����������ݒn�ɉ��Z����
	pos += vecTarget * 0.1f;

	// �ʒu��K�p
	SetVec3Position(pos);

	// �ڕW�̕�������������
	ViewTarget(rPos);
}

//==========================================
//  �ڕW�̕�������������
//==========================================
void CPlayerClone::ViewTarget(const D3DXVECTOR3& rPos)
{
	// �ڕW�����Ƃ̍��������߂�
	D3DXVECTOR3 vecTarget = rPos - GetVec3Position();

	// �����x�N�g���̌��������߂�
	float fRot = -atan2f(vecTarget.x, -vecTarget.z);

	// ������K�p����
	D3DXVECTOR3 rot = GetVec3Rotation();
	rot.y = fRot;
	SetVec3Rotation(rot);
}
