//============================================================
//
// �W�����v��w�b�_�[ [gimmick_jumptable.h]
// Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GIMMICK_JUMPTABLE_H_
#define _GIMMICK_JUMPTABLE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gimmick.h"

//************************************************************
//	�N���X��`
//************************************************************
// �W�����v��N���X
class CGimmickJumpTable : public CGimmick
{
public:

	// �R���X�g���N�^
	CGimmickJumpTable();

	// �f�X�g���N�^
	~CGimmickJumpTable() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

private:

};

#endif	// _GIMMICK_H_