//============================================================
//
//	エディターヘッダー [editor.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDITOR_H_
#define _EDITOR_H_

//************************************************************
//	前方宣言
//************************************************************
class CEditManager;	// エディットマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// エディタークラス
class CEditor
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_STAGE = 0,	// ステージ
		TYPE_COLLISION,	// 当たり判定
		TYPE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CEditor(CEditManager *pEditManager);

	// デストラクタ
	virtual ~CEditor();

	// 純粋仮想関数
	virtual HRESULT Init(void)	= 0;	// 初期化
	virtual void Uninit(void)	= 0;	// 終了
	virtual void Update(void)	= 0;	// 更新
	virtual HRESULT Save(void)	= 0;	// 保存
	virtual bool IsSave(void)	= 0;	// 保存状況取得
	virtual void SaveInfo(void)	= 0;	// 情報保存
	virtual void LoadInfo(void)	= 0;	// 情報読込
	virtual void DrawDebugControl(void)	= 0;	// 操作表示描画
	virtual void DrawDebugInfo(void)	= 0;	// 情報表示描画

	// 静的メンバ関数
	static CEditor *Create(CEditManager *pEditManager, EType type);	// 生成
	static void Release(CEditor *&prEditor);	// 破棄

	// メンバ関数
	CEditManager *GetPtrEditManager(void) const;	// エディットマネージャー取得

private:
	// メンバ変数
	CEditManager *m_pEditManager;	// エディットマネージャー
};

#endif	// _EDITOR_H_
