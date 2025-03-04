//============================================================
//
//	当たり判定マネージャーヘッダー [collManager.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _COLL_MANAGER_H_
#define _COLL_MANAGER_H_

//************************************************************
// インクルードファイル
//************************************************************
#include "actor.h"

//************************************************************
//	クラス定義
//************************************************************
// 当たり判定マネージャークラス
class CCollManager
{
public:

	// キューブの当たり判定の情報構造体
	struct SCollCube
	{
		D3DXVECTOR3 offset;	// オフセット座標
		float fWidth;		// 幅
		float fHeight;		// 高さ
		float fDepth;		// 奥行
	};

	// シリンダーの当たり判定の情報構造体
	struct SCollCylinder
	{
		D3DXVECTOR3 offset;	// オフセット座標
		float fRadius;		// 半径
		float fHeight;		// 高さ
	};

	// スフィアの当たり判定の情報構造体
	struct SCollSphere
	{
		D3DXVECTOR3 offset;	// オフセット座標
		float fRadius;		// 半径
	};

	// ポリゴンの当たり判定の情報構造体
	struct SCollPolygon
	{
		D3DXVECTOR3 offset;	// オフセット座標
		D3DXVECTOR3 rot;	// 向き
		D3DXVECTOR3 size;	// サイズ
	};

	// 当たり判定関係の構造体
	struct SCollision
	{
		SCollision()			// コンストラクタ
		{
			m_cube = {};			// キューブの情報配列
			m_cylinder = {};		// シリンダーの情報配列
			m_sphere = {};			// スフィアの情報配列
			m_polygon = {};			// ポリゴンの情報配列
		}
		SCollision				// コンストラクタ(オーバーロード)
		(
			std::vector<SCollCube> cube,
			std::vector<SCollCylinder> cylinder,
			std::vector<SCollSphere> sphere,
			std::vector<SCollPolygon> polygon
		)
		{
			m_cube = cube;			// キューブの情報配列
			m_cylinder = cylinder;	// シリンダーの情報配列
			m_sphere = sphere;		// スフィアの情報配列
			m_polygon = polygon;	// ポリゴンの情報配列
		}

		std::vector<SCollCube> m_cube;			// キューブの情報配列
		std::vector<SCollCylinder> m_cylinder;	// シリンダーの情報配列
		std::vector<SCollSphere> m_sphere;		// スフィアの情報配列
		std::vector<SCollPolygon> m_polygon;	// ポリゴンの情報配列
	};

	// コンストラクタ
	CCollManager();

	// デストラクタ
	~CCollManager();

	// メンバ関数
	void Uninit(void);	// 終了

	HRESULT Load(void);	// ロード処理

	void SetCollInfo(const CActor::EType type, const SCollision coll);	// 情報の取得処理
	SCollision GetCollInfo(const CActor::EType type);	// 情報の取得処理

	// 静的メンバ関数
	static CCollManager* Create(void);		// 生成

private:

	// メンバ変数
	SCollision m_aCollInfo[CActor::TYPE_MAX];			// 当たり判定の情報
};

#endif	// _DELTATIME_H_
