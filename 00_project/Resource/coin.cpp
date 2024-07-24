//============================================================
//
//	�R�C������ [coin.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "coin.h"
#include "renderer.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* MODEL = "data\\MODEL\\PLAYER\\02_head.x";	// ���f���̃p�X
	const char* SETUP_TXT = "data\\TXT\\coin.txt";			// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const int PRIORITY = 4;	// �R�C���̗D�揇��
}

//************************************************************
// �ÓI�����o�ϐ��錾
//************************************************************
CListManager<CCoin>* CCoin::m_pList = nullptr;		// ���X�g�\��

//************************************************************
//	�q�N���X [CCoin] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CCoin::CCoin() : CObjectModel(CObject::LABEL_COIN, CObject::DIM_3D, PRIORITY)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CCoin::~CCoin()
{

}

//============================================================
//	����������
//============================================================
HRESULT CCoin::Init(void)
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
		m_pList = CListManager<CCoin>::Create();

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
void CCoin::Uninit(void)
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
void CCoin::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CCoin::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CCoin* CCoin::Create(const D3DXVECTOR3& rPos)
{
	// ���f��UI�̐���
	CCoin* pCoin = new CCoin;

	if (pCoin == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �R�C���̏�����
		if (FAILED(pCoin->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �R�C���̔j��
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
		pCoin->BindModel(MODEL);

		// �m�ۂ����A�h���X��Ԃ�
		return pCoin;
	}
}

//============================================================
// ���X�g�\���̎擾����
//============================================================
CListManager<CCoin>* CCoin::GetList(void)
{
	// ���X�g�\����Ԃ�
	return m_pList;
}

//============================================================
// �����蔻�菈��
//============================================================
void CCoin::Collision
(
	D3DXVECTOR3& rPos,				// �ʒu
	const float fRadius,			// ���a
	const float fHeight				// ����
)
{

}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
HRESULT CCoin::LoadSetup(const char* pPass)
{
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p

	// �t�@�C�����J��
	std::ifstream file(pPass);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�R�C���Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

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
		else if (str == "STAGE_COINSET")
		{
			do
			{ // END_STAGE_COINSET��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str.front() == '#')
				{ // �R�����g�A�E�g����Ă���ꍇ

					// ��s�S�ēǂݍ���
					std::getline(file, str);
				}
				else if (str == "COINSET")
				{
					do
					{ // END_COINSET��ǂݍ��ނ܂Ń��[�v

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
					} while (str != "END_COINSET");	// END_COINSET��ǂݍ��ނ܂Ń��[�v

					// �R�C���I�u�W�F�N�g�̐���
					if (CCoin::Create(pos) == nullptr)
					{ // �m�ۂɎ��s�����ꍇ

						// ���s��Ԃ�
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_COINSET");	// END_STAGE_COINSET��ǂݍ��ނ܂Ń��[�v
		}
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}
