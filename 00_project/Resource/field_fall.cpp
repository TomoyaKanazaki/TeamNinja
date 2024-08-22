//=========================================
//
//  ���Ƃ����M�~�b�N (field_fall.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "field_fall.h"
#include "manager.h"
#include "player_clone.h"
#include "player.h"

#include "objectMeshCube.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const int FALL_NUM = 3; // ��ɏ�邱�Ƃ̂ł��镪�g�̐�
}

//==========================================
//  �R���X�g���N�^
//==========================================
CGimmickFall::CGimmickFall() : CField(),
m_bFall(false) // �����t���O
{

}

//==========================================
//  �f�X�g���N�^
//==========================================
CGimmickFall::~CGimmickFall()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickFall::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CField::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < NUM_FLOOR; nCnt++)
	{
		// TODO�F�����N���G�C�g!!
	}

	// ������Ԃ�
	return S_OK;
}

//=========================================
//  �I������
//=========================================
void CGimmickFall::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_FLOOR; nCnt++)
	{
		// ���̏I������
		SAFE_UNINIT(m_apFloor[nCnt]);
	}

	// �e�N���X�̏I��
	CField::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickFall::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CField::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickFall::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CField::Draw(pShader);
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickFall::Hit(CPlayerClone* pClone)
{
	// ���g�ɕ������n��
	pClone->AddFrags(GetFlag());

	// ���g�Ɏ��g�̏���n��
	pClone->SetField(this);

	// ���g�����J�E���g
	Count();
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickFall::Miss(CPlayerClone* pClone)
{
	// ���g����t���O���폜����
	pClone->SabFrags(GetFlag());

	// ���g����t�B�[���h���폜����
	pClone->DeleteField(this);
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickFall::Hit(CPlayer* pPlayer)
{
	// ���g�ɕ������n��
	pPlayer->AddFrags(GetFlag());

	// ���g�����J�E���g
	Count();
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickFall::Miss(CPlayer* pPlayer)
{
	// ���g����t���O���폜����
	pPlayer->SabFrags(GetFlag());
}

//===========================================
//  ����Ă���L�����N�^�[�����̌v�Z����
//===========================================
void CGimmickFall::Count()
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
	if (nNum < FALL_NUM) { m_bFall = false; return; }

	// �����t���O�𗧂Ă�
	m_bFall = true;
}
