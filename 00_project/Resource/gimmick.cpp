//============================================================
//
//	�M�~�b�N���� [gimmick.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "gimmick.h"

#include "gimmick_jumptable.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY = 2;			// �M�~�b�N�͈̓|���S���̗D�揇��
	const D3DXVECTOR3 RADIUS = D3DXVECTOR3(200.0f, 0.0f, 50.0f);		// ���a(�T�C�Y)
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CGimmick>* CGimmick::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CGimmick] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGimmick::CGimmick() : CObject3D(CObject::LABEL_GIMMICK, CObject::DIM_3D, PRIORITY),
m_type(TYPE_JUMPTABLE),
m_nNumClone(0),
m_bActive(false)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGimmick::~CGimmick()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGimmick::Init(void)
{
	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CGimmick>::Create();
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
void CGimmick::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGimmick::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CGimmick::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CGimmick* CGimmick::Create(const D3DXVECTOR3& rPos, const EType type)
{
	// ���g��UI�̐���
	CGimmick* pGimmick = nullptr;

	switch (type)
	{
	case CGimmick::TYPE_JUMPTABLE:		// �W�����v��

		pGimmick = new CGimmickJumpTable;

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	if (pGimmick == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���g��UI�̏�����
		if (FAILED(pGimmick->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���g��UI�̔j��
			SAFE_DELETE(pGimmick);
			return nullptr;
		}

		// �ʒu��ݒ�
		pGimmick->SetVec3Position(rPos);

		// �T�C�Y��ݒ�
		pGimmick->SetVec3Sizing(RADIUS);

		// ��ނ�ݒ�
		pGimmick->SetType(type);

#ifdef _DEBUG

		// �F�̐ݒ�
		pGimmick->SetColor(XCOL_YELLOW);

#else

		// �F�̐ݒ�
		pGimmick->SetColor(XCOL_AWHITE);

#endif // _DEBUG


		// �m�ۂ����A�h���X��Ԃ�
		return pGimmick;
	}
}

//============================================================
// ���X�g�擾
//============================================================
CListManager<CGimmick>* CGimmick::GetList(void)
{
	// ���X�g�\����Ԃ�
	return m_pList;
}

////==========================================
//// �M�~�b�N�̓����蔻��
////==========================================
//void CPlayerClone::CollisionGimmick(void)
//{
//	// �M�~�b�N���Ȃ������ꍇ������
//	if (CGimmick::GetList() == nullptr) { return; }
//
//	std::list<CGimmick*> list = CGimmick::GetList()->GetList();	// ���X�g���擾
//	auto gimBegin = list.begin();			// �ŏ��̃M�~�b�N
//	auto gimEnd = list.end();				// �Ō�̃M�~�b�N
//	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu
//	D3DXVECTOR3 size = D3DXVECTOR3(RADIUS, 0.0f, RADIUS);		// �T�C�Y
//	D3DXVECTOR3 posGim = VEC3_ZERO;			// �M�~�b�N�̈ʒu
//	D3DXVECTOR3 sizeGim = VEC3_ZERO;		// �M�~�b�N�̃T�C�Y
//
//	for (auto gim : list)
//	{
//		// �ʒu�ƃT�C�Y���擾
//		posGim = gim->GetVec3Position();
//		sizeGim = gim->GetVec3Sizing() / 2;
//
//		if (collision::Box2D
//		(
//			pos,		// ����ʒu
//			posGim,		// ����ڕW�ʒu
//			size,		// ����T�C�Y(�E�E��E��)
//			size,		// ����T�C�Y(���E���E�O)
//			sizeGim,	// ����ڕW�T�C�Y(�E�E��E��)
//			sizeGim		// ����ڕW�T�C�Y(���E���E�O)
//		))
//		{ // �l�p�̒��ɓ������ꍇ
//
//		}
//		else
//		{ // �����ĂȂ������ꍇ
//
//		}
//	}
//}