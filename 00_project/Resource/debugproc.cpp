//==========================================================
//
// �f�o�b�O�\������ [debugproc.cpp]
// Author : Ibuki Okusada
// Aded by Tomoya Kanazaki
//
//==========================================================
#include "debugproc.h"
#include <string.h>
#include <stdio.h>
#include "input.h"
#include "manager.h"
#include "renderer.h"

//==========================================
//  �ÓI�ϐ��錾
//==========================================
namespace
{
	// �萔��`
	const int MAX_FLOATNUM = 4; // �����_�ȉ��̕\������
	const int MAX_DEBUGSTRING = 2048; // �f�o�b�O�\���̍ő啶����
	const char* FONT_PASS = "Terminal"; // �g�p����t�H���g
	const D3DXCOLOR FONT_COL = D3DCOLOR_RGBA(250, 250, 250, 255);	// �t�H���g�F

	// �ÓI�ϐ�
	LPD3DXFONT m_pFont = nullptr; // �f�o�b�O�t�H���g�ւ̃|�C���^
	char m_aStr[DebugProc::POINT_MAX][MAX_DEBUGSTRING] = {}; // �f�o�b�O�\���p�̕�����
	bool m_bDisp = true; // �\���t���O
}

//==========================================================
//�f�o�b�O�\���̏���������
//==========================================================
void DebugProc::Init()
{
	// �f�o�b�O�\�����̏�����
	m_bDisp = false;
	m_pFont = nullptr;
	memset(&m_aStr[0][0], 0, sizeof(m_aStr));

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//�f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FONT_PASS, &m_pFont);

	//�����\���ݒ�
#if _DEBUG
	m_bDisp = true;
#else NDEBUG
	m_bDisp = false;
#endif
}

//==========================================================
//�f�o�b�O�\���̏I������
//==========================================================
void DebugProc::Uninit(void)
{
	//�f�o�b�O�\���p�t�H���g�̔p��
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

//==========================================================
//�f�o�b�O�\���̍X�V����
//==========================================================
void DebugProc::Update(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h�̃|�C���^

#ifdef _DEBUG


	if(pKeyboard->IsTrigger(DIK_F1) == true)
	{//F1�L�[�������ꂽ�Ƃ�
		m_bDisp = m_bDisp ? false : true;
	}

#endif // _DEBUG
}

//==========================================================
//�f�o�b�O�\���̕`�揈��
//==========================================================
void DebugProc::Draw(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	if (m_bDisp == true)
	{//�f�o�b�N���[�h���I���̎�
	 //�e�L�X�g�̕`��
		m_pFont->DrawText(nullptr, &m_aStr[POINT_CENTER][0], NONE_IDX, &rect, DT_CENTER, FONT_COL);
		m_pFont->DrawText(nullptr, &m_aStr[POINT_LEFT][0], NONE_IDX, &rect, DT_LEFT, FONT_COL);
		m_pFont->DrawText(nullptr, &m_aStr[POINT_RIGHT][0], NONE_IDX, &rect, DT_RIGHT, FONT_COL);
	}

	//�f�o�b�O�\�����̃N���A
	memset(&m_aStr[0][0], 0, sizeof(m_aStr));
}

//==========================================================
//�f�o�b�O�\���̐ݒ菈��
//==========================================================
void DebugProc::Print(const EPoint point, const char *fmt, ...)
{
	va_list args;
	char aString[MAX_DEBUGSTRING] = {};		// �w�蕶���i�[�p
	char aSaveString[MAX_DEBUGSTRING] = {};	// �ψ������g�i�[�p
	int nLength = 0;	// �ψ������̕����̒���
	int nStopLength = 0;	// �ψ����}���ꏊ���

	//������̑��
	strcpy(&aString[0], fmt);

	va_start(args, fmt);

	for (char *p = &aString[0]; *p != '\0'; p++)
	{
		if (*p == '%')
		{//�m�F����������ꍇ

			p++;	//�m�F�����܂Ői�߂�

			nLength = 0;	// �����̒������Z�b�g

			// �w�肳�ꂽ��ނ��Ƃɕϐ��̒l�𔲂��o��(va_arg)
			switch (*p)
			{
			case 'd':	//����

				sprintf(&aSaveString[0], "%d", va_arg(args, int));

				break;
			case 'f':	//����

				sprintf(&aSaveString[0], "%f", va_arg(args, double));

				//������̒������擾
				nLength = (int)strlen(&aSaveString[0]);

				//�����_�ȉ���1�����ڂ܂Ŋm�F
				for (int nCntlength = 0; aSaveString[nCntlength] != '\0'; nCntlength++)
				{
					if (aSaveString[nCntlength] == '.')
					{//�����_���������ꍇ

						//�����_�ȉ��̌��������߂�
						int nMin = (int)strlen(&aSaveString[nCntlength + 1]);

						//�����_����̕������Ǝw�肵���l�܂ł̍��������߂�
						nMin -= MAX_FLOATNUM;

						//������̒����������_�ȉ����ʂ܂łɕ␳
						nLength -= nMin;
					}
				}

				break;
			case 'c':	//����

				sprintf(&aSaveString[0], "%c", va_arg(args, char));

				break;
			case 's':	//������

				sprintf(&aSaveString[0], "%s", va_arg(args, const char*));

				break;
			}

			if (nLength == 0)
			{// �����̒������擾���Ă��Ȃ��ꍇ

				//������̒������擾
				nLength = (int)strlen(&aSaveString[0]);
			}

			nStopLength = (int)strlen(p) + 1;

			//���������̊m�F����(%�`)�����̕���������炷
			memmove(p + nLength - 2, p, nStopLength);

			p--;	//�|�C���^��%�܂Ŗ߂�

			//�ψ������w�肳�ꂽ�ꏊ�ɑ}��
			memcpy(p, &aSaveString[0], nLength);
		}
	}

	va_end(args);

	//�������A������
	strcat(&m_aStr[point][0], &aString[0]);
}
