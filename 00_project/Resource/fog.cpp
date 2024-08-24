//==========================================
//
//  フォグを管理する名前空間(fog.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "fog.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"

//==========================================
//  変数宣言
//==========================================
namespace
{
	bool m_bUse = true;
	float m_fStart = 1500.0f;
	float m_fEnd = 3000.0f;
	D3DXCOLOR m_col = D3DXCOLOR(0.15f, 0.15f, 0.35f, 0.0f);

	/*
	ステージによって色変えてみるとか？(章先生)
	*/
}

//==========================================
//  使用状態の設定
//==========================================
void Fog::Set(bool bUse)
{
	m_bUse = bUse;
}

//==========================================
//  使用状態の取得
//==========================================
bool Fog::Get()
{
	return m_bUse;
}

//==========================================
//  描画処理
//==========================================
void Fog::Draw()
{
	//デバッグ表示
	if (m_bUse)
	{
		DebugProc::Print(DebugProc::POINT_RIGHT, "フォグ 【 ON 】F\n");
		DebugProc::Print
		(
			DebugProc::POINT_RIGHT,
			"フォグ手前 : %f\n"
			"フォグの奥 : %f\n"
			"フォグの色 : %f, %f, %f, %f\n",
			m_fStart,
			m_fEnd,
			m_col.r, m_col.g, m_col.b, m_col.a
		);
	}
	else
	{
		DebugProc::Print(DebugProc::POINT_RIGHT, "\n\nフォグ 【 OFF 】F\n");
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (m_bUse)
	{
		//霧を有効化
		pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

		//霧の色を設定
		pDevice->SetRenderState(D3DRS_FOGCOLOR, m_col);

		//霧の状態を設定
		pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
		pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

		//霧の発生範囲を設定
		pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fStart));
		pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fEnd));
	}
	else
	{
		//霧を無効化
		pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}

	// デバッグ機能
#ifdef _DEBUG
	
		// 入力情報を受け取るポインタ
	CInputKeyboard* pKeyboard = GET_INPUTKEY;

	// 切り替え
	if (pKeyboard->IsTrigger(DIK_F))
	{
		Fog::Set(!Fog::Get());
	}

	// 距離を広げる
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
		m_fEnd-= 1.0f;
	}

	// rの調整
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

	// gの調整
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

	// bの調整
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
	
	// 限界値の設定
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
}

//==========================================
//  色の設定
//==========================================
void Fog::SetCol(D3DXCOLOR col)
{
	m_col = col;
}

//==========================================
//  最大距離の設定
//==========================================
void Fog::SetStart(float fStart)
{
	m_fStart = fStart;
}

//==========================================
//  最低距離の設定
//==========================================
void Fog::SetEnd(float fEnd)
{
	m_fEnd = fEnd;
}

//==========================================
//  色の取得
//==========================================
D3DXCOLOR Fog::GetCol(void)
{
	return m_col;
}

//==========================================
//  最大距離の取得
//==========================================
float Fog::GetStart(void)
{
	return m_fStart;
}

//==========================================
//  最低距離の取得
//==========================================
float Fog::GetEnd(void)
{
	return m_fEnd;
}
