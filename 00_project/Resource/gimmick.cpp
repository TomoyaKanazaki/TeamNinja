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
#include "gimmick_step.h"
#include "gimmick_post.h"
#include "gimmick_multi.h"
#include "gimmick_jumpoff.h"
#include "gimmick_heavydoor.h"
#include "gimmick_bridge.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 2;				// �M�~�b�N�͈̓|���S���̗D�揇��
	const int INIT_NUM_ACTIVE = -1;		// �����\�l���̏����l
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
CGimmick::CGimmick() : CObject3D(CObject::LABEL_GIMMICK, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
m_type(TYPE_JUMPTABLE),			// ���
m_nNumActive(INIT_NUM_ACTIVE)	// �����\�ȕ��g�̐�
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

	// �f�o�b�O�̎��ȊO�͕`�悵�Ȃ�
#ifndef _DEBUG
	SetEnableDraw(false);
#endif
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
CGimmick* CGimmick::Create
(
	const D3DXVECTOR3& rPos,		// �ʒu
	const EAngle& eAngle,			// �p�x
	const D3DXVECTOR3& rSize,		// �T�C�Y
	const EType type,				// ���
	const int nNumActive			// �����\�ȕ��g�̐�
)
{
	// ���g��UI�̐���
	CGimmick* pGimmick = nullptr;

	switch (type)
	{
	case CGimmick::TYPE_JUMPTABLE:		// �W�����v��

		pGimmick = new CGimmickJumpTable;

		break;

	case CGimmick::TYPE_STEP:			// ���ݑ�

		pGimmick = new CGimmickStep;

		break;

	case CGimmick::TYPE_POST:			// �ݒu

		pGimmick = new CGimmickPost;

		break;

	case CGimmick::TYPE_JUMPOFF:		// ��э~��

		pGimmick = new CGimmickJumpOff;

		break;

	case CGimmick::TYPE_HEAVYDOOR:		// �d����

		pGimmick = new CGimmickHeavyDoor;

		break;

	case CGimmick::TYPE_BRIDGE:		// ��

		pGimmick = new CGimmickBridge;

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

		// ��ނ�ݒ�
		pGimmick->m_type = type;

		// �����\�ȕ��g�̐���ݒ�
		pGimmick->m_nNumActive = nNumActive;

		// ������ݒ�
		pGimmick->m_eAngle = eAngle;

		// �ʒu��ݒ�
		pGimmick->SetVec3Position(rPos + D3DXVECTOR3(0.0f, 0.01f, 0.0f));

		// �T�C�Y��ݒ�
		pGimmick->SetVec3Sizing(rSize);

		// ���g��`�悵�Ȃ�
		//pGimmick->SetEnableDraw(false);

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

//============================================================
// �Z�b�g�A�b�v����
//============================================================
HRESULT CGimmick::LoadSetup(const char* pPass)
{
	D3DXVECTOR3 pos		= VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 size	= VEC3_ZERO;	// �傫���̑���p
	int nAngle	= 0;	// �����̑���p
	int nType	= 0;	// ��ނ̑���p
	int nNumAct	= 0;	// �K�v�l���̑���p

	// �t�@�C�����J��
	std::ifstream file(pPass);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�M�~�b�N�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

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
		else if (str == "STAGE_GIMMICKSET")
		{
			do { // END_STAGE_GIMMICKSET��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str.front() == '#')
				{ // �R�����g�A�E�g����Ă���ꍇ

					// ��s�S�ēǂݍ���
					std::getline(file, str);
				}

				// �ʏ�M�~�b�N�̓Ǎ�
				else if (str == "GIMMICKSET")
				{
					do { // END_GIMMICKSET��ǂݍ��ނ܂Ń��[�v

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
						else if (str == "ANGLE")
						{
							file >> str;	// ����Ǎ�
							file >> nAngle;	// ������Ǎ�
						}
						else if (str == "SIZE")
						{
							file >> str;	// ����Ǎ�

							// �傫����Ǎ�
							file >> size.x;
							file >> size.y;
							file >> size.z;
						}
						else if (str == "TYPE")
						{
							file >> str;	// ����Ǎ�
							file >> nType;	// ��ނ�Ǎ�
						}
						else if (str == "NUMACT")
						{
							file >> str;		// ����Ǎ�
							file >> nNumAct;	// �K�v�l����Ǎ�
						}
					} while (str != "END_GIMMICKSET");	// END_GIMMICKSET��ǂݍ��ނ܂Ń��[�v

					// �M�~�b�N�̐���
					if (CGimmick::Create(pos, (EAngle)nAngle, size, (CGimmick::EType)nType, nNumAct) == nullptr)
					{ // �m�ۂɎ��s�����ꍇ

						// ���s��Ԃ�
						assert(false);
						return E_FAIL;
					}
				}

				// �{�^���M�~�b�N�̓Ǎ�
				else if (str == "BUTTON_GIMMICKSET")
				{
					// ���W�ƌ����̕ϐ�
					D3DXVECTOR3 posMulch = VEC3_ZERO;
					int angle = ANGLE_0;

					// �������ǂݍ���
					file >> str;

					if (str.front() == '#')
					{ // �R�����g�A�E�g����Ă���ꍇ

						// ��s�S�ēǂݍ���
						std::getline(file, str);
					}
					else if (str == "MULCHSET")
					{
						do
						{ // END_MULCHSET��ǂݍ��ނ܂Ń��[�v

							// �������ǂݍ���
							file >> str;

							if (str == "POS")
							{
								file >> str;	// ����Ǎ�

								// �ʒu��Ǎ�
								file >> posMulch.x;
								file >> posMulch.y;
								file >> posMulch.z;
							}
							else if (str == "ANGLE")
							{
								file >> str;	// ����Ǎ�

								// ������Ǎ�
								file >> angle;
							}
						} while (str != "END_MULCHSET");	// END_MULCHSET��ǂݍ��ނ܂Ń��[�v
					}

					std::vector<CGimmickMulti::SButton> vecButton;	// �{�^�����z��
					do { // END_BUTTON_GIMMICKSET��ǂݍ��ނ܂Ń��[�v

						// �������ǂݍ���
						file >> str;

						if (str.front() == '#')
						{ // �R�����g�A�E�g����Ă���ꍇ

							// ��s�S�ēǂݍ���
							std::getline(file, str);
						}
						else if (str == "BUTTONSET")
						{
							do { // END_BUTTONSET��ǂݍ��ނ܂Ń��[�v

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
								else if (str == "SIZE")
								{
									file >> str;	// ����Ǎ�

									// �傫����Ǎ�
									file >> size.x;
									file >> size.y;
									file >> size.z;
								}
							} while (str != "END_BUTTONSET");	// END_BUTTONSET��ǂݍ��ނ܂Ń��[�v

							// �{�^�������Ō���ɒǉ�
							vecButton.push_back(CGimmickMulti::SButton(pos, size));
						}
					} while (str != "END_BUTTON_GIMMICKSET");	// END_BUTTON_GIMMICKSET��ǂݍ��ނ܂Ń��[�v

					// �M�~�b�N�{�^�������N���X�̐���
					if (CGimmickMulti::Create(posMulch, (EAngle)angle, vecButton) == nullptr)
					{ // �m�ۂɎ��s�����ꍇ

						// ���s��Ԃ�
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_GIMMICKSET");	// END_STAGE_GIMMICKSET��ǂݍ��ނ܂Ń��[�v
		}
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}
