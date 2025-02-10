//==========================================
//
//  �ڕW�Ɍ������Ĉړ�����G�t�F�N�g�̃N���X(effect_move.cpp)
//  Author : Tomoya Kanazaki 
//
//==========================================
#include "effect_move.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float EFFECT_SCALE = 50.0f; // �G�t�F�N�g�̑傫��
	const float TARGET_HEIGHT = 50.0f; // �ڕW���W�̍����␳�l
}

//==========================================
//  �R���X�g���N�^
//==========================================
CMoveEffect::CMoveEffect() : CObject(LABEL_EFFECT),
	m_pos(VEC3_ZERO),
	m_fDeleteDistance(0.0f),
	m_fSpeed(0.0f),
	m_pTargetObj(nullptr),
	m_posTarget(VEC3_ZERO),
	m_pEffect(nullptr)
{
	// Do Nothing
}

//==========================================
//  �f�X�g���N�^
//==========================================
CMoveEffect::~CMoveEffect()
{
	// Do Nothing
}

//==========================================
//  ����������
//==========================================
HRESULT CMoveEffect::Init()
{
	return S_OK;
}

//==========================================
//  �I������
//==========================================
void CMoveEffect::Uninit()
{
	// �G�t�F�N�g���̔j��
	SAFE_DELETE(m_pEffect);

	// ���g��j��
	Release();
}

//==========================================
//  �X�V����
//==========================================
void CMoveEffect::Update(const float fDeltaTime)
{
	// �ړ�����
	Move(fDeltaTime);
}

//==========================================
//  �I������
//==========================================
void CMoveEffect::Draw(CShader* pShader)
{
	// �����ڂ̓G�t�F�N�V�A���ŕ`�悳���
}

//==========================================
//  ��������
//==========================================
CMoveEffect* CMoveEffect::Create(std::string path, const D3DXVECTOR3& posDef, float fSpeed, const D3DXVECTOR3& posTarget, float fDistanceDelete)
{
	// �|�C���^��錾
	CMoveEffect* pEffect = new CMoveEffect;	// �v���C���[���

	// �����Ɏ��s�����ꍇnull��Ԃ�
	if (pEffect == nullptr) { assert(false); return nullptr; }

	// �v���C���[�̏�����
	if (FAILED(pEffect->Init()))
	{ // �������Ɏ��s�����ꍇ
		assert(false);
		// �v���C���[�̔j��
		SAFE_DELETE(pEffect);
		return nullptr;
	}

	// �e�����ݒ肷��
	pEffect->m_pos = posDef; // �����ʒu
	pEffect->m_fSpeed = fSpeed; // �ړ����x
	pEffect->m_posTarget = posTarget; // �ڕW���W
	pEffect->m_fDeleteDistance = fDistanceDelete; // �폜�͈�

	// �m�ۂ����A�h���X��Ԃ�
	return pEffect;

}

//==========================================
//  ��������
//==========================================
CMoveEffect* CMoveEffect::Create(std::string path, const D3DXVECTOR3& posDef, float fSpeed, CObject* pObj, float fDistanceDelete)
{
	// �|�C���^��錾
	CMoveEffect* pEffect = new CMoveEffect;	// �v���C���[���

	// �����Ɏ��s�����ꍇnull��Ԃ�
	if (pEffect == nullptr) { return nullptr; }

	// �v���C���[�̏�����
	if (FAILED(pEffect->Init()))
	{ // �������Ɏ��s�����ꍇ

		// �v���C���[�̔j��
		SAFE_DELETE(pEffect);
		return nullptr;
	}

	// �e�����ݒ肷��
	pEffect->m_pos = posDef; // �����ʒu
	pEffect->m_fSpeed = fSpeed; // �ړ����x
	pEffect->m_pTargetObj = pObj; // �ڕW�I�u�W�F�N�g
	pEffect->m_fDeleteDistance = fDistanceDelete; // �폜�͈�

	// �G�t�F�N�g�𐶐�����
	pEffect->CreateEffect(path);

	// �m�ۂ����A�h���X��Ԃ�
	return pEffect;
}

//==========================================
//  �G�t�F�N�g�̐���
//==========================================
void CMoveEffect::CreateEffect(std::string path)
{
	m_pEffect = GET_EFFECT->Create(path, m_pos, VEC3_ZERO, VEC3_ZERO, EFFECT_SCALE, true, false);
}

//==========================================
//  �ړ�����
//==========================================
void CMoveEffect::Move(const float fDeltaTime)
{
	DebugProc::Print(DebugProc::POINT_CENTER, "Effectpos : (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);

	// �ڕW�I�u�W�F�N�g���ݒ肳��Ă���ꍇ
	if (m_pTargetObj != nullptr)
	{
		// �ڕW�̈ʒu���擾
		m_posTarget = m_pTargetObj->GetVec3Position();
		m_posTarget.y += TARGET_HEIGHT;
	}

	// �ڕW�n�_�Ǝ��g�̍��W�����ԃx�N�g�����Z�o
	D3DXVECTOR3 vec = m_posTarget - m_pos;

	// �Z�o�����x�N�g���̑傫�����폜�͈͓��̏ꍇ
	if (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z <= m_fDeleteDistance * m_fDeleteDistance)
	{
		// ���g���I�����֐��𔲂���
		Uninit();
		return;
	}

	// �Z�o�����l�𐳋K������
	D3DXVec3Normalize(&vec, &vec);

	// ���K�������l�Ɉړ��ʂ��������킹��
	vec *= m_fSpeed * fDeltaTime;

	// ���݂̍��W�Ɉړ��ʂ����Z����
	m_pos += vec;

	// ���W�������ڂɓK�p����
	m_pEffect->m_pos = Effekseer::Vector3D(m_pos.x, m_pos.y, m_pos.z);
}
