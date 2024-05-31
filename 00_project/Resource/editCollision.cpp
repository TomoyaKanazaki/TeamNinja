////============================================================
////
////	�G�f�B�b�g�R���W�������� [editCollision.cpp]
////	Author�F��������
////
////============================================================
////************************************************************
////	�C���N���[�h�t�@�C��
////************************************************************
//#include "manager.h"
//#include "editCollision.h"
//#include "editManager.h"
//
//#include "editorObject.h"
//
////************************************************************
////	�}�N����`
////************************************************************
//#define KEY_CHANGE_OBJECT	(DIK_2)	// �I�u�W�F�N�g�^�C�v�ύX�L�[
//#define NAME_CHANGE_OBJECT	("2")	// �I�u�W�F�N�g�^�C�v�ύX�\��
//
////************************************************************
////	�萔�錾
////************************************************************
//namespace
//{
//	const char* TYPE_NAME[] =	// �I�u�W�F�N�g�^�C�v��
//	{
//		"�L���[�u",
//		"�~��",
//		"��",
//	};
//}
//
////************************************************************
////	�X�^�e�B�b�N�A�T�[�g
////************************************************************
//static_assert(NUM_ARRAY(TYPE_NAME) == CEditCollision::TYPE_MAX, "ERROR : Type Count Mismatch");
//
////************************************************************
////	�e�N���X [CEditCollision] �̃����o�֐�
////************************************************************
////============================================================
////	�R���X�g���N�^
////============================================================
//CEditCollision::CEditCollision(CEditManager* pEditManager) : CEditor(pEditManager)
//{
//#if _DEBUG
//
//	// �����o�ϐ����N���A
//	m_pEditor = nullptr;	// �G�f�B�^�[���
//	m_type = TYPE_CUBE;		// �I�u�W�F�N�g�^�C�v
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�f�X�g���N�^
////============================================================
//CEditCollision::~CEditCollision()
//{
//#if _DEBUG
//#endif	// _DEBUG
//}
//
////============================================================
////	����������
////============================================================
//HRESULT CEditCollision::Init(void)
//{
//#if _DEBUG
//
//	// �����o�ϐ���������
//	m_pEditor = nullptr;	// �G�f�B�^�[���
//	m_type = TYPE_CUBE;		// �I�u�W�F�N�g�^�C�v
//
//	// �G�f�B�^�[���̐���
//	//m_pEditor = CEditorObject::Create(m_type);
//	if (m_pEditor == nullptr)
//	{ // �����Ɏ��s�����ꍇ
//
//		// ���s��Ԃ�
//		assert(false);
//		return E_FAIL;
//	}
//
//	// ������Ԃ�
//	return S_OK;
//
//#else	// NDEBUG
//
//	// ������Ԃ�
//	return S_OK;
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�I������
////============================================================
//void CEditCollision::Uninit(void)
//{
//#if _DEBUG
//
//	// �G�f�B�^�[���̔j��
//	SAFE_REF_RELEASE(m_pEditor);
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�X�V����
////============================================================
//void CEditCollision::Update(void)
//{
//#if _DEBUG
//
//	// �I�u�W�F�N�g�^�C�v�̕ύX
//	ChangeObjectType();
//
//	// �G�f�B�^�[���̍X�V
//	assert(m_pEditor != nullptr);
//	m_pEditor->Update();
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�ۑ�����
////============================================================
//HRESULT CEditCollision::Save(void)
//{
//#if _DEBUG
//
//	// �G�f�B�^�[���̕ۑ�
//	assert(m_pEditor != nullptr);
//	if (FAILED(m_pEditor->Save()))
//	{ // �Z�[�u�Ɏ��s�����ꍇ
//
//		// ���s��Ԃ�
//		assert(false);
//		return E_FAIL;
//	}
//
//	// ������Ԃ�
//	return S_OK;
//
//#else	// NDEBUG
//
//	// ������Ԃ�
//	return S_OK;
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�ۑ��󋵎擾����
////============================================================
//bool CEditCollision::IsSave(void)
//{
//#if _DEBUG
//
//	// �G�f�B�^�[���̕ۑ�
//	assert(m_pEditor != nullptr);
//	return m_pEditor->IsSave();
//
//#else	// NDEBUG
//
//	// false��Ԃ�
//	return false;
//
//#endif	// _DEBUG
//}
//
////============================================================
////	���ۑ�����
////============================================================
//void CEditCollision::SaveInfo(void)
//{
//#if _DEBUG
//
//	// �G�f�B�^�[���̏��ۑ�
//	assert(m_pEditor != nullptr);
//	m_pEditor->SaveInfo();
//
//#endif	// _DEBUG
//}
//
////============================================================
////	���Ǎ�����
////============================================================
//void CEditCollision::LoadInfo(void)
//{
//#if _DEBUG
//
//	// �G�f�B�^�[���̏��Ǎ�
//	assert(m_pEditor != nullptr);
//	m_pEditor->LoadInfo();
//
//#endif	// _DEBUG
//}
//
////============================================================
////	����\���̕`�揈��
////============================================================
//void CEditCollision::DrawDebugControl(void)
//{
//#if _DEBUG
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, "�G�f�B�b�g�X�e�[�W�^�C�v�ύX�F[%s]\n", NAME_CHANGE_OBJECT);
//
//	// �G�f�B�^�[���̑���\��
//	assert(m_pEditor != nullptr);
//	m_pEditor->DrawDebugControl();
//
//#endif	// _DEBUG
//}
//
////============================================================
////	���\���̕`�揈��
////============================================================
//void CEditCollision::DrawDebugInfo(void)
//{
//#if _DEBUG
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, "%s�F[�G�f�B�b�g�X�e�[�W�^�C�v]\n", TYPE_NAME[m_type]);
//
//	// �G�f�B�^�[���̏��\��
//	assert(m_pEditor != nullptr);
//	m_pEditor->DrawDebugInfo();
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�I�u�W�F�N�g�^�C�v�̕ύX����
////============================================================
//void CEditCollision::ChangeObjectType(void)
//{
//	// �I�u�W�F�N�g�^�C�v�̕ύX
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
//	if (pKeyboard->IsTrigger(KEY_CHANGE_OBJECT))
//	{
//		// �G�f�B�^�[���̔j��
//		SAFE_REF_RELEASE(m_pEditor);
//
//		// �I�u�W�F�N�g�^�C�v�̕ύX
//		m_type = (CEditCollision::EType)((m_type + 1) % CEditCollision::TYPE_MAX);
//
//		if (m_pEditor == nullptr)
//		{
//			// �G�f�B�^�[���̐���
//			//m_pEditor = CEditorObject::Create(m_type);
//			assert(m_pEditor != nullptr);	// �������s
//		}
//	}
//}
