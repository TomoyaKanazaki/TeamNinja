//============================================================
//
//	��ԃM�~�b�N���� [gimmick_state.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "gimmick_state.h"
#include "player_clone.h"
#include "collision.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const D3DXVECTOR3 CLONE_RADIUS = D3DXVECTOR3(20.0f, 0.0f, 20.0f);	// ���a
}

//************************************************************
//	�q�N���X [CGimmickState] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGimmickState::CGimmickState() : CGimmick()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGimmickState::~CGimmickState()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGimmickState::Init(void)
{
	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CGimmick::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGimmickState::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I��
	CGimmick::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGimmickState::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g3D�̍X�V
	CGimmick::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CGimmickState::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CGimmick::Draw(pShader);
}

//===========================================
//  ���g�Ƃ̓����蔻��
//===========================================
void CGimmickState::CollisionClone()
{
	// ���g�̃��X�g�\����������Δ�����
	if (CPlayerClone::GetList() == nullptr) { return; }

	std::list<CPlayerClone*> list = CPlayerClone::GetList()->GetList();	// ���X�g���擾
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu
	D3DXVECTOR3 size = GetVec3Sizing() * 0.5f;	// �T�C�Y
	D3DXVECTOR3 posClone = VEC3_ZERO;		// ���g�̈ʒu
	D3DXVECTOR3 sizeClone = CLONE_RADIUS;	// ���g�̃T�C�Y

	for (auto clone : list)
	{
		// �ʒu���擾
		posClone = clone->GetVec3Position();

		if (!collision::Box2D
		(
			pos,		// ����ʒu
			posClone,	// ����ڕW�ʒu
			size,		// ����T�C�Y(�E�E��E��)
			size,		// ����T�C�Y(���E���E�O)
			sizeClone,	// ����ڕW�T�C�Y(�E�E��E��)
			sizeClone	// ����ڕW�T�C�Y(���E���E�O)
		))
		{ continue; }
	}
}
