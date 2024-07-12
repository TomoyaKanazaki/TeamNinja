//============================================================
//
//	セレクト画面ヘッダー [sceneSelect.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCENE_SELECT_H_
#define _SCENE_SELECT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "scene.h"

//************************************************************
//	前方宣言
//************************************************************
class CSelectManager;	// セレクトマネージャークラス
class CEditManager;		// エディットマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// セレクト画面クラス
class CSceneSelect : public CScene
{
public:
	// コンストラクタ
	explicit CSceneSelect(const EMode mode);

	// デストラクタ
	~CSceneSelect() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新

	// 静的メンバ関数
	static CSelectManager	*GetSelectManager(void);	// セレクトマネージャー取得
	static CEditManager		*GetEditManager(void);		// エディターマネージャー取得

private:
	// 静的メンバ変数
	static CSelectManager	*m_pSelectManager;	// セレクトマネージャー
	static CEditManager		*m_pEditManager;	// エディターマネージャー
};

#endif	// _SCENE_SELECT_H_
