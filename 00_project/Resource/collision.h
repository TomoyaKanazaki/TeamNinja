//============================================================
//
//	判定関数ヘッダー [collision.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

//************************************************************
//	名前空間宣言
//************************************************************
// 判定空間
namespace collision
{
	bool Box2D	// XZ平面の矩形の当たり判定
	( // 引数
		D3DXVECTOR3 centerPos,		// 判定位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
		D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
		D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
		D3DXVECTOR3 targetSizeDown	// 判定目標サイズ(左・下・前)
	);
	bool Box3D	// 3軸の矩形の当たり判定
	( // 引数
		D3DXVECTOR3 centerPos,		// 判定位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
		D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
		D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
		D3DXVECTOR3 targetSizeDown	// 判定目標サイズ(左・下・前)
	);
	bool Box3D	// 3軸の矩形の当たり判定(向きの列挙判定入り)
	( // 引数
		D3DXVECTOR3 centerPos,		// 判定位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
		D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
		D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
		D3DXVECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
		const EAngle centerAngle,	// 判定方向列挙
		const EAngle targetAngle	// 判定目標方向列挙
	);
	bool Circle2D	// XZ平面の円の当たり判定
	( // 引数
		D3DXVECTOR3 centerPos,	// 判定位置
		D3DXVECTOR3 targetPos,	// 判定目標位置
		float fCenterRadius,	// 判定半径
		float fTargetRadius,	// 判定目標半径
		float *pLength = nullptr	// 判定目標との距離
	);
	bool Circle3D	// 3軸の円の当たり判定
	( // 引数
		D3DXVECTOR3 centerPos,	// 判定位置
		D3DXVECTOR3 targetPos,	// 判定目標位置
		float fCenterRadius,	// 判定半径
		float fTargetRadius		// 判定目標半径
	);
	bool Sector	// XZ平面の扇形の当たり判定
	( // 引数
		D3DXVECTOR3 centerPos,		// 判定位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		float fCenterRot,			// 判定向き
		float fRadius,				// 視界範囲
		float fAngle,				// 視野角
		float *pLength = nullptr	// 判定目標との距離
	);

	bool BoxPillar	// 角柱の衝突判定
	( // 引数
		D3DXVECTOR3& rCenterPos,	// 判定位置
		D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
		D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
		D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
		D3DXVECTOR3 targetSizeDown	// 判定目標サイズ(左・下・前)
	);
	bool CirclePillar	// 円柱の衝突判定
	( // 引数
		D3DXVECTOR3& rCenterPos,	// 判定位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		float fCenterRadius,		// 判定半径
		float fTargetRadius			// 判定目標半径
	);

	bool ResponseBox3D	// 三軸の矩形の衝突判定
	( // 引数
		D3DXVECTOR3& rCenterPos,	// 判定位置
		D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
		D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
		D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
		D3DXVECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
		D3DXVECTOR3 *pMove = nullptr,	// 移動量
		bool *pUp = nullptr,	// 上からの判定
		bool *pSide = nullptr,	// 横からの判定
		bool *pDown = nullptr	// 下からの判定
	);
	bool ResponseBox3D	// 三軸の矩形の衝突判定(向きの列挙判定入り)
	( // 引数
		D3DXVECTOR3& rCenterPos,	// 判定位置
		D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
		D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
		D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
		D3DXVECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
		const EAngle angle,			// 方向の列挙
		D3DXVECTOR3* pMove = nullptr,	// 移動量
		bool* pUp = nullptr,	// 上からの判定
		bool* pSide = nullptr,	// 横からの判定
		bool* pDown = nullptr	// 下からの判定
	);
	bool ResponseBox2D	// 三軸の矩形の衝突判定
	( // 引数
		D3DXVECTOR3& rCenterPos,	// 判定位置
		D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
		D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
		D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
		D3DXVECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
		D3DXVECTOR3* pMove = nullptr,	// 移動量
		bool* pUp = nullptr,	// 上からの判定
		bool* pSide = nullptr,	// 横からの判定
		bool* pDown = nullptr	// 下からの判定
	);
	bool ResponseBox2D	// 三軸の矩形の衝突判定(向きの列挙判定入り)
	( // 引数
		D3DXVECTOR3& rCenterPos,	// 判定位置
		D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
		D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
		D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
		D3DXVECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
		const EAngle angle,			// 方向の列挙
		D3DXVECTOR3* pMove = nullptr,	// 移動量
		bool* pUp = nullptr,	// 上からの判定
		bool* pSide = nullptr,	// 横からの判定
		bool* pDown = nullptr	// 下からの判定
	);
	bool ResponseCircle3D	// 三軸の円の衝突判定
	( // 引数
		D3DXVECTOR3& rCenterPos,	// 判定位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		float fCenterRadius,		// 判定半径
		float fTargetRadius			// 判定目標半径
	);
	bool ResponseCapsule3D	// 三軸のカプセルの衝突判定
	( // 引数
		D3DXVECTOR3 *pCenterPos,	// 判定位置
		D3DXVECTOR3& rTargetPos,	// 判定目標位置
		float fCenterRadius,		// 判定半径
		float fTargetRadius,		// 判定目標半径
		float fTargetHeight			// 判定目標縦幅
	);

	bool ResponseSingleX	// X軸の衝突判定
	( // 引数
		D3DXVECTOR3& rCenterPos,	// 判定位置
		D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
		D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
		D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
		D3DXVECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
		D3DXVECTOR3 *pMove = nullptr,	// 移動量
		bool *pLeft = nullptr,			// 左からの判定
		bool *pRight = nullptr			// 右からの判定
	);
	bool ResponseSingleY	// Y軸の衝突判定
	( // 引数
		D3DXVECTOR3& rCenterPos,	// 判定位置
		D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
		D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
		D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
		D3DXVECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
		D3DXVECTOR3 *pMove = nullptr,	// 移動量
		bool *pDown = nullptr,			// 下からの判定
		bool *pUp = nullptr			// 上からの判定
	);
	bool ResponseSingleZ	// Z軸の衝突判定
	( // 引数
		D3DXVECTOR3& rCenterPos,	// 判定位置
		D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
		D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
		D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
		D3DXVECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
		D3DXVECTOR3 *pMove = nullptr,	// 移動量
		bool *pBefore = nullptr,		// 前からの判定
		bool *pAfter = nullptr			// 後からの判定
	);

	bool InBoxPillar	// 角柱の内側制限
	( // 引数
		D3DXVECTOR3& rCenterPos,	// 判定位置
		D3DXVECTOR3 originPos,		// 判定原点位置
		D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
		D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
		D3DXVECTOR3 originSizeUp,	// 判定原点サイズ(右・上・後)
		D3DXVECTOR3 originSizeDown	// 判定原点サイズ(左・下・前)
	);
	bool InCirclePillar	// 円柱の内側制限
	( // 引数
		D3DXVECTOR3& rCenterPos,	// 判定位置
		D3DXVECTOR3 originPos,		// 判定原点位置
		float fCenterRadius,		// 判定半径
		float fOriginRadius			// 判定原点半径
	);

	float LineOuterProduct	// 外積の左右判定
	( // 引数
		D3DXVECTOR3 posLeft,	// 境界線左座標
		D3DXVECTOR3 posRight,	// 境界線右座標
		D3DXVECTOR3 pos			// 判定位置
	);
	float LineHeightOuterProduct	// 外積の上下判定
	( // 引数
		D3DXVECTOR3 posLeft,	// 境界線左座標
		D3DXVECTOR3 posRight,	// 境界線右座標
		D3DXVECTOR3 pos			// 判定位置
	);

	bool TriangleOuterPillar	// 三角柱の当たり判定
	( // 引数
		D3DXVECTOR3 pos0,	// 三角の各頂点
		D3DXVECTOR3 pos1,	// 三角の各頂点
		D3DXVECTOR3 pos2,	// 三角の各頂点
		D3DXVECTOR3 pos		// 判定位置
	);
	bool BoxOuterPillar		// 四角柱の当たり判定
	( // 引数
		D3DXVECTOR3 pos0,	// 四角の各頂点
		D3DXVECTOR3 pos1,	// 四角の各頂点
		D3DXVECTOR3 pos2,	// 四角の各頂点
		D3DXVECTOR3 pos3,	// 四角の各頂点
		D3DXVECTOR3 pos		// 判定位置
	);
}

#endif	// _COLLISION_H_
