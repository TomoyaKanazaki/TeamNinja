//============================================================
//
//	�G�f�B�^�[�w�b�_�[ [editor.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDITOR_H_
#define _EDITOR_H_

//************************************************************
//	�O���錾
//************************************************************
class CEditManager;	// �G�f�B�b�g�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�^�[�N���X
class CEditor
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_STAGE = 0,	// �X�e�[�W
		TYPE_COLLISION,	// �����蔻��
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEditor(CEditManager *pEditManager);

	// �f�X�g���N�^
	virtual ~CEditor();

	// �������z�֐�
	virtual HRESULT Init(void)	= 0;	// ������
	virtual void Uninit(void)	= 0;	// �I��
	virtual void Update(void)	= 0;	// �X�V
	virtual HRESULT Save(void)	= 0;	// �ۑ�
	virtual bool IsSave(void)	= 0;	// �ۑ��󋵎擾
	virtual void SaveInfo(void)	= 0;	// ���ۑ�
	virtual void LoadInfo(void)	= 0;	// ���Ǎ�
	virtual void DrawDebugControl(void)	= 0;	// ����\���`��
	virtual void DrawDebugInfo(void)	= 0;	// ���\���`��

	// �ÓI�����o�֐�
	static CEditor *Create(CEditManager *pEditManager, EType type);	// ����
	static void Release(CEditor *&prEditor);	// �j��

	// �����o�֐�
	CEditManager *GetPtrEditManager(void) const;	// �G�f�B�b�g�}�l�[�W���[�擾

private:
	// �����o�ϐ�
	CEditManager *m_pEditManager;	// �G�f�B�b�g�}�l�[�W���[
};

#endif	// _EDITOR_H_
