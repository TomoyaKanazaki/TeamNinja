//=========================================
//
//  �f�o�b�O��p�I�u�W�F�N�g(��Ώ���) [debug_object.cpp]
//  Author�FTomoya Kanazaki
//
//=========================================
#include "manager.h"
#include "debug_object.h"

//=========================================
//  �萔��`
//=========================================
namespace
{
	const int PRIORITY = 2;				// �M�~�b�N�͈̓|���S���̗D�揇��
	const int INIT_NUM_ACTIVE = -1;		// �����\�l���̏����l
}

//=========================================
//	�ÓI�����o�ϐ��錾
//=========================================
CListManager<CDebugObject>* CDebugObject::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//===========================================
//  �R���X�g���N�^
//===========================================
CDebugObject::CDebugObject()
{
}

//=========================================
//	�f�X�g���N�^
//=========================================
CDebugObject::~CDebugObject()
{

}

//=========================================
//	����������
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

	// ������Ԃ�
	return S_OK;
}

//=========================================
//	�I������
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
//	�X�V����
//=========================================
void CDebugObject::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update(fDeltaTime);
}

//=========================================
//	�`�揈��
//=========================================
void CDebugObject::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw(pShader);
}

//=========================================
//	��������
//=========================================
CDebugObject* CDebugObject::Create
(
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rSize		// �T�C�Y
)
{
	// �C���X�^���X����
	CDebugObject* pDebug = new CDebugObject;

	// �����Ɏ��s�����ꍇ
	if (pDebug == nullptr) { return nullptr; }

	// ���g��UI�̏�����
	if (FAILED(pDebug->Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���g��UI�̔j��
		SAFE_DELETE(pDebug);
		return nullptr;
	}

	// �ʒu��ݒ�
	pDebug->SetVec3Position(rPos);

	// �T�C�Y��ݒ�
	pDebug->SetVec3Sizing(rSize);

	// �m�ۂ����A�h���X��Ԃ�
	return pDebug;
}

//=========================================
// ���X�g�擾
//=========================================
CListManager<CDebugObject>* CDebugObject::GetList(void)
{
	// ���X�g�\����Ԃ�
	return m_pList;
}
