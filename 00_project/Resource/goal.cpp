//==========================================
//
//  �`�F�b�N�|�C���g(checkpoint.cpp)
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
	const char* PARAM_FILE = "data\\TXT\\Goal.txt"; // �p�����[�^���ۑ����ꂽ�p�X
	const char* GOAL_TEXTURE = "data\\TEXTURE\\end.png";	// �S�[���̃e�N�X�`��
}

//------------------------------------------
// �ÓI�����o�ϐ��錾
//------------------------------------------
CListManager<CGoal>* CGoal::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//==========================================
//  �R���X�g���N�^
//==========================================
CGoal::CGoal():
	m_bClear(false),
	m_fRadius(0.0f),
	m_fRotSpeed(0.0f)
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
	m_fRadius = 0.0f;
	m_fRotSpeed = 0.0f;

	// �e�N���X�̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f�������蓖��
	BindModel("data\\MODEL\\ENEMY\\BOSS_DRAGON\\03_head.x");

	// ���g�̃��x����ݒ�
	SetLabel(LABEL_GOAL);

	// �萔�p�����[�^�̓ǂݍ���
	Load();

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
	rot.y += m_fRotSpeed;
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
CGoal* CGoal::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// �|�C���^��錾
	CGoal* pSavePoint = new CGoal;	// �Z�[�u�|�C���g�����p

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

	// ������ݒ�
	pSavePoint->SetVec3Rotation(rRot);

	// �m�ۂ����A�h���X��Ԃ�
	return pSavePoint;
}

//==========================================
// ���X�g�擾
//==========================================
CGoal* CGoal::GetGoal(void)
{
	if (m_pList == nullptr) { return nullptr; }		// ���X�g���g�p�̏ꍇ������
	if (m_pList->GetNumAll() != 1) { assert(false); return nullptr; }	// �S�[����1�l�ł͂Ȃ��ꍇ������
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
	if (!collision::CirclePillar(pos, Player->GetVec3Position(), m_fRadius, Player->GetRadius()))
	{ return; }

	// �S�[������UI��\������
	CPopUpUI::Create(GOAL_TEXTURE);

	// �N���A�t���O���I���ɂ���
	m_bClear = true;
}

//==========================================
//  �O�����̓ǂݍ���
//==========================================
void CGoal::Load()
{
	//���[�J���ϐ��錾
	FILE* pFile; // �t�@�C���|�C���^

	//�t�@�C����ǂݎ���p�ŊJ��
	pFile = fopen(PARAM_FILE, "r");

	// �t�@�C�����J���Ȃ������ꍇ
	if (pFile == NULL) { assert(false); return; }

	// ���̓ǂݍ���
	while (1)
	{
		// ������̋L�^�p
		char aStr[256];

		// ������ǂݍ���
		fscanf(pFile, "%s", &aStr[0]);

		// ��������
		if (strcmp(&aStr[0], "RADIUS") == 0) // �����蔻��̔��a�̐ݒ�
		{
			// �f�[�^���i�[
			fscanf(pFile, "%f", &m_fRadius);
		}
		if (strcmp(&aStr[0], "ROT_SPEED") == 0) // ��鑬�x
		{
			// �f�[�^���i�[
			fscanf(pFile, "%f", &m_fRotSpeed);
		}
		if (strcmp(&aStr[0], "END_OF_FILE") == 0) // �ǂݍ��ݏI��
		{
			break;
		}
	}
}
