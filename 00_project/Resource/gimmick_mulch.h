#pragma once
//=========================================
//
//  �����Ǘ��M�~�b�N (gimmick_mulch.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "gimmick.h"

//=========================================
//  �N���X��`
//=========================================
class CGimmickMulch : public CGimmick
{
public:
	// �����{�^���\����
	struct SButton
	{
		// �R���X�g���N�^
		SButton(D3DXVECTOR3 Pos, D3DXVECTOR3 Size) :
			pos		(Pos),	// �ʒu
			size	(Size)	// �傫��
		{}

		// �����o�ϐ�
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 size;	// �傫��
	};

	// �����o�֐�
	CGimmickMulch();
	~CGimmickMulch() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

	bool IsActive() { return m_bActive; }

	// �ÓI�����o�֐�
	static CGimmickMulch* Create(std::vector<SButton> vecButton); // ����

#ifdef _DEBUG
	// �����o�֐�
	const std::vector<CGimmick*> GetVectorButton(void) override { return m_vecButton; }	// �{�^�����I�z��擾
#endif // _DEBUG

private:
	// �����o�֐�
	HRESULT CreateButton(std::vector<SButton> vecButton);	// �{�^����񐶐�

	// �����o�ϐ�
	std::vector<CGimmick*> m_vecButton; // �{�^�����I�z��
	bool m_bActive; // �A�N�e�B�u�t���O
};
