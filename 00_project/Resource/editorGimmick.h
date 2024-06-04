#if 0

//============================================================
//
//	エディターギミックヘッダー [editorGimmick.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDITOR_GIMMICK_H_
#define _EDITOR_GIMMICK_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editStage.h"
#include "editor.h"

//************************************************************
//	クラス定義
//************************************************************
// エディターギミッククラス
class CEditorGimmick
{
public:
	// コンストラクタ
	CEditorGimmick();

	// デストラクタ
	virtual ~CEditorGimmick();

	// 純粋仮想関数
	virtual HRESULT Save(void) = 0;		// 保存
	virtual bool IsSave(void) = 0;		// 保存状況取得
	virtual void SaveInfo(void) = 0;	// 情報保存
	virtual void LoadInfo(void) = 0;	// 情報読込

	// 仮想関数
	virtual HRESULT Init(void);	// 初期化
	virtual void Uninit(void);	// 終了
	virtual void Update(void);	// 更新
	virtual void DrawDebugControl(void);	// 操作表示描画
	virtual void DrawDebugInfo(void);		// 情報表示描画

	// 静的メンバ関数
	static CEditorGimmick* Create(CEditStage::EType type);		// 生成
	static void Release(CEditorGimmick*& prEditorObject);		// 破棄

	// メンバ関数
	D3DXVECTOR3 GetVec3Position(void) const { return m_pos; }	// 位置取得
	D3DXVECTOR3 GetVec3Rotation(void) const { return m_rot; }	// 向き取得

protected:
	// 仮想関数
	virtual void UpdatePosition(void);	// 位置更新
	virtual void UpdateRotation(void);	// 向き更新

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 向き
	CEditor::EAngle m_angle;		// 角度
};

#endif	// _EDITOR_OBJECT_H_

#endif // 0
