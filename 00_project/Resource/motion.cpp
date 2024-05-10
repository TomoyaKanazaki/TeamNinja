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
	m_bUpdate	(true)		// �X�V��
{

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
	m_ppModel	= nullptr;	// ���f�����
	m_pChara	= nullptr;	// �I�u�W�F�N�g�L�����N�^�[���
	m_bUpdate	= true;		// �X�V��

	// ���[�V�������I����Ԃɂ���
	m_info.bFinish = true;

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CMotion::Uninit(void)
{
	for (auto& rMotionInfo : m_info.vecMotionInfo)
	{ // ���[�V�������̗v�f�����J��Ԃ�

		for (auto& rKeyInfo : rMotionInfo.vecKeyInfo)
		{ // �L�[���̗v�f�����J��Ԃ�

			// �L�[���N���A
			rKeyInfo.vecKey.clear();
		}

		// �L�[�����N���A
		rMotionInfo.vecKeyInfo.clear();
	}

	// ���[�V���������N���A
	m_info.vecMotionInfo.clear();
}

//============================================================
//	�X�V����
//============================================================
void CMotion::Update(const float fDeltaTime)
{
	if (!m_bUpdate) { return; }	// �X�V���Ȃ�
	if (m_info.vecMotionInfo[m_info.nType].GetNumKey() <= 0) { return; }	// �L�[�����ݒ�

	if (m_blend.nFrame > 0)
	{ // �u�����h�t���[�����ݒ肳��Ă���ꍇ

		// �u�����h�̍X�V
		UpdateBlend();
	}
	else
	{ // �u�����h�t���[�����ݒ肳��Ă��Ȃ��ꍇ

		// ���[�V�����̍X�V
		UpdateMotion();
	}

	// �ړ��̍X�V
	UpdateMove();
}

//============================================================
//	�p�[�c���̐ݒ菈��
//============================================================
void CMotion::BindPartsData(CMultiModel **ppModel)
{
	// �����̃��f������ݒ�
	m_ppModel = ppModel;
}

//============================================================
//	���[�V�������S�ݒ菈��
//============================================================
void CMotion::SetAllInfo(const SInfo& rInfo)
{
	for (auto& rVec : rInfo.vecMotionInfo)
	{ // �ǂݍ��񂾃��[�V���������J��Ԃ�

		// ���[�V�������̒ǉ�
		AddInfo(rVec);
	}
}

//============================================================
//	���[�V�������̒ǉ�����
//============================================================
void CMotion::AddInfo(const SMotionInfo& rInfo)
{
	int nSetMotionID = m_info.GetNumMotion();	// ���[�V������ݒ肷��z��ԍ�

	// ��̗v�f���Ō���ɒǉ�
	m_info.vecMotionInfo.emplace_back();

	// �����̃��[�V��������ݒ�
	m_info.vecMotionInfo[nSetMotionID] = rInfo;

	// ���[�V�����S�̃t���[������ݒ�
	int nSubKey = (m_info.vecMotionInfo[nSetMotionID].bLoop) ? 0 : 1;		// ���[�v���Ȃ��ꍇ�Ō�̃L�[�͊܂܂Ȃ�
	int nLoop = m_info.vecMotionInfo[nSetMotionID].GetNumKey() - nSubKey;	// �J��Ԃ��������߂�
	for (int nCntKey = 0; nCntKey < nLoop; nCntKey++)
	{ // �L�[�̑������J��Ԃ�

		// �L�[�̃t���[���������Z
		m_info.vecMotionInfo[nSetMotionID].nWholeFrame += m_info.vecMotionInfo[nSetMotionID].vecKeyInfo[nCntKey].nFrame;
	}
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
//	�p�[�c���ݒ�̐ݒ菈��
//============================================================
void CMotion::SetNumParts(const int nNumParts)
{
	// �L�[�p�[�c���_�����N���A
	m_info.vecOriginKey.clear();

	// �L�[�p�[�c���_�𐶐�
	m_info.vecOriginKey.resize((size_t)nNumParts);

	// �u�����h�J�n�p�[�c�����N���A
	m_blend.vecKey.clear();

	// �u�����h�J�n�p�[�c�𐶐�
	m_blend.vecKey.resize((size_t)nNumParts);
}

//============================================================
//	�ݒ菈��
//============================================================
void CMotion::Set(const int nType, const int nBlendFrame)
{
	// �����̃��[�V�����̎�ނ�ݒ�
	m_info.nType = nType;

	// �����̃u�����h�t���[����ݒ�
	m_blend.nFrame = nBlendFrame;

	// ���[�V��������������
	m_info.nKey			 = 0;		// ���[�V�����L�[�ԍ�
	m_info.nKeyCounter	 = 0;		// ���[�V�����L�[�J�E���^�[
	m_info.nWholeCounter = 0;		// ���[�V�����S�̃J�E���^�[
	m_info.bFinish		 = false;	// ���[�V�����I����

	// �u�����h����������
	m_blend.nWholeCounter = 0;		// �u�����h�S�̃J�E���^�[

	// �p�[�c�̈ʒu�̏�����
	if (m_blend.nFrame > 0)
	{ // �u�����h�t���[�����ݒ肳��Ă���ꍇ

		for (int nCntKey = 0; nCntKey < m_info.vecMotionInfo[nType].vecKeyInfo[m_info.nKey].GetNumParts(); nCntKey++)
		{ // �p�[�c�����J��Ԃ�

			// ���݈ʒu�ƌ��݌�����ۑ�
			m_blend.vecKey[nCntKey].pos = m_ppModel[nCntKey]->GetVec3Position();
			m_blend.vecKey[nCntKey].rot = m_ppModel[nCntKey]->GetVec3Rotation();
		}
	}
	else
	{ // �u�����h�t���[�����ݒ肳��Ă��Ȃ��ꍇ

		for (int nCntKey = 0; nCntKey < m_info.vecMotionInfo[nType].vecKeyInfo[m_info.nKey].GetNumParts(); nCntKey++)
		{ // �p�[�c�����J��Ԃ�

			// �����ʒu�Ə���������ݒ�
			m_ppModel[nCntKey]->SetVec3Position(m_info.vecMotionInfo[nType].vecKeyInfo[m_info.nKey].vecKey[nCntKey].pos);
			m_ppModel[nCntKey]->SetVec3Rotation(m_info.vecMotionInfo[nType].vecKeyInfo[m_info.nKey].vecKey[nCntKey].rot);
		}
	}
}

//============================================================
//	���_�ʒu�̐ݒ菈��
//============================================================
void CMotion::SetOriginPosition(const D3DXVECTOR3& rPos, const int nParts)
{
	if (nParts > NONE_IDX && nParts < motion::MAX_PARTS)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// ���_�ʒu��ݒ�
		m_info.vecOriginKey[nParts].pos = rPos;
	}
	else { assert(false); }	// �g�p�s��
}

//============================================================
//	���_�����̐ݒ菈��
//============================================================
void CMotion::SetOriginRotation(const D3DXVECTOR3& rRot, const int nParts)
{
	if (nParts > NONE_IDX && nParts < motion::MAX_PARTS)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// ���_������ݒ�
		m_info.vecOriginKey[nParts].rot = rRot;
	}
	else { assert(false); }	// �g�p�s��
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
int CMotion::GetNumType(void)
{
	// ���[�V�����̎�ނ̑�����Ԃ�
	return m_info.GetNumMotion();
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
int CMotion::GetNumKey(const int nType)
{
	// �������[�V�����̃L�[�̑�����Ԃ�
	int nSubKey = (m_info.vecMotionInfo[m_info.GetNumMotion()].bLoop) ? 0 : 1;	// ���[�v���Ȃ��ꍇ�Ō�̃L�[�͊܂܂Ȃ�
	return m_info.vecMotionInfo[nType].GetNumKey() - nSubKey;
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
	return m_info.vecMotionInfo[nType].nWholeFrame;
}

//============================================================
//	���[�V�����L�����Z���t���[���擾����
//============================================================
int CMotion::GetCancelFrame(const int nType) const
{
	// �������[�V�����̃L�����Z���t���[����Ԃ�
	return m_info.vecMotionInfo[nType].nCancelFrame;
}

//============================================================
//	���[�V�����R���{�t���[���擾����
//============================================================
int CMotion::GetComboFrame(const int nType) const
{
	// �������[�V�����̃R���{�t���[����Ԃ�
	return m_info.vecMotionInfo[nType].nComboFrame;
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
	return m_info.vecMotionInfo[nType].bLoop;
}

//============================================================
//	�L�����Z���擾����
//============================================================
bool CMotion::IsCancel(const int nType) const
{
	if (m_info.vecMotionInfo[nType].nCancelFrame != NONE_IDX)
	{ // �L�����Z���t���[�����ݒ肳��Ă���ꍇ

		// �������[�V�����̃L�����Z���󋵂�Ԃ�
		return (m_info.nWholeCounter >= m_info.vecMotionInfo[nType].nCancelFrame);
	}
	else
	{ // �L�����Z���t���[�����ݒ肳��Ă��Ȃ��ꍇ

		// �L�����Z���s��Ԃ�
		return false;
	}
}

//============================================================
//	�R���{�擾����
//============================================================
bool CMotion::IsCombo(const int nType) const
{
	if (m_info.vecMotionInfo[nType].nComboFrame != NONE_IDX)
	{ // �R���{�t���[�����ݒ肳��Ă���ꍇ

		// �������[�V�����̃R���{�󋵂�Ԃ�
		return (m_info.nWholeCounter >= m_info.vecMotionInfo[nType].nComboFrame);
	}
	else
	{ // �R���{�t���[�����ݒ肳��Ă��Ȃ��ꍇ

		// �R���{�s��Ԃ�
		return false;
	}
}

//============================================================
//	����\���̎擾����
//============================================================
bool CMotion::IsWeaponDisp(const int nType) const
{
	// �������[�V�����̕���\����ON/OFF�󋵂�Ԃ�
	return m_info.vecMotionInfo[nType].bWeaponDisp;
}

//============================================================
//	���̍U������t���O�̎擾����
//============================================================
bool CMotion::IsLeftWeaponCollision(void)
{
	SMotionInfo *pMotionInfo = &m_info.vecMotionInfo[m_info.nType];	// ���݂̃��[�V�������
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
	SMotionInfo *pMotionInfo = &m_info.vecMotionInfo[m_info.nType];	// ���݂̃��[�V�������
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
//	���_�ʒu�̎擾����
//============================================================
D3DXVECTOR3 CMotion::GetOriginPosition(const int nParts) const
{
	if (nParts > NONE_IDX && nParts < motion::MAX_PARTS)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// ���_�ʒu��Ԃ�
		return m_info.vecOriginKey[nParts].pos;
	}

	// �C���f�b�N�X�G���[
	assert(false);
	return VEC3_ZERO;
}

//============================================================
//	���_�����̎擾����
//============================================================
D3DXVECTOR3 CMotion::GetOriginRotation(const int nParts) const
{
	if (nParts > NONE_IDX && nParts < motion::MAX_PARTS)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// ���_������Ԃ�
		return m_info.vecOriginKey[nParts].rot;
	}

	// �C���f�b�N�X�G���[
	assert(false);
	return VEC3_ZERO;
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

//============================================================
//	�ړ��̍X�V����
//============================================================
void CMotion::UpdateMove(void)
{
	if (m_pChara == nullptr) { return; }	// �I�u�W�F�N�g�L�����N�^�[���ݒ�

	// �ϐ���錾
	D3DXMATRIX  mtxChara	= m_pChara->GetMtxWorld();				// �L�����}�g���b�N�X
	D3DXVECTOR3 posSetChara	= m_pChara->GetVec3Position();			// �L�����ݒ�ʒu
	D3DXVECTOR3 posOldChara	= useful::GetMatrixPosition(mtxChara);	// �L�����ߋ��ʒu
	D3DXVECTOR3 posCurChara	= VEC3_ZERO;							// �L�������݈ʒu

	// �ړ��ʂ����߂�
	float fRate = 1.0f / (float)m_info.vecMotionInfo[m_info.nType].vecKeyInfo[m_info.nKey].nFrame;	// �L�[�t���[������
	D3DXVECTOR3 moveRate = m_info.vecMotionInfo[m_info.nType].vecKeyInfo[m_info.nKey].move * fRate;	// �t���[���ړ���

	if (m_info.vecMotionInfo[m_info.nType].vecKeyInfo[m_info.nKey].nFrame > 0)
	{ // �t���[�����ݒ肳��Ă���ꍇ

		// �ړ��ʂ��}�g���b�N�X�ɔ��f
		D3DXMATRIX mtxMove;	// �}�g���b�N�X�v�Z�p
		D3DXMatrixTranslation(&mtxMove, moveRate.x, moveRate.y, moveRate.z);
		D3DXMatrixMultiply(&mtxChara, &mtxMove, &mtxChara);

		// �ړ��ʂ�^�����}�g���b�N�X�̃��[���h���W�����߂�
		posCurChara = useful::GetMatrixPosition(mtxChara);

		// �ߋ��ƌ��݂̈ʒu����ړ��ʂ����߁A�ʒu�ɗ^����
		posSetChara += posOldChara - posCurChara;

		// �ʒu�𔽉f
		m_pChara->SetVec3Position(posSetChara);
	}
}

//============================================================
//	���[�V�����̍X�V����
//============================================================
void CMotion::UpdateMotion(void)
{
	int nType = m_info.nType;	// ���[�V�������
	int nKey  = m_info.nKey;	// ���[�V�����L�[�ԍ�

	// ���̃��[�V�����L�[�ԍ������߂�
	int nNextKey = (nKey + 1) % m_info.vecMotionInfo[nType].GetNumKey();

	// �p�[�c�̈ʒu�̍X�V
	for (int nCntKey = 0; nCntKey < m_info.vecMotionInfo[nType].vecKeyInfo[m_info.nKey].GetNumParts(); nCntKey++)
	{ // �p�[�c�����J��Ԃ�

		// �ʒu�E�����̍��������߂�
		D3DXVECTOR3 diffPos = m_info.vecMotionInfo[nType].vecKeyInfo[nNextKey].vecKey[nCntKey].pos - m_info.vecMotionInfo[nType].vecKeyInfo[nKey].vecKey[nCntKey].pos;
		D3DXVECTOR3 diffRot = m_info.vecMotionInfo[nType].vecKeyInfo[nNextKey].vecKey[nCntKey].rot - m_info.vecMotionInfo[nType].vecKeyInfo[nKey].vecKey[nCntKey].rot;
		useful::NormalizeRot(diffRot);	// ���������̐��K��

		// ���݂̃p�[�c�̈ʒu�E�������X�V
		float fRate = (float)m_info.nKeyCounter / (float)m_info.vecMotionInfo[nType].vecKeyInfo[nKey].nFrame;	// �L�[�t���[������
		m_ppModel[nCntKey]->SetVec3Position(m_info.vecMotionInfo[nType].vecKeyInfo[nKey].vecKey[nCntKey].pos + diffPos * fRate);
		m_ppModel[nCntKey]->SetVec3Rotation(m_info.vecMotionInfo[nType].vecKeyInfo[nKey].vecKey[nCntKey].rot + diffRot * fRate);
	}

	// ���[�V�����̑J�ڂ̍X�V
	if (m_info.nKeyCounter < m_info.vecMotionInfo[nType].vecKeyInfo[nKey].nFrame)
	{ // ���݂̃L�[�̍Đ����I�����Ă��Ȃ��ꍇ

		// �J�E���^�[�����Z
		m_info.nKeyCounter++;
		m_info.nWholeCounter++;
	}
	else
	{ // ���݂̃L�[�̍Đ����I�������ꍇ

		// ���̃L�[�Ɉڍs
		if (m_info.vecMotionInfo[nType].bLoop)
		{ // ���[�V���������[�v����ꍇ

			// �L�[�J�E���^�[��������
			m_info.nKeyCounter = 0;

			// �L�[�J�E���g�����Z
			m_info.nKey = (m_info.nKey + 1) % m_info.vecMotionInfo[nType].GetNumKey();	// �ő�l��0�ɖ߂�

			if (m_info.nKey == 0)
			{ // �L�[���ŏ��ɖ߂����ꍇ

				// �S�̃J�E���^�[��������
				m_info.nWholeCounter = 0;
			}
		}
		else
		{ // ���[�V���������[�v���Ȃ��ꍇ

			if (m_info.nKey < m_info.vecMotionInfo[nType].GetNumKey() - 2)
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
//	�u�����h�X�V����
//============================================================
void CMotion::UpdateBlend(void)
{
	// �p�[�c�̈ʒu�̍X�V
	for (int nCntKey = 0; nCntKey < m_info.vecMotionInfo[m_info.nType].vecKeyInfo[m_info.nKey].GetNumParts(); nCntKey++)
	{ // �p�[�c�����J��Ԃ�

		// �ʒu�E�����̍��������߂�
		D3DXVECTOR3 diffPos = m_info.vecMotionInfo[m_info.nType].vecKeyInfo[0].vecKey[nCntKey].pos - m_blend.vecKey[nCntKey].pos;
		D3DXVECTOR3 diffRot = m_info.vecMotionInfo[m_info.nType].vecKeyInfo[0].vecKey[nCntKey].rot - m_blend.vecKey[nCntKey].rot;
		useful::NormalizeRot(diffRot);	// ���������̐��K��

		// ���݂̃p�[�c�̈ʒu�E�������X�V
		float fRate = (float)m_blend.nWholeCounter / (float)m_blend.nFrame;	// �L�[�t���[������
		m_ppModel[nCntKey]->SetVec3Position(m_blend.vecKey[nCntKey].pos + diffPos * fRate);
		m_ppModel[nCntKey]->SetVec3Rotation(m_blend.vecKey[nCntKey].rot + diffRot * fRate);
	}

	// ���[�V�����̑J�ڂ̍X�V
	if (m_blend.nWholeCounter < m_blend.nFrame)
	{ // �u�����h�̍Đ����I�����Ă��Ȃ��ꍇ

		// �J�E���^�[�����Z
		m_blend.nWholeCounter++;
	}
	else
	{ // �u�����h�̍Đ����I�������ꍇ

		// �u�����h�Đ��t���[������������
		m_blend.nFrame = 0;

		// �u�����h�S�̃J�E���^�[��������
		m_blend.nWholeCounter = 0;
	}
}
