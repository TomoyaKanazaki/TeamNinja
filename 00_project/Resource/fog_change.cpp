//===========================================
//
//  �t�H�O�ύX�N���X(fog_change.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "fog_change.h"
#include "manager.h"
#include "player.h"
#include "collision.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "fog.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const D3DXVECTOR3 CHANGE_POS = D3DXVECTOR3(5250.0f, 1400.0f, 75.0f); // �ʒu
	const D3DXVECTOR3 CHANGE_SIZE = D3DXVECTOR3(100.0f, 100.0f, 100.0f); // �T�C�Y

	const float MOVE_SPEED = 0.01f; // �ړ����Ă�������
	const float TARGET_START = 0.0f; // �n�_�̈ړ���
	const float TARGET_END = 0.0f; // �I�_�̈ړ���
}

//===========================================
//  �ÓI�����o�ϐ��錾
//===========================================
CListManager<CFogChanger>* CFogChanger::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//===========================================
//  �R���X�g���N�^
//===========================================
CFogChanger::CFogChanger() : CObjectMeshCube(CObject::LABEL_COLLISION, CObject::SCENE_MAIN, CObject::DIM_3D)
{
}

//===========================================
//  �f�X�g���N�^
//===========================================
CFogChanger::~CFogChanger()
{
}

//===========================================
//  ����������
//===========================================
HRESULT CFogChanger::Init()
{
	// �e�N���X�̏�����
	if (FAILED(CObjectMeshCube::Init())) { assert(false); return E_FAIL; }

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CFogChanger>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���_��ݒ�
	SetOrigin(ORIGIN_DOWN);

	// �ʒu��ݒ�
	SetVec3Position(CHANGE_POS);

	// �T�C�Y��ݒ�
	SetVec3Sizing(CHANGE_SIZE);

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// ������Ԃ�
	return S_OK;
}

//===========================================
//  �I������
//===========================================
void CFogChanger::Uninit()
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �e�N���X�̏I��
	CObjectMeshCube::Uninit();
}

//===========================================
//  �X�V����
//===========================================
void CFogChanger::Update(const float fDeltaTime)
{
	// ����������
	bool bFar = useful::IsNearPosR(GetVec3Position());
	SetEnableDraw(bFar);
	if (!bFar) { return; }

	// �Q�[��������Ȃ��ꍇ�֐��𔲂���
	if (GET_GAMEMANAGER->GetState() != CGameManager::STATE_NORMAL) { return; }

	// �ړ�
	Move();

	// �e�N���X�̍X�V����
	CObjectMeshCube::Update(fDeltaTime);
}

//===========================================
//  �`�揈��
//===========================================
void CFogChanger::Draw(CShader* pShader)
{
#ifndef _DEBUG
	// �f�o�b�O���ȊO�͕`�悵�����Ȃ�
	return;
#endif

	//�e�N���X�̕`�揈��
	CObjectMeshCube::Draw(pShader);
}

//===========================================
//	�q�b�g����̎擾
//===========================================
bool CFogChanger::CollChange()
{
	// �v���C���[���̎擾
	CPlayer* pPlayer = GET_PLAYER;

	// �v���C���[�����Ȃ��ꍇ������
	if (pPlayer == nullptr) { return false; }

	// �v���C���[�̏����擾
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();
	D3DXVECTOR3 sizePlayer = pPlayer->GetVec3Sizing();

	// �e�픻��p���̎擾
	D3DXVECTOR3 posThis = GetVec3Position(); // ���g�̍��W
	D3DXVECTOR3 sizeThis = GetVec3Sizing(); // ���g�̃T�C�Y

	// ��`����̔��茋�ʂ�Ԃ�
	return collision::Box3D(posThis, posPlayer, D3DXVECTOR3(sizeThis.x, sizeThis.y * 2.0f, sizeThis.z), D3DXVECTOR3(sizeThis.x, 0.0f, sizeThis.z), sizePlayer, sizePlayer);
}

//==========================================
//  �ړ�
//==========================================
void CFogChanger::Move()
{
	// �ڕW�l��ۑ�����ϐ�
	float fStart = 0.0f, fEnd = 0.0f;

	// �ڕW�l��ݒ�
	if (CollChange())
	{
		fStart = TARGET_START;
		fEnd = TARGET_END;
	}
	else
	{
		fStart = Fog::GetInitStart();
		fEnd = Fog::GetInitEnd();
	}

	// �ړ��ʂ̐ݒ�
	float fDestStart = (fStart - Fog::GetStart()) * MOVE_SPEED;
	float fDestEnd = (fEnd - Fog::GetEnd()) * MOVE_SPEED;

	// �t�H�O�̋�����ݒ�
	Fog::SetStart(fDestStart + Fog::GetStart());
	Fog::SetEnd(fDestEnd + Fog::GetEnd());
}

//===========================================
//  ��������
//===========================================
CFogChanger* CFogChanger::Create()
{
	// �������̊m��
	CFogChanger* pChanger = new CFogChanger;

	// �m�ۂɎ��s�����ꍇnull��Ԃ�
	if (pChanger == nullptr) { assert(false); return nullptr; }

	// ����������
	if (FAILED(pChanger->Init())) { assert(false); return nullptr; }

	return pChanger;
}

//===========================================
//  ���X�g�̎擾
//===========================================
CListManager<CFogChanger>* CFogChanger::GetList(void)
{
	return m_pList;
}
