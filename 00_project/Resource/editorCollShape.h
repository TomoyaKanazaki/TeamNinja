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
#include "editor.h"
#include "editCollision.h"

//************************************************************
//	前方宣言
//************************************************************
class CEditManager;			// エディットマネージャークラス
class CActor;				// アクター
class CEditCollCube;		// キューブ当たり判定
class CEditCollCylinder;	// シリンダー当たり判定
class CEditCollSphere;		// スフィア当たり判定

//************************************************************
//	クラス定義
//************************************************************
// 当たり判定エディタークラス
class CEditorCollShape
{
public:
	// コンストラクタ
	CEditorCollShape(const int nIdx);

	// デストラクタ
	virtual ~CEditorCollShape();

	// 純粋仮想関数
	virtual bool IsSave(void) = 0;		// 保存状況取得
	virtual void SaveInfo(void) = 0;	// 情報保存
	virtual void LoadInfo(void) = 0;	// 情報読込

	virtual void Create(void) = 0;		// 生成処理

	// 仮想関数
	virtual HRESULT Init(void);	// 初期化
	virtual void Uninit(void);	// 終了
	virtual void Update(void);	// 更新
	virtual void DrawDebugControl(void);	// 操作表示描画
	virtual void DrawDebugInfo(void);		// 情報表示描画

	// 静的メンバ関数
	static CEditorCollShape* Create(CEditCollision::EType type, const int nIdx);	// 生成
	static void Release(CEditorCollShape*& prEditorObject);			// 破棄

	// メンバ関数
	D3DXVECTOR3 GetVec3OffSet(void) const	{ return m_offset; }	// オフセット座標取得

protected:
	// 仮想関数
	virtual void UpdateOffset(void);	// オフセット座標更新

private:

	// メンバ変数
	D3DXVECTOR3 m_offset;				// オフセット座標
};

#endif	// _EDITOR_COLL_SHAPE_H_
