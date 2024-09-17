#pragma once
//===========================================
//
//  チュートリアルクラス(tutorial.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "objectBillboard.h"

//===========================================
//  クラス定義
//===========================================
class CTutorial : public CObjectBillboard
{
public:

	enum EType
	{
		TYPE_MOVE = 0, // 歩行
		TYPE_CLONE, // 分身
		TYPE_GIMMICK, // ギミック
		TYPE_GUARD, // ガード
		TYPE_DOGDE, // 回避
		TYPE_MAX, // 最大数
	};

	// メンバ関数
	CTutorial();
	~CTutorial();

	HRESULT Init(void) override; // 初期化処理
	void Uninit(void) override; // 終了処理
	void Update(const float fDeltaTime) override; // 更新処理
	void Draw(CShader* pShader = nullptr) override; // 描画処理

	// 静的メンバ関数
	static CTutorial* Create(const D3DXVECTOR3& rPos, const EType type);
	static CListManager<CTutorial>* GetList(); // リスト取得

private:

	// 静的メンバ変数
	static CListManager<CTutorial>* m_pList; // オブジェクトリスト

	// メンバ関数
	void BindTexture(const char* pTexturePass) override; // テクスチャ割当 (パス)
	void Scaling(const float fDeltaTime); // スケーリング変更処理

	// メンバ変数
	CListManager<CTutorial>::AIterator m_iterator; // イテレーター
	D3DXVECTOR3 m_sizeDefault; // 初期サイズ
	bool m_bIn; // 内部判定
	float m_fTime; // 移動時間

};
