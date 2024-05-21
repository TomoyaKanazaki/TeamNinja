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
	CEditStage(CEditManager *pEditManager);

	// �f�X�g���N�^
	virtual ~CEditStage();

	// �������z�֐�
	virtual void SaveInfo(void)		= 0;	// ���ۑ�
	virtual void LoadInfo(void)		= 0;	// ���Ǎ�
	virtual void Save(FILE *pFile)	= 0;	// �ۑ�

	// ���z�֐�
	virtual HRESULT Init(void);	// ������
	virtual void Uninit(void);	// �I��
	virtual void Update(void);	// �X�V
	virtual void DrawDebugControl(void);	// ����\���`��
	virtual void DrawDebugInfo(void);		// ���\���`��

	// �ÓI�����o�֐�
	static CEditStage *Create(CEditManager *pEditManager, EType type);	// ����
	static void Release(CEditStage *&prEditStage);	// �j��

	// �����o�֐�
	CEditManager *GetPtrEditManager(void) const;	// �G�f�B�b�g�}�l�[�W���[�擾
	D3DXVECTOR3 GetVec3Position(void) const { return m_pos; }	// �ʒu�擾

private:
	// �����o�֐�
	void UpdateChangeMove(void);	// �ړ��ʍX�V
	void UpdatePosition(void);		// �ʒu�X�V

	// �����o�ϐ�
	CEditManager *m_pEditManager;	// �G�f�B�b�g�}�l�[�W���[
	float m_fMove;		// �ʒu�ړ���
	D3DXVECTOR3 m_pos;	// �ʒu
};

#endif	// _EDIT_STAGE_H_
