//=========================================
//
//  �ݒu�M�~�b�N (gimmick_post.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_post.h"
#include "manager.h"
#include "objectMeshCube.h"

#include "player.h"
#include "player_clone.h"

#include "directxmath.h"
#include "collision.h"

//=========================================
//  �R���X�g���N�^
//=========================================
namespace
{
	const float BUTTON_OFF	= 15.0f;	// �{�^���I�t�̎��̃I�t�Z�b�g
	const float BUTTON_ON	= 6.0f;		// �{�^���I���̎��̃I�t�Z�b�g
	const float BUTTON_MOVE	= 60.0f;	// �{�^������/�߂����̈ړ���

	const D3DXVECTOR3 SCALE_BUTTON	= D3DXVECTOR3(-5.0f, 0.0f, -5.0f);				// �{�^���傫���̃I�t�Z�b�g
	const D3DXVECTOR3 SCALE_EDGE	= D3DXVECTOR3(0.0f, BUTTON_ON - 1.0f, 0.0f);	// �����傫���̃I�t�Z�b�g

	const D3DXCOLOR COLOR_DEFAULT	= D3DXCOLOR(0.31372549f, 0.89803922f, 0.97647059f, 1.0f);	// �F
	const D3DXCOLOR COLOR_ON		= D3DXCOLOR(0.98823529f, 0.20392157f, 0.53333333f, 1.0f);	// �F
}

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickPost::CGimmickPost() : CGimmickAction(),
m_pButton(nullptr), // �{�^��
m_pEdge(nullptr), // ��
m_fButtonHeight(BUTTON_OFF) // �{�^���c�I�t�Z�b�g
{

}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickPost::~CGimmickPost()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickPost::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CGimmickAction::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �{�^���̐���
	m_pButton = CObjectMeshCube::Create
	( // ����
		VEC3_ZERO,	// �ʒu
		VEC3_ZERO,	// ����
		VEC3_ZERO,	// �傫��
		COLOR_DEFAULT,	// �L���[�u�F
		XCOL_BLACK,	// �����F
		CObjectMeshCube::BORDER_OFF,			// �������
		0.0f,									// ����葾��
		CObjectMeshCube::TEXSTATE_ONE,			// �e�N�X�`�����
		CObjectMeshCube::SFaceTex(NONE_IDX),	// �e�N�X�`�����
		CObjectMeshCube::ORIGIN_DOWN			// ���_
	);
	if (m_pButton == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���x�����M�~�b�N�ɂ���
	m_pButton->SetLabel(CObject::LABEL_GIMMICK);

	// ���̐���
	m_pEdge = CObjectMeshCube::Create
	( // ����
		VEC3_ZERO,	// �ʒu
		VEC3_ZERO,	// ����
		VEC3_ZERO,	// �傫��
		XCOL_WHITE,	// �L���[�u�F
		XCOL_BLACK,	// �����F
		CObjectMeshCube::BORDER_OFF,			// �������
		0.0f,									// ����葾��
		CObjectMeshCube::TEXSTATE_ONE,			// �e�N�X�`�����
		CObjectMeshCube::SFaceTex(NONE_IDX),	// �e�N�X�`�����
		CObjectMeshCube::ORIGIN_DOWN			// ���_
	);
	if (m_pEdge == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���x�����M�~�b�N�ɂ���
	m_pEdge->SetLabel(CObject::LABEL_GIMMICK);

	// ������Ԃ�
	return S_OK;
}

//=========================================
//  �I������
//=========================================
void CGimmickPost::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmickAction::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickPost::Update(const float fDeltaTime)
{
	// ����������
	bool bFar = useful::IsNearPosR(GetVec3Position());
	if (!bFar) { return; }

	// �F��ς���
	m_pButton->SetCubeColor(COLOR_DEFAULT);

	// �A�N�e�B�u��ԂȂ�{�^����������Ă�
	if (IsActive())
	{ // �{�^����������Ă���ꍇ

		// �{�^������������
		m_fButtonHeight -= BUTTON_MOVE * fDeltaTime;

		// �F��ς���
		m_pButton->SetCubeColor(COLOR_ON);
	}
	else
	{ // �{�^����������Ă��Ȃ��ꍇ

		// �{�^���������グ��
		m_fButtonHeight += BUTTON_MOVE * fDeltaTime;
	}

	// �{�^���̑傫���ݒ�
	SetButtonSizing();

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer();

	// �e�N���X�̍X�V
	CGimmickAction::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickPost::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmickAction::Draw(pShader);
}

//=========================================
//	�ʒu�̐ݒ菈��
//=========================================
void CGimmickPost::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �e�N���X�̈ʒu�ݒ�
	CGimmickAction::SetVec3Position(rPos);

	// �{�^���̈ʒu�ݒ�
	m_pButton->SetVec3Position(rPos);

	// ���̈ʒu�ݒ�
	m_pEdge->SetVec3Position(rPos);
}

//=========================================
//	�傫���̐ݒ菈��
//=========================================
void CGimmickPost::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �T�C�Y�̐ݒ�
	CObject3D::SetVec3Sizing(rSize);

	// �{�^���̑傫���ݒ�
	SetButtonSizing();

	// ���̑傫���ݒ�
	m_pEdge->SetVec3Sizing(rSize * 0.5f + SCALE_EDGE);
}

//===========================================
//  �e���g���̑ҋ@�ʒu���Z�o
//===========================================
D3DXVECTOR3 CGimmickPost::CalcWaitPoint(const int Idx, const CPlayerClone* pClone)
{
	D3DXVECTOR3 posWait = GetVec3Position();	// �ҋ@�ʒu

	// �M�~�b�N���_�ʒu�ɓ��ݍ���ł�{�^���̍�����^����
	posWait.y += m_pButton->GetVec3Sizing().y * 2.0f;

	// �Z�o�����ʒu��Ԃ�
	return posWait;
}

//===========================================
//  �e���g���̑ҋ@�������Z�o
//===========================================
D3DXVECTOR3 CGimmickPost::CalcWaitRotation(const int Idx, const CPlayerClone* pClone)
{
	// �󂯎�����C���f�b�N�X���ő�l�𒴂��Ă���ꍇ�x��
	if (Idx > GetNumActive()) { assert(false); }

	// �v���C���[�̈ʒu���擾
	D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();

	// �ҋ@�ʒu���擾
	D3DXVECTOR3 posThis = GetActionPoint();

	// �ڕW�����Ƃ̍��������߂�
	D3DXVECTOR3 vecTarget = posPlayer - posThis;

	// �ҋ@���������߂�
	D3DXVECTOR3 rotWait = VEC3_ZERO;
	rotWait.y = atan2f(-vecTarget.x, -vecTarget.z);

	// �Z�o����������Ԃ�
	return rotWait;
}

//==========================================
//  �v���C���[�Ƃ̓����蔻��
//==========================================
bool CGimmickPost::CollisionPlayer()
{
	// �v���C���[���W���擾
	CPlayer* pPlayer = GET_PLAYER;
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();
	D3DXVECTOR3 oldPlayer = pPlayer->GetOldPosition();
	float fRadius = pPlayer->GetRadius();

	// ���g�̃T�C�Y���擾
	D3DXVECTOR3 size = GetVec3Sizing() * 0.5f;

	//�����蔻��
	bool bHit = collision::BoxPillar
	(
		posPlayer, oldPlayer,
		GetVec3Position(),
		D3DXVECTOR3(fRadius, fRadius, fRadius),
		D3DXVECTOR3(fRadius, fRadius, fRadius),
		size,
		size
	);

	// �������Ă���ꍇ�v���C���[���W��␳����
	if (bHit)
	{
		pPlayer->SetVec3Position(posPlayer);
	}

	return bHit;
}

//=========================================
//	�{�^���傫���̐ݒ菈��
//=========================================
void CGimmickPost::SetButtonSizing(void)
{
	D3DXVECTOR3 size = GetVec3Sizing() * 0.5f + SCALE_BUTTON;	// �{�^���̑傫��

	// �{�^���̓��܂��𔽉f
	useful::LimitNum(m_fButtonHeight, BUTTON_ON, BUTTON_OFF);	// ���܂���␳
	size.y = m_fButtonHeight;

	// �{�^���̑傫���ݒ�
	m_pButton->SetVec3Sizing(size);
}

//===========================================
//  �F�̕ύX
//===========================================
void CGimmickPost::ChangeColor()
{
	// �F���擾
	D3DXCOLOR col = m_pButton->GetCubeColor();
	col *= 255.0f;

	// �ϐ��錾
	float H = 0.0f, S = 0.0f, V = 0.0f;
	float fCol[3] = { col.r, col.g, col.b };

	// RGB -> HSV �ϊ�
	{
		float fMax = 0.0f, fMin = 1.0f;

		// �ő�ƍŏ��̒l���擾����
		for (int i = 0; i < 3; ++i)
		{
			if (fMax <= fCol[i]) { fMax = fCol[i]; }
			if (fMin >= fCol[i]) { fMin = fCol[i]; }
		}

		// �ő�l�ƈ�v����F�𔻕ʂ���
		if (col.r == col.g && col.g == col.b && col.b == col.r)
		{
			H = 0.0f;
		}
		else if (col.r == fMax)
		{
			H = 60.0f * ((col.g - col.b) / (fMax - fMin));
		}
		else if (col.g == fMax)
		{
			H = 60.0f * ((col.b - col.r) / (fMax - fMin)) + 120.0f;
		}
		else if (col.b == fMax)
		{
			H = 60.0f * ((col.r - col.g) / (fMax - fMin)) + 240.0f;
		}

		// �l�����̏ꍇ360���Z���Đ��̒l�ɂ���
		if (H < 0.0f)
		{
			H += 360.0f;
		}

		// �l���X�V����
		H += 0.5f;

		// 360�𒴂�����0�ɖ߂�
		if (H > 360.0f)
		{
			H = 0.0f;
		}

		// S�����߂�
		if (fMax == 0.0f)
		{
			S = 0.0f;
		}
		else
		{
			S = ((fMax - fMin) / fMax) * 255.0f;
		}

		// V�����߂�
		V = fMax;
	}

	// HSV -> RGB �ϊ�
	{
		float fMax = 0.0f, fMin = 0.0f;
		// �ő�l�����߂�
		fMax = V;

		// �ŏ��l�����߂�
		fMin = fMax - ((S / 255.0f) * fMax);

		// �ꍇ����������RGB�l�����肷��
		if (0.0f <= H && H <= 60.0f)
		{
			col.r = fMax;
			col.g = ((H / 60.0f) * (fMax - fMin) + fMin);
			col.b = fMin;
		}
		else if (60.0f <= H && H <= 120.0f)
		{
			col.r = (((120.0f - H) / 60.0f) * (fMax - fMin) + fMin);
			col.g = fMax;
			col.b = fMin;
		}
		else if (120.0f <= H && H <= 180.0f)
		{
			col.r = fMin;
			col.g = fMax;
			col.b = (((H - 120.0f) / 60.0f) * (fMax - fMin) + fMin);
		}
		else if (180.0f <= H && H <= 240.0f)
		{
			col.r = fMin;
			col.g = (((240.0f - H) / 60.0f) * (fMax - fMin) + fMin);
			col.b = fMax;
		}
		else if (240.0f <= H && H <= 300.0f)
		{
			col.r = (((H - 240.0f) / 60.0f) * (fMax - fMin) + fMin);
			col.g = fMin;
			col.b = fMax;
		}
		else if (300.0f <= H && H <= 360.0f)
		{
			col.r = fMax;
			col.g = fMin;
			col.b = (((360.0f - H) / 60.0f) * (fMax - fMin) + fMin);
		}
		col /= 255.0f;
		m_pButton->SetCubeColor(col);
	}
}
