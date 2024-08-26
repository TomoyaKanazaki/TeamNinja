#pragma once
//=========================================
//
//  �ݒu�M�~�b�N (gimmick_post.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "gimmick_action.h"

//=========================================
//  �O���錾
//=========================================
class CObjectMeshCube;

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
	void SetVec3Position(const D3DXVECTOR3& rPos) override;
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;
	D3DXVECTOR3 CalcWaitPoint(const int Idx, const CPlayerClone* pClone) override;	// �e���g���̑ҋ@�ʒu���Z�o
	D3DXVECTOR3 CalcWaitRotation(const int Idx, const CPlayerClone* pClone) override;	// �e���g���̑ҋ@�������Z�o

	bool IsSet() override { return IsActive(); } // �ݒu�t���O�̎擾

	bool CollisionPlayer() override; // �v���C���[�Ƃ̓����蔻��

private:
	// �����o�֐�
	void SetButtonSizing(void);	// �{�^���傫���ݒ�
	void ChangeColor(); // �{�^���̐F�ύX����

	// �����o�ϐ�
	CObjectMeshCube* m_pButton;	// �{�^��
	CObjectMeshCube* m_pEdge;	// ��
	float m_fButtonHeight;		// �{�^���c�I�t�Z�b�g
};
