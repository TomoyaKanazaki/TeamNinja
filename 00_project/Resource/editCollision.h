//============================================================
//
//	�G�f�B�b�g�R���W�����w�b�_�[ [editCollision.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_COLLISION_H_
#define _EDIT_COLLISION_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editor.h"
#include "actor.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditManager;			// �G�f�B�b�g�}�l�[�W���[�N���X
class CEditorCollShape;		// �G�f�B�^�[�R���W�����V�F�C�v�N���X
class CEditCollCube;		// �L���[�u�����蔻��
class CEditCollCylinder;	// �V�����_�[�����蔻��
class CEditCollSphere;		// �X�t�B�A�����蔻��

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�X�e�[�W�N���X
class CEditCollision : public CEditor
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_CUBE = 0,		// �L���[�u
		TYPE_CYLINDER,		// �V�����_�[
		TYPE_SPHERE,		// �X�t�B�A
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEditCollision(CEditManager* pEditManager);

	// �f�X�g���N�^
	virtual ~CEditCollision();

	// �����o�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	HRESULT Save(void) override;	// �ۑ�
	bool IsSave(void) override;		// �ۑ��󋵎擾
	void SaveInfo(void) override;	// ���ۑ�
	void LoadInfo(void) override;	// ���Ǎ�
	void DrawDebugControl(void) override;	// ����\���`��
	void DrawDebugInfo(void) override;		// ���\���`��

private:

	// �����o�֐�
	void ChangeObjectType(void);	// �I�u�W�F�N�g�^�C�v�ύX

	void InitAllColorCollCube(void);		// �L���[�u�̐F�S������
	void InitAllColorCollCylinder(void);	// �V�����_�[�̐F�S������
	void InitAllColorCollSphere(void);		// �X�t�B�A�̐F�S������

	// �����o�ϐ�
	CEditorCollShape* m_pEditor;	// �G�f�B�^�[���
	CActor* m_pActor;				// �A�N�^�[�̏��
	CEditCollision::EType m_type;	// �R���W�����^�C�v
	CActor::EType m_actorType;		// �A�N�^�[�^�C�v
	std::vector<CEditCollCube> m_cube;			// �L���[�u�̉ϒ��z��
	std::vector<CEditCollCylinder> m_cylinder;	// �V�����_�[�̉ϒ��z��
	std::vector<CEditCollSphere> m_sphere;		// �X�t�B�A�̉ϒ��z��

};

#endif	// _EDIT_STAGE_H_
