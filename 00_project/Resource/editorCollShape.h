//============================================================
//
//	�����蔻��G�f�B�^�[�w�b�_�[ [editorCollShape.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDITOR_COLL_SHAPE_H_
#define _EDITOR_COLL_SHAPE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editor.h"
#include "editCollision.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditManager;			// �G�f�B�b�g�}�l�[�W���[�N���X
class CActor;				// �A�N�^�[
class CEditCollCube;		// �L���[�u�����蔻��
class CEditCollCylinder;	// �V�����_�[�����蔻��
class CEditCollSphere;		// �X�t�B�A�����蔻��

//************************************************************
//	�N���X��`
//************************************************************
// �����蔻��G�f�B�^�[�N���X
class CEditorCollShape
{
public:
	// �R���X�g���N�^
	CEditorCollShape(const int nIdx);

	// �f�X�g���N�^
	virtual ~CEditorCollShape();

	// �������z�֐�
	virtual bool IsSave(void) = 0;		// �ۑ��󋵎擾
	virtual void SaveInfo(void) = 0;	// ���ۑ�
	virtual void LoadInfo(void) = 0;	// ���Ǎ�

	virtual void Create(void) = 0;		// ��������

	// ���z�֐�
	virtual HRESULT Init(void);	// ������
	virtual void Uninit(void);	// �I��
	virtual void Update(void);	// �X�V
	virtual void DrawDebugControl(void);	// ����\���`��
	virtual void DrawDebugInfo(void);		// ���\���`��

	// �ÓI�����o�֐�
	static CEditorCollShape* Create(CEditCollision::EType type, const int nIdx);	// ����
	static void Release(CEditorCollShape*& prEditorObject);			// �j��

	// �����o�֐�
	D3DXVECTOR3 GetVec3OffSet(void) const	{ return m_offset; }	// �I�t�Z�b�g���W�擾

protected:
	// ���z�֐�
	virtual void UpdateOffset(void);	// �I�t�Z�b�g���W�X�V

private:

	// �����o�ϐ�
	D3DXVECTOR3 m_offset;				// �I�t�Z�b�g���W
};

#endif	// _EDITOR_COLL_SHAPE_H_
