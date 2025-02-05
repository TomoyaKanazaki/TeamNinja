#pragma once
//==========================================
//
//  �ڕW�Ɍ������Ĉړ�����G�t�F�N�g�̃N���X(effect_move.h)
//  Author : Tomoya Kanazaki 
//
//==========================================
#include "object.h"
#include "effekseerControl.h"

//==========================================
//  �N���X��`
//==========================================
class CMoveEffect : public CObject
{
public:

	// �����o�֐�
	CMoveEffect();
	~CMoveEffect();

	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime);
	void Draw(CShader* pShader = nullptr);

	//�ÓI�����o�֐�
	static CMoveEffect* Create(std::string path, const D3DXVECTOR3& posDef, float fSpeed, const D3DXVECTOR3& posTarget, float fDistanceDelete); // ��������
	static CMoveEffect* Create(std::string path, const D3DXVECTOR3& posDef, float fSpeed, CObject* pObj, float fDistanceDelete); // ��������

private:

	// �����o�֐�
	void Release(void) override { CObject::Release(); }	// �j��
	void CreateEffect(std::string path); // �G�t�F�N�g�̐���
	void Move(const float fDeltaTime); // �ړ�����

	// �����o�ϐ�
	D3DXVECTOR3 m_pos; // ���g�̍��W
	float m_fDeleteDistance; // �폜�͈�
	float m_fSpeed; // �ړ���
	CObject* m_pTargetObj; // �ڕW�ɂȂ�I�u�W�F�N�g
	D3DXVECTOR3 m_posTarget; // �ڕW�n�_
	CEffekseer::CEffectData* m_pEffect; // �G�t�F�N�g���(������)

};
