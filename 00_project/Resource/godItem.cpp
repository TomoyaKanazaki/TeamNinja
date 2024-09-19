//============================================================
//
//	�_�폈�� [godItem.cpp]
//	Author�F��������
//	Adder �F���c�E��
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
#include "sound.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "player.h"
#include "stage.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* MODEL = "data\\MODEL\\GODITEM\\Magatama.x";		// ���f��
	const char* SETUP_TXT = "data\\TXT\\goditem.txt";			// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const int PRIORITY = 4;		// �_��̗D�揇��
	const D3DXVECTOR3 EFFECT_OFFSET = D3DXVECTOR3(0.0f, 36.0f, 0.0f);	// �G�t�F�N�g�p�I�t�Z�b�g
	const float COLLISION_RADIUS = 70.0f;						// �����蔻��p�̔��a

	const float ADD_ROT = 0.03f;		// �����̒ǉ���
	const float ADD_HEIGHT = 25.0f;		// �����̒ǉ���
	const D3DXVECTOR3 ROLL_DEST_POS = D3DXVECTOR3(0.0f, 100.0f, 0.0f);	// ��]��Ԃ̈ړ��ڕW�ʒu
	const D3DXVECTOR3 ROLL_ROT = D3DXVECTOR3(0.0f, 4.5f, 0.0f);			// ��]��Ԃ̌���
}

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
m_pEffectBody(nullptr),	// �{�̃G�t�F�N�g���
m_pEffectLand(nullptr),	// ���n�G�t�F�N�g���
m_fPosInitY(0.0f),		// �����ʒu(Y��)
m_type(TYPE_RED),		// ���
m_state(STATE_ITEM),	// ���
m_fHeightRot(0.0f)		// �����̌���
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
	// �{�̃G�t�F�N�g�̍폜
	SAFE_DELETE(m_pEffectBody);

	// ���n�G�t�F�N�g�̍폜
	SAFE_DELETE(m_pEffectLand);

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
	// ����������
	bool bFar = useful::IsNearPosR(GetVec3Position());
	SetEnableDraw(bFar);
	if (!bFar) { return; }

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_ITEM:

		// ��������
		Cycle();

		// �����ݒ菈��
		Height();

		break;

	case STATE_ROLL:

		// ��]�ړ��̍X�V
		UpdateRoll(fDeltaTime);
		break;
	}

	if (m_pEffectBody != nullptr)
	{
		// �G�t�F�N�g�ʒu�𔽉f
		m_pEffectBody->m_pos = GetVec3Position() + EFFECT_OFFSET;
	}

	if (m_pEffectLand != nullptr)
	{
		D3DXVECTOR3 posThis = GetVec3Position();

		// �n�ʂ̒��n�ʒu�̎擾
		posThis.y = GET_STAGE->GetFieldDownPositionHeight(posThis);

		// �G�t�F�N�g�ʒu�𔽉f
		m_pEffectLand->m_pos = posThis;
	}

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
	CGodItem* pItem = new CGodItem;

	if (pItem == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �_��̏�����
		if (FAILED(pItem->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �_��̔j��
			SAFE_DELETE(pItem);
			return nullptr;
		}

		// �ʒu��ݒ�
		pItem->SetVec3Position(rPos);

		// �����ʒu��ݒ�
		pItem->m_fPosInitY = rPos.y;

		// ������ݒ�
		pItem->SetVec3Rotation(VEC3_ZERO);

		// �g�嗦��ݒ�
		pItem->SetVec3Scaling(VEC3_ONE);

		// ���f���̊��蓖�ď���
		pItem->BindModel(MODEL);

		// �}�e���A���̊��蓖��
		switch (type)
		{
		case CGodItem::TYPE_RED:
			pItem->SetAllMaterial(material::Red());		// �ԐF
			break;

		case CGodItem::TYPE_GREEN:
			pItem->SetAllMaterial(material::Green());	// �ΐF
			break;

		case CGodItem::TYPE_BLUE:
			pItem->SetAllMaterial(material::Blue());	// �F
			break;

		default:
			assert(false);
			break;
		}

		// ��ނ�ݒ肷��
		pItem->m_type = type;

		// �擾�󋵂� false �ɂ���
		m_aGet[pItem->m_type] = false;

		// �{�̂��甭������̃G�t�F�N�g������
		pItem->m_pEffectBody = GET_EFFECT->Create("data\\EFFEKSEER\\magatama_light.efkefc", rPos + EFFECT_OFFSET, VEC3_ZERO, VEC3_ZERO, 30.0f, false, false);

		// �n�ʂ��甭������̃G�t�F�N�g������
		D3DXVECTOR3 posThis = rPos;
		posThis.y = GET_STAGE->GetFieldDownPositionHeight(posThis);	// �n�ʂ̒��n�ʒu�̎擾
		pItem->m_pEffectLand = GET_EFFECT->Create("data\\EFFEKSEER\\aura.efkefc", posThis, VEC3_ZERO, VEC3_ZERO, 50.0f, false, false);

		// �m�ۂ����A�h���X��Ԃ�
		return pItem;
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
//	��]�ړ���Ԃ̖ڕW�ʒu�ݒ菈��
//============================================================
void CGodItem::SetRollPosition(void)
{
	// �������S��ވȏ�̏ꍇ�G���[
	if (m_pList->GetNumAll() > TYPE_MAX) { assert(false); }

	D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 posDest = posPlayer + ROLL_DEST_POS;		// �ړ��ڕW�ʒu

	for (auto& item : m_pList->GetList())
	{
		if (item->m_state == STATE_ROLL)
		{ // ��]��Ԃ̏ꍇ

			// ���݈ʒu�𔽉f
			item->SetVec3Position(posDest);

			if (item->m_pEffectBody != nullptr)
			{
				// �G�t�F�N�g�ʒu�𔽉f
				item->m_pEffectBody->m_pos = item->GetVec3Position() + EFFECT_OFFSET;
			}
		}
	}
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
	// �A�C�e����Ԃł͂Ȃ��ꍇ������
	if (m_state != STATE_ITEM) { return false; }

	if (collision::Circle3D(rPos, GetVec3Position(), fRadius, COLLISION_RADIUS))
	{ // ���������ꍇ

		// �擾�󋵂� true �ɂ���
		m_aGet[m_type] = true;

		// �_��擾����炷
		PLAY_SOUND(CSound::LABEL_SE_GETGODITEM_000);

		if (GET_MANAGER->GetMode() == CScene::MODE_GAME)
		{ // �Q�[���V�[���̏ꍇ

			// �Q�[���}�l�[�W���[��_��l����Ԃɂ���
			CSceneGame::GetGameManager()->PossessGodItem(m_type);
		}

		// ���n�G�t�F�N�g�̍폜
		SAFE_DELETE(m_pEffectLand);

		// ��]�ړ���Ԃɂ���
		m_state = STATE_ROLL;

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
	return false;
}

//============================================================
// �����ʒu(Y��)�̎擾����
//============================================================
float CGodItem::GetInitPosY(void) const
{
	// �����ʒu��Ԃ�
	return m_fPosInitY;
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

					if (!m_aGet[type])
					{ // ���ʂ��擾���Ă��Ȃ��ꍇ

						// �_��I�u�W�F�N�g�̐���
						if (CGodItem::Create(pos, (EType)type) == nullptr)
						{ // �m�ۂɎ��s�����ꍇ

							// ���s��Ԃ�
							assert(false);
							return E_FAIL;
						}
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
// �l���󋵂̓Ǎ�����
//============================================================
HRESULT CGodItem::LoadPossess(const char* pPass, bool* pGet)
{
	// �t�@�C�����J��
	std::ifstream file(pPass, std::ios_base::binary);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

#ifdef _DEBUG
		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�_��l���󋵂̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
#endif

		// �l���󋵂̕ۑ�
		bool aGet[TYPE_MAX] = {};
		SavePossess(pPass, &aGet[0]);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �����̊l���󋵂������o��
	file.read((char*)pGet, sizeof(bool) * TYPE_MAX);

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}

//============================================================
// �l���󋵂̕ۑ�����
//============================================================
HRESULT CGodItem::SavePossess(const char* pPass, bool* pGet)
{
	// �t�@�C�����J��
	std::ofstream file(pPass, std::ios_base::binary);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�_��l���󋵂̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �����̊l���󋵂������o��
	file.write((char*)pGet, sizeof(bool) * TYPE_MAX);

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

//============================================================
// �擾�󋵂̏�����
//============================================================
void CGodItem::InitGet(void)
{
	for (int i = 0; i < TYPE_MAX; i++)
	{ // ��ނ̑������J��Ԃ�

		// �擾��OFF�ɂ���
		m_aGet[i] = false;
	}
}

//============================================================
//	�_��̍폜����
//============================================================
void CGodItem::Delete(const EType type)
{
	// �������S��ވȏ�̏ꍇ�G���[
	if (m_pList->GetNumAll() > TYPE_MAX) { assert(false); }

	for (auto& item : m_pList->GetList())
	{
		if (item->m_type == type)
		{ // ��ނ��d�����Ă����ꍇ

			// �I������
			item->Uninit();
		}
	}
}

//============================================================
// ��������
//============================================================
void CGodItem::Cycle(void)
{
	// �������擾
	D3DXVECTOR3 rot = GetVec3Rotation();

	// ��]������
	rot.y += ADD_ROT;

	// �����̐��K��
	useful::NormalizeRot(rot.y);

	// ������K�p
	SetVec3Rotation(rot);
}

//============================================================
// �����ݒ菈��
//============================================================
void CGodItem::Height(void)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = GetVec3Position();

	// ���������Z����
	m_fHeightRot += ADD_ROT;

	// �����̐��K��
	useful::NormalizeRot(m_fHeightRot);

	// ������ݒ肷��
	pos.y = m_fPosInitY + (cosf(m_fHeightRot) * -ADD_HEIGHT) + ADD_HEIGHT;

	// �ʒu��K�p
	SetVec3Position(pos);
}

//============================================================
//	��]�ړ��̍X�V����
//============================================================
void CGodItem::UpdateRoll(const float fDeltaTime)
{
	D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GET_PLAYER->GetVec3Rotation();	// �v���C���[����
	D3DXVECTOR3 posCur = GetVec3Position();	// ���݈ʒu

	//--------------------------------------------------------
	//	�ʒu�̐ݒ�
	//--------------------------------------------------------
	// �ڕW�ʒu���v�Z
	D3DXVECTOR3 posDest = posPlayer + D3DXVECTOR3
	(
		23.0f * sinf(rotPlayer.y - HALF_PI) + 14.0f * sinf(rotPlayer.y - D3DX_PI),
		118.0f,
		23.0f * cosf(rotPlayer.y - HALF_PI) + 14.0f * cosf(rotPlayer.y - D3DX_PI)
	);

	// �����ʒu���v�Z
	D3DXVECTOR3 posDiff = posDest - posCur;

	// ���݈ʒu���X�V
	posCur += posDiff * 4.0f * fDeltaTime;

	// ���݈ʒu�𔽉f
	SetVec3Position(posCur);

	//--------------------------------------------------------
	//	�g�嗦�̐ݒ�
	//--------------------------------------------------------
	D3DXVECTOR3 scaleCur = GetVec3Scaling();	// ���݊g�嗦

	// �����g�嗦���v�Z
	D3DXVECTOR3 scaleDiff = D3DXVECTOR3(0.75f, 0.75f, 0.75f) - scaleCur;

	// ���݊g�嗦���X�V
	scaleCur += scaleDiff * 2.0f * fDeltaTime;

	// ���݊g�嗦�𔽉f
	SetVec3Scaling(scaleCur);

	//--------------------------------------------------------
	//	�����̐ݒ�
	//--------------------------------------------------------
	// ��������]
	SetVec3Rotation(GetVec3Rotation() + ROLL_ROT * fDeltaTime);
}
