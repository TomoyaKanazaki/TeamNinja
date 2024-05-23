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
class CEditManager;	// エディットマネージャークラス

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
		TYPE_FIELD = 0,	// 地面
		TYPE_WALL,		// 床
		TYPE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CEditStage(CEditManager *pEditManager);

	// デストラクタ
	virtual ~CEditStage();

	// 純粋仮想関数
	virtual void SaveInfo(void)		= 0;	// 情報保存
	virtual void LoadInfo(void)		= 0;	// 情報読込
	virtual void Save(FILE *pFile)	= 0;	// 保存

	// 仮想関数
	virtual HRESULT Init(void);	// 初期化
	virtual void Uninit(void);	// 終了
	virtual void Update(void);	// 更新
	virtual void DrawDebugControl(void);	// 操作表示描画
	virtual void DrawDebugInfo(void);		// 情報表示描画

	// 静的メンバ関数
	static CEditStage *Create(CEditManager *pEditManager, EType type);	// 生成
	static void Release(CEditStage *&prEditStage);	// 破棄

	// メンバ関数
	D3DXVECTOR3 GetVec3Position(void) const { return m_pos; }	// 位置取得

private:
	// メンバ関数
	void UpdatePosition(void);	// 位置更新

	// メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
};

#endif	// _EDIT_STAGE_H_
