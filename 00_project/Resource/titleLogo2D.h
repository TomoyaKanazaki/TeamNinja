//============================================================
//
//	タイトルロゴ2Dヘッダー [titleLogo2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TITLE_LOGO2D_H_
#define _TITLE_LOGO2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "anim2D.h"
#include "blur2D.h"

//************************************************************
//	クラス定義
//************************************************************
// タイトルロゴ2Dクラス
class CTitleLogo2D : public CAnim2D
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,			// 何もしない
		STATE_ONE_MOVE_WAIT,	// 第一移動待機
		STATE_ONE_MOVE,			// 第一移動
		STATE_TWO_MOVE_WAIT,	// 第二移動待機
		STATE_TWO_MOVE,			// 第二移動
		STATE_AURA_WAIT,		// オーラ待機
		STATE_AURA,				// オーラ
		STATE_MAX				// この列挙型の総数
	};

	// コンストラクタ
	CTitleLogo2D(const char* pBlurTexPath);

	// デストラクタ
	~CTitleLogo2D() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// 大きさ設定
	void SetPattern(const int nPattern) override;			// パターンの設定
	void SetWidthPattern(const int nWidthPtrn) override;	// テクスチャの横分割数の設定
	void SetHeightPattern(const int nHeightPtrn) override;	// テクスチャの縦分割数の設定

	// 静的メンバ関数
	static CTitleLogo2D *Create	// 生成
	( // 引数
		const char* pBlurTexPath,			// ブラーテクスチャパス
		const D3DXVECTOR3& rDestPosOne,		// 第一目標位置
		const D3DXVECTOR3& rDestPosTwo,		// 第二目標位置
		const D3DXVECTOR3& rOffset,			// オフセット
		const D3DXVECTOR3& rSize,			// 大きさ
		const float fMoveTimeOne = 1.0f,	// 第一移動時間
		const float fMoveTimeTwo = 1.0f,	// 第二移動時間
		const float fWaitTimeOne = 1.0f,	// 第一待機時間
		const float fWaitTimeTwo = 1.0f		// 第二待機時間
	);

	// メンバ関数
	void SetStag(void)		{ m_state = STATE_ONE_MOVE_WAIT; }	// 演出開始設定
	bool IsStag(void) const	{ return (m_state != STATE_NONE); }	// 演出中フラグ取得
	void SetBlurColor(const D3DXCOLOR& rCol)		{ m_pBlur->SetColor(rCol); }			// ブラー色設定
	void BindAuraTexture(const char *pTexturePass)	{ m_pAura->BindTexture(pTexturePass); }	// テクスチャ割当 (インデックス)
	void BindAuraTexture(const int nTextureID)		{ m_pAura->BindTexture(nTextureID); }	// テクスチャ割当 (パス)

private:
	// メンバ関数
	void UpdateMoveOneWait(const float fDeltaTime);	// 第一移動待機更新
	void UpdateMoveOne(const float fDeltaTime);		// 第一移動更新
	void UpdateMoveTwoWait(const float fDeltaTime);	// 第二移動待機更新
	void UpdateMoveTwo(const float fDeltaTime);		// 第二移動更新
	void UpdateAuraWait(const float fDeltaTime);	// オーラ待機更新
	void UpdateAura(const float fDeltaTime);		// オーラ更新

	// メンバ変数
	const char* m_pBlurTexPath;	// ブラーテクスチャパス
	CAnim2D* m_pAura;			// オーラ情報
	CBlur2D* m_pBlur;			// ブラー情報
	EState m_state;				// 状態
	float m_fMoveTimeOne;		// 第一移動時間
	float m_fMoveTimeTwo;		// 第二移動時間
	float m_fWaitTimeOne;		// 第一待機時間
	float m_fWaitTimeTwo;		// 第二待機時間
	float m_fCurTime;			// 現在の待機時間
	D3DXVECTOR3 m_offset;		// 初期位置オフセット
	D3DXVECTOR3 m_initPos;		// 初期位置
	D3DXVECTOR3 m_destPosOne;	// 第一目標位置
	D3DXVECTOR3 m_destPosTwo;	// 第二目標位置
};

#endif	// _TITLE_LOGO2D_H_
