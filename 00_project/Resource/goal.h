//==========================================
//
//  �S�[��(goal.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#pragma once
#include "objectModel.h"
#include "effekseerControl.h"
#include "effekseerManager.h"

//==========================================
//  �N���X��`
//==========================================
class CGoal : public CObjectModel
{
public:

	// �����o�֐�
	CGoal();
	~CGoal();

	HRESULT Init() override; // ������
	void Uninit() override; // �I��
	void Update(const float fDeltaTime) override; // �X�V
	void Draw(CShader* pShader = nullptr) override; // �`��

	bool GetClear() const { return m_bClear; }// �N���A��Ԃ̎擾

	// �ÓI�����o�֐�
	static CGoal* Create(const D3DXVECTOR3& rPos); // ��������
	static CGoal* GetGoal(void);					// ���X�g�擾
	static HRESULT LoadSetup(const char* pPass);	// �Z�b�g�A�b�v

private:

	// �����o�֐�
	void CollisionPlayer();	// �v���C���[�Ƃ̓����蔻��

	// �����o�ϐ�
	CListManager<CGoal>::AIterator m_iterator;	// �C�e���[�^�[
	bool m_bClear; // �N���A�t���O
	CEffekseer::CEffectData* m_pEffectdata; //�ێ�����G�t�F�N�g���

	// �ÓI�����o�ϐ�
	static CListManager<CGoal>* m_pList;	// �I�u�W�F�N�g���X�g
};
