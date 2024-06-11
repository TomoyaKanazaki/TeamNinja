//=========================================
//
//  �f�o�b�O��p�I�u�W�F�N�g(��Ώ���) [debug_object.cpp]
//  Author�FTomoya Kanazaki
//
//=========================================
#include "manager.h"
#include "debug_object.h"

#include "collision.h"
#include "gimmick.h"

//=========================================
//  �萔��`
//=========================================
namespace
{
	const D3DXVECTOR3 DEBUGSIZE = D3DXVECTOR3(300.0f, 0.0f, 300.0f); // �����蔻����Ƃ�
	const D3DXVECTOR3 DEBUGPOS = D3DXVECTOR3(1500.0f, 0.5f, 0.0f); // �����\�l���̏����l
	const D3DXVECTOR3 CLONE_RADIUS = D3DXVECTOR3(20.0f, 0.0f, 20.0f);	// ���a
}

//=========================================
//  �ÓI�����o�ϐ��錾
//=========================================
CListManager<CDebugObject>* CDebugObject::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//===========================================
//  �R���X�g���N�^
//===========================================
CDebugObject::CDebugObject() : CObject3D(CObject::LABEL_GIMMICK, CObject::DIM_3D, 2)
{

}

//=========================================
//  �f�X�g���N�^
//=========================================
CDebugObject::~CDebugObject()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CDebugObject::Init(void)
{
	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CDebugObject>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

#ifdef _DEBUG

	// �F�̐ݒ�
	SetColor(XCOL_RED);

#else

	// �F�̐ݒ�
	SetColor(XCOL_AWHITE);

#endif // _DEBUG

	// �W�����v��ݒu
	CGimmick::Create(D3DXVECTOR3(DEBUGPOS.x, 0.2f, DEBUGPOS.z + DEBUGSIZE.z * 0.5f), D3DXVECTOR3(300.0f, 0.0f, 150.0f), CGimmick::TYPE_JUMPTABLE, 2);
	CGimmick::Create(D3DXVECTOR3(DEBUGPOS.x, 0.2f, DEBUGPOS.z - DEBUGSIZE.z * 0.5f), D3DXVECTOR3(300.0f, 0.0f, 150.0f), CGimmick::TYPE_JUMPTABLE, 2);
	CGimmick::Create(D3DXVECTOR3(DEBUGPOS.x + DEBUGSIZE.x * 0.5f, 0.2f, DEBUGPOS.z), D3DXVECTOR3(150.0f, 0.0f, 300.0f), CGimmick::TYPE_JUMPTABLE, 2);
	CGimmick::Create(D3DXVECTOR3(DEBUGPOS.x - DEBUGSIZE.x * 0.5f, 0.2f, DEBUGPOS.z), D3DXVECTOR3(150.0f, 0.0f, 300.0f), CGimmick::TYPE_JUMPTABLE, 2);

	// ������Ԃ�
	return S_OK;
}

//=========================================
//  �I������
//=========================================
void CDebugObject::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CDebugObject::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CDebugObject::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw(pShader);
}

//==========================================
//  �����蔻��
//==========================================
bool CDebugObject::Hit(const D3DXVECTOR3& rPos)
{
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu
	D3DXVECTOR3 size = GetVec3Sizing() * 0.5f;	// �T�C�Y

	if (collision::Box2D
	(
		pos,		// ����ʒu
		rPos,	// ����ڕW�ʒu
		size,		// ����T�C�Y(�E�E��E��)
		size,		// ����T�C�Y(���E���E�O)
		CLONE_RADIUS,	// ����ڕW�T�C�Y(�E�E��E��)
		CLONE_RADIUS	// ����ڕW�T�C�Y(���E���E�O)
	))
	{ // �l�p�̒��ɓ������ꍇ
		return true;
	}

	return false;
}

//=========================================
//  ��������
//=========================================
CDebugObject* CDebugObject::Create()
{
	// �f�o�b�O�ȊO�Ő�������ȃS�~�J�X���ˎ��ˎ���
#ifndef _DEBUG
	return nullptr;
#endif

	// �C���X�^���X����
	CDebugObject* pDebug = new CDebugObject;

	// �����Ɏ��s�����ꍇ
	if (pDebug == nullptr) { return nullptr; }

	// ������
	if (FAILED(pDebug->Init()))
	{ // �������Ɏ��s�����ꍇ

		// �j��
		SAFE_DELETE(pDebug);
		return nullptr;
	}

	// �ʒu��ݒ�
	pDebug->SetVec3Position(DEBUGPOS);

	// �T�C�Y��ݒ�
	pDebug->SetVec3Sizing(DEBUGSIZE);

	// �m�ۂ����A�h���X��Ԃ�
	return pDebug;
}

//=========================================
//  ���X�g�擾
//=========================================
CListManager<CDebugObject>* CDebugObject::GetList(void)
{
	// ���X�g�\����Ԃ�
	return m_pList;
}
