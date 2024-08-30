//============================================================
//
//	�A�N�^�[���� [actor.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "actor.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"

#include "sceneGame.h"
#include "collManager.h"
#include "collisionCube.h"
#include "collisionCylinder.h"
#include "collisionSphere.h"
#include "collisionPolygon.h"
#include "mash.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* MODEL[] =	// ���f���̃p�X
	{
		"data\\MODEL\\Rock\\Rock000.x",					// ��(��)
		"data\\MODEL\\Rock\\Rock001.x",					// ��(��)
		"data\\MODEL\\Rock\\Rock002.x",					// ��(��)
		"data\\MODEL\\Rock\\Moss-Rock000.x",			// �ۊ�(��)
		"data\\MODEL\\Rock\\Moss-Rock001.x",			// �ۊ�(��)
		"data\\MODEL\\Rock\\Moss-Rock002.x",			// �ۊ�(��)
		"data\\MODEL\\Tree\\BirchTree000.x",			// �����̖�(��)
		"data\\MODEL\\Tree\\BirchTree001.x",			// �����̖�(��)
		"data\\MODEL\\Tree\\CommonTree000.x",			// ���ʂ̖�(��)
		"data\\MODEL\\Tree\\CommonTree001.x",			// ���ʂ̖�(��)
		"data\\MODEL\\Tree\\Willow000.x",				// ���̖�(��)
		"data\\MODEL\\Tree\\Willow001.x",				// ���̖�(��)
		"data\\MODEL\\Tree\\WoodLog000.x",				// ����
		"data\\MODEL\\Tree\\TreeStump000.x",			// �؂̊�
		"data\\MODEL\\LilyPad\\LilyPad000.x",			// �@�̗t
		"data\\MODEL\\Plant\\Plant000.x",				// 4�����Ȃ����Ă��Ȃ���
		"data\\MODEL\\Plant\\Plant001.x",				// �����Ȃ����Ă��鑐
		"data\\MODEL\\Shrub\\Shrub000.x",				// ���
		"data\\MODEL\\House\\House000.x",				// ��1
		"data\\MODEL\\House\\House001.x",				// ��2
		"data\\MODEL\\House\\House002.x",				// ��3
		"data\\MODEL\\House\\House003.x",				// ��4
		"data\\MODEL\\House\\House004.x",				// ��5
		"data\\MODEL\\House\\House005.x",				// ��6
		"data\\MODEL\\House\\House006.x",				// ��7
		"data\\MODEL\\House\\House007.x",				// ��8
		"data\\MODEL\\House\\House008.x",				// ��9
		"data\\MODEL\\House\\House009.x",				// ��10
		"data\\MODEL\\Lantern\\Lantern001.x",			// �����^��
		"data\\MODEL\\GATE\\gate000.x",					// ��(�g)
		"data\\MODEL\\GATE\\gate001.x",					// ��(��)
		"data\\MODEL\\Kakejiku\\Kakejiku000.x",			// �|����
		"data\\MODEL\\Pier\\Pier000.x",					// ��
		"data\\MODEL\\Pillar\\WoodPillar000.x",			// ��ɔz�u����`�̒�
		"data\\MODEL\\Pillar\\WoodPillar001.x",			// �����l�p�������̒�
		"data\\MODEL\\Stairs\\WoodStairs001.x",			// �K�i
		"data\\MODEL\\B-Lattice\\BambooLattice001.x",	// �|�̊i�q
		"data\\MODEL\\RoofFloor\\FloorRooftop002.x",	// ���g�݂݂����ȉ������̖�
		"data\\MODEL\\RoofFloor\\FloorRooftop003.x",	// �����K�����̉������̖�
		"data\\MODEL\\FUSUMA\\Fusuma000.x",				// �ӂ���(����)
		"data\\MODEL\\FUSUMA\\Fusuma000.x",				// �ӂ���(�����Ȃ�)
		"data\\MODEL\\FENCE\\fence000.x",				// �t�F���X00
		"data\\MODEL\\FENCE\\fence001.x",				// �t�F���X01
		"data\\MODEL\\FENCE\\fence002.x",				// �t�F���X02
		"data\\MODEL\\FENCE\\fence003.x",				// �t�F���X03
	};

	const char *SETUP_TXT = "data\\TXT\\actor.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const int PRIORITY = 4;	// �A�N�^�[�̗D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(MODEL) == CActor::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
// �ÓI�����o�ϐ��錾
//************************************************************
CListManager<CActor>* CActor::m_pList = nullptr;		// ���X�g�\��

//************************************************************
//	�q�N���X [CActor] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CActor::CActor() : CObjectModel(CObject::LABEL_ACTOR, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
m_type(TYPE_ROCK_S)		// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CActor::~CActor()
{

}

//============================================================
//	����������
//============================================================
HRESULT CActor::Init(void)
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
		m_pList = CListManager<CActor>::Create();

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
void CActor::Uninit(void)
{
	// �����蔻��̏�������
	ClearCollision();

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
void CActor::Update(const float fDeltaTime)
{
	// ����������
	bool bFar = useful::IsNearPosR(GetVec3Position());
	SetEnableDraw(bFar);
	if (!bFar) { return; }

	// �����蔻��̃I�t�Z�b�g�ݒ菈��
	CollisionOffset();

	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CActor::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CActor* CActor::Create
( // ����
	const EType type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR3& rScale	// �g�嗦
)
{
	// ���f��UI�̐���
	CActor* pActor = nullptr;
	if (type == TYPE_MASH_MOVE) // �ӂ��܂����q�N���X�𐶐�
	{
		pActor = new CMash(rPos);
	}
	else
	{
		pActor = new CActor;
	}

	// �A�N�^�[�̏�����
	if (FAILED(pActor->Init()))
	{ // �������Ɏ��s�����ꍇ

		// �A�N�^�[�̔j��
		SAFE_DELETE(pActor);
		return nullptr;
	}

	// ���f���̊��蓖�ď���
	pActor->BindModel(MODEL[type]);

	// ������ݒ�
	pActor->SetVec3Rotation(rRot);

	// �ʒu��ݒ�
	pActor->SetVec3Position(rPos);

	// �g�嗦��ݒ�
	pActor->SetVec3Scaling(rScale);

	// ��ނ�ݒ�
	pActor->m_type = type;

	// �����蔻��̊��蓖�ď���
	pActor->BindCollision();

	// �m�ۂ����A�h���X��Ԃ�
	return pActor;
}

//============================================================
// ���X�g�\���̎擾����
//============================================================
CListManager<CActor>* CActor::GetList(void)
{
	// ���X�g�\����Ԃ�
	return m_pList;
}

//============================================================
// �����蔻�菈��
//============================================================
void CActor::Collision
(
	D3DXVECTOR3& rPos,				// �ʒu
	const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
	const float fRadius,			// ���a
	const float fHeight,			// ����
	D3DXVECTOR3& rMove,				// �ړ���
	bool& bJump						// �W�����v��
)
{
	// �X�t�B�A����
	for (auto sphere : m_sphere)
	{
		// �q�b�g����
		sphere->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);
	}

	// �L���[�u����
	for (auto cube : m_cube)
	{
		// �q�b�g����
		cube->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);
	}

	// �V�����_�[����
	for (auto cylinder : m_cylinder)
	{
		// �q�b�g����
		cylinder->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);
	}

	// �|���S������
	for (auto polygon : m_polygon)
	{
		// �q�b�g����
		polygon->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);
	}
}

//==========================================
//  �����Ԃ����Ƃ̂ł��铖���蔻��
//==========================================
void CActor::Collision
(
	D3DXVECTOR3& rPos,				// �ʒu
	const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
	const float fRadius,			// ���a
	const float fHeight,			// ����
	D3DXVECTOR3& rMove,				// �ړ���
	bool& bJump,					// �W�����v��
	bool& bHit						// �Փ˔���
)
{
	// �X�t�B�A����
	for (auto sphere : m_sphere)
	{
		// �q�b�g����
		if (!sphere->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump)) { continue; }

		// �����true�ɂ���
		bHit = true;
	}

	// �L���[�u����
	for (auto cube : m_cube)
	{
		// �q�b�g����
		if (!cube->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump)) { continue; }

		// �����true�ɂ���
		bHit = true;
	}

	// �V�����_�[����
	for (auto cylinder : m_cylinder)
	{
		// �q�b�g����
		if (!cylinder->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump)) { continue; }

		// �����true�ɂ���
		bHit = true;
	}

	// �|���S������
	for (auto polygon : m_polygon)
	{
		// �q�b�g����
		if (!polygon->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump)) { continue; }

		// �����true�ɂ���
		bHit = true;
	}
}

//============================================================
// �����蔻��̏�������
//============================================================
void CActor::ClearCollision(void)
{
	for (auto cube : m_cube)
	{
		// �I������
		cube->Uninit();
	}

	for (auto cylinder : m_cylinder)
	{
		// �I������
		cylinder->Uninit();
	}

	for (auto sphere : m_sphere)
	{
		// �I������
		sphere->Uninit();
	}

	for (auto polygon : m_polygon)
	{
		// �I������
		polygon->Uninit();
	}

	// �N���A����
	m_cube.clear();
	m_cylinder.clear();
	m_sphere.clear();
	m_polygon.clear();
}

//============================================================
// �}�g���b�N�X�v�Z����
//============================================================
D3DXMATRIX CActor::CalcMatrix(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX   mtx;

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	// �g�嗦�𔽉f
	D3DXMatrixScaling(&mtxScale, GetVec3Scaling().x, GetVec3Scaling().y, GetVec3Scaling().z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetVec3Rotation().y, GetVec3Rotation().x, GetVec3Rotation().z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, GetVec3Position().x, GetVec3Position().y, GetVec3Position().z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// �}�g���b�N�X�̌v�Z���ʂ�Ԃ�
	return mtx;
}

//============================================================
// �����蔻��̊��蓖�ď���
//============================================================
void CActor::BindCollision(void)
{
	// �����蔻����擾
	CCollManager::SCollision coll = CScene::GetCollManager()->GetCollInfo(m_type);

	for (int nCnt = 0; nCnt < static_cast<int>(coll.m_cube.size()); nCnt++)
	{
		// �L���[�u�̏���ǉ�����
		m_cube.push_back(CCollisionCube::Create
		(
			GetVec3Position(),				// �ʒu
			coll.m_cube[nCnt].offset,		// �I�t�Z�b�g���W
			coll.m_cube[nCnt].fWidth * GetVec3Scaling().x,		// ��
			coll.m_cube[nCnt].fHeight * GetVec3Scaling().y,		// ����
			coll.m_cube[nCnt].fDepth * GetVec3Scaling().z,		// ���s
			GetVec3Rotation().y				// ����
		));
	}

	for (int nCnt = 0; nCnt < static_cast<int>(coll.m_cylinder.size()); nCnt++)
	{
		// �V�����_�[�̏���ǉ�����
		m_cylinder.push_back(CCollisionCylinder::Create
		(
			GetVec3Position(),				// �ʒu
			coll.m_cylinder[nCnt].offset,	// �I�t�Z�b�g���W
			coll.m_cylinder[nCnt].fRadius * GetVec3Scaling().x,	// ���a
			coll.m_cylinder[nCnt].fHeight * GetVec3Scaling().y	// ����
		));
	}

	for (int nCnt = 0; nCnt < static_cast<int>(coll.m_sphere.size()); nCnt++)
	{
		// �X�t�B�A�̏���ǉ�����
		m_sphere.push_back(CCollisionSphere::Create
		(
			GetVec3Position(),				// �ʒu
			coll.m_sphere[nCnt].offset,		// �I�t�Z�b�g���W
			coll.m_sphere[nCnt].fRadius * GetVec3Scaling().x		// ���a
		));
	}

	for (int nCnt = 0; nCnt < static_cast<int>(coll.m_polygon.size()); nCnt++)
	{
		// �|���S���̏���ǉ�����
		m_polygon.push_back(CCollisionPolygon::Create
		(
			GetVec3Position(),				// �ʒu
			coll.m_polygon[nCnt].offset,	// �I�t�Z�b�g���W
			D3DXVECTOR3(coll.m_polygon[nCnt].rot.x,GetVec3Rotation().y, coll.m_polygon[nCnt].rot.z),		// ����
			D3DXVECTOR3(coll.m_polygon[nCnt].size.x * GetVec3Scaling().x, 0.0f, coll.m_polygon[nCnt].size.z * GetVec3Scaling().z)		// �T�C�Y
		));
	}

	// �����蔻��̏���������
	CollisionOffset();
}

//============================================================
// �����蔻��̃I�t�Z�b�g����
//============================================================
void CActor::CollisionOffset(void)
{
	// �}�g���b�N�X�̌v�Z���ʂ��擾
	D3DXMATRIX mtx = CalcMatrix();

	for (auto cube : m_cube)
	{
		// �ʒu���f
		cube->OffSet(mtx);
	}

	for (auto cylinder : m_cylinder)
	{
		// �ʒu���f
		cylinder->OffSet(mtx);
	}

	for (auto sphere : m_sphere)
	{
		// �ʒu���f
		sphere->OffSet(mtx);
	}

	for (auto polygon : m_polygon)
	{
		// �ʒu���f
		polygon->OffSet(mtx);
	}
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
HRESULT CActor::LoadSetup(const char* pPass)
{
	int nType = 0;					// ��ނ̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	D3DXVECTOR3 scale = VEC3_ONE;	// �g�嗦�̑���p

	// �t�@�C�����J��
	std::ifstream file(pPass);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�A�N�^�[�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

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
		else if (str == "STAGE_ACTORSET")
		{
			do
			{ // END_STAGE_ACTORSET��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str.front() == '#')
				{ // �R�����g�A�E�g����Ă���ꍇ

					// ��s�S�ēǂݍ���
					std::getline(file, str);
				}
				else if (str == "ACTORSET")
				{
					do
					{ // END_ACTORSET��ǂݍ��ނ܂Ń��[�v

						// �������ǂݍ���
						file >> str;

						if (str == "TYPE")
						{
							file >> str;	// ����Ǎ�

							// ��ނ�Ǎ�
							file >> nType;
						}
						else if (str == "POS")
						{
							file >> str;	// ����Ǎ�

							// �ʒu��Ǎ�
							file >> pos.x;
							file >> pos.y;
							file >> pos.z;
						}
						else if (str == "ROT")
						{
							file >> str;	// ����Ǎ�

							// ������Ǎ�
							file >> rot.x;
							file >> rot.y;
							file >> rot.z;
						}
						else if (str == "SCALE")
						{
							file >> str;	// ����Ǎ�

							// �g�嗦��Ǎ�
							file >> scale.x;
							file >> scale.y;
							file >> scale.z;
						}
					} while (str != "END_ACTORSET");	// END_ACTORSET��ǂݍ��ނ܂Ń��[�v

					// �A�N�^�[�I�u�W�F�N�g�̐���
					if (CActor::Create((EType)nType, pos, D3DXToRadian(rot), scale) == nullptr)
					{ // �m�ۂɎ��s�����ꍇ

						// ���s��Ԃ�
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_ACTORSET");	// END_STAGE_ACTORSET��ǂݍ��ނ܂Ń��[�v
		}
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}
