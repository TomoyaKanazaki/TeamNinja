//============================================================
//
//	当たり判定エディターヘッダー [editorCollShape.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDITOR_COLL_SHAPE_H_
#define _EDITOR_COLL_SHAPE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editStage.h"
#include "editor.h"

//************************************************************
//	前方宣言
//************************************************************
class CEditManager;	// エディットマネージャークラス
class CActor;		// アクター

//************************************************************
//	クラス定義
//************************************************************
// 当たり判定エディタークラス
class CEditorCollShape
{
public:
	// コンストラクタ
	CEditorCollShape();

	// デストラクタ
	virtual ~CEditorCollShape();

	// 純粋仮想関数
	virtual HRESULT Save(void) = 0;	// 保存
	virtual bool IsSave(void) = 0;	// 保存状況取得
	virtual void SaveInfo(void) = 0;	// 情報保存
	virtual void LoadInfo(void) = 0;	// 情報読込

	// 仮想関数
	virtual HRESULT Init(void);	// 初期化
	virtual void Uninit(void);	// 終了
	virtual void Update(void);	// 更新
	virtual void DrawDebugControl(void);	// 操作表示描画
	virtual void DrawDebugInfo(void);		// 情報表示描画

	// 静的メンバ関数
	static CEditorCollShape* Create(CEditStage::EType type);	// 生成
	static void Release(CEditorCollShape*& prEditorObject);		// 破棄

	// メンバ関数
	CActor* GetActor(void) const { return m_pActor; }		// アクターの取得処理

protected:
	// 仮想関数
	virtual void UpdatePosition(void);	// 位置更新
	virtual void UpdateRotation(void);	// 向き更新

private:

	// メンバ変数
	CActor* m_pActor;				// アクター
	CEditor::EAngle m_angle;		// 角度
};

#endif	// _EDITOR_OBJECT_H_
