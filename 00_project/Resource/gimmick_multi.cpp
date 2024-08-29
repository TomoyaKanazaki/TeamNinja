//=========================================
//
//  �����Ǘ��M�~�b�N (gimmick_multi.cpp)
//  Author : Tomoya kanazaki
//  Adder  : Yuichi Fujita
//
//=========================================
#include "gimmick_multi.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"
#include "actor.h"

#include "camera.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const int NUM_CLONE = 1;	// �{�^���������݂ɕK�v�Ȑl��
	const float MOVE_SPEED = 150.0f; // �ړ����x
	const D3DXVECTOR3 MOVE_POS = D3DXVECTOR3(0.0f, -250.0f, 0.0f); // �ړ���̈ʒu(�I�t�Z�b�g)
	const CCamera::SSwing CAMERA_SWING = CCamera::SSwing(3.0f, 1.8f, 0.02f);		// �|�i�q�������Ƃ��̗h��
}

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickMulti::CGimmickMulti() : CGimmick(),
m_bActive (true), // �A�N�e�B�u�t���O
m_bActiveOld(false), // �O��̃A�N�e�B�u�t���O
m_pModel (nullptr) // ���f�����
{
	// �{�^�����I�z����N���A
	m_vecButton.clear();
}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickMulti::~CGimmickMulti()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickMulti::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CGimmick::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f���̐���
	m_pModel = CActor::Create(CActor::TYPE_B_LATTICE, VEC3_ZERO, VEC3_ZERO);
	if (m_pModel == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
	}

	// ���f���̃��x����ύX
	m_pModel->SetLabel(CObject::LABEL_GIMMICK);

	// ������Ԃ�
	return S_OK;
}

//=========================================
//  �I������
//=========================================
void CGimmickMulti::Uninit(void)
{
	// �g���f���̏I��
	SAFE_UNINIT(m_pModel);

	// �{�^�����I�z����N���A
	m_vecButton.clear();

	// �e�N���X�̏I��
	CGimmick::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickMulti::Update(const float fDeltaTime)
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

	// ���f���̈ړ�
	MoveModel(fDeltaTime);

	// �e�N���X�̍X�V
	CGimmick::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickMulti::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmick::Draw(pShader);
}

//===========================================
//  �ʒu�̐ݒ�
//===========================================
void CGimmickMulti::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �e�N���X�̐ݒ菈�����Ă�
	CGimmick::SetVec3Position(rPos);

	// �����ڂ̈ʒu��ݒ�
	m_pModel->SetVec3Position(rPos);

	// �p�x���擾
	D3DXVECTOR3 rot = VEC3_ZERO;

	// �A���O���̌����ϊ�
	rot.y = useful::FourDireToRot(GetAngle());

	// �����ڂ̊p�x��ݒ�
	m_pModel->SetVec3Rotation(rot);

	// �|�i�q�̃I�t�Z�b�g�ݒ菈��
	m_pModel->CollisionOffset();
}

//===========================================
//  �ݒu�M�~�b�N�̐���
//===========================================
CGimmickMulti* CGimmickMulti::Create(const D3DXVECTOR3& rPos, const EAngle angle, std::vector<SButton> vecButton)
{
	// �����Ǘ��M�~�b�N�̐���
	CGimmickMulti *pGimmick = new CGimmickMulti;
	if (pGimmick == nullptr) { return nullptr; }

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

	// �����̐ݒ�
	pGimmick->SetAngle(angle);

	// ���W�̐ݒ�
	pGimmick->SetVec3Position(rPos);

	// �m�ۂ����A�h���X��Ԃ�
	return pGimmick;
}

//===========================================
//  �{�^�����̐���
//===========================================
HRESULT CGimmickMulti::CreateButton(std::vector<SButton> vecButton)
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

//===========================================
//  ���f���̈ړ�
//===========================================
void CGimmickMulti::MoveModel(const float fDeltaTime)
{	
	// �|�i�q�����������A�J������h�炷
	if (m_bActive != m_bActiveOld)
	{
		CManager::GetInstance()->GetCamera()->SetSwing(CCamera::TYPE_MAIN, CAMERA_SWING);
	}

	// �O��̃A�N�e�B�u�t���O��ݒ肷��
	m_bActiveOld = m_bActive;

	// ���f���̈ʒu���擾
	D3DXVECTOR3 posModel = m_pModel->GetVec3Position();

	// �ړ���
	float fMove = 0.0f;

	// �A�N�e�B�u�t���O���I���̏ꍇ�ړ���Ɍ������ē���
	if (m_bActive)
	{
		// ���f���̈ʒu���ړ������������ꍇ�֐��𔲂���
		if (posModel.y <= GetVec3Position().y + MOVE_POS.y)
		{
			posModel.y = GetVec3Position().y + MOVE_POS.y;
			return;
		}

		// �ړ��̐ݒ�
		fMove -= MOVE_SPEED;
	}
	else // �I�t�̏ꍇ��ʒu�ɖ߂�
	{
		// ���f���̈ʒu����_���������ꍇ�֐��𔲂���
		if (posModel.y >= GetVec3Position().y)
		{
			posModel.y = GetVec3Position().y;
			return;
		}

		// �ړ��̐ݒ�
		fMove += MOVE_SPEED;
	}

	// �ړ��ʂ̓K�p
	posModel.y += fMove * fDeltaTime;

	// ���f���̈ʒu��ݒ�
	m_pModel->SetVec3Position(posModel);
}
