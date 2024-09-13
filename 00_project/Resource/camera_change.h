#pragma once
//===========================================
//
//  �J���������ύX����N���X(camera_change.h)
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
		ROTATION_HALFUP, // ������Ə�
		ROTATION_MAX
	};

	// �����o�֐�
	CCameraChanger();
	~CCameraChanger();

	HRESULT Init() override; // ����������
	void Uninit() override; // �I������
	void Update(const float fDeltaTime) override; // �X�V����
	void Draw(CShader* pShader = nullptr) override; // �`�揈��

	bool CollChange(const D3DXVECTOR3& pos, const D3DXVECTOR3& size) const; // �q�b�g����̎擾
	float GetDirection() const; // �����̎擾
	float GetRotation() const; // �p�x�̎擾
	EDirection GetDir() const { return m_eDir; }
	ERotation GetRot() const { return m_eRot; }
	void SetDirection(const EDirection dir) { m_eDir = dir; }
	void SetRotation(const ERotation rot) { m_eRot = rot; }

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
	EDirection m_eDir; // �J��������
	ERotation m_eRot; // �J�����p�x
	CListManager<CCameraChanger>::AIterator m_iterator; // �C�e���[�^�[

	// �ÓI�����o�ϐ�
	static CListManager<CCameraChanger>* m_pList; // �I�u�W�F�N�g���X�g
};
