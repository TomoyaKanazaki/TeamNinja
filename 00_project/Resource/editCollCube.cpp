//#if 1
////============================================================
////
////	�G�f�B�b�g�R���W�����L���[�u���� [editCollisionCube.cpp]
////	Author�F��������
////
////============================================================
////************************************************************
////	�C���N���[�h�t�@�C��
////************************************************************
//#include "manager.h"
//#include "editManager.h"
//#include "editCollCube.h"
//
//#include "objectMeshCube.h"
//
////************************************************************
////	�}�N����`
////************************************************************
//#define KEY_CREATE		(DIK_0)	// �����L�[
//#define NAME_CREATE		("0")	// �����\��
//#define KEY_RELEASE		(DIK_9)	// �j���L�[
//#define NAME_RELEASE	("9")	// �j���\��
//#define KEY_TYPE		(DIK_3)	// ��ޕύX�L�[
//#define NAME_TYPE		("3")	// ��ޕύX�\��
//
//#define KEY_UP_SIZE_X		(DIK_T)	// X���g��L�[
//#define NAME_UP_SIZE_X		("T")	// X���g��\��
//#define KEY_DOWN_SIZE_X		(DIK_G)	// X���k���L�[
//#define NAME_DOWN_SIZE_X	("G")	// X���k���\��
//#define KEY_UP_SIZE_Y		(DIK_Y)	// Y���g��L�[
//#define NAME_UP_SIZE_Y		("Y")	// Y���g��\��
//#define KEY_DOWN_SIZE_Y		(DIK_H)	// Y���k���L�[
//#define NAME_DOWN_SIZE_Y	("H")	// Y���k���\��
//#define KEY_UP_SIZE_Z		(DIK_U)	// Z���g��L�[
//#define NAME_UP_SIZE_Z		("U")	// Z���g��\��
//#define KEY_DOWN_SIZE_Z		(DIK_J)	// Z���k���L�[
//#define NAME_DOWN_SIZE_Z	("J")	// Z���k���\��
//
////************************************************************
////	�萔�錾
////************************************************************
//namespace
//{
//	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_collisioncube.txt";	// �Z�[�u�e�L�X�g�p�X
//	const char* ANGLE_NAME[] =	// �����̖��O
//	{
//		"0�x",		// �p�x�F0�x
//		"90�x",		// �p�x�F90�x
//		"180�x",	// �p�x�F180�x
//		"270�x",	// �p�x�F270�x
//	};
//
//	const D3DXVECTOR3 INIT_SIZE = D3DXVECTOR3(30.0f, 30.0f, 30.0f);			// �T�C�Y
//	const float	INIT_ALPHA = 0.5f;	// �z�u�O�̃��l
//	const int DIGIT_FLOAT = 2;		// �����_�ȉ��̌���
//}
//
////************************************************************
////	�X�^�e�B�b�N�A�T�[�g
////************************************************************
//static_assert(NUM_ARRAY(ANGLE_NAME) == CEditor::ANGLE_MAX, "ERROR : Angle Count Mismatch");
//
////************************************************************
////	�e�N���X [CEditCollisionCube] �̃����o�֐�
////************************************************************
////============================================================
////	�R���X�g���N�^
////============================================================
//CEditCollisionCube::CEditCollisionCube()
//{
//#if _DEBUG
//
//	// �����o�ϐ����N���A
//	m_pCube = nullptr;							// �L���[�u���
//	m_infoCreate.angle = CEditor::ANGLE_0;		// �t�B�[���h�z�u���
//	m_infoCreate.size = INIT_SIZE;				// �T�C�Y
//	m_infoCreate.fLength = 0.0f;				// ����
//	m_infoCreate.fAngle = 0.0f;					// ����
//	m_bSave = false;							// �ۑ���
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�f�X�g���N�^
////============================================================
//CEditCollisionCube::~CEditCollisionCube()
//{
//#if _DEBUG
//#endif	// _DEBUG
//}
//
////============================================================
////	����������
////============================================================
//HRESULT CEditCollisionCube::Init(void)
//{
//#if _DEBUG
//
//	// �����o�ϐ���������
//	m_pCube = nullptr;							// �L���[�u���
//	m_infoCreate.angle = CEditor::ANGLE_0;		// �t�B�[���h�z�u���
//	m_infoCreate.size = INIT_SIZE;				// �T�C�Y
//	m_infoCreate.fLength = 0.0f;				// ����
//	m_infoCreate.fAngle = 0.0f;					// ����
//	m_bSave = false;							// �ۑ���
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
//	// �L���[�u�̐���
//	D3DXCOLOR colField = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// �n�ʐF
//	m_pCube = CCollisionCube::Create
//	( // ����
//		VEC3_ZERO,
//		m_infoCreate.fLength,
//		m_infoCreate.fAngle,
//		m_infoCreate.size.x,
//		m_infoCreate.size.y,
//		m_infoCreate.size.z
//	);
//	if (m_pCube == nullptr)
//	{ // �����Ɏ��s�����ꍇ
//
//		// ���s��Ԃ�
//		assert(false);
//		return E_FAIL;
//}
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
//void CEditCollisionCube::Uninit(void)
//{
//#if _DEBUG
//
//	// �e�N���X�̏I��
//	CEditorObject::Uninit();
//
//	// �L���[�u�̐F�̑S������
//	InitAllColorCube();
//
//	// �L���[�u�̏I��
//	SAFE_UNINIT(m_pCube);
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�X�V����
////============================================================
//void CEditCollisionCube::Update(void)
//{
//#if _DEBUG
//
//	// �e�N���X�̍X�V
//	CEditorObject::Update();
//
//	// �傫���̍X�V
//	UpdateSizing();
//
//	// �L���[�u�̐���
//	CreateCube();
//
//	// �L���[�u�̔j��
//	ReleaseCube();
//
//	// �ʒu�𔽉f
//	m_pCube->GetCube()->SetVec3Position(GetVec3Position());
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�ۑ��󋵎擾����
////============================================================
//bool CEditCollisionCube::IsSave(void)
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
//void CEditCollisionCube::SaveInfo(void)
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
//void CEditCollisionCube::LoadInfo(void)
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
//void CEditCollisionCube::DrawDebugControl(void)
//{
//#if _DEBUG
//
//	// ����\���̕`��
//	CEditorObject::DrawDebugControl();
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, "�傫���F[%s/%s/%s/%s/%s/%s+%s]\n", NAME_UP_SIZE_X, NAME_DOWN_SIZE_X, NAME_UP_SIZE_Y, NAME_DOWN_SIZE_Y, NAME_UP_SIZE_Z, NAME_DOWN_SIZE_Z, NAME_TRIGGER);
//	DebugProc::Print(DebugProc::POINT_RIGHT, "��ޕύX�F[%s]\n", NAME_TYPE);
//	DebugProc::Print(DebugProc::POINT_RIGHT, "�폜�F[%s]\n", NAME_RELEASE);
//	DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);
//
//#endif	// _DEBUG
//}
//
////============================================================
////	���\���̕`�揈��
////============================================================
//void CEditCollisionCube::DrawDebugInfo(void)
//{
//#if _DEBUG
//
//	// ���\���̕`��
//	CEditorObject::DrawDebugInfo();
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, "%s�F[�����̎��]\n", ANGLE_NAME[m_infoCreate.angle]);
//	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f�F[�傫��]\n", m_infoCreate.size.x, m_infoCreate.size.y * 2, m_infoCreate.size.y);
//
//#endif	// _DEBUG
//}
//
////============================================================
//// �ʒu�X�V
////============================================================
//void CEditCollisionCube::UpdatePosition(void)
//{
//	// �ʒu�̍X�V
//	CEditorObject::UpdatePosition();
//
//	// �ʒu�𔽉f
//	m_pCube->GetCube()->SetVec3Position(GetVec3Position());
//}
//
////============================================================
////	�����X�V����
////============================================================
//void CEditCollisionCube::UpdateRotation(void)
//{
//	// �����̍X�V
//	CEditorObject::UpdateRotation();
//
//	// �����𔽉f
//	m_pCube->GetCube()->SetVec3Rotation(GetVec3Rotation());
//}
//
////============================================================
////	�傫���̍X�V����
////============================================================
//void CEditCollisionCube::UpdateSizing(void)
//{
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
//
//	// �傫����ύX
//	if (!pKeyboard->IsPress(KEY_TRIGGER))
//	{
//		if (pKeyboard->IsPress(KEY_UP_SIZE_X))
//		{
//			m_infoCreate.size.x += INIT_SIZE.x;
//		}
//		if (pKeyboard->IsPress(KEY_DOWN_SIZE_X))
//		{
//			m_infoCreate.size.x -= INIT_SIZE.x;
//		}
//		if (pKeyboard->IsPress(KEY_UP_SIZE_Y))
//		{
//			m_infoCreate.size.y += INIT_SIZE.y;
//		}
//		if (pKeyboard->IsPress(KEY_DOWN_SIZE_Y))
//		{
//			m_infoCreate.size.y -= INIT_SIZE.y;
//		}
//		if (pKeyboard->IsPress(KEY_UP_SIZE_Z))
//		{
//			m_infoCreate.size.z += INIT_SIZE.z;
//		}
//		if (pKeyboard->IsPress(KEY_DOWN_SIZE_Z))
//		{
//			m_infoCreate.size.z -= INIT_SIZE.z;
//		}
//	}
//	else
//	{
//		if (pKeyboard->IsTrigger(KEY_UP_SIZE_X))
//		{
//			m_infoCreate.size.x += INIT_SIZE.x;
//		}
//		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_X))
//		{
//			m_infoCreate.size.x -= INIT_SIZE.x;
//		}
//		if (pKeyboard->IsTrigger(KEY_UP_SIZE_Y))
//		{
//			m_infoCreate.size.y += INIT_SIZE.y;
//		}
//		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_Y))
//		{
//			m_infoCreate.size.y -= INIT_SIZE.y;
//		}
//		if (pKeyboard->IsTrigger(KEY_UP_SIZE_Z))
//		{
//			m_infoCreate.size.z += INIT_SIZE.z;
//		}
//		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_Z))
//		{
//			m_infoCreate.size.z -= INIT_SIZE.z;
//		}
//	}
//
//	// �傫����␳
//	useful::LimitMinNum(m_infoCreate.size.x, INIT_SIZE.x);
//	useful::LimitMinNum(m_infoCreate.size.y, INIT_SIZE.y);
//	useful::LimitMinNum(m_infoCreate.size.z, INIT_SIZE.z);
//
//	// �傫���𔽉f
//	m_pCube->GetCube()->SetVec3Sizing(m_infoCreate.size);
//}
//
////============================================================
////	�L���[�u�̐�������
////============================================================
//void CEditCollisionCube::CreateCube(void)
//{
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
//	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
//	D3DXCOLOR colField = XCOL_WHITE;			// �F�ۑ��p
//
//	// �L���[�u��z�u
//	if (pKeyboard->IsTrigger(KEY_CREATE))
//	{
//		//----------------------------------------------------
//		//	�L���[�u�̏���z�u�p�ɕύX
//		//----------------------------------------------------
//		// �����X�V�E�����`���ON�ɂ���
//		m_pCube->GetCube()->SetEnableUpdate(true);
//		m_pCube->GetCube()->SetEnableDraw(true);
//
//		// �F��ݒ�
//		colField = m_pCube->GetCube()->GetCubeColor();	// ���̐F���擾
//		m_pCube->GetCube()->SetCubeColor(D3DXCOLOR(colField.r, colField.g, colField.b, 1.0f));
//
//		// ���ۑ���ݒ�
//		m_bSave = false;
//
//		//----------------------------------------------------
//		//	�V�����L���[�u�̐���
//		//----------------------------------------------------
//		// �L���[�u�̐���
//		m_pCube = CCollisionCube::Create
//		( // ����
//			GetVec3Position(),
//			m_infoCreate.fLength,
//			m_infoCreate.fAngle,
//			m_infoCreate.size.x,
//			m_infoCreate.size.y,
//			m_infoCreate.size.z
//		);
//		assert(m_pCube != nullptr);
//
//		// �F��ݒ�
//		colField = m_pCube->GetCube()->GetCubeColor();	// ���̐F���擾
//		m_pCube->GetCube()->SetCubeColor(D3DXCOLOR(colField.r, colField.g, colField.b, INIT_ALPHA));
//	}
//}
//
////============================================================
////	�L���[�u�̔j������
////============================================================
//void CEditCollisionCube::ReleaseCube(void)
//{
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
//	bool bRelease = false;	// �j����
//
//	// �L���[�u���폜
//	if (pKeyboard->IsTrigger(KEY_RELEASE))
//	{
//		// �j�������Ԃ�ݒ�
//		bRelease = true;
//	}
//
//	// �L���[�u�̍폜����
//	DeleteCollisionCube(bRelease);
//}
//
////============================================================
////	�L���[�u�̍폜����
////============================================================
//void CEditCollisionCube::DeleteCollisionCube(const bool bRelase)
//{
//	CListManager<CField>* pListManager = CField::GetList();	// �L���[�u���X�g�}�l�[�W���[
//	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
//	std::list<CField*> listField = pListManager->GetList();	// �L���[�u���X�g���
//
//	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
//	for (auto& rList : listField)
//	{ // �L���[�u�����J��Ԃ�
//
//		// �����A�h���X�������ꍇ����
//		if (rList == m_pCube) { continue; }
//
//		D3DXVECTOR3 posOther = rList->GetVec3Position();	// �Ώۂ̒n�ʈʒu
//		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// ���g�̑傫��
//		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// �Ώۂ̑傫��
//
//		// ���g�̑傫����ݒ�
//		D3DXVECTOR2 sizeThisField = m_pCube->GetVec2Sizing();	// ���g�̒n�ʂ̑傫��
//		sizeThis.x = sizeThisField.x;	// ����T�C�YX��ݒ�
//		sizeThis.y = editstage::SIZE;	// ����T�C�YY��ݒ�
//		sizeThis.z = sizeThisField.y;	// ����T�C�YZ��ݒ�
//		sizeThis *= 0.5f;				// ����T�C�Y�𔼕���
//
//		// �Ώۂ̑傫����ݒ�
//		D3DXVECTOR2 sizeOtherField = rList->GetVec2Sizing();	// �Ώۂ̒n�ʂ̑傫��
//		sizeOther.x = sizeOtherField.x;	// ����T�C�YX��ݒ�
//		sizeOther.y = editstage::SIZE;	// ����T�C�YY��ݒ�
//		sizeOther.z = sizeOtherField.y;	// ����T�C�YZ��ݒ�
//		sizeOther *= 0.5f;				// ����T�C�Y�𔼕���
//
//		// ��`�̓����蔻��
//		if (collision::Box3D
//		( // ����
//			posEdit,	// ����ʒu
//			posOther,	// ����ڕW�ʒu
//			sizeThis,	// ����T�C�Y(�E�E��E��)
//			sizeThis,	// ����T�C�Y(���E���E�O)
//			sizeOther,	// ����ڕW�T�C�Y(�E�E��E��)
//			sizeOther	// ����ڕW�T�C�Y(���E���E�O)
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
//				rList->SetColor(XCOL_RED);
//			}
//		}
//		else
//		{ // ����O�������ꍇ
//
//			// �ʏ�F��ݒ�
//			rList->SetColor(XCOL_WHITE);
//		}
//	}
//}
//
////============================================================
////	�L���[�u�̐F�̑S����������
////============================================================
//void CEditCollisionCube::InitAllColorCube(void)
//{
//	CListManager<CField>* pListManager = CField::GetList();	// �L���[�u���X�g�}�l�[�W���[
//	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
//	std::list<CField*> listField = pListManager->GetList();	// �L���[�u���X�g���
//
//	for (auto& rList : listField)
//	{ // �L���[�u�����J��Ԃ�
//
//		// �����A�h���X�������ꍇ����
//		if (rList == m_pCube) { continue; }
//
//		// �ʏ�F��ݒ�
//		rList->SetColor(XCOL_WHITE);
//	}
//}
//
////============================================================
////	�ۑ�����
////============================================================
//HRESULT CEditCollisionCube::Save(void)
//{
//#if _DEBUG
//
//	// �n�ʂ̃��X�g���擾
//	CListManager<CField>* pListManager = CField::GetList();	// ���X�g�}�l�[�W���[
//	std::list<CField*> listField;	// �n�ʃ��X�g
//	if (pListManager != nullptr)
//	{ // ���X�g�}�l�[�W���[����������Ă���ꍇ
//
//		// ���X�g���擾
//		listField = pListManager->GetList();
//	}
//
//	// �t�@�C�����J��
//	std::ofstream  file(SAVE_PASS);	// �t�@�C���X�g���[��
//	if (file.fail())
//	{ // �t�@�C�����J���Ȃ������ꍇ
//
//		// �G���[���b�Z�[�W�{�b�N�X
//		MessageBox(nullptr, "�X�e�[�W�n�ʔz�u�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);
//
//		// ���s��Ԃ�
//		return E_FAIL;
//	}
//
//	// ���o���������o��
//	file << "#==============================================================================" << std::endl;
//	file << "#" << std::endl;
//	file << "#	�X�e�[�W�n�ʔz�u�̃Z�[�u�f�[�^ [save_collisioncube.txt]" << std::endl;
//	file << "#	Author : ���c �E��" << std::endl;
//	file << "#" << std::endl;
//	file << "#==============================================================================" << std::endl;
//	file << "# ���̍s���牺���R�s�[�� [stage.txt] �ɒ���t��\n" << std::endl;
//
//	// �L���[�u�̐F�̑S������
//	InitAllColorCube();
//
//	// �����_�����o���̕��@���w��
//	file << std::fixed << std::setprecision(DIGIT_FLOAT);
//
//	// �ǂݍ��݊J�n������������o��
//	file << "STAGE_CUBESET\n" << std::endl;
//
//	for (const auto& rList : listField)
//	{ // �n�ʂ̑������J��Ԃ�
//
//		// �����A�h���X�������ꍇ����
//		if (rList == m_pCube) { continue; }
//
//		// �����o�������擾
//		CField::EType type = rList->GetType();			// ���
//		D3DXVECTOR3 pos = rList->GetVec3Position();	// �ʒu
//		D3DXVECTOR3 rot = rList->GetVec3Rotation();	// ����
//		D3DXVECTOR2 size = rList->GetVec2Sizing();	// �傫��
//		D3DXCOLOR col = rList->GetColor();		// �F
//		POSGRID2 part = rList->GetPattern();		// ������
//		POSGRID2 texPart = rList->GetTexPattern();	// �e�N�X�`��������
//
//		// ������360�x�ɕϊ�
//		D3DXToDegree(rot);
//
//		// ���������o��
//		file << "	CUBESET" << std::endl;
//		file << "		TYPE	= " << type << std::endl;
//		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
//		file << "		ROT		= " << rot.x << " " << rot.y << " " << rot.z << std::endl;
//		file << "		SIZE	= " << size.x << " " << size.y << std::endl;
//		file << "		COL		= " << col.r << " " << col.g << " " << col.b << " " << col.a << std::endl;
//		file << "		PART	= " << part.x << " " << part.y << std::endl;
//		file << "		TEXPART	= " << texPart.x << " " << texPart.y << std::endl;
//		file << "	END_CUBESET\n" << std::endl;
//	}
//
//	// �ǂݍ��ݏI��������������o��
//	file << "END_STAGE_CUBESET" << std::endl;
//
//	// �L���[�u�̍폜����
//	DeleteCollisionCube(false);
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
