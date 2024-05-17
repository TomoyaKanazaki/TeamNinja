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
//	前方宣言
//************************************************************
class CEditManager;	// エディットマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// エディットステージクラス
class CEditStage
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
	CEditStage();

	// デストラクタ
	virtual ~CEditStage();

	// 純粋仮想関数
	virtual HRESULT Init(void)	= 0;	// 初期化
	virtual void Uninit(void)	= 0;	// 終了
	virtual void Update(void)	= 0;	// 更新
	virtual void SaveInfo(void)	= 0;	// 情報保存
	virtual void LoadInfo(void)	= 0;	// 情報読込
	virtual void DrawDebugControl(void)	= 0;	// 操作表示描画
	virtual void DrawDebugInfo(void)	= 0;	// 情報表示描画
	virtual void Save(FILE *pFile)		= 0;	// 保存

	// 静的メンバ関数
	static CEditStage *Create(CEditManager *pEditManager, EType type);	// 生成
	static void Release(CEditStage *&prEditStage);	// 破棄

	// メンバ関数
	const CEditManager *GetPtrEditManager(void) const;	// エディットマネージャー取得

private:
	// メンバ変数
	const CEditManager *m_pEditManager;	// エディットマネージャー
	CEditStage *m_pStage;	// ステージエディター
};

#endif	// _EDIT_STAGE_H_
