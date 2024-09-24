//============================================================
//
//	キューブの当たり判定ヘッダー [collisionCube.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _COLLISION_CUBE_H_
#define _COLLISION_CUBE_H_

//***********************************
// インクルードファイル
//***********************************
#include "collisionModel.h"
#include "listManager.h"

#ifdef _DEBUG

#include "objectMeshCube.h"

#endif // _DEBUG

// クラス定義(当たり判定クラス)
class CCollisionCube : public CCollision
{
public:

	CCollisionCube();			// コンストラクタ
	~CCollisionCube();			// デストラクタ

	void Init(void);			// 初期化処理
	void Uninit(void) override;	// 終了処理

	bool Hit					// ヒット処理
	(
		D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rPosOld,		// 前回の位置
		const float fRadius,			// 半径
		const float fHeight,			// 高さ
		D3DXVECTOR3& rMove,				// 移動量
		bool& rJump						// ジャンプ状況
	) override;

	void OffSet(const D3DXMATRIX& mtx) override;		// オフセット設定処理

	// セット・ゲット関係
	void SetWidth(const float fWidth) { m_fWidth = fWidth; }		// 幅の設定処理
	float GetWidth(void) const { return m_fWidth; }					// 幅の取得処理
	void SetHeight(const float fHeight) { m_fHeight = fHeight; }	// 高さの設定処理
	float GetHeight(void) const { return m_fHeight; }				// 高さの取得処理
	void SetDepth(const float fDepth) { m_fDepth = fDepth; }		// 奥行の設定処理
	float GetDepth(void) const { return m_fDepth; }					// 奥行の取得処理

#ifdef _DEBUG

	// セット・ゲット関係
	CObjectMeshCube* GetCube(void) const { return m_pCube; }	// キューブの取得処理

#endif // _DEBUG

	// 静的メンバ関数
	static CCollisionCube* Create			// 生成処理
	(
		const D3DXVECTOR3& rPos,		// 位置
		const D3DXVECTOR3& rOffset,		// オフセット
		const float fWidth,				// 幅
		const float fHeight,			// 高さ
		const float fDepth,				// 奥行
		const float fRot				// 向き
	);
	static CListManager<CCollisionCube>* GetList(void);	// リスト取得

private:

	// メンバ関数
	bool Convert(const float fWidth, const float fDepth, const float fRot);		// 向きによる変換処理

#ifdef _DEBUG

	bool RotWarning(const float fRot);		// 向きの警告処理

#endif


	// メンバ変数
	CListManager<CCollisionCube>::AIterator m_iterator;	// イテレーター

	float m_fWidth;		// 幅
	float m_fHeight;	// 高さ
	float m_fDepth;		// 奥行

#ifdef _DEBUG

	CObjectMeshCube* m_pCube;	// メッシュキューブ

#endif // _DEBUG

	// 静的メンバ変数
	static CListManager<CCollisionCube>* m_pList;			// オブジェクトリスト
};

#endif	// _COLLISION_CUBE_H_
