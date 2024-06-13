//============================================================
//
//	�����蔻��}�l�[�W���[���� [collManager.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "collManager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* COLLISION_TXT = "data\\TXT\\Collision.txt";		// �����蔻��̃e�L�X�g
}

//************************************************************
//	�e�N���X [CCollManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CCollManager::CCollManager()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CCollManager::~CCollManager()
{

}

//============================================================
//	�I������
//============================================================
void CCollManager::Uninit(void)
{
	// �S�Ă̒l���N���A����
	m_aCollInfo->m_cube.clear();		// �L���[�u
	m_aCollInfo->m_cylinder.clear();	// �V�����_�[
	m_aCollInfo->m_sphere.clear();		// �X�t�B�A

	// ���g��j������
	delete this;
}

//============================================================
// ���[�h����
//============================================================
HRESULT CCollManager::Load(void)
{
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	SCollCube cubeInfo;				// �L���[�u�̏��
	SCollCylinder cylinderInfo;		// �V�����_�[�̏��
	SCollSphere sphereInfo;			// �X�t�B�A�̏��
	int nNum = 0;					// ����

	// �t�@�C�����J��
	std::ifstream file(COLLISION_TXT);		// �t�@�C���X�g���[��

	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�����蔻��̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

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
		else if (str == "STAGE_COLLISIONSET")
		{ // �X�e�[�W�̓����蔻��̐ݒ��ǂݍ��񂾏ꍇ

			do
			{ // END_STAGE_COLLISIONSET��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str.front() == '#')
				{ // �R�����g�A�E�g����Ă���ꍇ

					// ��s�S�ēǂݍ���
					std::getline(file, str);
				}
				else if (str == "CUBE_COLLISIONSET")
				{ // �L���[�u�̓����蔻���ǂݍ��񂾏ꍇ

					do
					{ // END_CUBE_COLLISIONSET��ǂݍ��ނ܂Ń��[�v

						// �������ǂݍ���
						file >> str;

						if (str.front() == '#')
						{ // �R�����g�A�E�g����Ă���ꍇ

							// ��s�S�ēǂݍ���
							std::getline(file, str);
						}
						else if (str == "CUBE_NUM")
						{ // �L���[�u�̐���ǂݍ��񂾏ꍇ

							file >> str;	// ����Ǎ�
							file >> nNum;	// ������Ǎ�

							for (int nCnt = 0; nCnt < nNum; nCnt++)
							{
								while (file >> str)
								{
									if (str == "COLLISIONSET")
									{ // �R���W�����ݒ��ǂݍ��񂾏ꍇ

										// OFFSET��Ǎ�
										file >> str;
										file >> str;
										file >> cubeInfo.offset.x;
										file >> cubeInfo.offset.y;
										file >> cubeInfo.offset.z;

										// WIDTH��ǂݍ���
										file >> str;	// WIDTH��Ǎ�
										file >> str;	// ����Ǎ�
										file >> cubeInfo.fWidth;	// ����Ǎ�

										// HEIGHT��ǂݍ���
										file >> str;	// HEIGHT��Ǎ�
										file >> str;	// ����Ǎ�
										file >> cubeInfo.fHeight;	// ������Ǎ�

										// DEPTH��ǂݍ���
										file >> str;	// DEPTH��Ǎ�
										file >> str;	// ����Ǎ�
										file >> cubeInfo.fDepth;	// ���s��Ǎ�

										// END_COLLISIONSET��Ǎ�
										file >> str;

										// while���𔲂��o��
										break;
									}
								}

								// �L���[�u�̏���ݒ肷��
								m_aCollInfo->m_cube.push_back(cubeInfo);

								// �L���[�u�̓����蔻���������
								cubeInfo.offset = VEC3_ZERO;	// �I�t�Z�b�g���W
								cubeInfo.fWidth = 0.0f;			// ��
								cubeInfo.fHeight = 0.0f;		// ����
								cubeInfo.fDepth = 0.0f;			// ���s
							}
						}
					} while (str != "END_CUBE_COLLISIONSET");	// END_CUBE_COLLISIONSET��ǂݍ��ނ܂Ń��[�v
				}
				else if (str == "CYLINDER_COLLISIONSET")
				{ // �V�����_�[�̓����蔻���ǂݍ��񂾏ꍇ

					do
					{ // END_CYLINDER_COLLISIONSET��ǂݍ��ނ܂Ń��[�v

						// �������ǂݍ���
						file >> str;

						if (str.front() == '#')
						{ // �R�����g�A�E�g����Ă���ꍇ

							// ��s�S�ēǂݍ���
							std::getline(file, str);
						}
						else if (str == "CYLINDER_NUM")
						{ // �V�����_�[�̐���ǂݍ��񂾏ꍇ

							file >> str;	// ����Ǎ�
							file >> nNum;	// ������Ǎ�

							for (int nCnt = 0; nCnt < nNum; nCnt++)
							{
								while (file >> str)
								{
									if (str == "COLLISIONSET")
									{ // �R���W�����ݒ��ǂݍ��񂾏ꍇ

										// OFFSET��Ǎ�
										file >> str;
										file >> str;
										file >> cylinderInfo.offset.x;
										file >> cylinderInfo.offset.y;
										file >> cylinderInfo.offset.z;

										// RADIUS��ǂݍ���
										file >> str;	// RADIUS��Ǎ�
										file >> str;	// ����Ǎ�
										file >> cylinderInfo.fRadius;	// ���a��Ǎ�

										// HEIGHT��ǂݍ���
										file >> str;	// HEIGHT��Ǎ�
										file >> str;	// ����Ǎ�
										file >> cylinderInfo.fHeight;	// ������Ǎ�

										// END_COLLISIONSET��Ǎ�
										file >> str;

										// while���𔲂��o��
										break;
									}
								}

								// �V�����_�[�̏���ݒ肷��
								m_aCollInfo->m_cylinder.push_back(cylinderInfo);

								// �V�����_�[�̓����蔻���������
								cylinderInfo.offset = VEC3_ZERO;	// �I�t�Z�b�g���W
								cylinderInfo.fRadius = 0.0f;		// ���a
								cylinderInfo.fHeight = 0.0f;		// ����
							}
						}
					} while (str != "END_CYLINDER_COLLISIONSET");	// END_CYLINDER_COLLISIONSET��ǂݍ��ނ܂Ń��[�v
				}
				else if (str == "SPHERE_COLLISIONSET")
				{ // �X�t�B�A�̓����蔻���ǂݍ��񂾏ꍇ

				do
				{ // END_SPHERE_COLLISIONSET��ǂݍ��ނ܂Ń��[�v

					// �������ǂݍ���
					file >> str;

					if (str.front() == '#')
					{ // �R�����g�A�E�g����Ă���ꍇ

						// ��s�S�ēǂݍ���
						std::getline(file, str);
					}
					else if (str == "SPHERE_NUM")
					{ // �X�t�B�A�̐���ǂݍ��񂾏ꍇ

						file >> str;	// ����Ǎ�
						file >> nNum;	// ������Ǎ�

						for (int nCnt = 0; nCnt < nNum; nCnt++)
						{
							while (file >> str)
							{
								if (str == "COLLISIONSET")
								{ // �R���W�����ݒ��ǂݍ��񂾏ꍇ

									// OFFSET��Ǎ�
									file >> str;
									file >> str;
									file >> sphereInfo.offset.x;
									file >> sphereInfo.offset.y;
									file >> sphereInfo.offset.z;

									// RADIUS��ǂݍ���
									file >> str;	// RADIUS��Ǎ�
									file >> str;	// ����Ǎ�
									file >> sphereInfo.fRadius;	// ���a��Ǎ�

									// END_COLLISIONSET��Ǎ�
									file >> str;

									// while���𔲂��o��
									break;
								}
							}

							// �X�t�B�A�̏���ݒ肷��
							m_aCollInfo->m_sphere.push_back(sphereInfo);

							// �X�t�B�A�̓����蔻���������
							sphereInfo.offset = VEC3_ZERO;	// �I�t�Z�b�g���W
							sphereInfo.fRadius = 0.0f;		// ���a
						}
					}
				} while (str != "END_SPHERE_COLLISIONSET");	// END_SPHERE_COLLISIONSET��ǂݍ��ނ܂Ń��[�v
				}
			} while (str != "END_STAGE_COLLISIONSET");	// END_STAGE_COLLISIONSET��ǂݍ��ނ܂Ń��[�v
		}
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}

//============================================================
// ���̎擾����
//============================================================
CCollManager::SCollision CCollManager::GetCollInfo(const CActor::EType type)
{
	// �����蔻��̏���Ԃ�
	return m_aCollInfo[type];
}

//============================================================
//	��������
//============================================================
CCollManager* CCollManager::Create(void)
{
	// �����蔻��̐���
	CCollManager* pColl = new CCollManager;

	if (pColl == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}

	if (FAILED(pColl->Load()))
	{ // �Ǎ��Ɏ��s�����ꍇ

		return nullptr;
	}

	// �m�ۂ����A�h���X��Ԃ�
	return pColl;
}
