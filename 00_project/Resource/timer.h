//============================================================
//
//	タイマーヘッダー [timer.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TIMER_H_
#define _TIMER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// タイマークラス
class CTimer : public CObject
{
public:
	// 計測列挙
	enum EState
	{
		STATE_NONE = 0,	// 処理なし
		STATE_MEASURE,	// 計測中
		STATE_END,		// 計測終了 
		STATE_MAX		// この列挙型の総数
	};

	// 時間形式列挙
	enum ETime
	{
		TIME_MSEC,	// ミリ秒
		TIME_SEC,	// 秒
		TIME_MIN,	// 分
		TIME_MAX	// この列挙型の総数
	};

	// コンストラクタ
	CTimer();

	// デストラクタ
	~CTimer() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CTimer *Create	// 生成
	( // 引数

	);

	// メンバ関数
	void Start(void);	// 計測開始
	void End(void);		// 計測終了
	void EnableStop(const bool bStop);	// 計測停止設定
	void SetLimit(const ETime stateTime, const float fTime);	// 制限時間設定

	EState GetState(void) const	{ return m_state; }		// 計測状態取得
	float GetLimit(void) const	{ return m_fLimit; }	// 制限時間取得

private:
	// オーバーライド関数
	void Release(void) override { CObject::Release(); }	// 破棄

	// メンバ変数
	EState	m_state;	// 計測状態
	float	m_fTime;	// 計測時間
	float	m_fLimit;	// 制限時間
	bool	m_bStop;	// 計測停止状況
};

#endif	// _TIMER_H_
