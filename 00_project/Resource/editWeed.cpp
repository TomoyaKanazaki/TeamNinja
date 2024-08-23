////===========================================
////
////  ���G�f�B�^�[(editWeed.cpp)
////  Author : Tomoya Kanazaki
////
////===========================================
//#include "editWeed.h"
//#include "editManager.h"
//#include "manager.h"
//#include "collision.h"
//#include "stage.h"
//#include "objectMeshCylinder.h"
//
////===========================================
////  �萔�錾
////===========================================
//namespace
//{
//	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_weed.txt"; // �Z�[�u�e�L�X�g�p�X
//
//	const float	INIT_ALPHA = 0.5f; // �z�u�O�̃��l
//	const int DIGIT_FLOAT = 2; // �����_�ȉ��̌���
//
//	const int KEY_CREATE		= DIK_0; // �����L�[
//	const int KEY_RELEASE		= DIK_9; // �j���L�[
//	const int KEY_DIRECTION		= DIK_4; // �����ύX�L�[
//	const int KEY_ROTATION		= DIK_5; // �p�x�ύX�L�[
//	const int KEY_UP_SIZE_X		= DIK_T; // X���g��L�[
//	const int KEY_DOWN_SIZE_X	= DIK_G; // X���k���L�[
//	const int KEY_UP_SIZE_Y		= DIK_Y; // Y���g��L�[
//	const int KEY_DOWN_SIZE_Y	= DIK_H; // Y���k���L�[
//	const int KEY_UP_SIZE_Z		= DIK_U; // Z���g��L�[
//	const int KEY_DOWN_SIZE_Z	= DIK_J; // Z���k���L�[
//
//	const char* NAME_CREATE			= "0"; // �����\��
//	const char* NAME_RELEASE		= "9"; // �j���\��
//	const char* NAME_DIRECTION		= "4"; // �����ύX�\��
//	const char* NAME_ROTATION		= "5"; // �p�x�ύX�\��
//	const char* NAME_UP_SIZE_X		= "T"; // X���g��\��
//	const char* NAME_DOWN_SIZE_X	= "G"; // X���k���\��
//	const char* NAME_UP_SIZE_Y		= "Y"; // Y���g��\��
//	const char* NAME_DOWN_SIZE_Y	= "H"; // Y���k���\��
//	const char* NAME_UP_SIZE_Z		= "U"; // Z���g��\��
//	const char* NAME_DOWN_SIZE_Z	= "J"; // Z���k���\��
//}
//
////===========================================
////	�R���X�g���N�^
////===========================================
//CEditWeed::CEditWeed(CEditStage* pEditor) : CEditorObject(pEditor),
//m_pCylinder(nullptr),
//m_bSave(false)
//{
//	// �����o�ϐ����N���A
//	memset(&m_infoCreate, 0, sizeof(m_infoCreate)); // �z�u���
//}
//
////===========================================
////	�f�X�g���N�^
////===========================================
//CEditWeed::~CEditWeed()
//{
//}
//
////===========================================
////	����������
////===========================================
//HRESULT CEditWeed::Init(void)
//{
//#if _DEBUG
//
//	// �e�N���X�̏�����
//	if (FAILED(CEditorObject::Init())) { assert(false); return E_FAIL; }
//
//	// �I�u�W�F�N�g�̐���
//	m_pCylinder = CObjectMeshCylinder::Create
//	(
//		GetVec3Position(),					// �ʒu
//		VEC3_ZERO,							// ����
//		D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),	// �F
//		POSGRID2(16, 1),					// ������
//		POSGRID2(1, 1),						// �e�N�X�`��������
//		m_infoCreate.fSize,					// ���a
//		25.0f								// ����
//	);
//
//	//�����Ɏ��s�����ꍇ�֐��𔲂���
//	if (m_pCylinder == nullptr) { assert(false); return E_FAIL; }
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
////===========================================
////	�I������
////===========================================
//void CEditWeed::Uninit(void)
//{
//#if _DEBUG
//
//	// �e�N���X�̏I��
//	CEditorObject::Uninit();
//
//	// �I�u�W�F�N�g�̏I��
//	SAFE_UNINIT(m_pCylinder);
//
//#endif	// _DEBUG
//}
//
////===========================================
////	�X�V����
////===========================================
//void CEditWeed::Update(void)
//{
//#if _DEBUG
//
//	// �e�N���X�̍X�V
//	CEditorObject::Update();
//
//	// �傫���̍X�V
//	UpdateSize();
//
//	// ����
//	Create();
//
//	// �j��
//	Release();
//
//	// �ʒu�𔽉f
//	m_pCylinder->SetVec3Position(GetVec3Position());
//
//	// �����𔽉f
//	m_pCylinder->SetVec3Rotation(GetVec3Rotation());
//
//#endif	// _DEBUG
//}
//
////===========================================
////	�ۑ��󋵎擾����
////===========================================
//bool CEditWeed::IsSave(void)
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
////===========================================
////	���ۑ�����
////===========================================
//void CEditWeed::SaveInfo(void)
//{
//#if _DEBUG
//
//	// ���݂̏���ۑ�
//	//m_save = m_infoCreate;
//
//#endif	// _DEBUG
//}
//
////===========================================
////	���Ǎ�����
////===========================================
//void CEditWeed::LoadInfo(void)
//{
//#if _DEBUG
//
//	// �ۑ�����ݒ�
//	//m_infoCreate = m_save;
//
//#endif	// _DEBUG
//}
//
////===========================================
////	����\���̕`�揈��
////===========================================
//void CEditWeed::DrawDebugControl(void)
//{
//#if _DEBUG
//
//	// ����\���̕`��
//	CEditorObject::DrawDebugControl();
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, "�傫���F[%s/%s+%s/%s/%s+%s/%s/%s+%s]\n",
//		NAME_UP_SIZE_X, NAME_DOWN_SIZE_X, NAME_TRIGGER,
//		NAME_UP_SIZE_Y, NAME_DOWN_SIZE_Y, NAME_TRIGGER,
//		NAME_UP_SIZE_Z, NAME_DOWN_SIZE_Z, NAME_TRIGGER);
//	DebugProc::Print(DebugProc::POINT_RIGHT, "�����ύX�F[%s]\n", NAME_DIRECTION);
//	DebugProc::Print(DebugProc::POINT_RIGHT, "�p�x�ύX�F[%s]\n", NAME_ROTATION);
//	DebugProc::Print(DebugProc::POINT_RIGHT, "�폜�F[%s]\n", NAME_RELEASE);
//	DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);
//
//#endif	// _DEBUG
//}
//
////===========================================
////	���\���̕`�揈��
////===========================================
//void CEditWeed::DrawDebugInfo(void)
//{
//#if _DEBUG
//
//	// ���\���̕`��
//	CEditorObject::DrawDebugInfo();
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f�F[�傫��]\n", m_infoCreate.size.x, m_infoCreate.size.y);
//
//#endif	// _DEBUG
//}
//
////===========================================
////	�����X�V����
////===========================================
//void CEditWeed::UpdateRotation(void)
//{
//	// �����̍X�V
//	CEditorObject::UpdateRotation();
//
//	// �����𔽉f
//	m_pCylinder->SetVec3Rotation(GetVec3Rotation());
//}
//
////===========================================
////	�傫���̍X�V����
////===========================================
//void CEditWeed::UpdateSize(void)
//{
//	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
//
//	// �傫����ύX
//	if (!pKeyboard->IsPress(KEY_TRIGGER))
//	{
//		if (pKeyboard->IsPress(KEY_UP_SIZE_X))
//		{
//			m_infoCreate.fSize += editstage::SIZE * 0.1f;
//		}
//		if (pKeyboard->IsPress(KEY_DOWN_SIZE_X))
//		{
//			m_infoCreate.fSize -= editstage::SIZE * 0.1f;
//		}
//	}
//	else
//	{
//		if (pKeyboard->IsTrigger(KEY_UP_SIZE_X))
//		{
//			m_infoCreate.fSize += editstage::SIZE * 0.1f;
//		}
//		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_X))
//		{
//			m_infoCreate.fSize -= editstage::SIZE * 0.1f;
//		}
//	}
//
//	// �傫����␳
//	useful::LimitMinNum(m_infoCreate.fSize, editstage::SIZE);
//
//	// �傫���𔽉f
//	m_pCylinder->SetVec3Sizing(m_infoCreate.fSize);
//}
//
////===========================================
////	��������
////===========================================
//void CEditWeed::Create(void)
//{
//	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
//
//	// �z�u
//	if (pKeyboard->IsTrigger(KEY_CREATE))
//	{
//		// ���ۑ���ݒ�
//		m_bSave = false;
//
//		InitAllColor();
//	}
//}
//
////===========================================
////	�j������
////===========================================
//void CEditWeed::Release(void)
//{
//	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
//	bool bRelease = false;	// �j����
//
//	// �폜
//	if (pKeyboard->IsTrigger(KEY_RELEASE))
//	{
//		// �j�������Ԃ�ݒ�
//		bRelease = true;
//	}
//
//	// �폜����
//	DeleteCollision(bRelease);
//}
//
////===========================================
////	�폜����
////===========================================
//void CEditWeed::DeleteCollision(const bool bRelase)
//{
//
//	// �����̃V�����_�[�͑S���E�B�[�h
//
//
//	// ���X�g���擾
//	CListManager<CObjectMeshCylinder> *pListManager = CObjectMeshCylinder::GetList();
//	if (pListManager == nullptr) { return; }
//	std::list<CObjectMeshCylinder*> listChanger = pListManager->GetList();
//
//	// ���X�g����S�Ċm�F����
//	for (auto& rList : listChanger)
//	{
//		// �����A�h���X�������ꍇ����
//		if (rList == m_pCylinder) { continue; }
//
//		D3DXVECTOR3 posOther = rList->GetVec3Position();	// �Ώۂ̒n�ʈʒu
//		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// ���g�̑傫��
//		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// �Ώۂ̑傫��
//
//		// ���g�̑傫����ݒ�
//		D3DXVECTOR3 sizeThisChanger = m_pCylinder->GetVec3Sizing();	// ���g�̒n�ʂ̑傫��
//		sizeThis.x = sizeThisChanger.x;	// ����T�C�YX��ݒ�
//		sizeThis.y = sizeThisChanger.y;	// ����T�C�YY��ݒ�
//		sizeThis.z = sizeThisChanger.z;	// ����T�C�YZ��ݒ�
//		sizeThis *= 0.5f;				// ����T�C�Y�𔼕���
//
//		// �Ώۂ̑傫����ݒ�
//		D3DXVECTOR3 sizeOtherChanger = rList->GetVec3Sizing();	// �Ώۂ̒n�ʂ̑傫��
//		sizeOther.x = sizeOtherChanger.x;	// ����T�C�YX��ݒ�
//		sizeOther.y = sizeOtherChanger.y;	// ����T�C�YY��ݒ�
//		sizeOther.z = sizeOtherChanger.z;	// ����T�C�YZ��ݒ�
//		sizeOther *= 0.5f;				// ����T�C�Y�𔼕���
//
//		// ��`�̓����蔻��
//		if (collision::Box3D
//		( // ����
//			GetVec3Position(),	// ����ʒu
//			posOther,	// ����ڕW�ʒu
//			sizeThis,	// ����T�C�Y(�E�E��E��)
//			sizeThis,	// ����T�C�Y(���E���E�O)
//			sizeOther,	// ����ڕW�T�C�Y(�E�E��E��)
//			sizeOther,	// ����ڕW�T�C�Y(���E���E�O)
//			useful::RotToFourDire(GetVec3Rotation().y),			// ���������
//			useful::RotToFourDire(rList->GetVec3Rotation().y)	// ����ڕW������
//		))
//		{ // ������������ꍇ
//
//			if (bRelase)
//			{ // �j������ꍇ
//
//				// �I������
//				rList->Uninit();
//
//				// ���ۑ���ݒ�
//				m_bSave = false;
//			}
//			else
//			{ // �j�����Ȃ��ꍇ
//
//				// �Ԃ�ݒ�
//				rList->SetCubeColor(XCOL_RED);
//			}
//		}
//		else
//		{ // ����O�������ꍇ
//
//			// �ʏ�F��ݒ�
//			rList->SetCubeColor(XCOL_ACYAN);
//		}
//	}
//}
//
////===========================================
////	�ۑ�����
////===========================================
//HRESULT CEditWeed::Save(void)
//{
//
//	// �����̃V�����_�[�͑S����
//
//#if _DEBUG
//
//	// �n�ʂ̃��X�g���擾
//	CListManager<CObjectMeshCylinder> *pListManager = CObjectMeshCylinder::GetList();	// ���X�g�}�l�[�W���[
//	std::list<CObjectMeshCylinder*> listChanger;	// �n�ʃ��X�g
//	if (pListManager != nullptr)
//	{ // ���X�g�}�l�[�W���[����������Ă���ꍇ
//
//		// ���X�g���擾
//		listChanger = pListManager->GetList();
//	}
//
//	// �t�@�C�����J��
//	std::ofstream  file(SAVE_PASS);	// �t�@�C���X�g���[��
//	if (file.fail())
//	{ // �t�@�C�����J���Ȃ������ꍇ
//
//		// �G���[���b�Z�[�W�{�b�N�X
//		MessageBox(nullptr, "���̏����o���Ɏ��s^^", "�G���[��www", MB_ICONWARNING);
//
//		// ���s��Ԃ�
//		return E_FAIL;
//	}
//
//	// ���o���������o��
//	file << "#==============================================================================" << std::endl;
//	file << "#" << std::endl;
//	file << "#	���̃Z�[�u�f�[�^w [save_weed.txt]" << std::endl;
//	file << "#	Author : ���� ����" << std::endl;
//	file << "#" << std::endl;
//	file << "#==============================================================================" << std::endl;
//	file << "# ���̍s���牺���R�s�[�� [stage.txt] �ɒ���t��\n" << std::endl;
//
//	// �����_�����o���̕��@���w��
//	file << std::fixed << std::setprecision(DIGIT_FLOAT);
//
//	// �ǂݍ��݊J�n������������o��
//	file << "STAGE_WEEDSET\n" << std::endl;
//
//	for (const auto& rList : listChanger)
//	{ // �n�ʂ̑������J��Ԃ�
//
//		// �����A�h���X�������ꍇ����
//		if (rList == m_pCylinder) { continue; }
//
//		// �����o�������擾
//		CObjectMeshCylinder::EDirection dir = rList->GetDir(); // ����
//		CObjectMeshCylinder::ERotation rot = rList->GetRot(); // �p�x
//		D3DXVECTOR3 pos = rList->GetVec3Position(); // �ʒu
//		D3DXVECTOR3 size = rList->GetVec3Sizing(); // �傫��
//
//		// ���������o��
//		file << "	WEEDSET" << std::endl;
//		file << "		DIR		= " << dir << std::endl;
//		file << "		ROT		= " << rot << std::endl;
//		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
//		file << "		SIZE	= " << size.x << " " << size.y << " " << size.z << std::endl;
//		file << "	END_WEEDSET\n" << std::endl;
//	}
//
//	// �ǂݍ��ݏI��������������o��
//	file << "END_STAGE_WEEDSET" << std::endl;
//
//	// �폜����
//	DeleteCollision(false);
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
