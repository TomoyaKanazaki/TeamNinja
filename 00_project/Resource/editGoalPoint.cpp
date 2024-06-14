//#if 1
////============================================================
////
////	�G�f�B�b�g�S�[���|�C���g���� [editGoalPoint.cpp]
////	Author�F��������
////
////============================================================
////************************************************************
////	�C���N���[�h�t�@�C��
////************************************************************
//#include "editManager.h"
//#include "editGoalPoint.h"
//#include "manager.h"
//#include "collision.h"
//#include "useful.h"
//#include "stage.h"
//#include "gameManager.h"
//#include "sceneGame.h"
//
////************************************************************
////	�}�N����`
////************************************************************
//#define KEY_CREATE		(DIK_0)	// �����L�[
//#define NAME_CREATE		("0")	// �����\��
//#define KEY_RELEASE		(DIK_9)	// �j���L�[
//#define NAME_RELEASE	("9")	// �j���\��
//
////************************************************************
////	�萔�錾
////************************************************************
//namespace
//{
//	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_goalpoint.txt";	// �Z�[�u�e�L�X�g�p�X
//
//	const float	INIT_ALPHA = 0.5f;	// �z�u�O�̃��l
//	const int DIGIT_FLOAT = 2;		// �����_�ȉ��̌���
//}
//
////************************************************************
////	�e�N���X [CEditGoalPoint] �̃����o�֐�
////************************************************************
////============================================================
////	�R���X�g���N�^
////============================================================
//CEditGoalPoint::CEditGoalPoint()
//{
//#if _DEBUG
//
//	// �����o�ϐ����N���A
//	m_pPoint = nullptr;	// �|�C���g
//	m_bSave = false;	// �ۑ���
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�f�X�g���N�^
////============================================================
//CEditGoalPoint::~CEditGoalPoint()
//{
//#if _DEBUG
//#endif	// _DEBUG
//}
//
////============================================================
////	����������
////============================================================
//HRESULT CEditGoalPoint::Init(void)
//{
//#if _DEBUG
//
//	// �����o�ϐ���������
//	m_pPoint = nullptr;		// �|�C���g
//	m_bSave = false;		// �ۑ���
//
//	// �e�N���X�̏�����
//	if (FAILED(CEditorObject::Init()))
//	{ // �������Ɏ��s�����ꍇ
//
//		// ���s��Ԃ�
//		assert(false);
//		return E_FAIL;
//	}
//
//	// �`�F�b�N�|�C���g�̐���
//	m_pPoint = CGoal::Create
//	( // ����
//		GetVec3Position(),				// �ʒu
//		D3DXVECTOR3(0.0f, 0.01f, 0.0f)	// ����
//	);
//	if (m_pPoint == nullptr)
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
//void CEditGoalPoint::Uninit(void)
//{
//#if _DEBUG
//
//	// �e�N���X�̏I��
//	CEditorObject::Uninit();
//
//	// �`�F�b�N�|�C���g�̏I��
//	SAFE_UNINIT(m_pPoint);
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�X�V����
////============================================================
//void CEditGoalPoint::Update(void)
//{
//#if _DEBUG
//
//	// �e�N���X�̍X�V
//	CEditorObject::Update();
//
//	// �`�F�b�N�|�C���g�̐���
//	CreateCheckPoint();
//
//	// �`�F�b�N�|�C���g�̔j��
//	ReleaseCheckPoint();
//
//	// �ʒu�𔽉f
//	m_pPoint->SetVec3Position(GetVec3Position());
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�ۑ��󋵎擾����
////============================================================
//bool CEditGoalPoint::IsSave(void)
//{
//#if _DEBUG
//
//	// �ۑ��󋵂�Ԃ�
//	return m_bSave;
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
//void CEditGoalPoint::SaveInfo(void)
//{
//#if _DEBUG
//
//	// ���݂̏���ۑ�
//	//m_save = m_infoCreate;
//
//#endif	// _DEBUG
//}
//
////============================================================
////	���Ǎ�����
////============================================================
//void CEditGoalPoint::LoadInfo(void)
//{
//#if _DEBUG
//
//	// �ۑ�����ݒ�
//	//m_infoCreate = m_save;
//
//#endif	// _DEBUG
//}
//
////============================================================
////	����\���̕`�揈��
////============================================================
//void CEditGoalPoint::DrawDebugControl(void)
//{
//#if _DEBUG
//
//	// ����\���̕`��
//	CEditorObject::DrawDebugControl();
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, "�폜�F[%s]\n", NAME_RELEASE);
//	DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);
//
//#endif	// _DEBUG
//}
//
////============================================================
////	���\���̕`�揈��
////============================================================
//void CEditGoalPoint::DrawDebugInfo(void)
//{
//#if _DEBUG
//
//	// ���\���̕`��
//	CEditorObject::DrawDebugInfo();
//
//	if (CSceneGame::GetGameManager()->GetGoal() == nullptr)
//	{ // �S�[���� NULL �̏ꍇ
//
//		DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�\\n");
//	}
//	else
//	{ // ��L�ȊO
//
//		DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�s�\\n");
//	}
//
//#endif	// _DEBUG
//}
//
////============================================================
//// �ʒu�X�V
////============================================================
//void CEditGoalPoint::UpdatePosition(void)
//{
//	// �ʒu�̍X�V
//	CEditorObject::UpdatePosition();
//
//	// �ʒu�𔽉f
//	m_pPoint->SetVec3Position(GetVec3Position());
//}
//
////============================================================
////	�`�F�b�N�|�C���g�̐�������
////============================================================
//void CEditGoalPoint::CreateCheckPoint(void)
//{
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
//	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
//
//	// �`�F�b�N�|�C���g��z�u
//	if (pKeyboard->IsTrigger(KEY_CREATE))
//	{
//		//----------------------------------------------------
//		//	�`�F�b�N�|�C���g�̏���z�u�p�ɕύX
//		//----------------------------------------------------
//		// �����X�V�E�����`���ON�ɂ���
//		m_pPoint->SetEnableUpdate(true);
//		m_pPoint->SetEnableDraw(true);
//
//		// ���ۑ���ݒ�
//		m_bSave = false;
//
//		//----------------------------------------------------
//		//	�V�����`�F�b�N�|�C���g�̐���
//		//----------------------------------------------------
//		// �`�F�b�N�|�C���g�̐���
//		m_pPoint = CGoal::Create
//		( // ����
//			GetVec3Position(),				// �ʒu
//			D3DXVECTOR3(0.0f, 0.01f, 0.0f)	// ����
//		);
//		assert(m_pPoint != nullptr);
//	}
//}
//
////============================================================
////	�`�F�b�N�|�C���g�̔j������
////============================================================
//void CEditGoalPoint::ReleaseCheckPoint(void)
//{
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
//	bool bRelease = false;	// �j����
//
//	// �`�F�b�N�|�C���g���폜
//	if (pKeyboard->IsTrigger(KEY_RELEASE))
//	{
//		// �j�������Ԃ�ݒ�
//		bRelease = true;
//	}
//
//	// �`�F�b�N�|�C���g�̍폜����
//	DeleteCollisionCheckPoint(bRelease);
//}
//
////============================================================
////	�`�F�b�N�|�C���g�̍폜����
////============================================================
//void CEditGoalPoint::DeleteCollisionCheckPoint(const bool bRelase)
//{
//	// �S�[�����擾
//	CGoal* pGoal = CSceneGame::GetGameManager()->GetGoal();
//
//	if (pGoal != nullptr)
//	{ // �S�[���� NULL ����Ȃ��ꍇ
//
//		if (bRelase == true)
//		{ // �폜����ꍇ
//
//			// �S�[�����I������
//			pGoal->Uninit();
//			CSceneGame::GetGameManager()->SetGoal(nullptr);
//		}
//	}
//}
//
////============================================================
////	�`�F�b�N�|�C���g�̐F�̑S����������
////============================================================
//void CEditGoalPoint::InitAllColorCheckPoint(void)
//{
//	CListManager<CCheckPoint>* pListManager = CCheckPoint::GetList();	// �`�F�b�N�|�C���g���X�g�}�l�[�W���[
//	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
//	std::list<CCheckPoint*> listCheckPoint = pListManager->GetList();	// �`�F�b�N�|�C���g���X�g���
//
//	int nCnt = 0;
//	for (auto& rList : listCheckPoint)
//	{ // �`�F�b�N�|�C���g�����J��Ԃ�
//
//		// �����A�h���X�������ꍇ����
//		if (rList == m_pPoint) { continue; }
//
//		// �ʏ�F��ݒ�
//		rList->ResetMaterial();
//
//		nCnt++;
//	}
//}
//
////============================================================
////	�ۑ�����
////============================================================
//HRESULT CEditGoalPoint::Save(void)
//{
//#if _DEBUG
//
//	// �n�ʂ̃��X�g���擾
//	CListManager<CCheckPoint>* pListManager = CCheckPoint::GetList();	// ���X�g�}�l�[�W���[
//	std::list<CCheckPoint*> listCheckPoint;	// �n�ʃ��X�g
//	if (pListManager != nullptr)
//	{ // ���X�g�}�l�[�W���[����������Ă���ꍇ
//
//		// ���X�g���擾
//		listCheckPoint = pListManager->GetList();
//	}
//
//	// �t�@�C�����J��
//	std::ofstream  file(SAVE_PASS);	// �t�@�C���X�g���[��
//	if (file.fail())
//	{ // �t�@�C�����J���Ȃ������ꍇ
//
//		// �G���[���b�Z�[�W�{�b�N�X
//		MessageBox(nullptr, "�`�F�b�N�|�C���g�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);
//
//		// ���s��Ԃ�
//		return E_FAIL;
//	}
//
//	// ���o���������o��
//	file << "#==============================================================================" << std::endl;
//	file << "#" << std::endl;
//	file << "#	�`�F�b�N�|�C���g�̃Z�[�u�f�[�^ [save_checkpoint.txt]" << std::endl;
//	file << "#	Author : ���c �E��" << std::endl;
//	file << "#" << std::endl;
//	file << "#==============================================================================" << std::endl;
//	file << "# ���̍s���牺���R�s�[�� [checkpoint.txt] �ɒ���t��\n" << std::endl;
//
//	// �`�F�b�N�|�C���g�̐F�̑S������
//	InitAllColorCheckPoint();
//
//	// �����_�����o���̕��@���w��
//	file << std::fixed << std::setprecision(DIGIT_FLOAT);
//
//	// �ǂݍ��݊J�n������������o��
//	file << "STAGE_POINTSET\n" << std::endl;
//
//	for (const auto& rList : listCheckPoint)
//	{ // �n�ʂ̑������J��Ԃ�
//
//		// �����A�h���X�������ꍇ����
//		if (rList == m_pPoint) { continue; }
//
//		// �����o�������擾
//		D3DXVECTOR3 pos = rList->GetVec3Position();	// �ʒu
//
//		// ���������o��
//		file << "	POINTSET" << std::endl;
//		file << "		POS = " << pos.x << " " << pos.y << " " << pos.z << std::endl;
//		file << "	END_POINTSET\n" << std::endl;
//	}
//
//	// �ǂݍ��ݏI��������������o��
//	file << "END_STAGE_POINTSET" << std::endl;
//
//	// �`�F�b�N�|�C���g�̍폜����
//	DeleteCollisionCheckPoint(false);
//
//	// �ۑ��ς݂ɂ���
//	m_bSave = true;
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
//#endif
