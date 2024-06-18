#if 1
//============================================================
//
//	�~���蓝���w�b�_�[ [chiefCollSphere.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _CHIEF_COLL_SPHERE_H_
#define _CHIEF_COLL_SPHERE_H_

//************************************************************
//	�O���錾
//************************************************************
class CCollisionSphere;	// �X�t�B�A�̓����蔻��N���X

//************************************************************
//	�N���X��`
//************************************************************
// �~���蓝���N���X
class CChiefCollSphere
{
public:
	// �R���X�g���N�^
	CChiefCollSphere();

	// �f�X�g���N�^
	~CChiefCollSphere();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void OffSet(const D3DXMATRIX& rMtx);	// �I�t�Z�b�g�X�V
	D3DXVECTOR3 GetWorldPosition(const int nCollID) const;	// ����ʒu�擾

#if 0
	void SetInfo(const SInfo& rInfo, const int nID);	// ������ݒ�
	SInfo GetInfo(const int nID) const;					// ������擾
	void SetVector(const std::vector<SInfo>& rVector);	// �z��ݒ�
	std::vector<SInfo> GetVector(void) const;			// �z��擾
#endif

	HRESULT AddColl	// ����ǉ�
	( // ����
		const D3DXVECTOR3& rOffset,	// ����ʒu�I�t�Z�b�g
		const float fRadius			// ���蔼�a
	);
	void SubColl(const int nCollID);	// ����폜

	// �ÓI�����o�֐�
	static CChiefCollSphere *Create(void);	// ����
	static void Release(CChiefCollSphere *&prChiefCollSphere);	// �j��

private:
	// �����o�ϐ�
	std::vector<CCollisionSphere*> m_vecColl;	// ������z��
};

#endif	// _CHIEF_COLL_SPHERE_H_
#endif
