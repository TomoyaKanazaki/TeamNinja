#if 1
//============================================================
//
//	�G�f�B�b�g�R���W�����|���S���w�b�_�[ [editCollPolygon.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_COLL_POLYGON_H_
#define _EDIT_COLL_POLYGON_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editorCollShape.h"
#include "collisionPolygon.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�R���W�����|���S���N���X
class CEditCollPolygon : public CEditorCollShape
{
public:
	// �R���X�g���N�^
	CEditCollPolygon(const int nIdx);

	// �f�X�g���N�^
	~CEditCollPolygon() override;

	// �|���S�����\����
	struct SInfo
	{
		D3DXVECTOR3 rot;	// ����
		D3DXVECTOR3 size;	// �T�C�Y
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	bool IsSave(void) override;		// �ۑ��󋵎擾
	void SaveInfo(void) override;	// ���ۑ�
	void LoadInfo(void) override;	// ���Ǎ�
	void DrawDebugControl(void) override;	// ����\���`��
	void DrawDebugInfo(void) override;		// ���\���`��
	void Create(void) override;		//��������

	// �Z�b�g�E�Q�b�g�֌W
	CCollisionPolygon* GetPolygon(void) const { return m_pPolygon; }	// �|���S���̎擾����
	SInfo GetInfo(void) const { return m_infoCreate; }					// ���̎擾����

private:
	// �I�[�o�[���C�h�֐�
	void UpdateOffset(void) override;		// �I�t�Z�b�g���W�X�V

	// �����o�֐�
	void UpdateRot(void);			// �����X�V
	void UpdateSize(void);		// �傫���X�V

	// �����o�ϐ�
	CCollisionPolygon* m_pPolygon;	// �|���S�����
	SInfo m_infoCreate;	// �|���S���z�u���
	bool m_bSave;		// �ۑ���
};

#endif	// _EDIT_COLL_SPHERE_H_
#endif
