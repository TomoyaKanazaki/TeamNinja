//============================================================
//
//	�G�̓��w�b�_�[ [enemy_katana.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_KATANA_H_
#define _ENEMY_KATANA_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy_item.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�̓��N���X
class CEnemyKatana : public CEnemyItem
{
public:

	// �R���X�g���N�^
	CEnemyKatana();

	// �f�X�g���N�^
	~CEnemyKatana() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

private:

};

#endif	// _ACTOR_H_
