//============================================================
//
//	ゲーム画面ヘッダー [sceneGame.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCENE_GAME_H_
#define _SCENE_GAME_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "scene.h"

//************************************************************
//	前方宣言
//************************************************************
class CGameManager;	// ゲームマネージャークラス
class CPause;		// ポーズクラス
class CEdit;		// エディットクラス

//************************************************************
//	クラス定義
//************************************************************
// ゲーム画面クラス
class CSceneGame : public CScene
{
public:
	// コンストラクタ
	explicit CSceneGame(const EMode mode);

	// デストラクタ
	~CSceneGame() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CGameManager	*GetGameManager(void);	// ゲームマネージャー取得
	static CPause		*GetPause(void);		// ポーズ取得

	static void  SetEdit(CEdit *pEdit);	// エディット設定
	static CEdit *GetEdit(void);		// エディット取得

private:
	// 静的メンバ変数
	static CGameManager	*m_pGameManager;	// ゲームマネージャー
	static CPause		*m_pPause;			// ポーズ
	static CEdit		*m_pEdit;			// エディット
};

#endif	// _SCENE_GAME_H_
