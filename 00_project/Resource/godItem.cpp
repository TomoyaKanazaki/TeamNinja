//============================================================
//
//	�_�폈�� [godItem.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "godItem.h"
#include "renderer.h"
#include "collision.h"
#include "effekseerControl.h"
#include "effekseerManager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* MODEL[] =		// ���f��
	{
		"data\\MODEL\\PLAYER\\02_head.x",	// ���@��
		"data\\MODEL\\PLAYER\\02_head.x",	// ���㌕
		"data\\MODEL\\PLAYER\\02_head.x",	// ����������
	}; 
	const char* SETUP_TXT = "data\\TXT\\goditem.txt";			// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const int PRIORITY = 4;	// �_��̗D�揇��
	const D3DXVECTOR3 OFFSET = D3DXVECTOR3(0.0f, 80.0f, 0.0f);// �G�t�F�N�g�p�I�t�Z�b�g
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(MODEL) == CGodItem::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
// �ÓI�����o�ϐ��錾
//************************************************************
CListManager<CGodItem>* CGodItem::m_pList = nullptr;		// ���X�g�\��
bool CGodItem::m_aGet[TYPE_MAX] = {};						// �擾��

//************************************************************
//	�q�N���X [CGodItem] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGodItem::CGodItem() : CObjectModel(CObject::LABEL_GODITEM, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
m_type(TYPE_MIRROR)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGodItem::~CGodItem()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGodItem::Init(void)
{
	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CGodItem>::Create();

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
void CGodItem::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGodItem::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CGodItem::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CGodItem* CGodItem::Create(const D3DXVECTOR3& rPos, const EType type)
{
	// ��ނ̏d���`�F�b�N����
	assert(DuplicationCheck(type));

	// ���f��UI�̐���
	CGodItem* pCoin = new CGodItem;

	if (pCoin == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �_��̏�����
		if (FAILED(pCoin->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �_��̔j��
			SAFE_DELETE(pCoin);
			return nullptr;
		}

		// �ʒu��ݒ�
		pCoin->SetVec3Position(rPos);

		// ������ݒ�
		pCoin->SetVec3Rotation(VEC3_ZERO);

		// �g�嗦��ݒ�
		pCoin->SetVec3Scaling(VEC3_ONE);

		// ���f���̊��蓖�ď���
		pCoin->BindModel(MODEL[type]);

		// ��ނ�ݒ肷��
		pCoin->m_type = type;

		// �擾�󋵂� false �ɂ���
		m_aGet[pCoin->m_type] = false;

		// �m�ۂ����A�h���X��Ԃ�
		return pCoin;
	}
}

//============================================================
// ���X�g�\���̎擾����
//============================================================
CListManager<CGodItem>* CGodItem::GetList(void)
{
	// ���X�g�\����Ԃ�
	return m_pList;
}

//============================================================
// �����蔻�菈��
//============================================================
bool CGodItem::Collision
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const float fRadius			// ���a
)
{
	if (collision::Circle3D(rPos, GetVec3Position(), fRadius, GetModelData().fRadius))
	{ // ���������ꍇ

		// �擾�󋵂� true �ɂ���
		m_aGet[m_type] = true;

		// TODO : ������ۂ��G�t�F�N�g�ɂ���
		GET_EFFECT->Create("data\\EFFEKSEER\\check.efkefc", rPos + OFFSET, VEC3_ZERO, VEC3_ZERO, 30.0f);

		// �I������
		Uninit();

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
	return false;
}

//============================================================
// ��ނ̐ݒ菈��
//============================================================
void CGodItem::SetType(const EType type)
{
	// ��ނ�ݒ肷��
	m_type = type;
}

//============================================================
// ��ނ̎擾����
//============================================================
CGodItem::EType CGodItem::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
HRESULT CGodItem::LoadSetup(const char* pPass)
{
	int type = 0;					// ���
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p

	// �t�@�C�����J��
	std::ifstream file(pPass);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�_��Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

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
		else if (str == "STAGE_GODITEMSET")
		{
			do
			{ // END_STAGE_GODITEMSET��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str.front() == '#')
				{ // �R�����g�A�E�g����Ă���ꍇ

					// ��s�S�ēǂݍ���
					std::getline(file, str);
				}
				else if (str == "GODITEMSET")
				{
					do
					{ // END_GODITEMSET��ǂݍ��ނ܂Ń��[�v

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
						else if (str == "TYPE")
						{
							file >> str;	// ����Ǎ�

							// ��ނ�Ǎ�
							file >> type;
						}
					} while (str != "END_GODITEMSET");	// END_GODITEMSET��ǂݍ��ނ܂Ń��[�v

					// �_��I�u�W�F�N�g�̐���
					if (CGodItem::Create(pos, (EType)type) == nullptr)
					{ // �m�ۂɎ��s�����ꍇ

						// ���s��Ԃ�
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_GODITEMSET");	// END_STAGE_GODITEMSET��ǂݍ��ނ܂Ń��[�v
		}
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}

//============================================================
// �d���`�F�b�N����
//============================================================
bool CGodItem::DuplicationCheck(const EType type)
{
	if (m_pList == nullptr)
	{ // �_�킪������݂��Ă��Ȃ��ꍇ

		// true ��Ԃ�
		return true;
	}
	else
	{ // ��L�ȊO

		if (m_pList->GetNumAll() >= TYPE_MAX)
		{ // �������S��ވȏ�̏ꍇ

			// false ��Ԃ�
			return false;
		}

		for (auto& item : m_pList->GetList())
		{
			if (item->m_type == type)
			{ // ��ނ��d�����Ă����ꍇ

				// false ��Ԃ�
				return false;
			}
		}

		// true ��Ԃ�
		return true;
	}
}

//============================================================
// �擾�󋵎擾
//============================================================
bool CGodItem::IsGet(const EType type)
{
	// �擾�󋵂�Ԃ�
	return m_aGet[type];
}
