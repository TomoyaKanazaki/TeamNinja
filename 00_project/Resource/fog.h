#pragma once
//==========================================
//
//  フォグを管理する名前空間(fog.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _FOG_
#define _FOG_

//==========================================
//  名前空間の定義
//==========================================
namespace Fog
{
	// 使用に必須の関数
	void Init(); // フォグの初期設定
	void Set(bool bUse); //フォグの設定
	bool Get(); // フォグの仕様状態を取得
	void Draw(); //描画

	// 情報を設定するための関数
	void SetCol(D3DXCOLOR col);
	void SetStart(float fStart);
	void SetEnd(float fEnd);

	// 情報を取得するための関数
	D3DXCOLOR GetCol();
	float GetStart();
	float GetEnd();

	// 定数の取得
	D3DXCOLOR GetInitCol();
	float GetInitStart();
	float GetInitEnd();
}

#endif
