//============================================================
//
// ��э~��w�b�_�[ [gimmick_jumpoff.h]
// Author�F����������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GIMMICK_JUMPOFF_H_
#define _GIMMICK_JUMPOFF_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gimmick_action.h"

//************************************************************
//	�N���X��`
//************************************************************
class CGimmickJumpOff : public CGimmickAction
{
public:
	CGimmickJumpOff();
	~CGimmickJumpOff();

	HRESULT Init(void);	// ������
	void Uninit(void);		// �I��
	void Update(const float fDeltaTime);	// �X�V
	void Draw(CShader* pShader = nullptr);	// �`��

private:

};

#endif
