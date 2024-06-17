//============================================================
//
//	エディターオブジェクトヘッダー [editorObject.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDITOR_OBJECT_H_
#define _EDITOR_OBJECT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editStage.h"
#include "editCollision.h"
#include "editor.h"

//************************************************************
//	前方宣言
//************************************************************
class CEditManager;	// エディットマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// エディターオブジェクトクラス
class CEditorObject
{
public:
	// コンストラクタ
	CEditorObject(CEditStage* pEditor);

	// デストラクタ
	virtual ~CEditorObject();

	// 純粋仮想関数
	virtual HRESULT Save(void)	= 0;	// 保存
	virtual bool IsSave(void)	= 0;	// 保存状況取得
	virtual void SaveInfo(void)	= 0;	// 情報保存
	virtual void LoadInfo(void)	= 0;	// 情報読込

	// 仮想関数
	virtual HRESULT Init(void);	// 初期化
	virtual void Uninit(void);	// 終了
	virtual void Update(void);	// 更新
	virtual void DrawDebugControl(void);	// 操作表示描画
	virtual void DrawDebugInfo(void);		// 情報表示描画

	// 静的メンバ関数
	static CEditorObject* Create(CEditStage* pEditor, CEditStage::EType type);	// 生成
	static void Release(CEditorObject *&prEditorObject);	// 破棄

protected:
	// 仮想関数
	virtual void UpdatePosition(void);	// 位置更新
	virtual void UpdateRotation(void);	// 向き更新

	// メンバ関数
	void SetVec3Position(const D3DXVECTOR3 pos)	{ m_pEditor->SetVec3Position(pos); }		// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const		{ return m_pEditor->GetVec3Position(); }	// 位置取得
	void SetVec3Rotation(const D3DXVECTOR3 rot)	{ m_pEditor->SetVec3Rotation(rot); }		// 向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const		{ return m_pEditor->GetVec3Rotation(); }	// 向き取得
	void SetAngle(const EAngle angle)			{ m_pEditor->SetAngle(angle); }				// 方向設定
	EAngle GetAngle(void) const					{ return m_pEditor->GetAngle(); }			// 方向取得

private:
	// メンバ変数
	CEditStage* m_pEditor;	// エディター情報
};

#endif	// _EDITOR_OBJECT_H_
