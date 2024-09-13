//============================================================
//
//	���S��ʃw�b�_�[ [sceneGame.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_LOGO_H_
#define _SCENE_LOGO_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CLogoManager;	// ���S�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���S��ʃN���X
class CSceneLogo : public CScene
{
public:
	// �R���X�g���N�^
	explicit CSceneLogo(const EMode mode);

	// �f�X�g���N�^
	~CSceneLogo() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V

	// �ÓI�����o�֐�
	static CLogoManager* GetLogoManager(void);	// ���S�}�l�[�W���[�擾

private:

	// �ÓI�����o�ϐ�
	static CLogoManager* m_pLogoManager;		// ���S�}�l�[�W���[
};

#endif	// _SCENE_LOGO_H_
