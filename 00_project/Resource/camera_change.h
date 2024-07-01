#pragma once
//===========================================
//
//  カメラ方向変更判定クラス(caera_change.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "objectMeshCube.h"

//===========================================
//  クラス定義
//===========================================
class CCameraChanger : public CObjectMeshCube
{
public:

	// 方向列挙
	enum EDirection
	{
		DIRECTION_FRONT = 0, // 正面
		DIRECTION_BACK, // 後方
		DIRECTION_LEFT, // 左
		DIRECTION_RIGHT, // 右
		DIRECTION_MAX
	};

	// 角度列挙
	enum ERotation
	{
		ROTATION_DEFAULT = 0, // デフォルトの角度
		ROTATION_UP, // 上
		ROTATION_DOWN, // 下
		ROTATION_MAX
	};

	// メンバ関数
	CCameraChanger();
	~CCameraChanger();

	HRESULT Init() override; // 初期化処理
	void Uninit() override; // 終了処理
	void Update(const float fDeltaTime) override; // 更新処理
	void Draw(CShader* pShader = nullptr) override; // 描画処理

	bool GetChange() const { return m_bChange; } // フラグの取得

	// 静的メンバ関数
	static CCameraChanger* Create // 生成処理
	(
		const D3DXVECTOR3& rPos, // 中心座標
		const D3DXVECTOR3& rSize, // 大きさ
		const EDirection Dir = DIRECTION_FRONT, // カメラ方向
		const ERotation Rot = ROTATION_DEFAULT // カメラ角度
	);
	static CListManager<CCameraChanger>* GetList(void);	// リスト取得

private:

	// メンバ変数
	bool m_bChange; // カメラ方向変更フラグ
	EDirection m_eDir; // カメラ方向
	ERotation m_eRot; // カメラ角度
	CListManager<CCameraChanger>::AIterator m_iterator; // イテレーター

	// 静的メンバ変数
	static CListManager<CCameraChanger>* m_pList; // オブジェクトリスト
};
