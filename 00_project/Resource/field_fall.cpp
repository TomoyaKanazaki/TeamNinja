//=========================================
//
//  ���Ƃ����M�~�b�N (field_fall.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "field_fall.h"
#include "manager.h"
#include "texture.h"
#include "player_clone.h"
#include "player.h"

#include "objectMeshCube.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const int FALL_NUM = 3; // ��ɏ�邱�Ƃ̂ł��镪�g�̐�

	// ���֘A
	namespace floor
	{
		const float HEIGHT = 5.0f;				// ����
		const float NONE_ROT = D3DX_PI * 0.5f;	// �ʏ��Ԃ̌���
	}
}

//==========================================
//  �R���X�g���N�^
//==========================================
CGimmickFall::CGimmickFall() : CField(),
m_state(STATE_NONE),	// ���
m_bFall(false)			// �����t���O
{

}

//==========================================
//  �f�X�g���N�^
//==========================================
CGimmickFall::~CGimmickFall()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickFall::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CField::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < NUM_FLOOR; nCnt++)
	{
		m_apFloor[nCnt] = CObjectMeshCube::Create
		(
			VEC3_ZERO,
			VEC3_ZERO,
			VEC3_ZERO,
			XCOL_WHITE,
			XCOL_ABLACK,
			CObjectMeshCube::BORDER_OFF,
			0.0f,
			CObjectMeshCube::TEXSTATE_ONE,
			CObjectMeshCube::SFaceTex(-1),
			CObjectMeshCube::ORIGIN_UP
		);
	}

	// ������Ԃ�
	return S_OK;
}

//=========================================
//  �I������
//=========================================
void CGimmickFall::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_FLOOR; nCnt++)
	{
		// ���̏I������
		SAFE_UNINIT(m_apFloor[nCnt]);
	}

	// �e�N���X�̏I��
	CField::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickFall::Update(const float fDeltaTime)
{
	// ��ԏ���
	State();

	// �e�N���X�̍X�V
	CField::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickFall::Draw(CShader* pShader)
{
	//// �e�N���X�̕`��
	//CField::Draw(pShader);
}

//===========================================
// �ʒu�̐ݒ菈��
//===========================================
void CGimmickFall::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �ʒu��ݒ肷��
	CField::SetVec3Position(rPos);

	// ���̏���
	Floor();
}

//===========================================
// �����̐ݒ菈��
//===========================================
void CGimmickFall::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// ������ݒ肷��
	CField::SetVec3Rotation(rRot);

	// ���̏���
	Floor();
}

//===========================================
// �T�C�Y�̐ݒ菈��
//===========================================
void CGimmickFall::SetVec2Sizing(const D3DXVECTOR2& rSize)
{
	// �T�C�Y��ݒ肷��
	CField::SetVec2Sizing(rSize);

	// ���̏���
	Floor();
}

//===========================================
// ��ނ̐ݒ菈��
//===========================================
void CGimmickFall::SetType(const EType type)
{
	// ��ނ�ݒ肷��
	CField::SetType(type);

	for (int nCnt = 0; nCnt < NUM_FLOOR; nCnt++)
	{		
		// ���� NULL �̏ꍇ�A������
		if (m_apFloor[nCnt] == nullptr) { continue; }

		// ���̃e�N�X�`����ݒ肷��
		m_apFloor[nCnt]->BindTexture(GetTextureIndex());
	}
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickFall::Hit(CPlayerClone* pClone)
{
	// ���g�ɕ������n��
	pClone->AddFrags(GetFlag());

	// ���g�Ɏ��g�̏���n��
	pClone->SetField(this);

	// ���g�����J�E���g
	Count();
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickFall::Miss(CPlayerClone* pClone)
{
	// ���g����t���O���폜����
	pClone->SabFrags(GetFlag());

	// ���g����t�B�[���h���폜����
	pClone->DeleteField(this);
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickFall::Hit(CPlayer* pPlayer)
{
	// ���g�ɕ������n��
	pPlayer->AddFrags(GetFlag());

	// ���g�����J�E���g
	Count();
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickFall::Miss(CPlayer* pPlayer)
{
	// ���g����t���O���폜����
	pPlayer->SabFrags(GetFlag());
}

//===========================================
//  ����Ă���L�����N�^�[�����̌v�Z����
//===========================================
void CGimmickFall::Count()
{
	// ���g�̃��X�g�\����������Δ�����
	if (CPlayerClone::GetList() == nullptr) { return; }

	// ���X�g���擾
	std::list<CPlayerClone*> list = CPlayerClone::GetList()->GetList();

	// ���g�̏�ɂ��镪�g�̐����J�E���g
	int nNum = 0; // ����Ă��镪�g��
	for (CPlayerClone* clone : list)
	{
		// ���g���������Ă���t�B�[���h�����g�łȂ��ꍇ���ɐi��
		if (clone->GetField() != this) { continue; }

		// ���g�̏�̕��g�������Z
		++nNum;
	}

	// ���g�̐����ő吔�����̏ꍇ�֐��𔲂���
	if (nNum < FALL_NUM) { m_bFall = false; return; }

	// �J����Ԃɂ���
	m_state = STATE_OPEN;

	// �����t���O�𗧂Ă�
	m_bFall = true;
}

//===========================================
// ��ԏ���
//===========================================
void CGimmickFall::State(void)
{
	// ����
	D3DXVECTOR3 rot = VEC3_ZERO;

	switch (m_state)
	{
	case CGimmickFall::STATE_NONE:

		break;

	case CGimmickFall::STATE_OPEN:

		rot = m_apFloor[0]->GetVec3Rotation();
		rot.x += 0.25f;
		if (rot.x >= 0.0f)
		{ // ������0.0f�ȏ�̏ꍇ

			// ������␳����
			rot.x = 0.0f;
		}
		m_apFloor[0]->SetVec3Rotation(rot);

		rot = m_apFloor[1]->GetVec3Rotation();
		rot.x -= 0.25f;
		if (rot.x <= 0.0f)
		{ // ������0.0f�ȉ��̏ꍇ

			// ������␳����
			rot.x = 0.0f;
		}
		m_apFloor[1]->SetVec3Rotation(rot);

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//===========================================
// ���̏���
//===========================================
void CGimmickFall::Floor(void)
{
	// �ʒu�E�����E�T�C�Y���擾
	D3DXVECTOR3 pos = GetVec3Position();
	D3DXVECTOR3 rot = GetVec3Rotation();
	D3DXVECTOR2 size = GetVec2Sizing();
	D3DXVECTOR3 posFloor = VEC3_ZERO;
	D3DXVECTOR3 rotFloor = VEC3_ZERO;
	D3DXVECTOR3 sizeFloor = VEC3_ZERO;

	// �������������ꍇ�A��~
	if (m_apFloor[0] == nullptr || m_apFloor[1] == nullptr) { assert(false); return; }

	for (int nCnt = 0; nCnt < NUM_FLOOR; nCnt++)
	{
		// ���� NULL �̏ꍇ�A������
		if (m_apFloor[nCnt] == nullptr) { continue; }

		// ���̈ʒu��ݒ肷��
		posFloor.x = pos.x + sinf(rot.y) * ((nCnt * size.x) - (size.x * 0.5f));
		posFloor.y = pos.y - floor::HEIGHT;
		posFloor.z = pos.z + cosf(rot.y) * ((nCnt * size.y) - (size.y * 0.5f));
		m_apFloor[nCnt]->SetVec3Position(posFloor);

		// ������ݒ肷��
		rotFloor = rot;
		rotFloor.x = ((floor::NONE_ROT * 2.0f) * nCnt) - floor::NONE_ROT;
		m_apFloor[nCnt]->SetVec3Rotation(rotFloor);

		// �T�C�Y��ݒ肷��
		sizeFloor.x = size.x * 0.5f;
		sizeFloor.y = size.y * 0.25f;
		sizeFloor.z = floor::HEIGHT;
		m_apFloor[nCnt]->SetVec3Sizing(sizeFloor);
	}
}
