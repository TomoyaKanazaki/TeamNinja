//============================================================
//
//	オブジェクトメッシュフィールドヘッダー [objectMeshField.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT_MESHFIELD_H_
#define _OBJECT_MESHFIELD_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトメッシュフィールドクラス
class CObjectMeshField : public CObject
{
public:
	// コンストラクタ
	explicit CObjectMeshField
	( // 引数
		const ELabel label = LABEL_NONE,		// ラベル
		const EScene scene = SCENE_MAIN,		// シーン
		const EDim dim = DIM_3D,				// 次元
		const int nPrio = object::DEFAULT_PRIO	// 優先順位
	);

	// デストラクタ
	~CObjectMeshField() override;

	// メッシュフィールド構造体
	struct SMeshField
	{
		D3DXVECTOR3	pos;		// 位置
		D3DXVECTOR3	rot;		// 向き
		D3DXVECTOR2	size;		// 大きさ
		D3DXCOLOR	col;		// 色
		D3DXMATRIX	mtxWorld;	// ワールドマトリックス
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;			// 更新
	void Draw(CShader *pShader = nullptr) override;			// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	void SetVec2Sizing(const D3DXVECTOR2& rSize) override;	// 大きさ設定
	D3DXVECTOR3 GetVec3Position(void) const override	{ return m_meshField.pos; }			// 位置取得
	D3DXVECTOR3 GetVec3Rotation(void) const override	{ return m_meshField.rot; }			// 向き取得
	D3DXVECTOR2 GetVec2Sizing(void) const override		{ return m_meshField.size; }		// 大きさ取得
	D3DXMATRIX *GetPtrMtxWorld(void) override			{ return &m_meshField.mtxWorld; }	// マトリックスポインタ取得
	D3DXMATRIX GetMtxWorld(void) const override			{ return m_meshField.mtxWorld; }	// マトリックス取得

	// 静的メンバ関数
	static CObjectMeshField *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR2& rSize,	// 大きさ
		const D3DXCOLOR& rCol,		// 色
		const POSGRID2& rPart,		// 分割数
		const POSGRID2& rTexPart	// テクスチャ分割数
	);

	// メンバ関数
	CRenderState *GetRenderState(void);				// レンダーステート情報取得
	void BindTexture(const int nTextureID);			// テクスチャ割当 (インデックス)
	void BindTexture(const char *pTexturePass);		// テクスチャ割当 (パス)
	void SetColor(const D3DXCOLOR& rCol);			// 色設定
	HRESULT SetPattern(const POSGRID2& rPart);		// 分割数設定
	void SetTexPattern(const POSGRID2& rTexPart);	// テクスチャ分割数設定
	int GetTextureIndex(void) const		{ return m_nTextureID; }	// テクスチャインデックス取得
	D3DXCOLOR GetColor(void) const		{ return m_meshField.col; }	// 色取得
	POSGRID2 GetPattern(void) const		{ return m_part; }			// 分割数取得
	POSGRID2 GetTexPattern(void) const	{ return m_texPart; }		// テクスチャ分割数取得
	int GetNumVertex(void) const		{ return m_nNumVtx; }		// 頂点数取得

	void SetGapPosition(const int nID, const D3DXVECTOR3& rPos);		// 座標のずれ設定
	D3DXVECTOR3 GetGapPosition(const int nID);							// 座標のずれ取得
	void SetMeshVertexPosition(const int nID, const D3DXVECTOR3& rPos);	// メッシュの頂点位置設定
	void SetTerrain(const POSGRID2& rPart, D3DXVECTOR3 *pPosGap);		// 地形設定

	void NormalizeNormal(void);	// 法線の正規化
	bool LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove);	// メッシュの着地

	bool IsPositionRange(const D3DXVECTOR3& rPos);		// メッシュの範囲内取得
	float GetPositionHeight(const D3DXVECTOR3& rPos);	// メッシュの着地位置取得
	D3DXVECTOR3 GetMeshVertexPosition(const int nID);	// メッシュの頂点位置取得

protected:
	// メンバ関数
	void SetVtx(bool bNor);	// 頂点情報の設定
	void SetIdx(void);		// インデックス情報の設定
	void SetScrollTex		// スクロールのテクスチャ座標の設定
	( // 引数
		const float fTexU,	// テクスチャの横座標の開始位置
		const float fTexV	// テクスチャの縦座標の開始位置
	);

private:
	// オーバーライド関数
	void Release(void) override { CObject::Release(); }	// 破棄

	// メンバ関数
	void DrawNormal(void);	// 通常描画
	void DrawShader(CShader *pShader);	// シェーダー描画
	void DrawZTexture(void);
	void DrawToonShadow(void);
	D3DXVECTOR3 GetNormalLeft(VERTEX_3D *pVtx);			// 法線の取得 (左)
	D3DXVECTOR3 GetNormalLeftTop(VERTEX_3D *pVtx);		// 法線の取得 (左上)
	D3DXVECTOR3 GetNormalLeftBottom(VERTEX_3D *pVtx);	// 法線の取得 (左下)
	D3DXVECTOR3 GetNormalRight(VERTEX_3D *pVtx);		// 法線の取得 (右)
	D3DXVECTOR3 GetNormalRightTop(VERTEX_3D *pVtx);		// 法線の取得 (右上)
	D3DXVECTOR3 GetNormalRightBottom(VERTEX_3D *pVtx);	// 法線の取得 (右下)

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	CRenderState *m_pRenderState;		// レンダーステートの情報
	D3DXVECTOR3 *m_pPosGapBuff;	// 座標のずれバッファへのポインタ
	D3DXVECTOR3 *m_pNorBuff;	// 法線バッファへのポインタ
	int *m_pNumNorBuff;			// 法線の使用数バッファへのポインタ

	SMeshField m_meshField;	// メッシュフィールドの情報
	POSGRID2 m_part;		// 分割数
	POSGRID2 m_texPart;		// テクスチャ分割数
	int m_nNumVtx;			// 必要頂点数
	int m_nNumIdx;			// 必要インデックス数
	int m_nTextureID;		// テクスチャインデックス
};

#endif	// _OBJECT_MESHFIELD_H_
