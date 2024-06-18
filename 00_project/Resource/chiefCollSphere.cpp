#if 1
//============================================================
//
//	�~���蓝������ [chiefCollSphere.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "chiefCollSphere.h"
#include "collisionSphere.h"

//************************************************************
//	�e�N���X [CChiefCollSphere] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CChiefCollSphere::CChiefCollSphere()
{
	// ������z����N���A
	m_vecColl.clear();
}

//============================================================
//	�f�X�g���N�^
//============================================================
CChiefCollSphere::~CChiefCollSphere()
{

}

//============================================================
//	����������
//============================================================
HRESULT CChiefCollSphere::Init(void)
{
	// ������z���������
	m_vecColl.clear();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CChiefCollSphere::Uninit(void)
{
	// ������z����N���A
	m_vecColl.clear();
}

//============================================================
//	�I�t�Z�b�g�X�V����
//============================================================
void CChiefCollSphere::OffSet(const D3DXMATRIX& rMtx)
{
	for (auto& rSphere : m_vecColl)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		// �I�t�Z�b�g�̍X�V
		rSphere->OffSet(rMtx);
	}
}

//============================================================
//	����ʒu�̎擾����
//============================================================
D3DXVECTOR3 CChiefCollSphere::GetWorldPosition(const int nCollID) const
{
	auto info = m_vecColl.begin();	// �z��̐擪�C�e���[�^�[

	// �C�e���[�^�[���C���f�b�N�X��������
	info += nCollID;

	// ����ʒu��Ԃ�
	return (*info)->GetPos();
}

#if 0
//============================================================
//	������̐ݒ菈��
//============================================================
void CChiefCollSphere::SetInfo(const SInfo& rInfo, const int nID)
{
	auto info = m_coll.begin();	// �z��̐擪�C�e���[�^�[
	info += nID;	// �C�e���[�^�[���C���f�b�N�X��������

	// �C�e���[�^�[�̒��g��ݒ�
	*info = rInfo;
}

//============================================================
//	������̎擾����
//============================================================
CChiefCollSphere::SInfo CChiefCollSphere::GetInfo(const int nID) const
{
	auto info = m_coll.begin();	// �z��̐擪�C�e���[�^�[
	info += nID;	// �C�e���[�^�[���C���f�b�N�X��������

	// �C�e���[�^�[�̒��g��Ԃ�
	return *info;
}

//============================================================
//	�z��̐ݒ菈��
//============================================================
void CChiefCollSphere::SetVector(const std::vector<SInfo>& rVector)
{
	// �z���ݒ�
	m_coll = rVector;
}

//============================================================
//	�z��̎擾����
//============================================================
std::vector<CChiefCollSphere::SInfo> CChiefCollSphere::GetVector(void) const
{
	// �z���Ԃ�
	return m_coll;
}
#endif

//============================================================
//	����̒ǉ�����
//============================================================
HRESULT CChiefCollSphere::AddColl
(
	const D3DXVECTOR3& rOffset,	// ����ʒu�I�t�Z�b�g
	const float fRadius			// ���蔼�a
)
{
	// ����ݒ�
	CCollisionSphere *pColl = CCollisionSphere::Create
	( // ����
		VEC3_ZERO,	// �ʒu
		rOffset,	// �I�t�Z�b�g
		fRadius		// ���a
	);
	if (pColl == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �z��Ō���ɒǉ�����ݒ�
	m_vecColl.push_back(pColl);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	����̍폜����
//============================================================
void CChiefCollSphere::SubColl(const int nCollID)
{
	auto info = m_vecColl.begin();	// �z��̐擪�C�e���[�^�[

	// �C�e���[�^�[���C���f�b�N�X��������
	info += nCollID;

	// ����̏I��
	SAFE_UNINIT(*info);

	// �z��̗v�f����폜
	m_vecColl.erase(info);
}

//============================================================
//	��������
//============================================================
CChiefCollSphere *CChiefCollSphere::Create(void)
{
	// �~���蓝���̐���
	CChiefCollSphere *pChiefCollSphere = new CChiefCollSphere;
	if (pChiefCollSphere == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �~���蓝���̏�����
		if (FAILED(pChiefCollSphere->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �~���蓝���̔j��
			SAFE_DELETE(pChiefCollSphere);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pChiefCollSphere;
	}
}

//============================================================
//	�j������
//============================================================
void CChiefCollSphere::Release(CChiefCollSphere *&prChiefCollSphere)
{
	// �~���蓝���̏I��
	assert(prChiefCollSphere != nullptr);
	prChiefCollSphere->Uninit();

	// �������J��
	SAFE_DELETE(prChiefCollSphere);
}
#endif
