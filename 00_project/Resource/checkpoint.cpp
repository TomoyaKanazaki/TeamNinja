//==========================================
//
//  �`�F�b�N�|�C���g(checkpoint.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "checkpoint.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "collision.h"
#include "popupUI.h"
#include "goal.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const char *SETUP_TXT	= "data\\TXT\\Point.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const float RADIUS		= 50.0f;	// ���a
	const float ROT_SPEED	= 0.01f;	// ��鑬�x
}

//==========================================
//  �ÓI�����o�ϐ��錾
//==========================================
int CCheckPoint::m_nNumAll = 0;
CListManager<CCheckPoint>* CCheckPoint::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//==========================================
//  �R���X�g���N�^
//==========================================
CCheckPoint::CCheckPoint():
	m_bSave(false),
	m_nSaveTension(0)
{
	// ���������Z
	++m_nNumAll;
}

//==========================================
//  �f�X�g���N�^
//==========================================
CCheckPoint::~CCheckPoint()
{
	// ���������Z
	--m_nNumAll;
}

//==========================================
//  ����������
//==========================================
HRESULT CCheckPoint::Init(void)
{
	// �l�̏�����
	m_bSave = false;
	m_nSaveTension = 0;

	// �e�N���X�̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f�������蓖��
	BindModel("data\\MODEL\\FONT\\name_boss000.x");

	// ���g�̃��x����ݒ�
	SetLabel(LABEL_CHECKPOINT);

	// �T�C�Y�𒲐�
	SetVec3Scaling(D3DXVECTOR3(0.2f, 0.2f, 0.2f));

	// �}�e���A����ύX
	SetAllMaterial(material::GlowCyan());

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CCheckPoint>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//==========================================
//  �I������
//==========================================
void CCheckPoint::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �e�N���X�̏I��
	CObjectModel::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CCheckPoint::Update(const float fDeltaTime)
{
	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer();

	// ���邭�邵�Ă݂�
	D3DXVECTOR3 rot = GetVec3Rotation();
	rot.y += ROT_SPEED;
	SetVec3Rotation(rot);

	// �e�N���X�̍X�V
	CObjectModel::Update(fDeltaTime);
}

//==========================================
//  �`�揈��
//==========================================
void CCheckPoint::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CObjectModel::Draw(pShader);
}

//==========================================
//  ��������
//==========================================
CCheckPoint* CCheckPoint::Create(const D3DXVECTOR3& rPos)
{
	// �|�C���^��錾
	CCheckPoint* pSavePoint = new CCheckPoint;	// �Z�[�u�|�C���g�����p

	// �������̊m�ۂɎ��s���Ă����ꍇnull��Ԃ�
	if (pSavePoint == nullptr) { assert(false); return nullptr; }

	// �Z�[�u�|�C���g�̏�����
	if (FAILED(pSavePoint->Init()))
	{
		// �������J��
		delete pSavePoint;

		// null��Ԃ�
		return nullptr;
	}

	// �ʒu��ݒ�
	pSavePoint->SetVec3Position(rPos);

	// �m�ۂ����A�h���X��Ԃ�
	return pSavePoint;
}

//==========================================
// ���X�g�擾
//==========================================
CListManager<CCheckPoint>* CCheckPoint::GetList(void)
{
	// ���X�g�\����Ԃ�
	return m_pList;
}

//==========================================
//  �v���C���[�Ƃ̓����蔻��
//==========================================
void CCheckPoint::CollisionPlayer(void)
{
	// �Z�[�u�t���O���I���Ȃ�֐��𔲂���
	if (m_bSave) { return; }

	//�@���g�̈ʒu���擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �v���C���[�̏����擾
	CPlayer* Player = GET_PLAYER; // ���W

	// �������Ă��Ȃ��ꍇ�֐��𔲂���
	if (!collision::CirclePillar(pos, Player->GetVec3Position(), RADIUS, Player->GetRadius()))
	{ return; }

	// �v���C���[���񕜂���
	Player->RecoverCheckPoint();

	// �v���C���[�Ɏ��g�̏���^����
	Player->SetCheckPoint(this);

	// �m�C�͂�ۑ�����
	m_nSaveTension = Player->GetTension();

	// �}�e���A����ύX
	SetAllMaterial(material::Red());

	// �Z�[�u�t���O���I���ɂ���
	m_bSave = true;
	
	// UI��\��
	CPopUpUI::Create();
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
HRESULT CCheckPoint::LoadSetup(void)
{
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p

	// �t�@�C�����J��
	std::ifstream file(SETUP_TXT);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�`�F�b�N�|�C���g�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

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
		else if (str == "STAGE_CHECKSET")
		{
			do
			{ // END_STAGE_CHECKSET��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str.front() == '#')
				{ // �R�����g�A�E�g����Ă���ꍇ

					// ��s�S�ēǂݍ���
					std::getline(file, str);
				}
				else if (str == "CHECKSET")
				{
					do
					{ // END_CHECKSET��ǂݍ��ނ܂Ń��[�v

						// �������ǂݍ���
						file >> str;

						if (str == "POS")
						{
							file >> str;	// ����Ǎ�

							// �ʒu��Ǎ�
							file >> pos.x;
							file >> pos.y;
							file >> pos.z;
						}
					} while (str != "END_CHECKSET");	// END_CHECKSET��ǂݍ��ނ܂Ń��[�v

					// �`�F�b�N�|�C���g�̐���
					if (CCheckPoint::Create(pos) == nullptr)
					{ // �m�ۂɎ��s�����ꍇ

						// ���s��Ԃ�
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_CHECKSET");	// END_STAGE_CHECKSET��ǂݍ��ނ܂Ń��[�v
		}
		else if (str == "STAGE_GOALSET")
		{
			do
			{ // END_STAGE_GOALSET��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str == "POS")
				{
					file >> str;	// ����Ǎ�

					// �ʒu��Ǎ�
					file >> pos.x;
					file >> pos.y;
					file >> pos.z;
				}
			} while (str != "END_STAGE_GOALSET");	// END_STAGE_CHECKSET��ǂݍ��ނ܂Ń��[�v

			// �`�F�b�N�|�C���g�̐���
			if (CGoal::Create(pos) == nullptr)
			{ // �m�ۂɎ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}
		}
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}
