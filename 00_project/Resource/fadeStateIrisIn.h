//============================================================
//
//	�A�C���X�C����ԃw�b�_�[ [fadeStateIrisIn.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FADE_STATE_IRISIN_H_
#define _FADE_STATE_IRISIN_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fadeState.h"

//************************************************************
//	�N���X��`
//************************************************************
// �A�C���X�C����ԃN���X
class CFadeStateIrisIn : public CFadeState
{
public:
	// �R���X�g���N�^
	CFadeStateIrisIn(std::function<D3DXVECTOR3(void)> pFuncPos);

	// �f�X�g���N�^
	~CFadeStateIrisIn() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V

private:
	// �����o�ϐ�
	std::function<D3DXVECTOR3(void)> m_pFuncPos;	// �؂蔲���^�ʒu�֐��|�C���^
	float m_fInitRad;	// �������a
	float m_fCurTime;	// ���݂̌o�ߎ���
};

#endif	// _FADE_STATE_IRISIN_H_
