//============================================================
//
//	�v���C���[�̉���G�t�F�N�g�w�b�_�[ [player_dodge.h]
//	Author�F��������
// 
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_DODGE_H_
#define _PLAYER_DODGE_H_

//************************************************************
// �C���N���[�h�t�@�C��
//************************************************************
#include "effekseerControl.h"

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�̉���G�t�F�N�g�N���X
class CPlayerDodge
{
public:
	// �R���X�g���N�^
	CPlayerDodge();

	// �f�X�g���N�^
	~CPlayerDodge();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��
	void Update(const D3DXVECTOR3& rPos);	// �X�V

	// �o�^����
	void Regist(CEffekseer::CEffectData* pEffect);

	// �ÓI�����o�֐�
	static CPlayerDodge* Create(void);		// ��������

private:

	// �����o�ϐ�
	std::vector<CEffekseer::CEffectData*> m_effectData;	// �G�t�F�N�g���
};

#endif	// _PLAYER_TITLE_H_
