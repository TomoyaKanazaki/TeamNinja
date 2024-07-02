#pragma once
//=========================================
//
//  ���̃M�~�b�N(gimmick_bridge.h)
//  Author : Tomoya Kanazaki
//
//=========================================
#include "gimmick_action.h"

//===========================================
//  �O���錾
//===========================================
class CField;

//=========================================
//  �N���X��`
//=========================================
class CGimmickBridge : public CGimmickAction
{
public:

	// �����o�֐�
	CGimmickBridge();
	~CGimmickBridge() override;

	HRESULT Init(void) override; // ����������
	void Uninit(void) override; // �I������
	void Update(const float fDeltaTime) override; // �X�V����
	void Draw(CShader* pShader = nullptr) override; // �`�揈��
	D3DXVECTOR3 CalcWaitPoint(const int Idx) override; // �e���g���̑ҋ@�ʒu���Z�o
	D3DXVECTOR3 CalcWaitRotation(const int Idx, const D3DXVECTOR3& rPos) override; // �e���g���̑ҋ@�������Z�o

private:

	// �����o�֐�
	void CalcConectPoint(); // ���̒[�̌v�Z����
	void SetWait(int nIdx) { m_nIdxWait = nIdx; } // �ҋ@���S�̃C���f�b�N�X�ݒ�
	void Active(); // �A�N�e�B�u��Ԃ̏���

	// �����o�ϐ�
	bool m_bSet; // �ݒ�ς݃t���O
	D3DXVECTOR3 m_ConectPoint[2]; // ���̒[
	D3DXVECTOR3 m_vecToWait; // ���S���W����ҋ@���S�ւ̃x�N�g��(�P�ʃx�N�g��)
	int m_nIdxWait; // �ҋ@���S�̃C���f�b�N�X
	CField* m_pField; // �A�N�e�B�u��Ԃł̑���
};
