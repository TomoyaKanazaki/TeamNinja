//============================================================
//
//	���[�V�������� [motion.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "motion.h"
#include "multiModel.h"
#include "objectChara.h"

//************************************************************
//	�e�N���X [CMotion] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMotion::CMotion() :
	m_ppModel	(nullptr),	// ���f�����
	m_pChara	(nullptr),	// �I�u�W�F�N�g�L�����N�^�[���
	m_nNumModel	(0),		// ���f���̃p�[�c��
	m_bUpdate	(true)		// �X�V��
{
	// �����o�ϐ����N���A
	memset(&m_info, 0, sizeof(m_info));	// ���[�V�������
}

//============================================================
//	�f�X�g���N�^
//============================================================
CMotion::~CMotion()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMotion::Init(void)
{
	// �����o�ϐ����N���A
	memset(&m_info, 0, sizeof(m_info));	// ���[�V�������
	m_ppModel	= nullptr;	// ���f�����
	m_pChara	= nullptr;	// �I�u�W�F�N�g�L�����N�^�[���
	m_nNumModel	= 0;		// ���f���̃p�[�c��
	m_bUpdate	= true;		// �X�V��

	// ���[�V�������I����Ԃɂ���
	m_info.bFinish = true;

	for (int nCntMotion = 0; nCntMotion < motion::MAX_MOTION; nCntMotion++)
	{ // ���[�V�����̍ő吔���J��Ԃ�

		// �L�����Z���t���[�����Ȃ��ɂ���
		m_info.aMotionInfo[nCntMotion].nCancelFrame = NONE_IDX;

		// �U���������������
		m_info.aMotionInfo[nCntMotion].collLeft.nMin  = NONE_IDX;
		m_info.aMotionInfo[nCntMotion].collLeft.nMax  = NONE_IDX;
		m_info.aMotionInfo[nCntMotion].collRight.nMin = NONE_IDX;
		m_info.aMotionInfo[nCntMotion].collRight.nMax = NONE_IDX;

		// ����\����OFF�ɂ���
		m_info.aMotionInfo[nCntMotion].bWeaponDisp = false;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CMotion::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CMotion::Update(void)
{
	if (!m_bUpdate) { return; }	// �X�V���Ȃ�
	if (m_info.aMotionInfo[m_info.nType].nNumKey <= 0) { return; }	// �L�[�����ݒ�

	// �p�[�c�̍X�V
	UpdateParts();

	// �ړ��̍X�V
	UpdateMove();
}

//============================================================
//	�ړ��̍X�V����
//============================================================
void CMotion::UpdateMove(void)
{
	if (m_pChara == nullptr) { return; }	// �I�u�W�F�N�g�L�����N�^�[���ݒ�

	// �ϐ���錾
	D3DXMATRIX  mtxChara	= m_pChara->GetMtxWorld();					// �L�����}�g���b�N�X
	D3DXVECTOR3 posSetChara	= m_pChara->GetVec3Position();				// �L�����ݒ�ʒu
	D3DXVECTOR3 posOldChara	= useful::GetMtxWorldPosition(mtxChara);	// �L�����ߋ��ʒu
	D3DXVECTOR3 posCurChara = VEC3_ZERO;								// �L�������݈ʒu

	// �ړ��ʂ����߂�
	float fRate = 1.0f / (float)m_info.aMotionInfo[m_info.nType].aKeyInfo[m_info.nKey].nFrame;	// �L�[�t���[������
	D3DXVECTOR3 moveRate = m_info.aMotionInfo[m_info.nType].aKeyInfo[m_info.nKey].move * fRate;	// �t���[���ړ���

	if (m_info.aMotionInfo[m_info.nType].aKeyInfo[m_info.nKey].nFrame > 0)
	{ // �t���[�����ݒ肳��Ă���ꍇ

		// �ړ��ʂ��}�g���b�N�X�ɔ��f
		D3DXMATRIX mtxMove;	// �}�g���b�N�X�v�Z�p
		D3DXMatrixTranslation(&mtxMove, moveRate.x, moveRate.y, moveRate.z);
		D3DXMatrixMultiply(&mtxChara, &mtxMove, &mtxChara);

		// �ړ��ʂ�^�����}�g���b�N�X�̃��[���h���W�����߂�
		posCurChara = useful::GetMtxWorldPosition(mtxChara);

		// �ߋ��ƌ��݂̈ʒu����ړ��ʂ����߁A�ʒu�ɗ^����
		posSetChara += posOldChara - posCurChara;

		// �ʒu�𔽉f
		m_pChara->SetVec3Position(posSetChara);
	}
}

//============================================================
//	�p�[�c�̍X�V����
//============================================================
void CMotion::UpdateParts(void)
{
	// �ϐ���錾
	D3DXVECTOR3 diffPos;		// ���L�[�܂ł̍����ʒu
	D3DXVECTOR3 diffRot;		// ���L�[�܂ł̍�������
	D3DXVECTOR3 currentPos;		// ���݃t���[���̈ʒu
	D3DXVECTOR3 currentRot;		// ���݃t���[���̌���
	int nType = m_info.nType;	// ���[�V�������
	int nKey  = m_info.nKey;	// ���[�V�����L�[�ԍ�
	int nNextKey;				// ���̃��[�V�����L�[�ԍ�

	// ���̃��[�V�����L�[�ԍ������߂�
	nNextKey = (nKey + 1) % m_info.aMotionInfo[nType].nNumKey;

	// �p�[�c�̈ʒu�̍X�V
	for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
	{ // ���f���̃p�[�c�����J��Ԃ�

		// �ʒu�E�����̍��������߂�
		diffPos = m_info.aMotionInfo[nType].aKeyInfo[nNextKey].aKey[nCntKey].pos - m_info.aMotionInfo[nType].aKeyInfo[nKey].aKey[nCntKey].pos;
		diffRot = m_info.aMotionInfo[nType].aKeyInfo[nNextKey].aKey[nCntKey].rot - m_info.aMotionInfo[nType].aKeyInfo[nKey].aKey[nCntKey].rot;

		// ���������̐��K��
		useful::Vec3NormalizeRot(diffRot);

		// ���݂̃p�[�c�̈ʒu�E�������X�V
		float fRate = (float)m_info.nKeyCounter / (float)m_info.aMotionInfo[nType].aKeyInfo[nKey].nFrame;	// �L�[�t���[������
		m_ppModel[nCntKey]->SetVec3Position(m_info.aMotionInfo[nType].aKeyInfo[nKey].aKey[nCntKey].pos + diffPos * fRate);
		m_ppModel[nCntKey]->SetVec3Rotation(m_info.aMotionInfo[nType].aKeyInfo[nKey].aKey[nCntKey].rot + diffRot * fRate);
	}

	// ���[�V�����̑J�ڂ̍X�V
	if (m_info.nKeyCounter < m_info.aMotionInfo[nType].aKeyInfo[nKey].nFrame)
	{ // ���݂̃L�[�̍Đ����I�����Ȃ��ꍇ

		// �J�E���^�[�����Z
		m_info.nKeyCounter++;
		m_info.nWholeCounter++;
	}
	else
	{ // ���݂̃L�[�̍Đ����I�������ꍇ

		// ���̃L�[�Ɉڍs
		if (m_info.aMotionInfo[nType].bLoop)
		{ // ���[�V���������[�v����ꍇ

			// �L�[�J�E���^�[��������
			m_info.nKeyCounter = 0;

			// �L�[�J�E���g�����Z
			m_info.nKey = (m_info.nKey + 1) % m_info.aMotionInfo[nType].nNumKey;	// �ő�l��0�ɖ߂�

			if (m_info.nKey == 0)
			{ // �L�[���ŏ��ɖ߂����ꍇ

				// �S�̃J�E���^�[��������
				m_info.nWholeCounter = 0;
			}
		}
		else
		{ // ���[�V���������[�v���Ȃ��ꍇ

			if (m_info.nKey < m_info.aMotionInfo[nType].nNumKey - 2)
			{ // ���݂̃L�[���ŏI�̃L�[�ł͂Ȃ��ꍇ

				// �L�[�J�E���^�[��������
				m_info.nKeyCounter = 0;

				// �L�[�J�E���g�����Z
				m_info.nKey++;
			}
			else
			{ // ���݂̃L�[���ŏI�̃L�[�̏ꍇ

				// ���[�V�������I����Ԃɂ���
				m_info.bFinish = true;
			}
		}
	}
}

//============================================================
//	�ݒ菈��
//============================================================
void CMotion::Set(const int nType)
{
	// ���[�V��������������
	m_info.nKey			 = 0;		// ���[�V�����L�[�ԍ�
	m_info.nKeyCounter	 = 0;		// ���[�V�����L�[�J�E���^�[
	m_info.nWholeCounter = 0;		// ���[�V�����S�̃J�E���^�[
	m_info.bFinish		 = false;	// ���[�V�����I����

	// �����̃��[�V�����̎�ނ�ݒ�
	m_info.nType = nType;

	// ���[�V�������Đ���Ԃɂ���
	m_info.bFinish = false;

	// �p�[�c�̈ʒu�̏�����
	for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
	{ // ���f���̃p�[�c�����J��Ԃ�

		// �����ʒu�Ə���������ݒ�
		m_ppModel[nCntKey]->SetVec3Position(m_info.aMotionInfo[nType].aKeyInfo[m_info.nKey].aKey[nCntKey].pos);
		m_ppModel[nCntKey]->SetVec3Rotation(m_info.aMotionInfo[nType].aKeyInfo[m_info.nKey].aKey[nCntKey].rot);
	}
}

//============================================================
//	�Đݒ菈��
//============================================================
void CMotion::Reset(void)
{
	// ���[�V��������������
	m_info.nType		 = 0;		// ���[�V�������
	m_info.nKey			 = 0;		// ���[�V�����L�[�ԍ�
	m_info.nKeyCounter	 = 0;		// ���[�V�����L�[�J�E���^�[
	m_info.nWholeCounter = 0;		// ���[�V�����S�̃J�E���^�[
	m_info.bFinish		 = false;	// ���[�V�����I����

	// �p�[�c�̈ʒu�̏�����
	for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
	{ // ���f���̃p�[�c�����J��Ԃ�

		// ���_�ʒu�ƌ��_������ݒ�
		m_ppModel[nCntKey]->SetVec3Position(m_info.aOriginKey[nCntKey].pos);
		m_ppModel[nCntKey]->SetVec3Rotation(m_info.aOriginKey[nCntKey].rot);
	}
}

//============================================================
//	���_�ʒu�̐ݒ菈��
//============================================================
void CMotion::SetOriginPosition(const D3DXVECTOR3 &rPos, const int nParts)
{
	if (nParts > NONE_IDX && nParts < motion::MAX_PARTS)
	{
		// ���_�ʒu��ݒ�
		m_info.aOriginKey[nParts].pos = rPos;
	}
	else { assert(false); }
}

//============================================================
//	���_�����̐ݒ菈��
//============================================================
void CMotion::SetOriginRotation(const D3DXVECTOR3 &rRot, const int nParts)
{
	if (nParts > NONE_IDX && nParts < motion::MAX_PARTS)
	{
		// ���_������ݒ�
		m_info.aOriginKey[nParts].rot = rRot;
	}
	else { assert(false); }
}

//============================================================
//	���[�V�������̐ݒ菈��
//============================================================
void CMotion::SetInfo(const SMotionInfo info)
{
	// �����̃��[�V��������ݒ�
	m_info.aMotionInfo[m_info.nNumType] = info;

	// ���[�V�����S�̃t���[������ݒ�
	int nSubKey = (m_info.aMotionInfo[m_info.nNumType].bLoop) ? 0 : 1;	// ���[�v���Ȃ��ꍇ�Ō�̃L�[�͊܂܂Ȃ�
	int nLoop = m_info.aMotionInfo[m_info.nNumType].nNumKey - nSubKey;	// �J��Ԃ��������߂�
	for (int nCntKey = 0; nCntKey < nLoop; nCntKey++)
	{ // �L�[�̑������J��Ԃ�

		// �L�[�̃t���[���������Z
		m_info.aMotionInfo[m_info.nNumType].nWholeFrame += m_info.aMotionInfo[m_info.nNumType].aKeyInfo[nCntKey].nFrame;
	}

	// ���[�V�����̏�񐔂����Z
	m_info.nNumType++;

	// ��O����
	assert(m_info.nNumType <= motion::MAX_MOTION);	// ���[�V�������I�[�o�[
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CMotion::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	m_bUpdate = bUpdate;
}

//============================================================
//	���f�����̐ݒ菈��
//============================================================
void CMotion::SetModel(CMultiModel **ppModel, const int nNum)
{
	// �����̃��f������ݒ�
	m_ppModel = ppModel;

	// �����̃��f���̃p�[�c����ݒ�
	m_nNumModel = nNum;

	// ��O����
	assert(m_nNumModel <= motion::MAX_PARTS);	// �p�[�c���I�[�o�[
}

//============================================================
//	��ގ擾����
//============================================================
int CMotion::GetType(void) const
{
	// ���݂̃��[�V�����̎�ނ�Ԃ�
	return m_info.nType;
}

//============================================================
//	��ނ̑����擾����
//============================================================
int CMotion::GetNumType(void) const
{
	// ���[�V�����̎�ނ̑�����Ԃ�
	return m_info.nNumType;
}

//============================================================
//	�L�[�ԍ��擾����
//============================================================
int CMotion::GetKey(void) const
{
	// ���݂̃L�[�ԍ���Ԃ�
	return m_info.nKey;
}

//============================================================
//	�L�[�̑����擾����
//============================================================
int CMotion::GetNumKey(const int nType) const
{
	// �������[�V�����̃L�[�̑�����Ԃ�
	int nSubKey = (m_info.aMotionInfo[m_info.nNumType].bLoop) ? 0 : 1;	// ���[�v���Ȃ��ꍇ�Ō�̃L�[�͊܂܂Ȃ�
	return m_info.aMotionInfo[nType].nNumKey - nSubKey;
}

//============================================================
//	���[�V�����L�[�J�E���^�[�擾����
//============================================================
int CMotion::GetKeyCounter(void) const
{
	// ���݂̃��[�V�����L�[�J�E���^�[��Ԃ�
	return m_info.nKeyCounter;
}

//============================================================
//	���[�V�����S�̃J�E���^�[�擾����
//============================================================
int CMotion::GetWholeCounter(void) const
{
	// ���݂̃��[�V�����S�̃J�E���^�[��Ԃ�
	return m_info.nWholeCounter;
}

//============================================================
//	���[�V�����S�̃t���[�����擾����
//============================================================
int CMotion::GetWholeFrame(const int nType) const
{
	// �������[�V�����̑S�̃t���[������Ԃ�
	return m_info.aMotionInfo[nType].nWholeFrame;
}

//============================================================
//	���[�V�����L�����Z���t���[���擾����
//============================================================
int CMotion::GetCancelFrame(const int nType) const
{
	// �������[�V�����̃L�����Z���t���[����Ԃ�
	return m_info.aMotionInfo[nType].nCancelFrame;
}

//============================================================
//	�I���擾����
//============================================================
bool CMotion::IsFinish(void) const
{
	// ���݂̃��[�V�����̏I���󋵂�Ԃ�
	return m_info.bFinish;
}

//============================================================
//	���[�v�擾����
//============================================================
bool CMotion::IsLoop(const int nType) const
{
	// �������[�V�����̃��[�v��ON/OFF�󋵂�Ԃ�
	return m_info.aMotionInfo[nType].bLoop;
}

//============================================================
//	�L�����Z���擾����
//============================================================
bool CMotion::IsCancel(const int nType) const
{
	if (m_info.aMotionInfo[nType].nCancelFrame != NONE_IDX)
	{ // �L�����Z���t���[�����ݒ肳��Ă���ꍇ

		// �������[�V�����̃L�����Z���󋵂�Ԃ�
		return (m_info.nWholeCounter >= m_info.aMotionInfo[nType].nCancelFrame);
	}
	else
	{ // �L�����Z���t���[�����ݒ肳��Ă��Ȃ��ꍇ

		// �L�����Z���s��Ԃ�
		return false;
	}
}

//============================================================
//	����\���̎擾����
//============================================================
bool CMotion::IsWeaponDisp(const int nType) const
{
	// �������[�V�����̕���\����ON/OFF�󋵂�Ԃ�
	return m_info.aMotionInfo[nType].bWeaponDisp;
}

//============================================================
//	���̍U������t���O�̎擾����
//============================================================
bool CMotion::IsLeftWeaponCollision(void)
{
	SMotionInfo *pMotionInfo = &m_info.aMotionInfo[m_info.nType];	// ���݂̃��[�V�������
	if (pMotionInfo->collLeft.nMin == NONE_IDX) { return false; }	// �J�n�J�E���g���ݒ�
	if (pMotionInfo->collLeft.nMax == NONE_IDX) { return false; }	// �I���J�E���g���ݒ�

	if (m_info.nWholeCounter >= pMotionInfo->collLeft.nMin
	&&  m_info.nWholeCounter <= pMotionInfo->collLeft.nMax)
	{ // �J�E���^�[���J�n�ƏI���͈͓̔��̏ꍇ

		return true;
	}

	return false;
}

//============================================================
//	�E�̍U������t���O�̎擾����
//============================================================
bool CMotion::IsRightWeaponCollision(void)
{
	SMotionInfo *pMotionInfo = &m_info.aMotionInfo[m_info.nType];	// ���݂̃��[�V�������
	if (pMotionInfo->collRight.nMin == NONE_IDX) { return false; }	// �J�n�J�E���g���ݒ�
	if (pMotionInfo->collRight.nMax == NONE_IDX) { return false; }	// �I���J�E���g���ݒ�

	if (m_info.nWholeCounter >= pMotionInfo->collRight.nMin
	&&  m_info.nWholeCounter <= pMotionInfo->collRight.nMax)
	{ // �J�E���^�[���J�n�ƏI���͈͓̔��̏ꍇ

		return true;
	}

	return false;
}

//============================================================
//	��������
//============================================================
CMotion *CMotion::Create(CObjectChara *pChara)
{
	// ���[�V�����̐���
	CMotion *pMotion = new CMotion;
	if (pMotion == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���[�V�����̏�����
		if (FAILED(pMotion->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���[�V�����̔j��
			SAFE_DELETE(pMotion);
			return nullptr;
		}

		// �I�u�W�F�N�g�L�����N�^�[��ݒ�
		pMotion->m_pChara = pChara;

		// �m�ۂ����A�h���X��Ԃ�
		return pMotion;
	}
}

//============================================================
//	�j������
//============================================================
void CMotion::Release(CMotion *&prMotion)
{
	// ���[�V�����̏I��
	assert(prMotion != nullptr);
	prMotion->Uninit();

	// �������J��
	SAFE_DELETE(prMotion);
}