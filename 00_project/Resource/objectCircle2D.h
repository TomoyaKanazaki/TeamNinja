#if 1
//============================================================
//
//	オブジェクトサークル2Dヘッダー [objectCircle2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT_CIRCLE2D_H_
#define _OBJECT_CIRCLE2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトサークル2Dクラス
class CObjectCircle2D : public CObject
{
public:
	// コンストラクタ
	explicit CObjectCircle2D
	( // 引数
		const ELabel label = LABEL_NONE,		// ラベル
		const EScene scene = SCENE_MAIN,		// シーン
		const EDim dim = DIM_2D,				// 次元
		const int nPrio = object::DEFAULT_PRIO	// 優先順位
	);

	// デストラクタ
	~CObjectCircle2D() override;

	// サークル構造体
	struct SCircle
	{
		D3DXVECTOR3	pos;		// 位置
		D3DXVECTOR3	rot;		// 向き
		D3DXCOLOR	col;		// 色
		float		fRadius;	// 半径
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;			// 更新
	void Draw(CShader *pShader = nullptr) override;			// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	D3DXVECTOR3 GetVec3Position(void) const override { return m_meshCircle.pos; }	// 位置取得
	D3DXVECTOR3 GetVec3Rotation(void) const override { return m_meshCircle.rot; }	// 向き取得

	// 静的メンバ関数
	static CObjectCircle2D *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXCOLOR& rCol,		// 色
		const POSGRID2& rPart,		// 分割数
		const float fRadius			// 半径
	);

	// メンバ関数
	CRenderState *GetRenderState(void);			// レンダーステート情報取得
	void BindTexture(const int nTextureID);		// テクスチャ割当 (インデックス)
	void BindTexture(const char *pTexturePass);	// テクスチャ割当 (パス)
	void SetColor(const D3DXCOLOR& rCol);		// 色設定
	void SetRadius(const float fRadius);		// 半径設定
	HRESULT SetPattern(const POSGRID2& rPart);	// 分割数設定
	int GetTextureIndex(void) const	{ return m_nTextureID; }			// テクスチャインデックス取得
	D3DXCOLOR GetColor(void) const	{ return m_meshCircle.col; }		// 色取得
	float GetRadius(void) const		{ return m_meshCircle.fRadius; }	// 半径取得
	POSGRID2 GetPattern(void) const	{ return m_part; }					// 分割数取得

protected:
	// メンバ関数
	void SetVtx(void);	// 頂点情報の設定
	void SetIdx(void);	// インデックス情報の設定

private:
	// オーバーライド関数
	void Release(void) override { CObject::Release(); }	// 破棄

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	CRenderState *m_pRenderState;		// レンダーステートの情報
	SCircle m_meshCircle;	// サークルの情報
	POSGRID2 m_part;		// 分割数
	int m_nNumVtx;			// 必要頂点数
	int m_nNumIdx;			// 必要インデックス数
	int m_nTextureID;		// テクスチャインデックス
};

#endif	// _OBJECT_CIRCLE2D_H_
#endif
