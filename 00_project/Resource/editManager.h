//============================================================
//
//	エディットマネージャーヘッダー [editManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_MANAGER_H_
#define _EDIT_MANAGER_H_

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_DOUBLE		(DIK_LCONTROL)	// 二重化キー
#define NAME_DOUBLE		("LCTRL")		// 二重化表示
#define KEY_TRIGGER		(DIK_LSHIFT)	// トリガーキー
#define NAME_TRIGGER	("LSHIFT")		// トリガー表示
#define KEY_REVERSE		(DIK_LALT)		// 操作逆転キー
#define NAME_REVERSE	("LALT")		// 操作逆転表示

//************************************************************
//	クラス定義
//************************************************************
// エディットマネージャークラス
class CEditManager
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_STAGE = 0,	// ステージ種類
		TYPE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CEditManager();

	// デストラクタ
	~CEditManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void UnSave(void);	// 未保存設定
	void SwitchEnableEdit(void);			// エディット状況切替
	void SetEnableEdit(const bool bEdit);	// エディット状況設定
	bool IsEdit(void) const;				// エディット状況取得

	// 静的メンバ関数
	static CEditManager *Create(void);	// 生成
	static void Release(CEditManager *&prEditManager);	// 破棄

private:
	// メンバ関数
	void ChangeType(void);			// エディター変更
	void DrawDebugControl(void);	// 操作表示描画
	void DrawDebugInfo(void);		// 情報表示描画
	void SaveStage(void);			// ステージ保存
	void Save(void);				// 保存

	// メンバ変数
	EType m_type;	// エディットタイプ
	bool m_bSave;	// 保存状況
	bool m_bEdit;	// エディット状況
};

#endif	// _EDIT_MANAGER_H_
