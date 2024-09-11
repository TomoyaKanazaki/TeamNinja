//==========================================
//
//  �`�F�b�N�|�C���g(checkpoint.h)
//  Author : Tomoya Kanazaki
//  Adder�FRitsuki Obara
//
//==========================================
#pragma once
#include "object.h"
#include "effekseerControl.h"
#include "effekseerManager.h"

//==========================================
//  �N���X��`
//==========================================
class CCheckPoint : public CObject
{
public:

	// �����o�֐�
	CCheckPoint();
	~CCheckPoint();

	HRESULT Init() override; // ������
	void Uninit() override; // �I��
	void Update(const float fDeltaTime) override; // �X�V
	void Draw(CShader* pShader = nullptr) override; // �`��
	float GetRadius(void) const;		// ���a�擾����

	int GetSaveTension() const { return m_nSaveTension; }	// �Z�[�u���̎m�C�͎擾
	void SetVec3Position(const D3DXVECTOR3& rPos) override { m_pos = rPos; }	// �ʒu�ݒ菈��
	D3DXVECTOR3 GetVec3Position(void) const override { return m_pos; }			// �ʒu�擾����

	// �ÓI�����o�֐�
	static CCheckPoint* Create(const D3DXVECTOR3& rPos);	// ��������
	static CListManager<CCheckPoint>* GetList(void);		// ���X�g�擾
	static HRESULT LoadSetup(const char* pPass);			// �Z�b�g�A�b�v

private:

	// �I�[�o�[���C�h�֐�
	void Release(void) override { CObject::Release(); }	// �j��

	// �����o�֐�
	void CollisionPlayer();	// �v���C���[�Ƃ̓����蔻��

	// �����o�ϐ�
	CListManager<CCheckPoint>::AIterator m_iterator;	// �C�e���[�^�[
	D3DXVECTOR3 m_pos; // �ʒu
	bool m_bSave; // �Z�[�u�t���O
	int m_nSaveTension; // �Z�[�u���̎m�C��
	CEffekseer::CEffectData* m_pEffectdata; //�ێ�����G�t�F�N�g���

	// �ÓI�����o�ϐ�
	static CListManager<CCheckPoint>* m_pList;	// �I�u�W�F�N�g���X�g
};
