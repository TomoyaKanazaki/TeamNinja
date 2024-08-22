//============================================================
//
//	�A�N�V�����M�~�b�N���� [gimmick_action.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "gimmick_action.h"

#include "player_clone.h"
#include "player.h"
#include "collision.h"

#include "effekseerControl.h"
#include "effekseerManager.h"
#include "sound.h"

#include "camera.h"
#include "multi_plant.h"

#include "object3D.h"

// �萔��`
namespace
{
	const float HEIGHT = 50.0f; // ����p�̍���
	const float RADIUS = 70.0f;	// ���a
	const bool ACTION_SET_FRAG[] = // �A�N�V�����|�C���g�̐ݒ�\�t���O
	{
		true, // �W�����v��
		true, // ���ݑ�
		false, // �ݒu
		true, // ��э~��
		false, // �d����
		false, // ��
	};
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CGimmickAction>* CGimmickAction::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//===========================================
//  �ÓI�x������
//===========================================
static_assert(NUM_ARRAY(ACTION_SET_FRAG) == CGimmick::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�q�N���X [CGimmickAction] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGimmickAction::CGimmickAction() : CGimmick(),
m_nNumClone(0),					// �͈͂ɓ����Ă��镪�g�̐�
m_bActive(false),				// ������
m_bOldActive(false),			// ������
m_bMoment(false),				// ������
m_posAction(VEC3_ZERO),			// �A�N�V�����|�C���g(�ҋ@���W)
m_pEffect(nullptr)				// �G�t�F�N�g�̃|�C���^
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGimmickAction::~CGimmickAction()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGimmickAction::Init(void)
{
	// �����o�ϐ��̏�����
	m_posAction = GetVec3Position();

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CGimmick::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CGimmickAction>::Create();
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
void CGimmickAction::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �G�t�F�N�g�̏I��
	if (m_pEffect != nullptr)
	{
		SAFE_DELETE(m_pEffect);
		m_pEffect = nullptr;
	}

	// �I�u�W�F�N�g3D�̏I��
	CGimmick::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGimmickAction::Update(const float fDeltaTime)
{
	OnScreen();

	// ���g�Ƃ̓����蔻��
	CollisionClone();

	// �O��t���O��ۑ�
	m_bOldActive = m_bActive;

	// �����\������ false �ɂ���
	m_bActive = false;

	// �K�v�ȕ��g�������Ă���΃t���O�𗧂Ă�
	if (GetNumActive() <= m_nNumClone)
	{
		// �O�t���[���̃t���O���I�t�̏ꍇ�G�t�F�N�g�𔭐�
		if (!m_bOldActive)
		{
			GET_EFFECT->Create
			(
				"data\\EFFEKSEER\\guide_wind_ball.efkefc",
				GetVec3Position(),
				VEC3_ZERO,
				VEC3_ZERO,
				7.5f,
				false
			);

			// �M�~�b�N��������炷
			PLAY_SOUND(CSound::LABEL_SE_GIMMICKSET_001);
		}

		// �t���O���I��
		m_bActive = true;
	}

	// �G�t�F�N�g�̐���
	ControlEffect();

	// ���g����ۑ�����
	m_nOldNum = m_nNumClone;

	// �e�N���X�̍X�V
	CGimmick::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CGimmickAction::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CGimmick::Draw(pShader);
}

//============================================================
// �v���C���[�Ƃ̓����蔻��(��`)
//============================================================
bool CGimmickAction::CollisionPlayer(void)
{
	// ����t���O
	bool bHit = false;

	// �v���C���[�̃��X�g�\����������Δ�����
	if (CPlayer::GetList() == nullptr) { return bHit; }

	std::list<CPlayer*> list = CPlayer::GetList()->GetList();	// ���X�g���擾
	D3DXVECTOR3 pos = GetVec3Position();		// �ʒu
	D3DXVECTOR3 size = GetVec3Sizing() * 0.5f;	// �T�C�Y
	size.y = HEIGHT;							// ����ɍ�����ݒ肷��
	D3DXVECTOR3 posPlayer = VEC3_ZERO;			// �v���C���[�̈ʒu
	D3DXVECTOR3 sizePlayer = D3DXVECTOR3(RADIUS, 0.0f, RADIUS);		// �v���C���[�̃T�C�Y

	for (auto player : list)
	{
		// �ʒu���擾
		posPlayer = player->GetVec3Position();

		if (collision::Box3D
		(
			pos,		// ����ʒu
			posPlayer,	// ����ڕW�ʒu
			size,		// ����T�C�Y(�E�E��E��)
			size,		// ����T�C�Y(���E���E�O)
			sizePlayer,	// ����ڕW�T�C�Y(�E�E��E��)
			sizePlayer	// ����ڕW�T�C�Y(���E���E�O)
		))
		{ // �l�p�̒��ɓ������ꍇ
			bHit = true;
		}
	}

	return bHit;
}

//===========================================
//  �v���C���[�Ƃ̓����蔻��(�~�`)
//===========================================
bool CGimmickAction::DistancePlayer()
{
	// ����t���O
	bool bHit = false;

	// �����\�t���O��off�Ȃ�֐��𔲂���
	if (!m_bActive) { return bHit; }

	// �v���C���[���̎擾
	CPlayer* player = GET_PLAYER;

	// �v���C���[�̍��W���擾
	D3DXVECTOR3 posPlayer = player->GetVec3Position();

	// �M�~�b�N�̒��S���W���擾
	D3DXVECTOR3 posActive = GetActionPoint();

	// ���S���W�ƃv���C���[�̋�����2����Z�o
	D3DXVECTOR3 vecToPlayer = posPlayer - posActive;
	float fDistance = vecToPlayer.x * vecToPlayer.x + vecToPlayer.z * vecToPlayer.z;

	// ������2�悪�v���C���[�̔��a2�斢���̏ꍇ�t���O�𗧂Ă�
	if (RADIUS * RADIUS > fDistance)
	{
		bHit = true;
	}

	return bHit;
}

//===========================================
//  ���g�̑��������Z
//===========================================
void CGimmickAction::AddNumClone()
{
	// �������K�v���ɒB���Ă�����֐��𔲂���
	if (m_nNumClone >= GetNumActive()) { return; }
	
	// ���Z
	++m_nNumClone;

	// �G�t�F�N�g�𐶐�
	if (m_pEffect == nullptr)
	{
		m_pEffect = GET_EFFECT->Create
		(
			"data\\EFFEKSEER\\guide_wind_ribbon.efkefc",
			m_posAction,
			VEC3_ZERO,
			VEC3_ZERO,
			7.5f,
			false,
			false
		);
	}
}

//===========================================
//  ���g�̑��������Z
//===========================================
void CGimmickAction::SabNumClone()
{
	// ������0�Ɉȉ��̏ꍇ�֐��𔲂���
	if (m_nNumClone <= 0) { return; }

	// ���Z
	--m_nNumClone;
}

//==========================================
//	���X�g�擾����
//==========================================
CListManager<CGimmickAction>* CGimmickAction::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//==========================================
//  �G�t�F�N�g�̐���
//==========================================
void CGimmickAction::ControlEffect()
{
	// �G�t�F�N�g�����݂��Ȃ��ꍇ�֐��𔲂���
	if (m_pEffect == nullptr) { return; }

	// ���[�v���I�����Ă����ꍇ�G�t�F�N�g���폜���Ċ֐��𔲂���
	if (!m_pEffect->GetExist())
	{
		SAFE_DELETE(m_pEffect);
		m_pEffect = nullptr;
		return;
	}

	// �ҋ@�ʒu�Ɉړ�
	m_pEffect->m_pos.X += (m_posAction.x - m_pEffect->m_pos.X) * 0.3f;
	m_pEffect->m_pos.Y += (m_posAction.y - m_pEffect->m_pos.Y) * 0.3f;
	m_pEffect->m_pos.Z += (m_posAction.z - m_pEffect->m_pos.Z) * 0.3f;
}

//==========================================
//  �`�攻��̃I���I�t
//==========================================
void CGimmickAction::OnScreen()
{
	// ���_���W��ۑ�����ϐ�
	D3DXVECTOR3 posVtx[4] = {}, posScreen[4] = {};

	// ���_���W���Z�o
	for (int i = 0; i < 4; ++i)
	{
		posVtx[i] = GetVec3Position() + GetVertexPosition(i);
		GET_CAMERA->OnScreen(posVtx[i], posScreen[i]);
	}

	// ��ʓ�����
	if (GET_CAMERA->OnScreenPolygon(&posVtx[0]))
	{
		//SetEnableDraw(true);
		//DebugProc::Print(DebugProc::POINT_CENTER, "�����Ă���\n");
	}
	else
	{
		SetEnableDraw(false);
	}
}

//===========================================
//  ��������ʒu�̐ݒ�
//===========================================
void CGimmickAction::SetActionPoint(const D3DXVECTOR3& pos)
{
	// �ݒ�\�t���O��off�̏ꍇ���S���W��ݒ肵�֐��𔲂���
	if (!ACTION_SET_FRAG[GetType()])
	{ m_posAction = GetVec3Position(); return; }

	// �ҋ@�ʒu�Ɉ�����ݒ肷��
	m_posAction = pos;
}

//===========================================
//  �T�C�Y�̐ݒ�
//===========================================
void CGimmickAction::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �T�C�Y�̐ݒ�
	CObject3D::SetVec3Sizing(rSize);

	// �A���̐���
	CMultiPlant::Create(GetVec3Position(), rSize, GetType(), GetNumActive());
}
