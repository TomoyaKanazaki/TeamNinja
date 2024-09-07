//============================================================
//
//	タイトルマネージャーヘッダー [titleManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TITLEMANAGER_H_
#define _TITLEMANAGER_H_

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// タイトルマネージャークラス
class CTitleManager
{
public:
	// 定数
	static constexpr int NUM_LOGO = 4;	// 文字数

	// コンストラクタ
	CTitleManager();

	// デストラクタ
	~CTitleManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(const float fDeltaTime);	// 更新

	// 静的メンバ関数
	static CTitleManager *Create(void);	// 生成
	static void Release(CTitleManager *&prTitleManager);	// 破棄

private:
	// メンバ関数


	// メンバ変数
	CObject2D* m_apLogo[NUM_LOGO];	// タイトル情報
};

#endif	// _TITLEMANAGER_H_
