//=========================================
//
//  ���������M�~�b�N (gimmick_decayed.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_decayed.h"
#include "manager.h"
#include "player_clone.h"
#include "player.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const int FALL_NUM = 3; // ��ɏ�邱�Ƃ̂ł��镪�g�̐�
}

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickDecayed::CGimmickDecayed() : CField(),
m_bFall(false) // ������t���O
{

}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickDecayed::~CGimmickDecayed()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickDecayed::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CField::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//=========================================
//  �I������
//=========================================
void CGimmickDecayed::Uninit(void)
{
	// �e�N���X�̏I��
	CField::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickDecayed::Update(const float fDeltaTime)
{
	if (m_bFall) { DebugProc::Print(DebugProc::POINT_CENTER, "������on\n"); }

	// �e�N���X�̍X�V
	CField::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickDecayed::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CField::Draw(pShader);
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickDecayed::Hit(CPlayerClone* pClone)
{
	// ���g�ɕ������n��
	pClone->AddFrags(GetFlag());

	// ���g�Ɏ��g�̏���n��
	pClone->SetField(this);

	// �����t���O��off�̏ꍇ���g�̐����v�Z
	if (!m_bFall) { Count(); }
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickDecayed::Miss(CPlayerClone* pClone)
{
	// ���g����t���O���폜����
	pClone->SabFrags(GetFlag());

	// ���g����t�B�[���h���폜����
	pClone->DeleteField(this);
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickDecayed::Hit(CPlayer* pPlayer)
{
	// �v���C���[�ɕ������n��
	pPlayer->AddFrags(GetFlag());

	// �����t���O��off�̏ꍇ���g�̐����v�Z
	if (!m_bFall) { Count(); }
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickDecayed::Miss(CPlayer* pPlayer)
{
	// �v���C���[����t���O���폜����
	pPlayer->SabFrags(GetFlag());
}

//===========================================
//  ����Ă���L�����N�^�[�����̌v�Z����
//===========================================
void CGimmickDecayed::Count()
{
	// ���g�̃��X�g�\����������Δ�����
	if (CPlayerClone::GetList() == nullptr) { return; }

	// ���X�g���擾
	std::list<CPlayerClone*> list = CPlayerClone::GetList()->GetList();

	// ���g�̏�ɂ��镪�g�̐����J�E���g
	int nNum = 0; // ����Ă��镪�g��
	for (CPlayerClone* clone : list)
	{
		// ���g���������Ă���t�B�[���h�����g�łȂ��ꍇ���ɐi��
		if (clone->GetField() != this) { continue; }

		// ���g�̏�̕��g�������Z
		++nNum;
	}

	// ���g�̐����ő吔�����̏ꍇ�֐��𔲂���
	if (nNum < FALL_NUM) { return; }

	// �����t���O�𗧂Ă�
	m_bFall = true;
}
