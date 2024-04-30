#if 0
//============================================================
//
//	オブジェクトスキンメッシュ3Dヘッダー [objectSkinMesh3D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT_SKINMESH3D_H_
#define _OBJECT_SKINMESH3D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトスキンメッシュ3Dクラス
class CObjectSkinMesh3D : public CObject
{
public:
	// コンストラクタ
	explicit CObjectSkinMesh3D(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = object::DEFAULT_PRIO);

	// デストラクタ
	~CObjectSkinMesh3D() override;

	// ボーン情報構造体
	struct SBone
	{
		D3DXVECTOR3 pos;	// ボーン位置オフセット
		D3DXVECTOR3 rot;	// ボーン向きオフセット
		D3DXMATRIX mtx;		// ボーンマトリックス
		int nParentID;		// 親ボーン
	};

	// 頂点追加情報
	struct SVtx
	{
		D3DXVECTOR3 pos;	// ボーン位置オフセット
		D3DXMATRIX mtx;		// 頂点マトリックス
		int aBoneRef[2];	// ボーン参照値
		float aWeight[2];	// ボーン影響度
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(CShader *pShader = nullptr) override;			// 描画
	void BindTexture(const int nTextureID) override;		// テクスチャ割当 (インデックス)
	void BindTexture(const char *pTexturePass) override;	// テクスチャ割当 (パス)
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// 向き取得

	// 静的メンバ関数
	static CObjectSkinMesh3D *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,			// 位置
		const D3DXVECTOR3& rRot = VEC3_ZERO	// 向き
	);

	// メンバ関数
	HRESULT CreateBone(const int nNumBone);		// ボーン生成
	HRESULT CreateVertex(const int nNumVtx);	// 分割数設定

	void SetBoneInfo	// ボーン情報設定
	( // 引数
		const int nID,				// パーツインデックス
		const int nParentID,		// 親インデックス
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);
	void SetVertexInfo	// 頂点情報設定
	( // 引数
		const int nVtxID,		// 頂点インデックス
		const SVtx& rVtx,		// 頂点追加情報
		const D3DXVECTOR2& rTex	// テクスチャ座標
	);

	CRenderState *GetRenderState(void);	// レンダーステート情報取得

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄

	// メンバ関数
	void DrawNormal(void);				// 通常描画
	void DrawShader(CShader *pShader);	// シェーダー描画
	void LoadSetup(void);				// セットアップ

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	CRenderState *m_pRenderState;		// レンダーステートの情報
	D3DXMATRIX	m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3	m_pos;			// 位置
	D3DXVECTOR3	m_rot;			// 向き
	D3DXVECTOR3	m_size;			// 大きさ
	D3DXCOLOR	m_col;			// 色
	int			m_nTextureID;	// テクスチャインデックス

	SBone *m_pBone;		// ボーン情報
	int m_nNumBone;		// ボーン数
	SVtx *m_pVtxPlus;	// 頂点追加情報
	int m_nNumVtx;		// 頂点数
};

#endif	// _OBJECT_SKINMESH3D_H_
#endif