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
#include "sound.h"
#include "goal.h"
#include "scene.h"
#include "sceneGame.h"
#include "gameManager.h"

//==========================================
//  �萔��`
//==========================================
namespace
{

	const float RADIUS = 50.0f;			// ���a
	const D3DXVECTOR3 OFFSET = D3DXVECTOR3(0.0f, 8.0f, 0.0f);//�G�t�F�N�g�p�I�t�Z�b�g
	const D3DXVECTOR3 OFFSET_CHECKEFFECT = D3DXVECTOR3(0.0f, 80.0f, 0.0f);//�`�F�b�N�G�t�F�N�g�p�I�t�Z�b�g
	const int SAVE_MOTION_BLEND = 8;	// �Z�[�u���[�V�����̃u�����h�J�E���g
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
	m_pos(VEC3_ZERO),
	m_bSave(false),
	m_nSaveTension(0)
{
	m_pEffectdata = NULL;
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
	m_pos = VEC3_ZERO;
	m_bSave = false;
	m_nSaveTension = 0;

	// ���g�̃��x����ݒ�
	SetLabel(LABEL_CHECKPOINT);

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

	//���݂̃G�t�F�N�g���폜
	if (m_pEffectdata != NULL)
	{
		delete m_pEffectdata;
		m_pEffectdata = NULL;
	}

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g��j��
	Release();
}

//==========================================
//  �X�V����
//==========================================
void CCheckPoint::Update(const float fDeltaTime)
{
#ifdef _DEBUG

	// ���G�f�B�^�[�̎��ɓ������̂Ńf�o�b�O�̂�

	// �G�t�F�N�g�̈ʒu��ݒ肷��
	m_pEffectdata->m_pos = m_pos;

#endif // _DEBUG

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer();
}

//==========================================
//  �`�揈��
//==========================================
void CCheckPoint::Draw(CShader* pShader)
{

}

//==========================================
// ���a�擾����
//==========================================
float CCheckPoint::GetRadius(void) const
{
	// ���a��Ԃ�
	return RADIUS;
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
	pSavePoint->m_pos = rPos;

	// �`�F�b�N�|�C���g�G�t�F�N�g���o��
	pSavePoint->m_pEffectdata = GET_EFFECT->Create("data\\EFFEKSEER\\checkpoint_red.efkefc", rPos + OFFSET, VEC3_ZERO, VEC3_ZERO, 50.0f, true,false);

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
	// �v���C���[�̏����擾
	CPlayer* Player = GET_PLAYER;

	// �v���C���[���������Ȃ�֐��𔲂���
	if (Player == nullptr) { return; }

	// �Z�[�u�t���O���I���Ȃ�֐��𔲂���
	if (m_bSave) { return; }

	// �������Ă��Ȃ��ꍇ�֐��𔲂���
	if (!collision::CirclePillar(m_pos, Player->GetVec3Position(), RADIUS, Player->GetRadius()))
	{ return; }

	// �v���C���[���񕜂���
	Player->RecoverCheckPoint();

	// �Z�[�u�|�C���g�̔ԍ���n��
	GET_GAMEMANAGER->SetSave(m_pList->GetIndex(this));

	// �Z�[�u����炷
	PLAY_SOUND(CSound::LABEL_SE_SAVE_000);

	// �Z�[�u���[�V������ݒ肷��
	Player->SetState(CPlayer::STATE_SAVE);

	//���݂̃G�t�F�N�g���폜
	std::vector<CEffekseer::CEffectData*> vEffect = GET_EFFECT->GetList();
	if (!vEffect.empty())
	{
		delete m_pEffectdata;
		m_pEffectdata = NULL;
	}
	
	// �`�F�b�N�|�C���g�G�t�F�N�g���o��
	m_pEffectdata = GET_EFFECT->Create("data\\EFFEKSEER\\checkpoint_blue.efkefc", m_pos + OFFSET, VEC3_ZERO, VEC3_ZERO, 50.0f, true, false);
	GET_EFFECT->Create("data\\EFFEKSEER\\check.efkefc", m_pos + OFFSET_CHECKEFFECT, VEC3_ZERO, VEC3_ZERO, 30.0f);

	// �Z�[�u�t���O���I���ɂ���
	m_bSave = true;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
HRESULT CCheckPoint::LoadSetup(const char* pPass)
{
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p

	// �t�@�C�����J��
	std::ifstream file(pPass);	// �t�@�C���X�g���[��
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
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}
