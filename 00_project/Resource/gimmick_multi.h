#pragma once
//=========================================
//
//  �����Ǘ��M�~�b�N (gimmick_multi.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "gimmick.h"

//=========================================
//  �O���錾
//=========================================
class CActor;	// �A�N�^�[�N���X

//=========================================
//  �N���X��`
//=========================================
class CGimmickMulti : public CGimmick
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
	CGimmickMulti();
	~CGimmickMulti() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

	void SetVec3Position(const D3DXVECTOR3& rPos);
	bool IsActive() { return m_bActive; }

	// �ÓI�����o�֐�
	static CGimmickMulti* Create(const D3DXVECTOR3& rPos, const EAngle angle, std::vector<SButton> vecButton); // ����

#ifdef _DEBUG
	// �����o�֐�
	const std::vector<CGimmick*> GetVectorButton(void) override { return m_vecButton; }	// �{�^�����I�z��擾
#endif // _DEBUG

private:
	// �����o�֐�
	HRESULT CreateButton(std::vector<SButton> vecButton);	// �{�^����񐶐�
	void MoveModel(const float fDeltaTime); // ���f���̈ړ�

	// �����o�ϐ�
	CActor* m_pModel; // ���f�����
	std::vector<CGimmick*> m_vecButton; // �{�^�����I�z��
	bool m_bActive; // �A�N�e�B�u�t���O
};
