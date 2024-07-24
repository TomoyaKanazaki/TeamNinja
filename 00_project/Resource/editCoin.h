#if 1
//============================================================
//
//	エディットコインヘッダー [editCoin.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_COIN_H_
#define _EDIT_COIN_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editorObject.h"
#include "coin.h"

//************************************************************
//	クラス定義
//************************************************************
// エディットコインクラス
class CEditCoin : public CEditorObject
{
public:
	// コンストラクタ
	CEditCoin(CEditStage* pEditor);

	// デストラクタ
	~CEditCoin() override;

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
	// オーバーライド関数
	void UpdatePosition(void) override;		// 位置更新

	// メンバ関数
	void CreateCoin(void);		// コイン生成
	void ReleaseCoin(void);		// コイン破棄
	void DeleteCollisionCoin(const bool bRelase);	// コインの削除判定
	void InitAllColorCoin(void);					// コインの色全初期化

	// メンバ変数
	CCoin* m_pCoin;		// コイン情報
	bool m_bSave;		// 保存状況
};

#endif	// _EDIT_COIN_H_
#endif
