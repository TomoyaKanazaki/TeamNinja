//=========================================
//
//  �����Ǘ��M�~�b�N (gimmick_malti.cpp)
//  Author : Tomoya kanazaki
//  Adder  : Yuichi Fujita
//
//=========================================
#include "gimmick_malti.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const int NUM_CLONE = 1;	// �{�^���������݂ɕK�v�Ȑl��
}

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickMalti::CGimmickMalti() : CGimmick(),
m_bActive (true) // �A�N�e�B�u�t���O
{
	// �{�^�����I�z����N���A
	m_vecButton.clear();
}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickMalti::~CGimmickMalti()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickMalti::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CGimmick::Init()))
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
void CGimmickMalti::Uninit(void)
{
	// �{�^�����I�z����N���A
	m_vecButton.clear();

	// �e�N���X�̏I��
	CGimmick::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickMalti::Update(const float fDeltaTime)
{
	// �t���O�����Z�b�g
	m_bActive = true;

	// �Ǘ����Ă���M�~�b�N�̐ݒu�t���O���擾
	for (const auto& rGimmick : m_vecButton)
	{ // �{�^���z�񕪌J��Ԃ�

		// �M�~�b�N�̃t���O�������Ă����玟�ɐi��
		if (rGimmick->IsSet()) { continue; }

		// 1�ł��t���O��off�Ȃ�t���O��false�ɂ���
		m_bActive = false;

		break;
	}

	if (m_bActive) { DebugProc::Print(DebugProc::POINT_CENTER, "�ݒuOK\n"); }

	// �e�N���X�̍X�V
	CGimmick::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickMalti::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmick::Draw(pShader);
}

//===========================================
//  �ݒu�M�~�b�N�̐���
//===========================================
CGimmickMalti* CGimmickMalti::Create(std::vector<SButton> vecButton)
{
	// �����Ǘ��M�~�b�N�̐���
	CGimmickMalti *pGimmickMalti = new CGimmickMalti;
	if (pGimmickMalti == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �����Ǘ��M�~�b�N�̏�����
		if (FAILED(pGimmickMalti->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �����Ǘ��M�~�b�N�̔j��
			SAFE_DELETE(pGimmickMalti);
			return nullptr;
		}

		// �{�^�����̐���
		if (FAILED(pGimmickMalti->CreateButton(vecButton)))
		{ // �����Ɏ��s�����ꍇ

			// �����Ǘ��M�~�b�N�̔j��
			SAFE_DELETE(pGimmickMalti);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pGimmickMalti;
	}
}

//===========================================
//  �{�^�����̐���
//===========================================
HRESULT CGimmickMalti::CreateButton(std::vector<SButton> vecButton)
{
	for (const auto& rButton : vecButton)
	{ // �{�^���z�񕪌J��Ԃ�

		// �{�^���̐���
		CGimmick* pButton = CGimmick::Create
		( // ����
			rButton.pos,			// �����ʒu
			rButton.size,			// �����傫��
			CGimmick::TYPE_POST,	// �M�~�b�N���x�� (�{�^��)
			NUM_CLONE				// �{�^�������ɕK�v�Ȑl��
		);
		if (pButton == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// �Ō���ɐ��������{�^����ۑ�
		m_vecButton.push_back(pButton);
	}

	return S_OK;
}
