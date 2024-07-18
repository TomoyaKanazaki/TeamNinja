//#if 1
////============================================================
////
////	�G�f�B�b�g�U���G�w�b�_�[ [editEnemyAttack.h]
////	Author�F��������
////
////============================================================
////************************************************************
////	��d�C���N���[�h�h�~
////************************************************************
//#ifndef _EDIT_ENEMY_ATTACK_H_
//#define _EDIT_ENEMY_ATTACK_H_
//
////************************************************************
////	�C���N���[�h�t�@�C��
////************************************************************
//#include "editorObject.h"
//#include "enemyAttack.h"
//
////************************************************************
////	�N���X��`
////************************************************************
//// �G�f�B�b�g�U���G�N���X
//class CEditEnemyAttack : public CEditorObject
//{
//public:
//
//	// �i�r�Q�[�V�����̎��
//	enum ENavType
//	{
//		NAVTYPE_RANDOM = 0,	// �����_���i�r
//		NAVTYPE_STREET,		// ����i�r
//		NAVTYPE_MAX			// ���̗񋓌^�̑���
//	};
//
//	// �R���X�g���N�^
//	CEditEnemyAttack(CEditStage* pEditor);
//
//	// �f�X�g���N�^
//	~CEditEnemyAttack() override;
//
//	// �G���\����
//	struct SInfo
//	{
//		CEnemyAttack::EType type;	// ���
//		float fMoveWidth;			// �ړ��͈͕�
//		float fMoveDepth;			// �ړ��͈͉��s
//		float fChaseWidth;			// �ǐՔ͈͕�
//		float fChaseDepth;			// �ǐՔ͈͉��s
//	};
//
//	// �I�[�o�[���C�h�֐�
//	HRESULT Init(void) override;	// ������
//	void Uninit(void) override;		// �I��
//	void Update(void) override;		// �X�V
//	HRESULT Save(void) override;	// �ۑ�
//	bool IsSave(void) override;		// �ۑ��󋵎擾
//	void SaveInfo(void) override;	// ���ۑ�
//	void LoadInfo(void) override;	// ���Ǎ�
//	void DrawDebugControl(void) override;	// ����\���`��
//	void DrawDebugInfo(void) override;		// ���\���`��
//
//private:
//
//	// �����o�֐�
//	void ChangeType(void);		// ��ލX�V
//	void CreateActor(void);		// �G����
//	void ReleaseActor(void);	// �G�j��
//	void DeleteCollisionActor(const bool bRelase);	// �G�̍폜����
//	void InitAllColorActor(void);					// �G�̐F�S������
//
//	// �����o�ϐ�
//	CEnemyAttack* m_pEnemy;	// �G�̏��
//	SInfo m_infoCreate;		// �G�z�u���
//	bool m_bSave;			// �ۑ���
//};
//
//#endif	// _EDIT_ACTOR_H_
//#endif
