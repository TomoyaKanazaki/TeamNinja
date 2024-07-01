#pragma once
//===========================================
//
//  �J���������ύX����N���X(caera_change.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "objectMeshCube.h"

//===========================================
//  �N���X��`
//===========================================
class CCameraChanger : public CObjectMeshCube
{
public:

	// ������
	enum EDirection
	{
		DIRECTION_FRONT = 0, // ����
		DIRECTION_BACK, // ���
		DIRECTION_LEFT, // ��
		DIRECTION_RIGHT, // �E
		DIRECTION_MAX
	};

	// �p�x��
	enum ERotation
	{
		ROTATION_DEFAULT = 0, // �f�t�H���g�̊p�x
		ROTATION_UP, // ��
		ROTATION_DOWN, // ��
		ROTATION_MAX
	};

	// �����o�֐�
	CCameraChanger();
	~CCameraChanger();

	HRESULT Init() override; // ����������
	void Uninit() override; // �I������
	void Update(const float fDeltaTime) override; // �X�V����
	void Draw(CShader* pShader = nullptr) override; // �`�揈��

	bool GetChange() const { return m_bChange; } // �t���O�̎擾

	// �ÓI�����o�֐�
	static CCameraChanger* Create // ��������
	(
		const D3DXVECTOR3& rPos, // ���S���W
		const D3DXVECTOR3& rSize, // �傫��
		const EDirection Dir = DIRECTION_FRONT, // �J��������
		const ERotation Rot = ROTATION_DEFAULT // �J�����p�x
	);
	static CListManager<CCameraChanger>* GetList(void);	// ���X�g�擾

private:

	// �����o�ϐ�
	bool m_bChange; // �J���������ύX�t���O
	EDirection m_eDir; // �J��������
	ERotation m_eRot; // �J�����p�x
	CListManager<CCameraChanger>::AIterator m_iterator; // �C�e���[�^�[

	// �ÓI�����o�ϐ�
	static CListManager<CCameraChanger>* m_pList; // �I�u�W�F�N�g���X�g
};
