//=========================================
//
//  �����Ǘ��M�~�b�N (gimmick_mulch.cpp)
//  Author : Tomoya kanazaki
//  Adder  : Yuichi Fujita
//
//=========================================
#include "gimmick_mulch.h"
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
CGimmickMulch::CGimmickMulch() : CGimmick(),
m_bActive (true) // �A�N�e�B�u�t���O
{
	// �{�^�����I�z����N���A
	m_vecButton.clear();
}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickMulch::~CGimmickMulch()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickMulch::Init(void)
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
void CGimmickMulch::Uninit(void)
{
	// �{�^�����I�z����N���A
	m_vecButton.clear();

	// �e�N���X�̏I��
	CGimmick::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickMulch::Update(const float fDeltaTime)
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

	// �e�N���X�̍X�V
	CGimmick::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickMulch::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmick::Draw(pShader);
}

//===========================================
//  �ݒu�M�~�b�N�̐���
//===========================================
CGimmickMulch* CGimmickMulch::Create(std::vector<SButton> vecButton)
{
	// �����Ǘ��M�~�b�N�̐���
	CGimmickMulch *pGimmick = new CGimmickMulch;
	if (pGimmick == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �����Ǘ��M�~�b�N�̏�����
		if (FAILED(pGimmick->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �����Ǘ��M�~�b�N�̔j��
			SAFE_DELETE(pGimmick);
			return nullptr;
		}

		// �{�^�����̐���
		if (FAILED(pGimmick->CreateButton(vecButton)))
		{ // �����Ɏ��s�����ꍇ

			// �����Ǘ��M�~�b�N�̔j��
			SAFE_DELETE(pGimmick);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pGimmick;
	}
}

//===========================================
//  �{�^�����̐���
//===========================================
HRESULT CGimmickMulch::CreateButton(std::vector<SButton> vecButton)
{
	for (const auto& rButton : vecButton)
	{ // �{�^���z�񕪌J��Ԃ�

		// �{�^���̐���
		CGimmick* pButton = CGimmick::Create
		( // ����
			rButton.pos,			// �����ʒu
			EAngle::ANGLE_0,		// �����p�x
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
