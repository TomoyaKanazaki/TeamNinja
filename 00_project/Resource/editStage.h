//============================================================
//
//	�G�f�B�b�g�X�e�[�W�w�b�_�[ [editStage.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_STAGE_H_
#define _EDIT_STAGE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editor.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditManager;		// �G�f�B�b�g�}�l�[�W���[�N���X
class CEditorObject;	// �G�f�B�^�[�I�u�W�F�N�g�N���X

//************************************************************
//	�萔�錾
//************************************************************
namespace editstage
{
	const float SIZE = 50.0f;	// ��u���b�N�̑傫��
}

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�X�e�[�W�N���X
class CEditStage : public CEditor
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_FIELD = 0,		// �n��
		TYPE_WALL,			// ��
		TYPE_ACTOR,			// �A�N�^�[
		TYPE_CHECKPOINT,	// �`�F�b�N�|�C���g
		TYPE_GOALPOINT,		// �S�[���|�C���g
		TYPE_GIMMICK,		// �M�~�b�N
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEditStage(CEditManager *pEditManager);

	// �f�X�g���N�^
	virtual ~CEditStage();

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

	void UpdatePosition(void);	// �ʒu�X�V
	void UpdateRotation(void);	// �����X�V
	void SetVec3Position(const D3DXVECTOR3 pos)	{ m_pos = pos; }		// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const		{ return m_pos; }		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3 rot) { m_rot = rot; useful::NormalizeRot(m_rot); }		// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const		{ return m_rot; }		// �����擾
	void SetAngle(const EAngle angle)			{ m_angle = angle; }	// �����ݒ�
	EAngle GetAngle(void) const					{ return m_angle; }		// �����擾

private:
	// �����o�֐�
	void ChangeObjectType(void);	// �I�u�W�F�N�g�^�C�v�ύX

	// �����o�ϐ�
	CEditorObject *m_pEditor;	// �G�f�B�^�[���
	CEditStage::EType m_type;	// �I�u�W�F�N�g�^�C�v
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_rot;	// ����
	EAngle m_angle;		// �p�x
};

#endif	// _EDIT_STAGE_H_
