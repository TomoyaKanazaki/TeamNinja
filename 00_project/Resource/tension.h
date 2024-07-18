#pragma once
//===========================================
//
//  �m�C�̓Q�[�W(tension.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "object2D.h"

//===========================================
//  �N���X��`
//===========================================
class CTension : CObject2D
{
public:

	// �����o�֐�
	CTension();
	~CTension();

	HRESULT Init(void) override; // ����������
	void Uninit(void) override; // �I������
	void Update(const float fDeltaTime) override; // �X�V����
	void Draw(CShader* pShader = nullptr) override; // �`�揈��

	// �ÓI�����o�֐�
	static CTension* Create();

private:

	// �����o�ϐ�


};