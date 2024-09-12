//============================================================
//
//	�J�ڃ|�C���g���� [transpoint.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "transpoint.h"
#include "collision.h"
#include "manager.h"
#include "stage.h"
#include "player.h"
#include "camera.h"
#include "balloon.h"
#include "balloonManager.h"
#include "sound.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char*	HIT_EFFECT_PASS		= "data\\EFFEKSEER\\stage_active.efkefc";	// �G��Ă���ۂ̃G�t�F�N�g�t�@�C��
	const char*	UNHIT_EFFECT_PASS	= "data\\EFFEKSEER\\stage_standby.efkefc";	// �G��Ă��Ȃ��ۂ̃G�t�F�N�g�t�@�C��
	const D3DXVECTOR3 OFFSET = D3DXVECTOR3(0.0f, 5.0f, 0.0f);	// �G�t�F�N�g�p�I�t�Z�b�g
	const int	PRIORITY	 = 2;		// �J�ڃ|�C���g�̗D�揇��
	const float	RADIUS		 = 120.0f;	// �J�ڃ|�C���g�ɐG����锼�a
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CTransPoint>* CTransPoint::m_pList	= nullptr;	// �I�u�W�F�N�g���X�g
CBalloonManager* CTransPoint::m_pBalloonManager	= nullptr;	// �����o���}�l�[�W���[���
CTransPoint* CTransPoint::m_pOpenTransPoint		= nullptr;	// ������ꂽ�J�ڃ|�C���g

//************************************************************
//	�q�N���X [CTransPoint] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTransPoint::CTransPoint(const char* pPass) : CObjectModel(CObject::LABEL_TRANSPOINT, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
	m_sTransMapPass		(pPass),		// �J�ڐ�}�b�v�p�X
	m_sScreenShotPass	(""),			// �X�N�V���p�X
	m_pEffectData		(nullptr),		// �ێ�����G�t�F�N�g���
	m_pBalloon			(nullptr),		// �����o�����
	m_state				(STATE_NORMAL),	// ���
	m_bOpen				(false),		// �X�e�[�W����t���O
	m_fCurTime			(0.0f)			// ���݂̎���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTransPoint::~CTransPoint()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTransPoint::Init(void)
{
	// �����o�ϐ���������
	m_sScreenShotPass = "";		// �X�N�V���p�X
	m_pEffectData = nullptr;	// �ێ�����G�t�F�N�g���
	m_pBalloon	= nullptr;		// �����o�����
	m_state		= STATE_NORMAL;	// ���
	m_bOpen		= false;		// �X�e�[�W����t���O
	m_fCurTime	= 0.0f;			// ���݂̎���

	std::filesystem::path fsPath(m_sTransMapPass);				// �J�ڐ�}�b�v�p�X
	std::filesystem::path fsOpen(fsPath.parent_path());			// ����p�X�̃f�B���N�g��
	fsOpen.append("open.txt");									// �f�B���N�g���ɉ���t���O�̃x�[�X�l�[���ǉ�
	std::filesystem::path fsScreenShot(fsPath.parent_path());	// �X�N�V���̃f�B���N�g��
	fsScreenShot.append("screenshot.txt");						// �f�B���N�g���ɃX�N�V���̃x�[�X�l�[���ǉ�

	// ����󋵂̓Ǎ�
	LoadOpen(fsOpen.string().c_str(), &m_bOpen);

	// �X�N�V���p�X�̓Ǎ�
	LoadScreenShot(fsScreenShot.string().c_str(), &m_sScreenShotPass);

	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����o���̐���
	m_pBalloon = CBalloon::Create(GetVec3Position());
	if (m_pBalloon == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (GET_STAGE->GetOpenMapDirectory() == fsPath.parent_path().string())
	{ // ������ꂽ�}�b�v�̃f�B���N�g���ƈ�v�����ꍇ

		// �������Ȃ���Ԃɂ���
		m_state = STATE_NONE;

		// ������ꂽ�J�ڃ|�C���g��ۑ�
		assert(m_pOpenTransPoint == nullptr);
		m_pOpenTransPoint = this;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CTransPoint>::Create();
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

//============================================================
//	�I������
//============================================================
void CTransPoint::Uninit(void)
{
	// �G�t�F�N�g�̔j��
	SAFE_DELETE(m_pEffectData);

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// �����o���}�l�[�W���[�̏I��
		SAFE_UNINIT(m_pBalloonManager);

		// ������ꂽ�J�ڃ|�C���g�̏�����
		m_pOpenTransPoint = nullptr;

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CTransPoint::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update(fDeltaTime);

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:

		if (GET_CAMERA->GetState() == CCamera::STATE_NONE)
		{ // �J�����̉��o���I������ꍇ

			if (m_pBalloonManager == nullptr)
			{ // �����o���}�l�[�W���[���������̏ꍇ

				// �X�e�[�W���e�N�X�`���쐬
				CreateStageTexture();

				// �����o���\����ON�ɂ���
				m_pBalloon->SetFirstDisp();

				// ���񉉏o���J�n����
				m_pBalloonManager->SetFirstStag();

				// �J������Ԃɂ���
				m_state = STATE_CAMERA;
			}
		}
		break;

	case STATE_CAMERA:

		if (m_pBalloon->IsSizeDisp())
		{ // �J�������삪�I�������ꍇ

			// ���݂̎��Ԃ����Z
			m_fCurTime += fDeltaTime;
			if (m_fCurTime >= 1.0f)
			{
				// ���݂̎��Ԃ�������
				m_fCurTime = 0.0f;

				// �t�F�[�h���J�n����
				GET_MANAGER->GetFade()->SetFade(3.0f, 1.5f, 7, XCOL_AWHITE);

				// �t�F�[�h��Ԃɂ���
				m_state = STATE_FADE;
			}
		}
		break;

	case STATE_FADE:

		if (GET_MANAGER->GetFade()->IsFadeIn())
		{
			// �����o���}�l�[�W���[�̏I��
			SAFE_UNINIT(m_pBalloonManager);

			// ��荞�݃J�����ɑJ��
			GET_CAMERA->SetState(CCamera::STATE_AROUND);
			GET_CAMERA->SetDestAround();

			// �v���C���[��ʏ��Ԃɂ���
			GET_PLAYER->SetState(CPlayer::STATE_SELECT_NORMAL);

			// �ʏ��Ԃɂ���
			m_state = STATE_NORMAL;
		}
		break;

	case STATE_NORMAL:
		break;

	default:
		assert(false);
		break;
	}

	// �����o�����������̏ꍇ������
	if (m_pBalloonManager == nullptr) { return; }

	if (m_pBalloonManager->IsNone()
	&&  m_pBalloon->IsSizeDisp())
	{ // ���o�̊J�n�^�C�~���O�̏ꍇ

		if (m_state != STATE_NORMAL)
		{ // �����Ԃ̏ꍇ

			// ���񉉏o���J�n����
			m_pBalloonManager->SetFirstStag();
		}
		else
		{ // �ʏ��Ԃ̏ꍇ

			// ���o���J�n����
			m_pBalloonManager->SetStag();
		}
	}

	// �J�ڃ|�C���g�̍X�V
	m_pBalloonManager->Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CTransPoint::Draw(CShader *pShader)
{
	CToonShader	*pToonShader = CToonShader::GetInstance();	// �g�D�[���V�F�[�_�[���
	if (pToonShader->IsEffectOK())
	{ // �G�t�F�N�g���g�p�\�ȏꍇ

		// �I�u�W�F�N�g���f���̕`��
		CObjectModel::Draw(pToonShader);
	}
	else
	{ // �G�t�F�N�g���g�p�s�\�ȏꍇ

		// �G�t�F�N�g�G���[
		assert(false);

		// �I�u�W�F�N�g���f���̕`��
		CObjectModel::Draw(pShader);
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CTransPoint::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// ���g�̈ʒu��ݒ�
	CObjectModel::SetVec3Position(rPos);

	// �����o���̈ʒu��ݒ�
	m_pBalloon->SetVec3Position(rPos);
}

//============================================================
//	��������
//============================================================
CTransPoint *CTransPoint::Create(const char* pPass, const D3DXVECTOR3& rPos)
{
	// �J�ڃ|�C���g�̐���
	CTransPoint *pTransPoint = new CTransPoint(pPass);
	if (pTransPoint == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �J�ڃ|�C���g�̏�����
		if (FAILED(pTransPoint->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �J�ڃ|�C���g�̔j��
			SAFE_DELETE(pTransPoint);
			return nullptr;
		}

		// �ʒu��ݒ�
		pTransPoint->SetVec3Position(rPos);

		// �G�t�F�N�g��ݒ�
		pTransPoint->m_pEffectData = GET_EFFECT->Create(UNHIT_EFFECT_PASS, rPos + OFFSET, VEC3_ZERO, VEC3_ZERO, 50.0f, true,false);

		// �m�ۂ����A�h���X��Ԃ�
		return pTransPoint;
	}
}

//============================================================
//	�J�ڃ|�C���g�Ƃ̓����蔻��
//============================================================
CTransPoint* CTransPoint::Collision(const D3DXVECTOR3& rPos, const float fRadius)
{
	// �J�ڃ|�C���g���Ȃ��ꍇ������
	if (m_pList == nullptr) { return nullptr; }

	CTransPoint* pHitTransPoint = nullptr;	// �q�b�g�����J�ڃ|�C���g
	std::list<CTransPoint*> list = m_pList->GetList();	// �������X�g
	for (const auto& rList : list)
	{ // �v�f�����J��Ԃ�

		D3DXVECTOR3 posTrans = rList->GetVec3Position();	// �J�ڃ|�C���g�ʒu

		// XZ���ʂ̉~�̓����蔻��
		bool bHit = collision::Circle2D
		(
			rPos,		// ����ʒu
			posTrans,	// ����ڕW�ʒu
			fRadius,	// ���蔼�a
			RADIUS		// ����ڕW���a
		);
		if (bHit && pHitTransPoint == nullptr)
		{ // �������Ă��銎�܂��ǂ̑J�ڃ|�C���g�Ƃ��������Ă��Ȃ��ꍇ

			// �������Ă���J�ڃ|�C���g��ۑ�
			pHitTransPoint = rList;

			// �����o���\����ON�ɂ���
			rList->m_pBalloon->SetDisp(true);

			if (rList->m_pEffectData->Path != HIT_EFFECT_PASS)
			{ // �q�b�g���̃G�t�F�N�g�ł͂Ȃ��ꍇ

				// �q�b�g���̃G�t�F�N�g��ݒ�
				SAFE_DELETE(rList->m_pEffectData);
				rList->m_pEffectData = GET_EFFECT->Create(HIT_EFFECT_PASS, posTrans + OFFSET, VEC3_ZERO, VEC3_ZERO, 75.0f, true,false);

				// �X�e�[�W���e�N�X�`���̍쐬
				if (FAILED(rList->CreateStageTexture()))
				{ // �����Ɏ��s�����ꍇ

					// ���s��Ԃ�
					assert(false);
					return nullptr;
				}

				// ���n���̍Đ�
				PLAY_SOUND(CSound::LABEL_SE_DECISION_000);
			}
		}
		else
		{ // �������Ă��Ȃ��ꍇ

			// �����o���\����OFF�ɂ���
			rList->m_pBalloon->SetDisp(false);

			if (rList->m_pEffectData->Path != UNHIT_EFFECT_PASS)
			{ // ���q�b�g���̃G�t�F�N�g�ł͂Ȃ��ꍇ

				// ���q�b�g���̃G�t�F�N�g��ݒ�
				SAFE_DELETE(rList->m_pEffectData);
				rList->m_pEffectData = GET_EFFECT->Create(UNHIT_EFFECT_PASS, posTrans + OFFSET, VEC3_ZERO, VEC3_ZERO, 50.0f, true,false);
			}
		}
	}

	if (pHitTransPoint != nullptr)
	{ // �J�ڃ|�C���g�ɐG��Ă����ꍇ

		if (!pHitTransPoint->m_bOpen)
		{ // �������Ă��Ȃ��J�ڃ|�C���g�̏ꍇ

			// ����Ȃ���nulllptr��Ԃ�
			return nullptr;
		}
	}

	// �ŏI�I�ɓ������Ă���J�ڃ|�C���g��Ԃ�
	return pHitTransPoint;
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CTransPoint> *CTransPoint::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	�X�e�[�W���e�N�X�`���̍쐬����
//============================================================
HRESULT CTransPoint::CreateStageTexture(void)
{
	// �����o���}�l�[�W���[�̏I��
	SAFE_UNINIT(m_pBalloonManager);

	// �����o���}�l�[�W���[�̐���
	m_pBalloonManager = CBalloonManager::Create(this, m_sScreenShotPass.c_str(), m_bOpen);
	if (m_pBalloonManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
HRESULT CTransPoint::LoadSetup(const char* pPass)
{
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	std::string sTrans;	// �J�ڐ�}�b�v�p�X�̑���p

	// �t�@�C�����J��
	std::ifstream file(pPass);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�J�ڃ|�C���g�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �t�@�C����Ǎ�
	std::string str;	// �Ǎ�������
	while (file >> str)
	{ // �t�@�C���̏I�[�ł͂Ȃ��ꍇ���[�v

		if (str.front() == '#')
		{ // �R�����g�A�E�g����Ă���ꍇ

			// ��s�S�ēǂݍ���
			std::getline(file, str);
		}
		else if (str == "STAGE_TRANSSET")
		{
			do
			{ // END_STAGE_TRANSSET��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str.front() == '#')
				{ // �R�����g�A�E�g����Ă���ꍇ

					// ��s�S�ēǂݍ���
					std::getline(file, str);
				}
				else if (str == "TRANSSET")
				{
					do
					{ // END_TRANSSET��ǂݍ��ނ܂Ń��[�v

						// �������ǂݍ���
						file >> str;

						if (str == "TRANS_PASS")
						{
							file >> str;	// ����Ǎ�
							file >> sTrans;	// �J�ڐ�}�b�v�p�X��Ǎ�
						}
						else if (str == "POS")
						{
							file >> str;	// ����Ǎ�

							// �ʒu��Ǎ�
							file >> pos.x;
							file >> pos.y;
							file >> pos.z;
						}
					} while (str != "END_TRANSSET");	// END_TRANSSET��ǂݍ��ނ܂Ń��[�v

					// �J�ڃ|�C���g�̐���
					if (CTransPoint::Create(sTrans.c_str(), pos) == nullptr)
					{ // �m�ۂɎ��s�����ꍇ

						// ���s��Ԃ�
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_TRANSSET");	// END_STAGE_TRANSSET��ǂݍ��ނ܂Ń��[�v
		}
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	����t���O�̓Ǎ�����
//============================================================
HRESULT CTransPoint::LoadOpen(const char* pPass, bool *pOpen)
{
	// �t�@�C�����J��
	std::ifstream file(pPass);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "����t���O�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ����󋵂̕ۑ�
		if (FAILED(SaveOpen(pPass, false)))
		{ // �ۑ��Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}
	}

	// �t�@�C����Ǎ�
	std::string sStr;	// �Ǎ�������
	while (file >> sStr)
	{ // �t�@�C���̏I�[�ł͂Ȃ��ꍇ���[�v

		if (sStr.front() == '#')
		{ // �R�����g�A�E�g����Ă���ꍇ

			// ��s�S�ēǂݍ���
			std::getline(file, sStr);
		}
		else if (sStr == "FLAG")
		{
			file >> sStr;	// ����Ǎ�
			file >> sStr;	// �t���O��Ǎ�

			// �t���O�������ϊ�
			*pOpen = (sStr == "TRUE");
		}
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	����t���O�̕ۑ�����
//============================================================
HRESULT CTransPoint::SaveOpen(const char* pPass, const bool bOpen)
{
	// �t�@�C�����J��
	std::ofstream file(pPass);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "����t���O�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���o���������o��
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	�}�b�v����̃Z�b�g�A�b�v [open.txt]" << std::endl;
	file << "#	Author : ���c �E��" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "#------------------------------------------------------------------------------" << std::endl;
	file << "#	����t���O" << std::endl;
	file << "#------------------------------------------------------------------------------" << std::endl;

	// �t���O�������o��
	std::string sFlag = (bOpen) ? "TRUE" : "FALSE";	// �t���O������
	file << "FLAG = " << sFlag << std::endl;

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�N�V���p�X�̓Ǎ�����
//============================================================
HRESULT CTransPoint::LoadScreenShot(const char* pPass, std::string* pStrPath)
{
	// �t�@�C�����J��
	std::ifstream file(pPass);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�X�N�V���p�X�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// �X�N�V���p�X�̕ۑ�
		if (FAILED(SaveScreenShot(pPass)))
		{ // �ۑ��Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}
	}

	// �t�@�C����Ǎ�
	std::string sStr;	// �Ǎ�������
	while (file >> sStr)
	{ // �t�@�C���̏I�[�ł͂Ȃ��ꍇ���[�v

		if (sStr.front() == '#')
		{ // �R�����g�A�E�g����Ă���ꍇ

			// ��s�S�ēǂݍ���
			std::getline(file, sStr);
		}
		else if (sStr == "SCREENSHOT_PATH")
		{
			file >> sStr;		// ����Ǎ�
			file >> *pStrPath;	// �p�X��Ǎ�
		}
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�N�V���p�X�̕ۑ�����
//============================================================
HRESULT CTransPoint::SaveScreenShot(const char* pPass)
{
	// �t�@�C�����J��
	std::ofstream file(pPass);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�X�N�V���p�X�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���o���������o��
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	�X�e�[�W�X�N�V���̃Z�b�g�A�b�v [screenshot.txt]" << std::endl;
	file << "#	Author : ���c �E��" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "#------------------------------------------------------------------------------" << std::endl;
	file << "#	�X�e�[�W�X�N�V���p�X" << std::endl;
	file << "#------------------------------------------------------------------------------" << std::endl;
	file << "SCREENSHOT_PATH = data\\TEXTURE\\stage000.png" << std::endl;

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}
