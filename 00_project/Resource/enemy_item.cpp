//============================================================
//
//	�G�̎��������� [enemy_item.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "enemy_item.h"
#include "renderer.h"
#include "model.h"

#include "enemy_katana.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* MODEL[] =		// ���f���̃p�X
	{
		"data\\MODEL\\ENEMY\\SAMURAI\\13_katana.x",		// ��
	};

	const int PRIORITY = 3;		// �D��x
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(MODEL) == CEnemyItem::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�q�N���X [CEnemyItem] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyItem::CEnemyItem() : CObjectModel(CObject::LABEL_NONE, CObject::DIM_3D, PRIORITY),
m_offset(VEC3_ZERO),	// �I�t�Z�b�g���W
m_type(TYPE_KATANA)		// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyItem::~CEnemyItem()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyItem::Init(void)
{
	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyItem::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyItem::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyItem::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw(pShader);
}

//============================================================
// �I�t�Z�b�g����
//============================================================
void CEnemyItem::Offset(const D3DXMATRIX& rMtx)
{
	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxTrans, mtxColl;

	// �}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxColl);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_offset.x, m_offset.y, m_offset.z);
	D3DXMatrixMultiply(&mtxColl, &mtxColl, &mtxTrans);

	// �Z�o�����u�p�[�c�̃��[���h�}�g���b�N�X�v�Ɓu�e�̃}�g���b�N�X�v���|�����킹��
	D3DXMatrixMultiply
	(
		&mtxColl,
		&mtxColl,
		&rMtx
	);

	// �ʒu��ݒ肷��
	SetVec3Position(D3DXVECTOR3(mtxColl._41, mtxColl._42, mtxColl._43));
}

//============================================================
//	��������
//============================================================
CEnemyItem* CEnemyItem::Create
( // ����
	const EType type,				// ���
	const D3DXVECTOR3& rOffset,		// �I�t�Z�b�g
	const D3DXMATRIX& rMtx			// �}�g���b�N�X���
)
{
	// ���f��UI�̐���
	CEnemyItem* pItem = nullptr;

	switch (type)
	{
	case CEnemyItem::TYPE_KATANA:

		// ���𐶐�
		pItem = new CEnemyKatana;

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	if (pItem == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �A�N�^�[�̏�����
		if (FAILED(pItem->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �A�N�^�[�̔j��
			SAFE_DELETE(pItem);
			return nullptr;
		}

		// ���f���̊��蓖�ď���
		pItem->BindModel(MODEL[type]);

		// �I�t�Z�b�g���W��ݒ�
		pItem->m_offset = rOffset;

		// ��ނ�ݒ�
		pItem->m_type = type;

		// �I�t�Z�b�g����
		pItem->Offset(rMtx);

		// �m�ۂ����A�h���X��Ԃ�
		return pItem;
	}
}
