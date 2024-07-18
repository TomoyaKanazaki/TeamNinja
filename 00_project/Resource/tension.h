#pragma once
//===========================================
//
//  士気力ゲージ(tension.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "object2D.h"

//===========================================
//  クラス定義
//===========================================
class CTension : CObject2D
{
public:

	// メンバ関数
	CTension();
	~CTension();

	HRESULT Init() override; // 初期化処理
	void Uninit() override; // 終了処理
	void Update(const float fDeltaTime) override; // 更新処理
	void Draw(CShader* pShader = nullptr) override; // 描画処理

	bool IsUse() { return m_bUse; } // 使用可能フラグの取得

	// 静的メンバ関数
	static CTension* Create(); // 生成処理
	static CListManager<CTension>* GetList(); // リスト取得
	static int GetUseNum(); // 使用可能な士気力の取得

private:

	// メンバ関数
	void CalcPosition(); // 座標の計算処理
	void ChangeColor(); //色の変更
	void SwitchUse(); // 使用可能状態の変更

	// メンバ変数
	CListManager<CTension>::AIterator m_iterator;	// イテレーター
	bool m_bUse; // 使用可能フラグ

	// 静的メンバ変数
	static CListManager<CTension>* m_pList;	// オブジェクトリスト
	static int m_nUseNum; // 使用可能な士気力の数

};