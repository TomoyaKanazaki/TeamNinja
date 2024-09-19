#if 1
//============================================================
//
//	エディットタッチアクターヘッダー [editTouchActor.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_TOUCH_ACTOR_H_
#define _EDIT_TOUCH_ACTOR_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editorObject.h"
#include "touchActor.h"

//************************************************************
//	クラス定義
//************************************************************
// エディットタッチアクタークラス
class CEditTouchActor : public CEditorObject
{
public:

	// コンストラクタ
	CEditTouchActor(CEditStage* pEditor);

	// デストラクタ
	~CEditTouchActor() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	HRESULT Save(void) override;	// 保存
	bool IsSave(void) override;		// 保存状況取得
	void SaveInfo(void) override;	// 情報保存
	void LoadInfo(void) override;	// 情報読込
	void DrawDebugControl(void) override;	// 操作表示描画
	void DrawDebugInfo(void) override;		// 情報表示描画

private:

	// メンバ関数
	void ChangeType(void);		// 種類変更
	void CreateCoin(void);		// コイン生成
	void ReleaseCoin(void);		// コイン破棄
	void DeleteCollisionCoin(const bool bRelase);	// コインの削除判定
	void InitAllColorCoin(void);					// コインの色全初期化

	// メンバ変数
	CTouchActor* m_pActor;		// コイン情報
	CTouchActor::EType m_type;	// 種類
	bool m_bSave;				// 保存状況
};

#endif	// _EDIT_COIN_H_
#endif
