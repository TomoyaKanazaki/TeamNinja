//============================================================
//
//	�X�e�[�W���� [stage.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "stage.h"
#include "manager.h"
#include "texture.h"
#include "collision.h"
#include "retentionManager.h"

#include "field.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "liquid.h"
#include "camera_change.h"
#include "enemyAttack.h"
#include "coin.h"
#include "godItem.h"
#include "actor.h"
#include "gimmick.h"
#include "touchActor.h"
#include "player.h"
#include "checkpoint.h"
#include "goal.h"
#include "transpoint.h"
#include "weed.h"
#include "tutorial.h"
#include "fog_change.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* INIT_MAPPASS = "data\\MAP\\LOGO\\map.txt";	// �����}�b�v�p�X
}

//************************************************************
//	�萔�錾
//************************************************************
//	�e�N���X [CStage] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CStage::CStage()
{
	// �����o�ϐ����N���A
	memset(&m_limit, 0, sizeof(m_limit));	// �͈͏��

	// �}�b�v�p�X�A�z�z����N���A
	m_mapPass.clear();
}

//============================================================
//	�f�X�g���N�^
//============================================================
CStage::~CStage()
{

}

//============================================================
//	����������
//============================================================
HRESULT CStage::Init(void)
{
	// �����o�ϐ���������
	memset(&m_limit, 0, sizeof(m_limit));	// �͈͏��
	m_sInitMapPass = INIT_MAPPASS;			// ���������ɐ�������}�b�v�p�X

	// �}�b�v�p�X�A�z�z���������
	m_mapPass.clear();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CStage::Uninit(void)
{
	// �}�b�v�p�X�A�z�z����N���A
	m_mapPass.clear();
}

//============================================================
//	�}�b�v�p�X�̓o�^����
//============================================================
CStage::SPass CStage::Regist(const char* pMapPass)
{
	// ���ɐ����ς݂�������
	auto itr = m_mapPass.find(pMapPass);	// �����̃t�H���g������
	if (itr != m_mapPass.end())
	{ // �����ς݂̏ꍇ

		// �Ǎ��ς݂̃t�H���g����Ԃ�
		return itr->second;
	}

	// �}�b�v�p�X����Ǎ�
	SPass tempPass;	// �}�b�v�p�X���
	if (FAILED(LoadPass(pMapPass, &tempPass)))
	{ // �Ǎ��Ɏ��s�����ꍇ

		// �����l��Ԃ�
		assert(false);
		return {};
	}

	// �t�H���g����ۑ�
	m_mapPass.insert(std::make_pair(pMapPass, tempPass));

	// ���������}�b�v�p�X����Ԃ�
	return tempPass;
}

//============================================================
//	�X�e�[�W�̊������� (�p�X)
//============================================================
HRESULT CStage::BindStage(const char* pPass)
{
	// �X�e�[�W�p�X���Ȃ��ꍇ������
	if (pPass == nullptr) { return S_OK; }

	// �X�e�[�W�̊���
	return BindStage(Regist(pPass));
}

//============================================================
//	�X�e�[�W�̊������� (���)
//============================================================
HRESULT CStage::BindStage(const SPass& rPass)
{
	// ���蓖�Ă�}�b�v�̃f�B���N�g����ۑ�
	std::filesystem::path path = rPass.sMap;
	m_sCurMapFolder = path.parent_path().string();

	// �X�e�[�W�̃Z�b�g�A�b�v�̓Ǎ�
	if (FAILED(LoadSetup(rPass.sStage.c_str())))
	{ // �ǂݍ��݂Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �A�N�^�[�̃Z�b�g�A�b�v�̓Ǎ�
	if (!rPass.sActor.empty())		// �p�X���w�肳��Ă���ꍇ
	if (FAILED(CActor::LoadSetup(rPass.sActor.c_str())))
	{ // �Z�b�g�A�b�v�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �M�~�b�N�̃Z�b�g�A�b�v�̓Ǎ�
	if (!rPass.sGimmick.empty())	// �p�X���w�肳��Ă���ꍇ
	if (FAILED(CGimmick::LoadSetup(rPass.sGimmick.c_str())))
	{ // �Z�b�g�A�b�v�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �^�b�`�A�N�^�[�̃Z�b�g�A�b�v�̓Ǎ�
	if(!rPass.sTouchActor.empty())	// �p�X���w�肳��Ă���ꍇ
	if (FAILED(CTouchActor::LoadSetup(rPass.sTouchActor.c_str())))
	{ // �Z�b�g�A�b�v�Ɏ��s�����ꍇ

		//���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �G�̃Z�b�g�A�b�v�̓Ǎ�
	if (!rPass.sEnemyAtc.empty())	// �p�X���w�肳��Ă���ꍇ
	if (FAILED(CEnemyAttack::LoadSetup(rPass.sEnemyAtc.c_str())))
	{ // �Z�b�g�A�b�v�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �R�C���̃Z�b�g�A�b�v�̓Ǎ�
	if(!rPass.sCoin.empty())	// �p�X���w�肳��Ă���ꍇ
	if (FAILED(CCoin::LoadSetup(rPass.sCoin.c_str())))
	{ // �Z�b�g�A�b�v�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �_��̃Z�b�g�A�b�v�̓Ǎ�
	if (!rPass.sGodItem.empty())	// �p�X���w�肳��Ă���ꍇ
	if (FAILED(CGodItem::LoadSetup(rPass.sGodItem.c_str())))
	{ // �Z�b�g�A�b�v�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (!rPass.sPoint.empty())
	{ // �p�X���w�肳��Ă���ꍇ

		// �`�F�b�N�|�C���g�̃Z�b�g�A�b�v�̓Ǎ�
		if (FAILED(CCheckPoint::LoadSetup(rPass.sPoint.c_str())))
		{ // �Z�b�g�A�b�v�Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �S�[���|�C���g�̃Z�b�g�A�b�v�̓Ǎ�
		if (FAILED(CGoal::LoadSetup(rPass.sPoint.c_str())))
		{ // �Z�b�g�A�b�v�Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �J�ڃ|�C���g�̃Z�b�g�A�b�v�̓Ǎ�
		if (FAILED(CTransPoint::LoadSetup(rPass.sPoint.c_str())))
		{ // �Z�b�g�A�b�v�Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �v���C���[�̃Z�b�g�A�b�v�̓Ǎ�
		if (FAILED(CPlayer::LoadSetup(rPass.sPoint.c_str())))
		{ // �Z�b�g�A�b�v�Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �`���[�g���A���̐���
		if (rPass.sMap.find("FOREST00") != std::string::npos)
		{
			CTutorial::Create(D3DXVECTOR3(-400.0f, 0.0f, -300.0f), CTutorial::TYPE_MOVE);
			CTutorial::Create(D3DXVECTOR3(2500.0f, 0.0f, -300.0f), CTutorial::TYPE_CLONE);
			CTutorial::Create(D3DXVECTOR3(4000.0f, 0.0f, -300.0f), CTutorial::TYPE_GIMMICK);
			CTutorial::Create(D3DXVECTOR3(12300.0f, 650.0f, -150.0f), CTutorial::TYPE_GUARD);
			//CTutorial::Create(D3DXVECTOR3(12900.0f, 650.0f, -150.0f), CTutorial::TYPE_DOGDE);
		}

		// �t�H�O�ύX�_�̐���
		if (rPass.sMap.find("FOREST02") != std::string::npos)
		{
			CFogChanger::Create();
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	���݃}�b�v�̃X�e�[�W�p�X�擾����
//============================================================
std::string CStage::GetCurMapStagePass(void) const
{
	// �}�b�v�f�B���N�g���ɃX�e�[�W�p�X��ǉ����Ԃ�
	std::filesystem::path path = m_sCurMapFolder;	// �}�b�v�f�B���N�g��
	path.append("stage.txt");	// �X�e�[�W�p�X��ǉ�
	return path.string();		// �����񉻂��ĕԂ�
}

//============================================================
//	���݃}�b�v�̐_��p�X�擾����
//============================================================
std::string CStage::GetCurMapGodItemPass(void) const
{
	// �}�b�v�f�B���N�g���ɐ_��p�X��ǉ����Ԃ�
	std::filesystem::path path = m_sCurMapFolder;	// �}�b�v�f�B���N�g��
	path.append("goditem.txt");	// �_��p�X��ǉ�
	return path.string();		// �����񉻂��ĕԂ�
}

//============================================================
//	���݃}�b�v�̐_��ۑ��p�X�擾����
//============================================================
std::string CStage::GetCurMapSaveGodItemPass(void) const
{
	// �}�b�v�f�B���N�g���ɐ_��ۑ��p�X��ǉ����Ԃ�
	std::filesystem::path path = m_sCurMapFolder;	// �}�b�v�f�B���N�g��
	path.append("goditem.bin");	// �_��ۑ��p�X��ǉ�
	return path.string();		// �����񉻂��ĕԂ�
}

//============================================================
//	���݃}�b�v�̃����L���O�ۑ��p�X�擾����
//============================================================
std::string CStage::GetCurMapSaveRankPass(void) const
{
	// �}�b�v�f�B���N�g���Ƀ����L���O�ۑ��p�X��ǉ����Ԃ�
	std::filesystem::path path = m_sCurMapFolder;	// �}�b�v�f�B���N�g��
	path.append("ranking.bin");	// �����L���O�ۑ��p�X��ǉ�
	return path.string();		// �����񉻂��ĕԂ�
}

//============================================================
//	�͈͊O�̒��n����
//============================================================
bool CStage::LandLimitPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight)
{
	if (rPos.y - fHeight < m_limit.fField)
	{ // �ʒu���n�ʂ�艺�̏ꍇ

		// �ʒu��␳
		rPos.y = m_limit.fField + fHeight;

		// �ړ��ʂ�������
		rMove.y = 0.0f;

		// ���n���Ă���󋵂�Ԃ�
		return true;
	}

	// ���n���Ă��Ȃ��󋵂�Ԃ�
	return false;
}

//============================================================
//	�n�ʂ̒��n����
//============================================================
bool CStage::LandFieldPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rOldPos, D3DXVECTOR3& rMove, CField** ppCollField)
{
	CListManager<CField> *pListManager = CField::GetList();	// �t�B�[���h���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return false; }			// ���X�g���g�p�̏ꍇ������
	std::list<CField*> listField = pListManager->GetList();	// �t�B�[���h���X�g���
	CField *pCurField = nullptr;	// ���n�\��̒n��
	float fCurPos = m_limit.fField;	// ���n�\���Y���W
	bool bLand = false;				// �n�ʂ̒��n����

	for (auto& rList : listField)
	{ // �n�ʂ̑������J��Ԃ�

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // �n�ʂ͈͓̔��̏ꍇ

			float fPosHeight = rList->GetPositionHeight(rPos);	// ���nY���W
			if (fPosHeight <= rOldPos.y && fPosHeight > rPos.y)
			{
				if (fCurPos <= fPosHeight)
				{ // ���݂̒��n�\��Y���W��荂���ʒu�ɂ���ꍇ

					// ���n�\��̒n�ʂ��X�V
					pCurField = rList;

					// ���n�\���Y���W���X�V
					fCurPos = fPosHeight;
				}
			}
		}
	}

	if (pCurField == nullptr)
	{ // ���n�\��̒n�ʂ����݂��Ȃ��ꍇ

		if (ppCollField != nullptr)
		{ // �n�ʂ̕ۑ��A�h���X������ꍇ

			// �n�ʖ�����ۑ�
			*ppCollField = nullptr;
		}
	}
	else
	{ // ���n�\��̒n�ʂ����݂���ꍇ

		// ���b�V���̒��n�󋵂�ۑ�
		bLand = pCurField->LandPosition(rPos, rMove);
		if (ppCollField != nullptr)
		{ // �n�ʂ̕ۑ��A�h���X������ꍇ

			if (bLand)
			{ // ���n���Ă���ꍇ

				// ���n�����n�ʂ�ۑ�
				*ppCollField = pCurField;
			}
		}
	}

	// ���n�����Ԃ�
	return bLand;
}

//============================================================
//	��ԏ�̒n�ʒ��n����
//============================================================
bool CStage::LandFieldPositionTop(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, CField** ppCollField)
{
	CListManager<CField> *pListManager = CField::GetList();	// �t�B�[���h���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return false; }			// ���X�g���g�p�̏ꍇ������
	std::list<CField*> listField = pListManager->GetList();	// �t�B�[���h���X�g���
	CField *pCurField = nullptr;	// ���n�\��̒n��
	float fCurPos = m_limit.fField;	// ���n�\���Y���W
	bool bLand = false;				// �n�ʂ̒��n����

	for (auto& rList : listField)
	{ // �n�ʂ̑������J��Ԃ�

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // �n�ʂ͈͓̔��̏ꍇ

			float fPosHeight = rList->GetPositionHeight(rPos);	// ���nY���W
			if (fCurPos <= fPosHeight)
			{ // ���݂̒��n�\��Y���W��荂���ʒu�ɂ���ꍇ

				// ���n�\��̒n�ʂ��X�V
				pCurField = rList;

				// ���n�\���Y���W���X�V
				fCurPos = fPosHeight;
			}
		}
	}

	if (pCurField == nullptr)
	{ // ���n�\��̒n�ʂ����݂��Ȃ��ꍇ������

		if (ppCollField != nullptr)
		{ // �n�ʂ̕ۑ��A�h���X������ꍇ

			// �n�ʖ�����ۑ�
			*ppCollField = pCurField;
		}
	}
	else
	{ // ���n�\��̒n�ʂ����݂��Ȃ��ꍇ������

		// ���b�V���̒��n�󋵂�ۑ�
		bLand = pCurField->LandPosition(rPos, rMove);
		if (ppCollField != nullptr)
		{ // �n�ʂ̕ۑ��A�h���X������ꍇ

			if (bLand)
			{ // ���n���Ă���ꍇ

				// ���n�����n�ʂ�ۑ�
				*ppCollField = pCurField;
			}
		}
	}

	// ���n�����Ԃ�
	return bLand;
}

//============================================================
//	�ǂƂ̓����蔻��
//============================================================
bool CStage::CollisionWall(D3DXVECTOR3& rPos, D3DXVECTOR3& rPosOld, const float fRadius, const float fHeight, D3DXVECTOR3& rMove, bool* pJump)
{
	CListManager<CWall>* pListManager = CWall::GetList();	// �t�B�[���h���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return false; }			// ���X�g���g�p�̏ꍇ������
	std::list<CWall*> listWall = pListManager->GetList();	// �t�B�[���h���X�g���
	bool bHit = false;			// �q�b�g��

	for (auto& rList : listWall)
	{ // �n�ʂ̑������J��Ԃ�

		assert(rList != nullptr);

		// �����蔻�菈��
		if (!rList->Collision(rPos, rPosOld, fRadius, fHeight, rMove, pJump)) { continue; }

		// �q�b�g�󋵂� true �ɂ���
		bHit = true;
	}

	// �q�b�g�󋵂�Ԃ�
	return bHit;
}

//============================================================
//	�ʒu�̕␳����
//============================================================
void CStage::LimitPosition(D3DXVECTOR3& rPos, const float fRadius)
{
	switch (m_limit.mode)
	{ // �������[�h���Ƃ̏���
	case LIMIT_NONE:	// �����Ȃ�

		// �������Ȃ��ꍇ�֐����Ăяo���K�v���Ȃ�
		//assert(false);

		break;

	case LIMIT_BOX:		// ��`�͈�

		// �p���̓�������
		collision::InBoxPillar
		( // ����
			rPos,				// ����ʒu
			VEC3_ZERO,			// ���茴�_�ʒu
			VEC3_ALL(fRadius),	// ����T�C�Y(�E�E��E��)
			VEC3_ALL(fRadius),	// ����T�C�Y(���E���E�O)
			D3DXVECTOR3(fabsf(m_limit.fRight), 0.0f, fabsf(m_limit.fFar)),	// ���茴�_�T�C�Y(�E�E��E��)
			D3DXVECTOR3(fabsf(m_limit.fLeft), 0.0f, fabsf(m_limit.fNear))	// ���茴�_�T�C�Y(���E���E�O)
		);

		break;

	case LIMIT_CIRCLE:	// �~�͈�

		// �~���̓�������
		collision::InCirclePillar
		( // ����
			rPos,			// ����ʒu
			VEC3_ZERO,		// ���茴�_�ʒu
			fRadius,		// ���蔼�a
			m_limit.fRadius	// ���茴�_���a
		);

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�L��Y���W�Ƃ̓����蔻��
//============================================================
bool CStage::CollisionKillY(const D3DXVECTOR3& rPos)
{
	if (rPos.y < m_limit.fKillY)
	{ // �ʒu���폜�̐����ʒu��艺�̏ꍇ

		// ���S���Ă���󋵂�Ԃ�
		return true;
	}

	// ���S���Ă��Ȃ��󋵂�Ԃ�
	return false;
}

//============================================================
//	�n�ʂ͈͓̔��̎擾����
//============================================================
bool CStage::IsFieldPositionRange(const D3DXVECTOR3& rPos)
{
	CListManager<CField> *pListManager = CField::GetList();	// �t�B�[���h���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return false; }			// ���X�g���g�p�̏ꍇ������
	std::list<CField*> listField = pListManager->GetList();	// �t�B�[���h���X�g���

	for (auto& rList : listField)
	{ // �n�ʂ̑������J��Ԃ�

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // �n�ʂ͈͓̔��̏ꍇ

			// �͈͓��ɂ����Ԃ�Ԃ�
			return true;
		}
	}

	// �͈͓��ɂ��Ȃ���Ԃ�Ԃ�
	return false;
}

//============================================================
//	�n�ʂ̒��n�ʒu�̎擾����
//============================================================
float CStage::GetFieldPositionHeight(const D3DXVECTOR3& rPos)
{
	CListManager<CField> *pListManager = CField::GetList();	// �t�B�[���h���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return false; }			// ���X�g���g�p�̏ꍇ������
	std::list<CField*> listField = pListManager->GetList();	// �t�B�[���h���X�g���
	CField *pCurField = nullptr;	// ���n�\��̒n��
	float fCurPos = m_limit.fField;	// ���n�\���Y���W

	for (auto& rList : listField)
	{ // �n�ʂ̑������J��Ԃ�

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // �n�ʂ͈͓̔��̏ꍇ

			float fPosHeight = rList->GetPositionHeight(rPos);	// ���nY���W
			if (fCurPos <= fPosHeight)
			{ // ���݂̒��n�\��Y���W��荂���ʒu�ɂ���ꍇ

				// ���n�\��̒n�ʂ��X�V
				pCurField = rList;

				// ���n�\���Y���W���X�V
				fCurPos = fPosHeight;
			}
		}
	}

	if (pCurField != nullptr)
	{ // ���n�\��̒n�ʂ����݂���ꍇ

		// ���n�ʒu��Ԃ�
		return fCurPos;
	}
	else
	{ // ���n�\��̒n�ʂ����݂��Ȃ��ꍇ

		// �����ʒu��Ԃ�
		return rPos.y;
	}
}

//============================================================
//	���b�V���̒��n�ʒu�̎擾���� (���݈ʒu���͉�)
//============================================================
float CStage::GetFieldDownPositionHeight(const D3DXVECTOR3& rPos)
{
	CListManager<CField> *pListManager = CField::GetList();	// �t�B�[���h���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return false; }			// ���X�g���g�p�̏ꍇ������
	std::list<CField*> listField = pListManager->GetList();	// �t�B�[���h���X�g���
	CField *pCurField = nullptr;	// ���n�\��̒n��
	float fCurPos = m_limit.fField;	// ���n�\���Y���W

	for (auto& rList : listField)
	{ // �n�ʂ̑������J��Ԃ�

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // �n�ʂ͈͓̔��̏ꍇ

			float fPosHeight = rList->GetPositionHeight(rPos);	// ���nY���W
			if (fCurPos <= fPosHeight && rPos.y >= fPosHeight)
			{ // ���݂̒��n�\��Y���W��荂���ʒu�ɂ���ꍇ

				// ���n�\��̒n�ʂ��X�V
				pCurField = rList;

				// ���n�\���Y���W���X�V
				fCurPos = fPosHeight;
			}
		}
	}

	if (pCurField != nullptr)
	{ // ���n�\��̒n�ʂ����݂���ꍇ

		// ���n�ʒu��Ԃ�
		return fCurPos;
	}
	else
	{ // ���n�\��̒n�ʂ����݂��Ȃ��ꍇ

		// �����ʒu��Ԃ�
		return rPos.y;
	}
}

//============================================================
//	��������
//============================================================
CStage *CStage::Create(void)
{
	// �X�e�[�W�̐���
	CStage *pStage = new CStage;
	if (pStage == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �X�e�[�W�̏�����
		if (FAILED(pStage->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �X�e�[�W�̔j��
			SAFE_DELETE(pStage);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pStage;
	}
}

//============================================================
//	�j������
//============================================================
void CStage::Release(CStage *&prStage)
{
	// �X�e�[�W�̏I��
	assert(prStage != nullptr);
	prStage->Uninit();

	// �������J��
	SAFE_DELETE(prStage);
}

//============================================================
//	�p�X���̓Ǎ�����
//============================================================
HRESULT CStage::LoadPass(const char* pMapPass, SPass* pPassInfo)
{
	// �t�@�C�����J��
	std::ifstream file(pMapPass);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�}�b�v�p�X�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �}�b�v�p�X��ۑ�
	pPassInfo->sMap = pMapPass;

	// �t�@�C����Ǎ�
	std::string str;	// �Ǎ�������
	while (file >> str)
	{ // �t�@�C���̏I�[�ł͂Ȃ��ꍇ���[�v

		if (str.front() == '#')
		{ // �R�����g�A�E�g����Ă���ꍇ

			// ��s�S�ēǂݍ���
			std::getline(file, str);
		}
		else if (str == "STAGE_PASS")
		{
			file >> str;	// ����Ǎ�
			file >> str;	// �X�e�[�W�Ǎ��p�X��Ǎ�

			// �X�e�[�W�Ǎ��p�X��ۑ�
			pPassInfo->sStage = str;
		}
		else if (str == "ENEMYATTACK_PASS")
		{
			file >> str;	// ����Ǎ�
			file >> str;	// �X�e�[�W�Ǎ��p�X��Ǎ�

			// �G���p�X��ۑ�
			pPassInfo->sEnemyAtc = str;
		}
		else if (str == "COIN_PASS")
		{
			file >> str;	// ����Ǎ�
			file >> str;	// �R�C���Ǎ��p�X��Ǎ�

			// �R�C�����p�X��ۑ�
			pPassInfo->sCoin = str;
		}
		else if (str == "GODITEM_PASS")
		{
			file >> str;	// ����Ǎ�
			file >> str;	// �_��Ǎ��p�X��Ǎ�

			// �_����p�X��ۑ�
			pPassInfo->sGodItem = str;
		}
		else if (str == "ACTOR_PASS")
		{
			file >> str;	// ����Ǎ�
			file >> str;	// �A�N�^�[�Ǎ��p�X��Ǎ�

			// �A�N�^�[�Ǎ��p�X��ۑ�
			pPassInfo->sActor = str;
		}
		else if (str == "GIMMICK_PASS")
		{
			file >> str;	// ����Ǎ�
			file >> str;	// �M�~�b�N�Ǎ��p�X��Ǎ�

			// �M�~�b�N�Ǎ��p�X��ۑ�
			pPassInfo->sGimmick = str;
		}
		else if (str == "TOUCH_ACTOR_PASS")
		{
			file >> str;	// ����Ǎ�
			file >> str;	// �^�b�`�A�N�^�[�Ǎ��p�X��Ǎ�

			// �^�b�`�A�N�^�[�Ǎ��p�X��ۑ�
			pPassInfo->sTouchActor = str;
		}
		else if (str == "POINT_PASS")
		{
			file >> str;	// ����Ǎ�
			file >> str;	// �|�C���g�Ǎ��p�X��Ǎ�

			// �|�C���g�Ǎ��p�X��ۑ�
			pPassInfo->sPoint = str;
		}
		else if (str == "WEED_PASS")
		{
			file >> str;	// ����Ǎ�
			file >> str;	// �|�C���g�Ǎ��p�X��Ǎ�

			// �|�C���g�Ǎ��p�X��ۑ�
			pPassInfo->sPoint = str;
		}
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
HRESULT CStage::LoadSetup(const char* pPass)
{
	// �ϐ���錾
	int nEnd = 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(pPass, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		while (1)
		{ // EOF�܂Ŗ������[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �I�[�̏ꍇ EOF��Ԃ�
			if (nEnd == EOF)
			{ // �t�@�C�����I�[�̏ꍇ

				// �����𔲂���
				break;
			}

			// ����}�b�v�f�B���N�g���̓Ǎ�
			if (FAILED(LoadOpen(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �͈͏��̓Ǎ�
			if (FAILED(LoadLimit(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �n�ʂ̓Ǎ�
			else if (FAILED(LoadField(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �ǂ̓Ǎ�
			else if (FAILED(LoadWall(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �i�F�̓Ǎ�
			else if (FAILED(LoadScenery(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// ��̓Ǎ�
			else if (FAILED(LoadSky(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �t�̂̓Ǎ�
			else if (FAILED(LoadLiquid(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}
			
			// �J�����ύX�n�_�̓Ǎ�
			else if (FAILED(LoadChanger(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// ���̓Ǎ�
			else if (FAILED(LoadWeed(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}
		}
		
		// �t�@�C�������
		fclose(pFile);

		// ������Ԃ�
		return S_OK;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�X�e�[�W�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}
}

//============================================================
//	����}�b�v�f�B���N�g���̓Ǎ�����
//============================================================
HRESULT CStage::LoadOpen(const char* pString, FILE *pFile)
{
	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	if (pString == nullptr || pFile == nullptr)
	{ // ������E�t�@�C�������݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�e�[�W�͈͂̐ݒ�
	if (strcmp(pString, "OPEN_PATH") == 0)
	{ // �ǂݍ��񂾕����� OPEN_PATH �̏ꍇ

		fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
		fscanf(pFile, "%s", &aString[0]);	// ����}�b�v�f�B���N�g����ǂݍ���

		// ����}�b�v�f�B���N�g����ۑ�
		m_sOpenMapFolder = &aString[0];

		// ����}�b�v�̉���t���O�p�X���쐬
		std::filesystem::path fsPath = m_sOpenMapFolder;
		fsPath.append("open.txt");

		// ����t���O�̎擾
		bool bOpen = false;
		CTransPoint::LoadOpen(fsPath.string().c_str(), &bOpen);

		if (bOpen)
		{ // ���ɉ���ς݂̏ꍇ

			// �p�X���N���A
			m_sOpenMapFolder.clear();
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�͈͏��̓Ǎ�����
//============================================================
HRESULT CStage::LoadLimit(const char* pString, FILE *pFile)
{
	// �ϐ���錾
	SLimit limit;	// �X�e�[�W�͈͂̑���p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	if (pString == nullptr || pFile == nullptr)
	{ // ������E�t�@�C�������݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�e�[�W�͈͂̐ݒ�
	if (strcmp(pString, "LIMITSET") == 0)
	{ // �ǂݍ��񂾕����� LIMITSET �̏ꍇ

		// �������[�h�𖳂��ɐݒ�
		limit.mode = LIMIT_NONE;

		do
		{ // �ǂݍ��񂾕����� END_LIMITSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "CENTER") == 0)
			{ // �ǂݍ��񂾕����� CENTER �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.center.x);	// ���S���WX��ǂݍ���
				fscanf(pFile, "%f", &limit.center.y);	// ���S���WY��ǂݍ���
				fscanf(pFile, "%f", &limit.center.z);	// ���S���WZ��ǂݍ���
			}
			else if (strcmp(&aString[0], "NEAR") == 0)
			{ // �ǂݍ��񂾕����� NEAR �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.fNear);		// �O�ʒu��ǂݍ���

				// �������[�h����`�͈͂ɐݒ�
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "FAR") == 0)
			{ // �ǂݍ��񂾕����� FAR �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.fFar);		// ��ʒu��ǂݍ���

				// �������[�h����`�͈͂ɐݒ�
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "RIGHT") == 0)
			{ // �ǂݍ��񂾕����� RIGHT �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.fRight);		// �E�ʒu��ǂݍ���

				// �������[�h����`�͈͂ɐݒ�
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "LEFT") == 0)
			{ // �ǂݍ��񂾕����� LEFT �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.fLeft);		// ���ʒu��ǂݍ���

				// �������[�h����`�͈͂ɐݒ�
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "RADIUS") == 0)
			{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.fRadius);	// ���a��ǂݍ���

				// �������[�h���~�͈͂ɐݒ�
				limit.mode = LIMIT_CIRCLE;
			}
			else if (strcmp(&aString[0], "FIELD") == 0)
			{ // �ǂݍ��񂾕����� FIELD �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.fField);		// �n�ʈʒu��ǂݍ���
			}
			else if (strcmp(&aString[0], "KILLY") == 0)
			{ // �ǂݍ��񂾕����� KILLY �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.fKillY);		// �폜�ʒu��ǂݍ���
			}
		} while (strcmp(&aString[0], "END_LIMITSET") != 0);	// �ǂݍ��񂾕����� END_LIMITSET �ł͂Ȃ��ꍇ���[�v

		// �X�e�[�W�͈͂̐ݒ�
		SetLimit(limit);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�n�ʏ��̓Ǎ�����
//============================================================
HRESULT CStage::LoadField(const char* pString, FILE *pFile)
{
	// �ϐ���錾
	int nType = 0;					// ��ނ̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	D3DXVECTOR2 size = VEC2_ZERO;	// �傫���̑���p
	D3DXCOLOR col = XCOL_WHITE;		// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p
	POSGRID2 texPart = GRID2_ZERO;	// �e�N�X�`���������̑���p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	if (pString == nullptr || pFile == nullptr)
	{ // ������E�t�@�C�������݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �n�ʂ̐ݒ�
	if (strcmp(pString, "STAGE_FIELDSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_FIELDSET �̏ꍇ

		do
		{ // �ǂݍ��񂾕����� END_STAGE_FIELDSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "FIELDSET") == 0)
			{ // �ǂݍ��񂾕����� FIELDSET �̏ꍇ
	
				do
				{ // �ǂݍ��񂾕����� END_FIELDSET �ł͂Ȃ��ꍇ���[�v
	
					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ
	
						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &size.x);		// �傫��X��ǂݍ���
						fscanf(pFile, "%f", &size.y);		// �傫��Y��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // �ǂݍ��񂾕����� TEXPART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &texPart.x);	// �e�N�X�`������������ǂݍ���
						fscanf(pFile, "%d", &texPart.y);	// �e�N�X�`���c��������ǂݍ���
					}
				} while (strcmp(&aString[0], "END_FIELDSET") != 0);	// �ǂݍ��񂾕����� END_FIELDSET �ł͂Ȃ��ꍇ���[�v

				// �n�ʃI�u�W�F�N�g�̐���
				if (CField::Create((CField::EType)nType, pos, D3DXToRadian(rot), size, col, part, texPart) == nullptr)
				{ // �m�ۂɎ��s�����ꍇ
	
					// ���s��Ԃ�
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_FIELDSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_FIELDSET �ł͂Ȃ��ꍇ���[�v
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�Ǐ��̓Ǎ�����
//============================================================
HRESULT CStage::LoadWall(const char* pString, FILE *pFile)
{
	// �ϐ���錾
	int nType = 0;					// ��ނ̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	D3DXVECTOR2 size = VEC2_ZERO;	// �傫���̑���p
	D3DXCOLOR col = XCOL_WHITE;		// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p
	POSGRID2 texPart = GRID2_ZERO;	// �e�N�X�`���������̑���p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	if (pString == nullptr || pFile == nullptr)
	{ // ������E�t�@�C�������݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �ǂ̐ݒ�
	if (strcmp(pString, "STAGE_WALLSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_WALLSET �̏ꍇ

		do
		{ // �ǂݍ��񂾕����� END_STAGE_SCENERYSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "WALLSET") == 0)
			{ // �ǂݍ��񂾕����� WALLSET �̏ꍇ
	
				do
				{ // �ǂݍ��񂾕����� END_WALLSET �ł͂Ȃ��ꍇ���[�v
	
					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ
	
						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &size.x);		// �傫��X��ǂݍ���
						fscanf(pFile, "%f", &size.y);		// �傫��Y��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // �ǂݍ��񂾕����� TEXPART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &texPart.x);	// �e�N�X�`������������ǂݍ���
						fscanf(pFile, "%d", &texPart.y);	// �e�N�X�`���c��������ǂݍ���
					}
				} while (strcmp(&aString[0], "END_WALLSET") != 0);	// �ǂݍ��񂾕����� END_WALLSET �ł͂Ȃ��ꍇ���[�v

				// �ǃI�u�W�F�N�g�̐���
				if (CWall::Create((CWall::EType)nType, pos, D3DXToRadian(rot), size, col, part, texPart) == nullptr)
				{ // �m�ۂɎ��s�����ꍇ

					// ���s��Ԃ�
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_WALLSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_WALLSET �ł͂Ȃ��ꍇ���[�v
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�i�F���̓Ǎ�����
//============================================================
HRESULT CStage::LoadScenery(const char* pString, FILE *pFile)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	D3DXCOLOR col = XCOL_WHITE;		// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p
	POSGRID2 texPart = GRID2_ZERO;	// �e�N�X�`���������̑���p

	int nType = 0;			// ��ނ̑���p
	float fRadius = 0.0f;	// ���a�̑���p
	float fHeight = 0.0f;	// �c���̑���p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	if (pString == nullptr || pFile == nullptr)
	{ // ������E�t�@�C�������݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �i�F�̐ݒ�
	if (strcmp(pString, "STAGE_SCENERYSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_SCENERYSET �̏ꍇ

		do
		{ // �ǂݍ��񂾕����� END_STAGE_SCENERYSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SCENERYSET") == 0)
			{ // �ǂݍ��񂾕����� SCENERYSET �̏ꍇ
	
				do
				{ // �ǂݍ��񂾕����� END_SCENERYSET �ł͂Ȃ��ꍇ���[�v
	
					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ
	
						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // �ǂݍ��񂾕����� TEXPART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &texPart.x);	// �e�N�X�`������������ǂݍ���
						fscanf(pFile, "%d", &texPart.y);	// �e�N�X�`���c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fRadius);		// ���a��ǂݍ���
					}
					else if (strcmp(&aString[0], "HEIGHT") == 0)
					{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fHeight);		// �c����ǂݍ���
					}
				} while (strcmp(&aString[0], "END_SCENERYSET") != 0);	// �ǂݍ��񂾕����� END_SCENERYSET �ł͂Ȃ��ꍇ���[�v

				// �i�F�I�u�W�F�N�g�̐���
				if (CScenery::Create((CScenery::EType)nType, pos, D3DXToRadian(rot), col, part, texPart, fRadius, fHeight) == nullptr)
				{ // �m�ۂɎ��s�����ꍇ

					// ���s��Ԃ�
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_SCENERYSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_SCENERYSET �ł͂Ȃ��ꍇ���[�v
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	����̓Ǎ�����
//============================================================
HRESULT CStage::LoadSky(const char* pString, FILE *pFile)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	D3DXCOLOR col = XCOL_WHITE;		// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p
	POSGRID2 texPart = GRID2_ZERO;	// �e�N�X�`���������̑���p

	int nType = 0;			// ��ނ̑���p
	float fRadius = 0.0f;	// ���a�̑���p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	if (pString == nullptr || pFile == nullptr)
	{ // ������E�t�@�C�������݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ��̐ݒ�
	if (strcmp(pString, "STAGE_SKYSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_SKYSET �̏ꍇ

		do
		{ // �ǂݍ��񂾕����� END_STAGE_SKYSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SKYSET") == 0)
			{ // �ǂݍ��񂾕����� SKYSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SKYSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // �ǂݍ��񂾕����� TEXPART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &texPart.x);	// �e�N�X�`������������ǂݍ���
						fscanf(pFile, "%d", &texPart.y);	// �e�N�X�`���c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fRadius);		// ���a��ǂݍ���
					}
				} while (strcmp(&aString[0], "END_SKYSET") != 0);	// �ǂݍ��񂾕����� END_SKYSET �ł͂Ȃ��ꍇ���[�v

				// ��I�u�W�F�N�g�̐���
				if (CSky::Create((CSky::EType)nType, pos, D3DXToRadian(rot), col, part, texPart, fRadius) == nullptr)
				{ // �m�ۂɎ��s�����ꍇ

					// ���s��Ԃ�
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_SKYSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_SKYSET �ł͂Ȃ��ꍇ���[�v
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�t�̏��̓Ǎ�����
//============================================================
HRESULT CStage::LoadLiquid(const char* pString, FILE *pFile)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	D3DXVECTOR2 size = VEC2_ZERO;	// �傫���̑���p
	D3DXCOLOR col = XCOL_WHITE;		// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p
	CLiquid::STexMove texMove;		// �e�N�X�`���ړ��ʂ̑���p

	int nType = 0;				// ��ނ̑���p
	float fMaxUp = 0.0f;		// �g�̍ō��㏸��
	float fAddSinRot = 0.0f;	// �g�ł��������Z��
	float fAddVtxRot = 0.0f;	// �הg�̌������Z��

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	if (pString == nullptr || pFile == nullptr)
	{ // ������E�t�@�C�������݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �t�̂̐ݒ�
	if (strcmp(pString, "STAGE_LIQUIDSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_LIQUIDSET �̏ꍇ

		do
		{ // �ǂݍ��񂾕����� END_STAGE_LIQUIDSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "LIQUIDSET") == 0)
			{ // �ǂݍ��񂾕����� LIQUIDSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_LIQUIDSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &size.x);		// �傫��X��ǂݍ���
						fscanf(pFile, "%f", &size.y);		// �傫��Y��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXMOVE_LOW") == 0)
					{ // �ǂݍ��񂾕����� TEXMOVE_LOW �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &texMove.texMoveLow.x);	// ���t�̂̃e�N�X�`���ړ��ʂ�ǂݍ���
						fscanf(pFile, "%f", &texMove.texMoveLow.y);	// ���t�̂̃e�N�X�`���ړ��ʂ�ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXMOVE_HIGH") == 0)
					{ // �ǂݍ��񂾕����� TEXMOVE_HIGH �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &texMove.texMoveHigh.x);	// ��t�̂̃e�N�X�`���ړ��ʂ�ǂݍ���
						fscanf(pFile, "%f", &texMove.texMoveHigh.y);	// ��t�̂̃e�N�X�`���ړ��ʂ�ǂݍ���
					}
					else if (strcmp(&aString[0], "MAX_UP") == 0)
					{ // �ǂݍ��񂾕����� MAX_UP �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fMaxUp);		// �g�̍ō��㏸�ʂ�ǂݍ���
					}
					else if (strcmp(&aString[0], "ADD_SINROT") == 0)
					{ // �ǂݍ��񂾕����� ADD_SINROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fAddSinRot);	// �g�ł��������Z�ʂ�ǂݍ���
					}
					else if (strcmp(&aString[0], "ADD_VTXROT") == 0)
					{ // �ǂݍ��񂾕����� ADD_VTXROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fAddVtxRot);	// �הg�̌������Z�ʂ�ǂݍ���
					}
				} while (strcmp(&aString[0], "END_LIQUIDSET") != 0);	// �ǂݍ��񂾕����� END_LIQUIDSET �ł͂Ȃ��ꍇ���[�v

				// �t�̃I�u�W�F�N�g�̐���
				CLiquid *pLiquid = CLiquid::Create
				( // ����
					(CLiquid::EType)nType,		// ���
					pos,						// �ʒu
					D3DXToRadian(rot),			// ����
					size,						// �傫��
					col,						// �F
					part,						// ������
					texMove,					// �e�N�X�`���ړ���
					fMaxUp,						// �g�̍ō��㏸��
					D3DXToRadian(fAddSinRot),	// �g�ł��������Z��
					D3DXToRadian(fAddVtxRot)	// �הg�̌������Z��
				);
				if (pLiquid == nullptr)
				{ // �m�ۂɎ��s�����ꍇ

					// ���s��Ԃ�
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_LIQUIDSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_LIQUIDSET �ł͂Ȃ��ꍇ���[�v
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
//  �J�����ύX�n�_���̓Ǎ�
//===========================================
HRESULT CStage::LoadChanger(const char* pString, FILE* pFile)
{
	// �Ǎ��Ɏ��s�����ꍇ�֐��𔲂���
	if (pString == nullptr || pFile == nullptr) { assert(false); return E_FAIL; }

	// �I�u�W�F�N�g�̐ݒ�
	if (strcmp(pString, "STAGE_CHANGERSET") == 0) // �Ǎ��J�n�t���O
	{
		// �ꎞ�i�[�p�ϐ�
		char aTemp[MAX_STRING];

		do // �Ǎ�����������܂Ń��[�v����
		{
			// ������̓ǂݍ���
			fscanf(pFile, "%s", &aTemp[0]);

			// �I�u�W�F�N�g�̔z�u
			if (strcmp(&aTemp[0], "CHANGERSET") == 0)
			{
				// �ϐ���錾
				int nDir = 0; // �����̊i�[�p
				int nRot = 0; // �p�x�̊i�[�p
				D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̊i�[�p
				D3DXVECTOR3 size = VEC3_ZERO;	// �傫���̊i�[�p

				do // �Ǎ�����������܂Ń��[�v����
				{
					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aTemp[0]);

					if (strcmp(&aTemp[0], "DIR") == 0)
					{
						fscanf(pFile, "%s", &aTemp[0]); // =
						fscanf(pFile, "%d", &nDir); // �����̓Ǎ�
					}
					if (strcmp(&aTemp[0], "ROT") == 0)
					{
						fscanf(pFile, "%s", &aTemp[0]); // =
						fscanf(pFile, "%d", &nRot); // �p�x�̓ǂݍ���
					}
					if (strcmp(&aTemp[0], "POS") == 0) // �ʒu�̓ǂݍ��� 
					{
						fscanf(pFile, "%s", &aTemp[0]); // =
						fscanf(pFile, "%f", &pos.x); // X
						fscanf(pFile, "%f", &pos.y); // Y
						fscanf(pFile, "%f", &pos.z); // Z
					}
					if (strcmp(&aTemp[0], "SIZE") == 0) // �傫���̓ǂݍ���
					{
						fscanf(pFile, "%s", &aTemp[0]); // =
						fscanf(pFile, "%f", &size.x); // X
						fscanf(pFile, "%f", &size.y); // Y
						fscanf(pFile, "%f", &size.z); // Z
					}

				} while (strcmp(&aTemp[0], "END_CHANGERSET") != 0);

				// �I�u�W�F�N�g�̐���
				if (CCameraChanger::Create(pos, size, (CCameraChanger::EDirection)nDir, (CCameraChanger::ERotation)nRot) == nullptr)
				{
					assert(false); return E_FAIL;
				}
			}
		} while (strcmp(&aTemp[0], "END_STAGE_CHANGERSET") != 0);
	}

	// ������Ԃ�
	return S_OK;
}

//==========================================
//  ���̓ǂݍ���
//==========================================
HRESULT CStage::LoadWeed(const char* pString, FILE* pFile)
{
	// �Ǎ��Ɏ��s�����ꍇ�֐��𔲂���
	if (pString == nullptr || pFile == nullptr) { assert(false); return E_FAIL; }

	// �I�u�W�F�N�g�̐ݒ�
	if (strcmp(pString, "STAGE_WEEDSET") == 0) // �Ǎ��J�n�t���O
	{
		// �ꎞ�i�[�p�ϐ�
		char aTemp[MAX_STRING];

		do // �Ǎ�����������܂Ń��[�v����
		{
			// ������̓ǂݍ���
			fscanf(pFile, "%s", &aTemp[0]);

			// �I�u�W�F�N�g�̔z�u
			if (strcmp(&aTemp[0], "WEEDSET") == 0)
			{
				// �ϐ���錾
				D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̊i�[�p
				D3DXVECTOR3 rot = VEC3_ZERO;	// �傫���̊i�[�p

				do // �Ǎ�����������܂Ń��[�v����
				{
					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aTemp[0]);

					if (strcmp(&aTemp[0], "POS") == 0) // �ʒu�̓ǂݍ��� 
					{
						fscanf(pFile, "%s", &aTemp[0]); // =
						fscanf(pFile, "%f", &pos.x); // X
						fscanf(pFile, "%f", &pos.y); // Y
						fscanf(pFile, "%f", &pos.z); // Z
					}
					if (strcmp(&aTemp[0], "ROT") == 0)
					{
						fscanf(pFile, "%s", &aTemp[0]); // =
						fscanf(pFile, "%f", &rot.x); // X
						fscanf(pFile, "%f", &rot.y); // Y
						fscanf(pFile, "%f", &rot.z); // Z
					}

				} while (strcmp(&aTemp[0], "END_WEEDSET") != 0);

				// �I�u�W�F�N�g�̐���
				if (CWeed::Create(pos, rot) == nullptr)
				{
					assert(false); return E_FAIL;
				}
			}
		} while (strcmp(&aTemp[0], "END_STAGE_WEEDSET") != 0);
	}

	// ������Ԃ�
	return S_OK;
}
