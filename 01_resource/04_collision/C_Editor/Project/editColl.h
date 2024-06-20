//============================================================
//
//	エディット当たり判定ヘッダー [editColl.h]
//	Author：藤田勇一
//
//============================================================
#if _DEBUG 

//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_COLL_H_
#define _EDIT_COLL_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "edit.h"

//************************************************************
//	クラス定義
//************************************************************
// エディット当たり判定クラス
class CEditColl : public CEdit
{
public:
	// コンストラクタ
	CEditColl();

	// デストラクタ
	~CEditColl();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void DrawEditControl(void) override;	// エディット操作表示
	void DrawEditData(void) override;		// エディット情報表示

private:
	// メンバ関数
	void SelectParts(void);			// パーツ選択更新
	void AddColl(void);				// 判定追加更新
	void SubColl(void);				// 判定削除更新
	void SelectColl(void);			// 判定選択更新
	void UpdateCollOffset(void);	// 判定オフセット更新
	void UpdateCollRadius(void);	// 判定半径更新
	void UpdateCollInfo(void);		// 判定情報更新
	void SaveColl(void);			// 判定保存
	void LoadColl(void);			// 判定読込

	// メンバ変数
	int m_nCurSelectParts;	// 現在の選択パーツ
	int m_nOldSelectParts;	// 前回の選択パーツ
	int m_nCurSelectColl;	// 現在の選択判定
	int m_nOldSelectColl;	// 前回の選択判定
};

#endif	// _EDIT_COLL_H_
#endif	// _DEBUG
