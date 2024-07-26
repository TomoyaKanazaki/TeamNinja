//==========================================
//
//  �S�[��(goal.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "goal.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "collision.h"
#include "popupUI.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const char* MODEL = "data\\MODEL\\Scroll\\Scroll000.x";		// ���f���̃p�X
	const D3DXVECTOR3 INIT_ROT = D3DXVECTOR3(0.7f, 0.0f, 0.0f);	// ��������
	const float ROT_SPEED = 0.01f;		// �����̑��x
	const float RADIUS = 50.0f;			// ���a
}

//------------------------------------------
// �ÓI�����o�ϐ��錾
//------------------------------------------
CListManager<CGoal>* CGoal::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//==========================================
//  �R���X�g���N�^
//==========================================
CGoal::CGoal():
	m_bClear(false)
{

}

//==========================================
//  �f�X�g���N�^
//==========================================
CGoal::~CGoal()
{
}

//==========================================
//  ����������
//==========================================
HRESULT CGoal::Init(void)
{
	// �l�̏�����
	m_bClear = false;

	// �e�N���X�̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f�������蓖��
	BindModel(MODEL);

	// ���g�̃��x����ݒ�
	SetLabel(LABEL_GOALPOINT);

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CGoal>::Create();
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
void CGoal::Uninit(void)
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
void CGoal::Update(const float fDeltaTime)
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
void CGoal::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CObjectModel::Draw(pShader);
}

//==========================================
//  ��������
//==========================================
CGoal* CGoal::Create(const D3DXVECTOR3& rPos)
{
	// �|�C���^��錾
	CGoal* pSavePoint = new CGoal;	// �Z�[�u�|�C���g�����p

	// �������̊m�ۂɎ��s���Ă����ꍇnull��Ԃ�
	if (pSavePoint == nullptr) { assert(false); return nullptr; }

	// �S�[���̏�����
	if (FAILED(pSavePoint->Init()))
	{
		// �������J��
		delete pSavePoint;

		// null��Ԃ�
		return nullptr;
	}

	// �ʒu��ݒ�
	pSavePoint->SetVec3Position(D3DXVECTOR3(rPos.x, rPos.y + RADIUS, rPos.z));

	// ������ݒ�
	pSavePoint->SetVec3Rotation(INIT_ROT);

	// �m�ۂ����A�h���X��Ԃ�
	return pSavePoint;
}

//==========================================
// ���X�g�擾
//==========================================
CGoal* CGoal::GetGoal(void)
{
	if (m_pList == nullptr) { return nullptr; }		// ���X�g���g�p�̏ꍇ������
	if (m_pList->GetNumAll() != 1) { assert(false); return nullptr; }	// �S�[�������݂����ꍇ�֐��𔲂���
	CGoal* pGoal = m_pList->GetList().front();		// �S�[���̏��

	// �S�[���̃|�C���^��Ԃ�
	return pGoal;
}

//==========================================
//  �v���C���[�Ƃ̓����蔻��
//==========================================
void CGoal::CollisionPlayer(void)
{
	// �N���A�t���O���I���Ȃ�֐��𔲂���
	if (m_bClear) { return; }

	//�@���g�̈ʒu���擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �v���C���[�̏����擾
	CPlayer* Player = GET_PLAYER; // ���W

	// �v���C���[�����Ȃ��ꍇ������
	if (Player == nullptr) { return; }

	// �������Ă��Ȃ��ꍇ�֐��𔲂���
	if (!collision::CirclePillar(pos, Player->GetVec3Position(), RADIUS, Player->GetRadius()))
	{ return; }

	// �N���A�t���O���I���ɂ���
	m_bClear = true;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
HRESULT CGoal::LoadSetup(const char* pPass)
{
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p

	// �t�@�C�����J��
	std::ifstream file(pPass);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�S�[���|�C���g�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

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

			// �S�[���|�C���g�̐���
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
