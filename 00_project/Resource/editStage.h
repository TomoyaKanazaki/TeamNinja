//============================================================
//
//	エディットステージヘッダー [editStage.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_STAGE_H_
#define _EDIT_STAGE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editor.h"

//************************************************************
//	前方宣言
//************************************************************
class CEditManager;		// エディットマネージャークラス
class CEditorObject;	// エディターオブジェクトクラス

//************************************************************
//	定数宣言
//************************************************************
namespace editstage
{
	const float SIZE = 50.0f;	// 一ブロックの大きさ
}

//************************************************************
//	クラス定義
//************************************************************
// エディットステージクラス
class CEditStage : public CEditor
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_FIELD = 0,		// 地面
		TYPE_WALL,			// 壁
		TYPE_ACTOR,			// アクター
		TYPE_CHECKPOINT,	// チェックポイント
		TYPE_GOALPOINT,		// ゴールポイント
		TYPE_GIMMICK,		// ギミック
		TYPE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEditStage(CEditManager *pEditManager);

	// デストラクタ
	virtual ~CEditStage();

	// メンバ関数
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
	void ChangeObjectType(void);	// オブジェクトタイプ変更

	// メンバ変数
	CEditorObject *m_pEditor;	// エディター情報
	CEditStage::EType m_type;	// オブジェクトタイプ
};

#endif	// _EDIT_STAGE_H_
