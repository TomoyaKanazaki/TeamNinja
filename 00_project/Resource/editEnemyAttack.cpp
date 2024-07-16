//#if 1
////============================================================
////
////	�G�f�B�b�g�U���G���� [editEnemyAttack.cpp]
////	Author�F��������
////
////============================================================
////************************************************************
////	�C���N���[�h�t�@�C��
////************************************************************
//#include "editEnemyAttack.h"
//#include "editManager.h"
//#include "manager.h"
//#include "collision.h"
//#include "useful.h"
//#include "stage.h"
//
//#include "enemyNavigation.h"
//#include "enemyNavRandom.h"
//
////************************************************************
////	�}�N����`
////************************************************************
//#define KEY_CREATE		(DIK_0)	// �����L�[
//#define NAME_CREATE		("0")	// �����\��
//#define KEY_RELEASE		(DIK_9)	// �j���L�[
//#define NAME_RELEASE	("9")	// �j���\��
//#define KEY_TYPE		(DIK_4)	// ��ޕύX�L�[
//#define NAME_TYPE		("4")	// ��ޕύX�\��
//
//#define KEY_ROTA_RIGHT	(DIK_Z)	// �E��]�L�[
//#define NAME_ROTA_RIGHT	("Z")	// �E��]�\��
//#define KEY_ROTA_LEFT	(DIK_C)	// ����]�L�[
//#define NAME_ROTA_LEFT	("C")	// ����]�\��
//
////************************************************************
////	�萔�錾
////************************************************************
//namespace
//{
//	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_enemyAttack.txt";	// �Z�[�u�e�L�X�g�p�X
//
//	const float	INIT_ALPHA = 0.5f;	// �z�u�O�̃��l
//	const int DIGIT_FLOAT = 2;		// �����_�ȉ��̌���
//
//	const float INIT_MOVE_WIDTH = 30.0f;		// �ړ��͈͂̏�����
//	const float INIT_MOVE_DEPTH = 30.0f;		// �ړ��͈͂̏������s
//	const float INIT_CHASE_WIDTH = 30.0f;		// �ǐՔ͈͂̏�����
//	const float INIT_CHASE_DEPTH = 30.0f;		// �ǐՔ͈͂̏������s
//
//	const float ROT_MOVE = 0.02f;	// �����̈ړ���
//}
//
////************************************************************
////	�e�N���X [CEditEnemyAttack] �̃����o�֐�
////************************************************************
////============================================================
////	�R���X�g���N�^
////============================================================
//CEditEnemyAttack::CEditEnemyAttack(CEditStage* pEditor) : CEditorObject(pEditor)
//{
//#if _DEBUG
//
//	// �����o�ϐ����N���A
//	m_pEnemy = nullptr;	// �U���G���
//	m_bSave = false;	// �ۑ���
//	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// �U���G�z�u���
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�f�X�g���N�^
////============================================================
//CEditEnemyAttack::~CEditEnemyAttack()
//{
//#if _DEBUG
//#endif	// _DEBUG
//}
//
////============================================================
////	����������
////============================================================
//HRESULT CEditEnemyAttack::Init(void)
//{
//#if _DEBUG
//
//	// �����o�ϐ���������
//	m_pEnemy = nullptr;								// ���
//	m_infoCreate.type = (CEnemyAttack::EType)0;		// ���
//	m_infoCreate.fMoveWidth = INIT_MOVE_WIDTH;		// �ړ��͈͕�
//	m_infoCreate.fMoveDepth = INIT_MOVE_DEPTH;		// �ړ��͈͉��s
//	m_infoCreate.fChaseWidth = INIT_CHASE_WIDTH;	// �ǐՔ͈͕�
//	m_infoCreate.fChaseDepth = INIT_CHASE_DEPTH;	// �ǐՔ͈͉��s
//	m_bSave = false;								// �ۑ���
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
//	// �U���G�̐���
//	D3DXCOLOR colActor = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// �n�ʐF
//	m_pEnemy = CEnemyAttack::Create
//	( // ����
//		GetVec3Position(),			// �ʒu
//		GetVec3Rotation(),			// ����
//		m_infoCreate.type,			// ���
//		m_infoCreate.fMoveWidth,	// �ړ���
//		m_infoCreate.fMoveDepth,	// �ړ����s
//		m_infoCreate.fChaseWidth,	// �ǐՕ�
//		m_infoCreate.fChaseDepth	// �ǐՉ��s
//	);
//	if (m_pEnemy == nullptr)
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
//void CEditEnemyAttack::Uninit(void)
//{
//#if _DEBUG
//
//	// �e�N���X�̏I��
//	CEditorObject::Uninit();
//
//	// �U���G�̐F�̑S������
//	InitAllColorActor();
//
//	// �U���G�̏I��
//	SAFE_UNINIT(m_pEnemy);
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�X�V����
////============================================================
//void CEditEnemyAttack::Update(void)
//{
//#if _DEBUG
//
//	// �e�N���X�̍X�V
//	CEditorObject::Update();
//
//	// ��ނ̕ύX
//	ChangeType();
//
//	// �U���G�̐���
//	CreateActor();
//
//	// �U���G�̔j��
//	ReleaseActor();
//
//	// �ʒu�𔽉f
//	m_pEnemy->SetVec3Position(GetVec3Position());
//
//#endif	// _DEBUG
//}
//
////============================================================
////	�ۑ��󋵎擾����
////============================================================
//bool CEditEnemyAttack::IsSave(void)
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
//void CEditEnemyAttack::SaveInfo(void)
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
//void CEditEnemyAttack::LoadInfo(void)
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
//void CEditEnemyAttack::DrawDebugControl(void)
//{
//#if _DEBUG
//
//	// ����\���̕`��
//	CEditorObject::DrawDebugControl();
//
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
//void CEditEnemyAttack::DrawDebugInfo(void)
//{
//#if _DEBUG
//
//	// ���\���̕`��
//	CEditorObject::DrawDebugInfo();
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, "%d�F[���]\n", m_infoCreate.type);
//
//#endif	// _DEBUG
//}
//
////============================================================
////	��ޕύX�̍X�V����
////============================================================
//void CEditEnemyAttack::ChangeType(void)
//{
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
//
//	// ��ނ�ύX
//	if (pKeyboard->IsTrigger(KEY_TYPE))
//	{
//		m_infoCreate.type = (CEnemyAttack::EType)((m_infoCreate.type + 1) % CEnemyAttack::TYPE_MAX);
//
//		// ���f���𐶐�������
//		m_pEnemy->Uninit();
//		m_pEnemy = CEnemyAttack::Create
//		(
//			GetVec3Position(),			// �ʒu
//			GetVec3Rotation(),			// ����
//			m_infoCreate.type,			// ���
//			m_infoCreate.fMoveWidth,	// �ړ���
//			m_infoCreate.fMoveDepth,	// �ړ����s
//			m_infoCreate.fChaseWidth,	// �ǐՕ�
//			m_infoCreate.fChaseDepth	// �ǐՉ��s
//		);
//	}
//}
//
////============================================================
////	�U���G�̐�������
////============================================================
//void CEditEnemyAttack::CreateActor(void)
//{
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
//
//	// �U���G��z�u
//	if (pKeyboard->IsTrigger(KEY_CREATE))
//	{
//		//----------------------------------------------------
//		//	�U���G�̏���z�u�p�ɕύX
//		//----------------------------------------------------
//		// �����X�V�E�����`���ON�ɂ���
//		m_pEnemy->SetEnableUpdate(true);
//		m_pEnemy->SetEnableDraw(true);
//
//		// ���ۑ���ݒ�
//		m_bSave = false;
//
//		//----------------------------------------------------
//		//	�V�����U���G�̐���
//		//----------------------------------------------------
//		// �U���G�̐���
//		m_pEnemy = CEnemyAttack::Create
//		( // ����
//			GetVec3Position(),			// �ʒu
//			GetVec3Rotation(),			// ����
//			m_infoCreate.type,			// ���
//			m_infoCreate.fMoveWidth,	// �ړ���
//			m_infoCreate.fMoveDepth,	// �ړ����s
//			m_infoCreate.fChaseWidth,	// �ǐՕ�
//			m_infoCreate.fChaseDepth	// �ǐՉ��s
//		);
//		assert(m_pEnemy != nullptr);
//	}
//}
//
////============================================================
////	�U���G�̔j������
////============================================================
//void CEditEnemyAttack::ReleaseActor(void)
//{
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
//	bool bRelease = false;	// �j����
//
//	// �U���G���폜
//	if (pKeyboard->IsTrigger(KEY_RELEASE))
//	{
//		// �j�������Ԃ�ݒ�
//		bRelease = true;
//	}
//
//	// �U���G�̍폜����
//	DeleteCollisionActor(bRelease);
//}
//
////============================================================
////	�U���G�̍폜����
////============================================================
//void CEditEnemyAttack::DeleteCollisionActor(const bool bRelase)
//{
//	CListManager<CEnemyAttack>* pListManager = CEnemyAttack::GetList();	// �U���G���X�g�}�l�[�W���[
//	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
//	std::list<CEnemyAttack*> listActor = pListManager->GetList();	// �U���G���X�g���
//
//	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
//	for (auto& rList : listActor)
//	{ // �U���G�����J��Ԃ�
//
//		// �����A�h���X�������ꍇ����
//		if (rList == m_pEnemy) { continue; }
//
//		D3DXVECTOR3 posOther = rList->GetVec3Position();	// �Ώۂ̒n�ʈʒu
//		D3DXVECTOR3 scaleThis = VEC3_ZERO;	// ���g�̑傫��
//		D3DXVECTOR3 scaleOther = VEC3_ZERO;	// �Ώۂ̑傫��
//
//		// ���g�̑傫����ݒ�
//		D3DXVECTOR3 scaleThisActor = m_pEnemy->GetVec3Scaling();	// ���g�̒n�ʂ̑傫��
//		scaleThis.x = scaleThisActor.x;	// ����T�C�YX��ݒ�
//		scaleThis.y = scaleThisActor.y;	// ����T�C�YY��ݒ�
//		scaleThis.z = scaleThisActor.z;	// ����T�C�YZ��ݒ�
//		scaleThis *= 0.5f;				// ����T�C�Y�𔼕���
//
//		// �Ώۂ̑傫����ݒ�
//		D3DXVECTOR3 scaleOtherActor = rList->GetVec3Scaling();		// �Ώۂ̒n�ʂ̑傫��
//		scaleOther.x = scaleOtherActor.x;	// ����T�C�YX��ݒ�
//		scaleOther.y = scaleOtherActor.y;	// ����T�C�YY��ݒ�
//		scaleOther.z = scaleOtherActor.z;	// ����T�C�YZ��ݒ�
//		scaleOther *= 0.5f;				// ����T�C�Y�𔼕���
//
//		// ��`�̓����蔻��
//		if (collision::Box3D
//		( // ����
//			posEdit,	// ����ʒu
//			posOther,	// ����ڕW�ʒu
//			scaleThis,	// ����T�C�Y(�E�E��E��)
//			scaleThis,	// ����T�C�Y(���E���E�O)
//			scaleOther,	// ����ڕW�T�C�Y(�E�E��E��)
//			scaleOther	// ����ڕW�T�C�Y(���E���E�O)
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
//				rList->SetAllMaterial(material::Red());
//			}
//		}
//		else
//		{ // ����O�������ꍇ
//
//			// �ʏ�F��ݒ�
//			rList->ResetMaterial();
//		}
//	}
//}
//
////============================================================
////	�U���G�̐F�̑S����������
////============================================================
//void CEditEnemyAttack::InitAllColorActor(void)
//{
//	CListManager<CEnemyAttack>* pListManager = CEnemyAttack::GetList();	// �U���G���X�g�}�l�[�W���[
//	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
//	std::list<CEnemyAttack*> listActor = pListManager->GetList();	// �U���G���X�g���
//
//	for (auto& rList : listActor)
//	{ // �U���G�����J��Ԃ�
//
//		// �����A�h���X�������ꍇ����
//		if (rList == m_pEnemy) { continue; }
//
//		// �ʏ�F��ݒ�
//		rList->ResetMaterial();
//	}
//}
//
////============================================================
////	�ۑ�����
////============================================================
//HRESULT CEditEnemyAttack::Save(void)
//{
//#if _DEBUG
//
//	// �n�ʂ̃��X�g���擾
//	CListManager<CEnemyAttack>* pListManager = CEnemyAttack::GetList();	// ���X�g�}�l�[�W���[
//	std::list<CEnemyAttack*> listActor;	// �n�ʃ��X�g
//	if (pListManager != nullptr)
//	{ // ���X�g�}�l�[�W���[����������Ă���ꍇ
//
//		// ���X�g���擾
//		listActor = pListManager->GetList();
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
//	file << "#	�U���G�z�u�̃Z�[�u�f�[�^ [save_actor.txt]" << std::endl;
//	file << "#	Author : ���c �E��" << std::endl;
//	file << "#" << std::endl;
//	file << "#==============================================================================" << std::endl;
//	file << "# ���̍s���牺���R�s�[�� [enemyAttack.txt] �ɒ���t��\n" << std::endl;
//
//	// �U���G�̐F�̑S������
//	InitAllColorActor();
//
//	// �����_�����o���̕��@���w��
//	file << std::fixed << std::setprecision(DIGIT_FLOAT);
//
//	// �ǂݍ��݊J�n������������o��
//	file << "STAGE_ENEMYATTCKSET\n" << std::endl;
//
//	for (const auto& rList : listActor)
//	{ // �n�ʂ̑������J��Ԃ�
//
//		// �����A�h���X�������ꍇ����
//		if (rList == m_pEnemy) { continue; }
//
//		// �����o�������擾
//		ENavType navType;
//		CEnemyAttack::EType type = rList->GetType();	// ���
//		D3DXVECTOR3 pos = rList->GetVec3Position();		// �ʒu
//		D3DXVECTOR3 rot = rList->GetVec3Rotation();		// ����
//		D3DXVECTOR3 scale = rList->GetVec3Scaling();	// �傫��
//		float fMoveWidth = rList->GetNavigation();
//
//		if (typeid(*rList->GetNavigation()) == typeid(CEnemyNavRandom))
//		{ // �����_���i�r�̏ꍇ
//
//			// �i�r�̎�ނ�ݒ�
//			navType = NAVTYPE_RANDOM;
//		}
//		else
//		{ // ��L�ȊO
//
//			// �i�r�̎�ނ�ݒ�
//			navType = NAVTYPE_STREET;
//		}
//
//		// ������360�x�ɕϊ�
//		rot = D3DXToDegree(rot);
//
//			m_infoCreate.fMoveWidth,	// �ړ���
//			m_infoCreate.fMoveDepth,	// �ړ����s
//			m_infoCreate.fChaseWidth,	// �ǐՕ�
//			m_infoCreate.fChaseDepth	// �ǐՉ��s
//
//		// ���������o��
//		file << "	ENEMYATTCKSET" << std::endl;
//		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
//		file << "		ROT		= " << rot.x << " " << rot.y << " " << rot.z << std::endl;
//		file << "		TYPE	= " << type << std::endl;
//		file << "		SCALE	= " << scale.x << " " << scale.y << " " << scale.z << std::endl;
//		file << "	END_ENEMYATTCKSET\n" << std::endl;
//	}
//
//	// �ǂݍ��ݏI��������������o��
//	file << "END_STAGE_ENEMYATTCKSET" << std::endl;
//
//	// �U���G�̍폜����
//	DeleteCollisionActor(false);
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
