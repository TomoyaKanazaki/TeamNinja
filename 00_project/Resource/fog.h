#pragma once
//==========================================
//
//  �t�H�O���Ǘ����閼�O���(fog.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _FOG_
#define _FOG_

//==========================================
//  ���O��Ԃ̒�`
//==========================================
namespace Fog
{
	// �g�p�ɕK�{�̊֐�
	void Init(); // �t�H�O�̏����ݒ�
	void Set(bool bUse); //�t�H�O�̐ݒ�
	bool Get(); // �t�H�O�̎d�l��Ԃ��擾
	void Draw(); //�`��

	// ����ݒ肷�邽�߂̊֐�
	void SetCol(D3DXCOLOR col);
	void SetStart(float fStart);
	void SetEnd(float fEnd);

	// �����擾���邽�߂̊֐�
	D3DXCOLOR GetCol();
	float GetStart();
	float GetEnd();

	// �萔�̎擾
	D3DXCOLOR GetInitCol();
	float GetInitStart();
	float GetInitEnd();
}

#endif
