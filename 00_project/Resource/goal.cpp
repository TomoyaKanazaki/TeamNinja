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
#include "startUI.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const char* PARAM_FILE = "data\\TXT\\Goal.txt"; // �p�����[�^���ۑ����ꂽ�p�X
}

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

	return S_OK;
}

//==========================================
//  �I������
//==========================================
void CGoal::Uninit(void)
{
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

	// �������Ă��Ȃ��ꍇ�֐��𔲂���
	if (!collision::CirclePillar(pos, Player->GetVec3Position(), m_fRadius, Player->GetRadius()))
	{ return; }

	// �S�[������UI��\������
	CStartUI::Create();

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
