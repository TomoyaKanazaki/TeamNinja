#pragma once
//=========================================
//
//  �ݒu�M�~�b�N (gimmick_post.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "gimmick_action.h"

//=========================================
//  �N���X��`
//=========================================
class CGimmickPost : public CGimmickAction
{
public:

	// �����o�֐�
	CGimmickPost();
	~CGimmickPost() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

	bool IsSet() { return m_bPost; } // �ݒu�t���O�̎擾

private:

	// �����o�ϐ�
	bool m_bPost; // �ݒu�t���O

};
