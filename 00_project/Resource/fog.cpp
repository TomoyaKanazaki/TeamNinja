//==========================================
//
//  �t�H�O���Ǘ����閼�O���(fog.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "fog.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"

//==========================================
//  �ϐ��錾
//==========================================
namespace
{
	const float START = 1500.0f;
	const float END = 3000.0f;
	const D3DXCOLOR COL = D3DXCOLOR(0.15f, 0.15f, 0.35f, 0.0f);
	
	// �����o�ϐ�
	bool m_bUse;
	float m_fStart;
	float m_fEnd;
	D3DXCOLOR m_col;
}

//==========================================
//  �����ݒ�
//==========================================
void Fog::Init()
{
	m_bUse = true;
	m_fStart = START;
	m_fEnd = END;
	m_col = COL;
}

//==========================================
//  �g�p��Ԃ̐ݒ�
//==========================================
void Fog::Set(bool bUse)
{
	m_bUse = bUse;
}

//==========================================
//  �g�p��Ԃ̎擾
//==========================================
bool Fog::Get()
{
	return m_bUse;
}

//==========================================
//  �`�揈��
//==========================================
void Fog::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �f�o�b�O�@�\
#ifdef _DEBUG

		// ���͏����󂯎��|�C���^
	CInputKeyboard* pKeyboard = GET_INPUTKEY;

	// �؂�ւ�
	if (pKeyboard->IsTrigger(DIK_F))
	{
		Fog::Set(!Fog::Get());
	}

	// �������L����
	if (pKeyboard->IsPress(DIK_UP))
	{
		m_fStart += 1.0f;
	}
	if (pKeyboard->IsPress(DIK_DOWN))
	{
		m_fStart -= 1.0f;
	}
	if (pKeyboard->IsPress(DIK_LEFT))
	{
		m_fEnd += 1.0f;
	}
	if (pKeyboard->IsPress(DIK_RIGHT))
	{
		m_fEnd -= 1.0f;
	}

	// r�̒���
	if (pKeyboard->IsPress(DIK_NUMPAD1))
	{
		m_col.r -= 0.01f;
	}
	if (pKeyboard->IsPress(DIK_NUMPAD7))
	{
		m_col.r += 0.01f;
	}
	if (pKeyboard->IsTrigger(DIK_NUMPAD4))
	{
		m_col.r = 0.0f;
	}

	// g�̒���
	if (pKeyboard->IsPress(DIK_NUMPAD2))
	{
		m_col.g -= 0.01f;
	}
	if (pKeyboard->IsPress(DIK_NUMPAD8))
	{
		m_col.g += 0.01f;
	}
	if (pKeyboard->IsTrigger(DIK_NUMPAD5))
	{
		m_col.g = 0.0f;
	}

	// b�̒���
	if (pKeyboard->IsPress(DIK_NUMPAD3))
	{
		m_col.b -= 0.01f;
	}
	if (pKeyboard->IsPress(DIK_NUMPAD9))
	{
		m_col.b += 0.01f;
	}
	if (pKeyboard->IsTrigger(DIK_NUMPAD6))
	{
		m_col.b = 0.0f;
	}

	// ���E�l�̐ݒ�
	if (m_fStart < 0.0f)
	{
		m_fStart = 0.0f;
	}
	if (m_fEnd < 0.0f)
	{
		m_fEnd = 0.0f;
	}
	if (m_col.r > 1.0f)
	{
		m_col.r = 1.0f;
	}
	if (m_col.r < 0.0f)
	{
		m_col.r = 0.0f;
	}
	if (m_col.g > 1.0f)
	{
		m_col.g = 1.0f;
	}
	if (m_col.g < 0.0f)
	{
		m_col.g = 0.0f;
	}
	if (m_col.b > 1.0f)
	{
		m_col.b = 1.0f;
	}
	if (m_col.b < 0.0f)
	{
		m_col.b = 0.0f;
	}

#endif

	if(!m_bUse)
	{
		// ���𖳌���
		pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

		// �f�o�b�O�\��
		DebugProc::Print(DebugProc::POINT_RIGHT, "�t�H�O �y OFF �zF\n");

		return;
	}

	//����L����
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	//���̐F��ݒ�
	pDevice->SetRenderState(D3DRS_FOGCOLOR, m_col);

	//���̏�Ԃ�ݒ�
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

	//���̔����͈͂�ݒ�
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fStart));
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fEnd));

	DebugProc::Print(DebugProc::POINT_RIGHT, "�t�H�O �y ON �zF\n");
	DebugProc::Print
	(
		DebugProc::POINT_RIGHT,
		"�t�H�O��O : %f\n"
		"�t�H�O�̉� : %f\n"
		"�t�H�O�̐F : %f, %f, %f, %f\n",
		m_fStart,
		m_fEnd,
		m_col.r, m_col.g, m_col.b, m_col.a
	);
}

//==========================================
//  �F�̐ݒ�
//==========================================
void Fog::SetCol(D3DXCOLOR col)
{
	m_col = col;
}

//==========================================
//  �n�_�̐ݒ�
//==========================================
void Fog::SetStart(float fStart)
{
	m_fStart = fStart;
}

//==========================================
//  �I�_�̐ݒ�
//==========================================
void Fog::SetEnd(float fEnd)
{
	m_fEnd = fEnd;
}

//==========================================
//  �F�̎擾
//==========================================
D3DXCOLOR Fog::GetCol()
{
	return m_col;
}

//==========================================
//  �n�_�̎擾
//==========================================
float Fog::GetStart()
{
	return m_fStart;
}

//==========================================
//  �I�_�̎擾
//==========================================
float Fog::GetEnd()
{
	return m_fEnd;
}

//==========================================
//  �����J���[�̎擾
//==========================================
D3DXCOLOR Fog::GetInitCol()
{
	return COL;
}

//==========================================
//  �����n�_�̎擾
//==========================================
float Fog::GetInitStart()
{
	return START;
}

//==========================================
//  �����I�_�̎擾
//==========================================
float Fog::GetInitEnd()
{
	return END;
}
