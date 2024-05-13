//==========================================
//
//  �}�b�v���(map.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "map.h"

#include "checkpoint.h"
#include "goal.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const char* MAP_TXT = "data\\TXT\\map.txt"; // �}�b�v���̃p�X
}

//==========================================
//  �}�b�v�̐���
//==========================================
HRESULT map::Create()
{
	//���[�J���ϐ��錾
	FILE* pFile; // �t�@�C���|�C���^

	//�t�@�C����ǂݎ���p�ŊJ��
	pFile = fopen(MAP_TXT, "r");

	// �t�@�C�����J���Ȃ������ꍇ
	if (pFile == NULL) { assert(false); return E_FAIL; }

	// ���̓ǂݍ���
	while (1)
	{
		// ������̋L�^�p
		char aStr[256];

		// ������ǂݍ���
		fscanf(pFile, "%s", &aStr[0]);

		// ��������
		if (strcmp(&aStr[0], "CHECKPOINT") == 0) // �`�F�b�N�|�C���g�̐���
		{
			// �f�[�^�̎擾�p�ϐ�
			D3DXVECTOR3 pos, rot;

			// ������ǂݍ��� (POS)
			fscanf(pFile, "%s", &aStr[0]);

			// �f�[�^�擾
			fscanf(pFile, "%f", &pos.x);
			fscanf(pFile, "%f", &pos.y);
			fscanf(pFile, "%f", &pos.z);

			// ������ǂݍ��� (ROT)
			fscanf(pFile, "%s", &aStr[0]);

			// �f�[�^�擾
			fscanf(pFile, "%f", &rot.x);
			fscanf(pFile, "%f", &rot.y);
			fscanf(pFile, "%f", &rot.z);

			// �`�F�b�N�|�C���g�𐶐�
			CCheckPoint::Create(pos, rot);
		}
		if (strcmp(&aStr[0], "GOAL") == 0) // �`�F�b�N�|�C���g�̐���
		{
			// �f�[�^�̎擾�p�ϐ�
			D3DXVECTOR3 pos, rot;

			// ������ǂݍ��� (POS)
			fscanf(pFile, "%s", &aStr[0]);

			// �f�[�^�擾
			fscanf(pFile, "%f", &pos.x);
			fscanf(pFile, "%f", &pos.y);
			fscanf(pFile, "%f", &pos.z);

			// ������ǂݍ��� (ROT)
			fscanf(pFile, "%s", &aStr[0]);

			// �f�[�^�擾
			fscanf(pFile, "%f", &rot.x);
			fscanf(pFile, "%f", &rot.y);
			fscanf(pFile, "%f", &rot.z);

			// �`�F�b�N�|�C���g�𐶐�
			CGoal::Create(pos, rot);
		}
		if (strcmp(&aStr[0], "END_OF_FILE") == 0) // �ǂݍ��ݏI��
		{
			break;
		}
	}

	return S_OK;
}
