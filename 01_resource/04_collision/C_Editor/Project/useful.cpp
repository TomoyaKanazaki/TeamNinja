//============================================================
//
//	�֗��֐����� [useful.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "useful.h"

//************************************************************
//	�ʏ�֐�
//************************************************************
//	�֗��֐����
//************************************************************
//============================================================
//	���l�̌������Ƃ̕���
//============================================================
//	�X�R�A�\���̍ۂȂǂɎg�p
//============================================================
void useful::DivideDigitNum
(
	int *pNumDivide,	// �������ʂ̊i�[�z��
	const int nNum,		// �������鐔�l
	const int nMaxDigit	// �������鐔���̌���
)
{
	// �ϐ���錾
	int nDigit = 1;	// �e���̐��l�̌v�Z�p

	for (int nCntDigit = 0; nCntDigit < nMaxDigit; nCntDigit++)
	{ // ���̍ő吔���J��Ԃ�

		// ��������Z����
		nDigit *= 10;
	}

	for (int nCntTex = nMaxDigit - 1; nCntTex >= 0; nCntTex--, pNumDivide++)
	{ // �����̍ő傩�猅���̍ŏ��܂ŌJ��Ԃ�

		// �������Ƃ̐��l�����߂�
		*pNumDivide = (nNum % nDigit) / (nDigit / 10);

		// ���Z���Ď��̌����ɍ��킹��
		nDigit /= 10;
	}
}

//============================================================
//	�@���̐��K��
//============================================================
void useful::NormalizeNormal
(
	const D3DXVECTOR3& rPosLeft,	// ���ʒu
	const D3DXVECTOR3& rPosCenter,	// ���S�ʒu
	const D3DXVECTOR3& rPosRight,	// �E�ʒu
	D3DXVECTOR3& rNor				// �@��
)
{
	// �ϐ���錾
	D3DXVECTOR3 vecLine[2];	// �Ӄx�N�g��

	// �Ӄx�N�g�������߂�
	vecLine[0] = rPosLeft - rPosCenter;
	vecLine[1] = rPosRight - rPosCenter;

	// �@�����v�Z
	rNor.x = (vecLine[0].y * vecLine[1].z) - (vecLine[0].z * vecLine[1].y);
	rNor.y = (vecLine[0].z * vecLine[1].x) - (vecLine[0].x * vecLine[1].z);
	rNor.z = (vecLine[0].x * vecLine[1].y) - (vecLine[0].y * vecLine[1].x);

	// �@���𐳋K��
	D3DXVec3Normalize(&rNor, &rNor);
}

//============================================================
//	�o�ߎ��ԁEX�ړ��ʁE�d�͂���������̈ʒu�����߂鏈��
//============================================================
D3DXVECTOR2 useful::CalcPosParabola
(
	const float fGravity,	// �d��
	const float fMoveX,		// X�ړ���
	const float fDestPosX,	// �ő�X���W
	const float fDestPosY,	// �ő�Y���W
	const float fTime,		// �o�ߎ���
	float *pMaxTime,		// �ő�o�ߎ���
	float *pMaxPosY			// �ő哞�BY���W
)
{
	const float fMaxTime	= fDestPosX / fMoveX;	// �ő�o�ߎ���
	const float fHalfTime	= fMaxTime * 0.5f;		// �����̍ő�o�ߎ���
	const float fMoveY		= -0.5f * fGravity * fMaxTime;	// Y�ړ���
	const float fMaxPosY	= 0.5f * fGravity * (fHalfTime * fHalfTime) + fMoveY * fHalfTime;	// �ő哞�BY���W

	if (pMaxTime != nullptr)
	{
		// �ő�o�ߎ��Ԃ�ۑ�
		*pMaxTime = fMaxTime;
	}
	if (pMaxPosY != nullptr)
	{
		// �ő哞�BY���W��ۑ�
		*pMaxPosY = fMaxPosY;
	}

	// ���ݎ��Ԃ̈ʒu�����߂�
	D3DXVECTOR2 posTime = VEC2_ZERO;
	posTime.x = fMoveX * fTime;
	posTime.y = (0.5f * fGravity * (fTime * fTime) + fMoveY * fTime) * (fDestPosY / fMaxPosY);

	// ���ݎ��Ԃ̈ʒu��Ԃ�
	return posTime;
}

//============================================================
//	�o�ߎ��ԁEX�ړ��ʁE�d�͂���������̈ړ��ʂ����߂鏈��
//============================================================
D3DXVECTOR2 useful::CalcMoveParabola
(
	const float fGravity,	// �d��
	const float fMoveX,		// X�ړ���
	const float fDestPosX,	// �ő�X���W
	const float fDestPosY,	// �ő�Y���W
	const float fTime,		// �o�ߎ���
	float *pMaxTime,		// �ő�o�ߎ���
	float *pMaxPosY			// �ő哞�BY���W
)
{
	const float fMaxTime	= fDestPosX / fMoveX;	// �ő�o�ߎ���
	const float fHalfTime	= fMaxTime * 0.5f;		// �����̍ő�o�ߎ���
	const float fMoveY		= -0.5f * fGravity * fMaxTime;	// Y�ړ���
	const float fMaxPosY	= 0.5f * fGravity * (fHalfTime * fHalfTime) + fMoveY * fHalfTime;	// �ő哞�BY���W

	if (pMaxTime != nullptr)
	{
		// �ő�o�ߎ��Ԃ�ۑ�
		*pMaxTime = fMaxTime;
	}
	if (pMaxPosY != nullptr)
	{
		// �ő哞�BY���W��ۑ�
		*pMaxPosY = fMaxPosY;
	}

	// ���ݎ��Ԃ̈ړ��ʂ����߂�
	D3DXVECTOR2 moveTime = VEC2_ZERO;
	moveTime.x = fMoveX;
	moveTime.y = (fGravity * fTime + fMoveY) * (fDestPosY / fMaxPosY);

	// ���ݎ��Ԃ̈ړ��ʂ�Ԃ�
	return moveTime;
}

//============================================================
//	�����̐��K��
//============================================================
void useful::NormalizeRot(float& rRot)
{
	if (rRot > D3DX_PI)
	{ // ������ 3.14 �𒴂����ꍇ

		// �����̐��K��
		rRot -= D3DX_PI * 2;
	}
	else if (rRot < -D3DX_PI)
	{ // ������ -3.14 �𒴂����ꍇ

		// �����̐��K��
		rRot += D3DX_PI * 2;
	}
}

//============================================================
//	�O�������̐��K��
//============================================================
void useful::Vec3NormalizeRot(D3DXVECTOR3& rRot)
{
	// �S�Ă̌����𐳋K��
	NormalizeRot(rRot.x);
	NormalizeRot(rRot.y);
	NormalizeRot(rRot.z);
}

//============================================================
//	�x�N�g���̌����ϊ�
//============================================================
void useful::VecToRot(const D3DXVECTOR3 vec, float *pPhi, float *pTheta)
{
	// ���ʊp�̌v�Z
	*pPhi = atan2f(vec.y, vec.x);

	// �p�̌v�Z
	*pTheta = atan2f(-vec.z, sqrtf((vec.x * vec.x) + (vec.y * vec.y)));
}

//============================================================
//	�����̃x�N�g���ϊ�
//============================================================
void useful::RotToVec(const float &rPhi, const float &rTheta, D3DXVECTOR3 *pVec)
{
	// �x�N�g���̌v�Z
	pVec->x = sinf(rTheta) * cosf(rPhi);
	pVec->y = sinf(rTheta) * sinf(rPhi);
	pVec->z = cosf(rTheta);
}

//============================================================
//	�����_�������擾
//============================================================
float useful::RandomRot(void)
{
	// �����_��������Ԃ�
	return (float)(rand() % 629 - 314) * 0.01f;
}

//============================================================
//	�}�g���b�N�X�̃��[���h���W�擾
//============================================================
D3DXVECTOR3 useful::GetMtxWorldPosition(const D3DXMATRIX &rMtx)
{
	// �}�g���b�N�X�̃��[���h���W��Ԃ�
	return D3DXVECTOR3(rMtx._41, rMtx._42, rMtx._43);
}

//************************************************************
//	�}�e���A�����
//************************************************************
//============================================================
//	���}�e���A���擾
//============================================================
D3DXMATERIAL material::White(void)
{
	// �ϐ���錾
	D3DXMATERIAL mat;	// �ݒ�p�}�e���A��

	// �}�e���A���̃������N���A
	ZeroMemory(&mat, sizeof(mat));

	// �}�e���A���̊g�U���𔒂ɂ���
	mat.MatD3D.Diffuse = XCOL_WHITE;

	// �}�e���A����Ԃ�
	return mat;
}

//============================================================
//	�ԃ}�e���A���擾
//============================================================
D3DXMATERIAL material::Red(void)
{
	// �ϐ���錾
	D3DXMATERIAL mat;	// �ݒ�p�}�e���A��

	// �}�e���A���̃������N���A
	ZeroMemory(&mat, sizeof(mat));

	// �}�e���A���̊g�U����Ԃɂ���
	mat.MatD3D.Diffuse = XCOL_RED;

	// �}�e���A����Ԃ�
	return mat;
}

//============================================================
//	�΃}�e���A���擾
//============================================================
D3DXMATERIAL material::Green(void)
{
	// �ϐ���錾
	D3DXMATERIAL mat;	// �ݒ�p�}�e���A��

	// �}�e���A���̃������N���A
	ZeroMemory(&mat, sizeof(mat));

	// �}�e���A���̊g�U����΂ɂ���
	mat.MatD3D.Diffuse = XCOL_GREEN;

	// �}�e���A����Ԃ�
	return mat;
}

//============================================================
//	�}�e���A���擾
//============================================================
D3DXMATERIAL material::Blue(void)
{
	// �ϐ���錾
	D3DXMATERIAL mat;	// �ݒ�p�}�e���A��

	// �}�e���A���̃������N���A
	ZeroMemory(&mat, sizeof(mat));

	// �}�e���A���̊g�U����ɂ���
	mat.MatD3D.Diffuse = XCOL_BLUE;

	// �}�e���A����Ԃ�
	return mat;
}

//============================================================
//	���}�e���A���擾
//============================================================
D3DXMATERIAL material::Yellow(void)
{
	// �ϐ���錾
	D3DXMATERIAL mat;	// �ݒ�p�}�e���A��

	// �}�e���A���̃������N���A
	ZeroMemory(&mat, sizeof(mat));

	// �}�e���A���̊g�U�������ɂ���
	mat.MatD3D.Diffuse = XCOL_YELLOW;

	// �}�e���A����Ԃ�
	return mat;
}

//============================================================
//	�_���[�W�ԃ}�e���A���擾
//============================================================
D3DXMATERIAL material::DamageRed(void)
{
	// �ϐ���錾
	D3DXMATERIAL mat;	// �ݒ�p�}�e���A��

	// �}�e���A���̃������N���A
	ZeroMemory(&mat, sizeof(mat));

	// �}�e���A���̊g�U����ݒ�
	mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.85f);

	// �}�e���A���̕��ˌ���ݒ�
	mat.MatD3D.Emissive = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);

	// �}�e���A����Ԃ�
	return mat;
}

//============================================================
//	�����΃}�e���A���擾
//============================================================
D3DXMATERIAL material::GlowGreen(void)
{
	// �ϐ���錾
	D3DXMATERIAL mat;	// �ݒ�p�}�e���A��

	// �}�e���A���̃������N���A
	ZeroMemory(&mat, sizeof(mat));

	// �}�e���A���̊g�U����΂ɂ���
	mat.MatD3D.Diffuse = XCOL_GREEN;

	// �}�e���A���̕��ˌ���΂ɂ���
	mat.MatD3D.Emissive = XCOL_GREEN;

	// �}�e���A����Ԃ�
	return mat;
}

//************************************************************
//	2�������W�̍\���̃����o�֐�
//************************************************************
//============================================================
//	���Z�q�̃I�[�o�[���[�h (��r���Z�q ==)
//============================================================
bool POSGRID2::operator==(const POSGRID2& grid) const
{
	if (grid.x == x && grid.y == y)
	{ // ������W�����ׂē����ꍇ

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	���Z�q�̃I�[�o�[���[�h (��r���Z�q !=)
//============================================================
bool POSGRID2::operator!=(const POSGRID2& grid) const
{
	if (grid.x != x || grid.y != y)
	{ // ������W����ł��Ⴄ�ꍇ

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//************************************************************
//	2�������W�̍\���̃����o�֐�
//************************************************************
//============================================================
//	���Z�q�̃I�[�o�[���[�h (��r���Z�q ==)
//============================================================
bool POSGRID3::operator==(const POSGRID3& grid) const
{
	if (grid.x == x && grid.y == y && grid.z == z)
	{ // ������W�����ׂē����ꍇ

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	���Z�q�̃I�[�o�[���[�h (��r���Z�q !=)
//============================================================
bool POSGRID3::operator!=(const POSGRID3& grid) const
{
	if (grid.x != x || grid.y != y || grid.z != z)
	{ // ������W����ł��Ⴄ�ꍇ

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}