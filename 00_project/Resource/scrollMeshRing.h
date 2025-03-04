//============================================================
//
//	スクロールメッシュリングヘッダー [scrollMeshRing.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCROLL_MESHRING_H_
#define _SCROLL_MESHRING_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectMeshRing.h"

//************************************************************
//	クラス定義
//************************************************************
// スクロールメッシュリングクラス
class CScrollMeshRing : public CObjectMeshRing
{
public:
	// コンストラクタ
	explicit CScrollMeshRing
	( // 引数
		const ELabel label = LABEL_NONE,		// ラベル
		const EScene scene = SCENE_MAIN,		// シーン
		const EDim dim = DIM_3D,				// 次元
		const int nPrio = object::DEFAULT_PRIO	// 優先順位
	);

	// デストラクタ
	~CScrollMeshRing() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;			// 更新
	void Draw(CShader *pShader = nullptr) override;			// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	void SetVec2Sizing(const D3DXVECTOR2& rSize) override;	// 大きさ設定

	// 静的メンバ関数
	static CScrollMeshRing *Create	// 生成
	( // 引数
		const float fMoveU,			// 横座標の移動量
		const float fMoveV,			// 縦座標の移動量
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR2& rSize,	// 大きさ
		const D3DXCOLOR& rCol,		// 色
		const POSGRID2& rPart		// 分割数
	);

	// メンバ関数
	void SetColor(const D3DXCOLOR& rCol);			// 色設定
	void SetTexDir(const ETexDir texDir);			// テクスチャ方向設定
	void SetHoleRadius(const float fHoleRadius);	// 穴の半径設定
	void SetThickness(const float fThickness);		// 太さ設定
	void SetOuterPlusY(const float fOuterPlusY);	// 外周のY座標加算量設定
	HRESULT SetPattern(const POSGRID2& rPart);		// 分割数設定
	void SetTexPattern(const POSGRID2& rTexPart);	// テクスチャ分割数設定
	void SetTexU(const float fTexU);	// 横座標の開始位置設定
	void SetTexV(const float fTexV);	// 縦座標の開始位置設定
	void SetMoveU(const float fMoveU);	// 横座標の移動量設定
	float GetMoveU(void) const;			// 横座標の移動量取得
	void SetMoveV(const float fMoveV);	// 縦座標の移動量設定
	float GetMoveV(void) const;			// 縦座標の移動量取得

private:
	// メンバ変数
	float m_fTexU;	// テクスチャ横座標の開始位置
	float m_fTexV;	// テクスチャ縦座標の開始位置
	float m_fMoveU;	// テクスチャ横座標の移動量
	float m_fMoveV;	// テクスチャ縦座標の移動量
};

#endif	// _SCROLL_MESHRING_H_
