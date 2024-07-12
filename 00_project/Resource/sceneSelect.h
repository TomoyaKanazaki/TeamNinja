//============================================================
//
//	�Z���N�g��ʃw�b�_�[ [sceneSelect.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_SELECT_H_
#define _SCENE_SELECT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CSelectManager;	// �Z���N�g�}�l�[�W���[�N���X
class CEditManager;		// �G�f�B�b�g�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �Z���N�g��ʃN���X
class CSceneSelect : public CScene
{
public:
	// �R���X�g���N�^
	explicit CSceneSelect(const EMode mode);

	// �f�X�g���N�^
	~CSceneSelect() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V

	// �ÓI�����o�֐�
	static CSelectManager	*GetSelectManager(void);	// �Z���N�g�}�l�[�W���[�擾
	static CEditManager		*GetEditManager(void);		// �G�f�B�^�[�}�l�[�W���[�擾

private:
	// �ÓI�����o�ϐ�
	static CSelectManager	*m_pSelectManager;	// �Z���N�g�}�l�[�W���[
	static CEditManager		*m_pEditManager;	// �G�f�B�^�[�}�l�[�W���[
};

#endif	// _SCENE_SELECT_H_
