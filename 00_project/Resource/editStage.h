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
//	�O���錾
//************************************************************
class CEditManager;	// �G�f�B�b�g�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�X�e�[�W�N���X
class CEditStage
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_FIELD = 0,	// �n��
		TYPE_WALL,		// ��
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEditStage();

	// �f�X�g���N�^
	virtual ~CEditStage();

	// �������z�֐�
	virtual HRESULT Init(void)	= 0;	// ������
	virtual void Uninit(void)	= 0;	// �I��
	virtual void Update(void)	= 0;	// �X�V
	virtual void SaveInfo(void)	= 0;	// ���ۑ�
	virtual void LoadInfo(void)	= 0;	// ���Ǎ�
	virtual void DrawDebugControl(void)	= 0;	// ����\���`��
	virtual void DrawDebugInfo(void)	= 0;	// ���\���`��
	virtual void Save(FILE *pFile)		= 0;	// �ۑ�

	// �ÓI�����o�֐�
	static CEditStage *Create(CEditManager *pEditManager, EType type);	// ����
	static void Release(CEditStage *&prEditStage);	// �j��

	// �����o�֐�
	const CEditManager *GetPtrEditManager(void) const;	// �G�f�B�b�g�}�l�[�W���[�擾

private:
	// �����o�ϐ�
	const CEditManager *m_pEditManager;	// �G�f�B�b�g�}�l�[�W���[
	CEditStage *m_pStage;	// �X�e�[�W�G�f�B�^�[
};

#endif	// _EDIT_STAGE_H_
