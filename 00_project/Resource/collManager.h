//============================================================
//
//	�����蔻��}�l�[�W���[�w�b�_�[ [collManager.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _COLL_MANAGER_H_
#define _COLL_MANAGER_H_

//************************************************************
// �C���N���[�h�t�@�C��
//************************************************************
#include "actor.h"

//************************************************************
//	�N���X��`
//************************************************************
// �����蔻��}�l�[�W���[�N���X
class CCollManager
{
public:

	// �L���[�u�̓����蔻��̏��\����
	struct SCollCube
	{
		D3DXVECTOR3 offset;	// �I�t�Z�b�g���W
		float fWidth;		// ��
		float fHeight;		// ����
		float fDepth;		// ���s
	};

	// �V�����_�[�̓����蔻��̏��\����
	struct SCollCylinder
	{
		D3DXVECTOR3 offset;	// �I�t�Z�b�g���W
		float fRadius;		// ���a
		float fHeight;		// ����
	};

	// �X�t�B�A�̓����蔻��̏��\����
	struct SCollSphere
	{
		D3DXVECTOR3 offset;	// �I�t�Z�b�g���W
		float fRadius;		// ���a
	};

	// �|���S���̓����蔻��̏��\����
	struct SCollPolygon
	{
		D3DXVECTOR3 offset;	// �I�t�Z�b�g���W
		D3DXVECTOR3 rot;	// ����
		D3DXVECTOR3 size;	// �T�C�Y
	};

	// �����蔻��֌W�̍\����
	struct SCollision
	{
		SCollision()			// �R���X�g���N�^
		{
			m_cube = {};			// �L���[�u�̏��z��
			m_cylinder = {};		// �V�����_�[�̏��z��
			m_sphere = {};			// �X�t�B�A�̏��z��
			m_polygon = {};			// �|���S���̏��z��
		}
		SCollision				// �R���X�g���N�^(�I�[�o�[���[�h)
		(
			std::vector<SCollCube> cube,
			std::vector<SCollCylinder> cylinder,
			std::vector<SCollSphere> sphere,
			std::vector<SCollPolygon> polygon
		)
		{
			m_cube = cube;			// �L���[�u�̏��z��
			m_cylinder = cylinder;	// �V�����_�[�̏��z��
			m_sphere = sphere;		// �X�t�B�A�̏��z��
			m_polygon = polygon;	// �|���S���̏��z��
		}

		std::vector<SCollCube> m_cube;			// �L���[�u�̏��z��
		std::vector<SCollCylinder> m_cylinder;	// �V�����_�[�̏��z��
		std::vector<SCollSphere> m_sphere;		// �X�t�B�A�̏��z��
		std::vector<SCollPolygon> m_polygon;	// �|���S���̏��z��
	};

	// �R���X�g���N�^
	CCollManager();

	// �f�X�g���N�^
	~CCollManager();

	// �����o�֐�
	void Uninit(void);	// �I��

	HRESULT Load(void);	// ���[�h����

	void SetCollInfo(const CActor::EType type, const SCollision coll);	// ���̎擾����
	SCollision GetCollInfo(const CActor::EType type);	// ���̎擾����

	// �ÓI�����o�֐�
	static CCollManager* Create(void);		// ����

private:

	// �����o�ϐ�
	SCollision m_aCollInfo[CActor::TYPE_MAX];			// �����蔻��̏��
};

#endif	// _DELTATIME_H_
