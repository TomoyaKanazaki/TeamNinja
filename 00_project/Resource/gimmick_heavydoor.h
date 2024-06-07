//============================================================
//
// �d�����w�b�_�[ [gimmick_heavydoor.h]
// Author�F����������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GIMMICK_HEAVYDOOR_H_
#define _GIMMICK_HEAVYDOOR_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gimmick_action.h"

//************************************************************
//	�N���X��`
//************************************************************
class CGimmickHeavyDoor : public CGimmickAction
{
public:
	CGimmickHeavyDoor();
	~CGimmickHeavyDoor();

	HRESULT Init(void);	// ������
	void Uninit(void);		// �I��
	void Update(const float fDeltaTime);	// �X�V
	void Draw(CShader* pShader = nullptr);	// �`��

private:
	void PushDoor(void);		// �h�A����

};

#endif
