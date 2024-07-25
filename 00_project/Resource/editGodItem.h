#if 1
//============================================================
//
//	エディット神器ヘッダー [editGodItem.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_GODITEM_H_
#define _EDIT_GODITEM_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editorObject.h"
#include "godItem.h"

//************************************************************
//	クラス定義
//************************************************************
// エディット神器クラス
class CEditGodItem : public CEditorObject
{
public:
	// コンストラクタ
	CEditGodItem(CEditStage* pEditor);

	// デストラクタ
	~CEditGodItem() override;

	// 神器情報構造体
	struct SInfo
	{
		CGodItem::EType type;	// 種類
	};

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
	void ChangeType(void);		// 種類更新
	void CreateGodItem(void);	// 神器生成
	void ReleaseGodItem(void);	// 神器破棄
	void DeleteCollisionGodItem(const bool bRelase);	// 神器の削除判定

	// メンバ変数
	CObjectModel* m_pGodItem;	// 神器情報
	SInfo m_infoCreate;	// 神器配置情報
	bool m_bSave;		// 保存状況
};

#endif	// _EDIT_GODITEM_H_
#endif
