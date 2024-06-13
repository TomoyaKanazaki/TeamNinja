//==========================================
//
//  チェックポイント(checkpoint.h)
//  Author : Tomoya Kanazaki
//  Adder：Ritsuki Obara
//
//==========================================
#pragma once
#include "objectModel.h"

//==========================================
//  クラス定義
//==========================================
class CCheckPoint : public CObjectModel
{
public:

	// メンバ関数
	CCheckPoint();
	~CCheckPoint();

	HRESULT Init() override; // 初期化
	void Uninit() override; // 終了
	void Update(const float fDeltaTime) override; // 更新
	void Draw(CShader* pShader = nullptr) override; // 描画

	int GetSaveTension() const { return m_nSaveTension; }	// セーブ時の士気力取得

	// 静的メンバ関数
	static CCheckPoint* Create(const D3DXVECTOR3& rPos);	// 生成処理
	static CListManager<CCheckPoint>* GetList(void);		// リスト取得
	static HRESULT LoadSetup(void);	// セットアップ

private:

	// メンバ関数
	void CollisionPlayer();	// プレイヤーとの当たり判定

	// メンバ変数
	CListManager<CCheckPoint>::AIterator m_iterator;	// イテレーター
	bool m_bSave; // セーブフラグ
	int m_nSaveTension; // セーブ時の士気力

	// 静的メンバ変数
	static int m_nNumAll;
	static CListManager<CCheckPoint>* m_pList;	// オブジェクトリスト
};
