//============================================================
//
//	エディットヘッダー [edit.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_H_
#define _EDIT_H_

//************************************************************
//	クラス定義
//************************************************************
// エディットクラス
class CEdit
{
public:
	// モード列挙
	enum EMode
	{
		MODE_PLAY = 0,	// プレイモード
		MODE_COLL_SET,	// 判定設定モード
		MODE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CEdit();

	// デストラクタ
	~CEdit();

	// 仮想関数
	virtual HRESULT Init(void);	// 初期化
	virtual void Uninit(void);	// 終了
	virtual void Update(void);	// 更新
	virtual void Draw(void);	// 描画
	virtual void DrawEditControl(void);	// エディット操作表示
	virtual void DrawEditData(void);	// エディット情報表示

	// メンバ関数
	EMode GetMode(void);	// エディットモード取得

	// 静的メンバ関数
	static CEdit *Create(const EMode mode);	// 生成
	static void Release(CEdit *&prEdit);	// 破棄
	static void NextMode(CEdit *&prEdit);	// 次のモード設定

private:
	// メンバ変数
	EMode m_mode;	// エディットモード
};

#endif	// _EDIT_H_
