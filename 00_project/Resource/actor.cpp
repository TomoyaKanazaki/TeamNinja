//============================================================
//
//	アクター処理 [actor.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "actor.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"

#include "sceneGame.h"
#include "collManager.h"
#include "collisionCube.h"
#include "collisionCylinder.h"
#include "collisionSphere.h"
#include "collisionPolygon.h"
#include "mash.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* MODEL[] =	// モデルのパス
	{
		"data\\MODEL\\Rock\\Rock000.x",					// 岩(小)
		"data\\MODEL\\Rock\\Rock001.x",					// 岩(中)
		"data\\MODEL\\Rock\\Rock002.x",					// 岩(大)
		"data\\MODEL\\Rock\\Moss-Rock000.x",			// 苔岩(小)
		"data\\MODEL\\Rock\\Moss-Rock001.x",			// 苔岩(中)
		"data\\MODEL\\Rock\\Moss-Rock002.x",			// 苔岩(大)
		"data\\MODEL\\Tree\\BirchTree000.x",			// 白樺の木(大)
		"data\\MODEL\\Tree\\BirchTree001.x",			// 白樺の木(中)
		"data\\MODEL\\Tree\\CommonTree000.x",			// 普通の木(大)
		"data\\MODEL\\Tree\\CommonTree001.x",			// 普通の木(中)
		"data\\MODEL\\Tree\\Willow000.x",				// 柳の木(大)
		"data\\MODEL\\Tree\\Willow001.x",				// 柳の木(中)
		"data\\MODEL\\Tree\\WoodLog000.x",				// 原木
		"data\\MODEL\\Tree\\TreeStump000.x",			// 木の幹
		"data\\MODEL\\LilyPad\\LilyPad000.x",			// 蓮の葉
		"data\\MODEL\\Plant\\Plant000.x",				// 4つしかつながっていない草
		"data\\MODEL\\Plant\\Plant001.x",				// 多くつながっている草
		"data\\MODEL\\Shrub\\Shrub000.x",				// 低木
		"data\\MODEL\\House\\House000.x",				// 家1
		"data\\MODEL\\House\\House001.x",				// 家2
		"data\\MODEL\\House\\House002.x",				// 家3
		"data\\MODEL\\House\\House003.x",				// 家4
		"data\\MODEL\\House\\House004.x",				// 家5
		"data\\MODEL\\House\\House005.x",				// 家6
		"data\\MODEL\\House\\House006.x",				// 家7
		"data\\MODEL\\House\\House007.x",				// 家8
		"data\\MODEL\\House\\House008.x",				// 家9
		"data\\MODEL\\House\\House009.x",				// 家10
		"data\\MODEL\\Lantern\\Lantern001.x",			// ランタン
		"data\\MODEL\\GATE\\gate000.x",					// 門(枠)
		"data\\MODEL\\GATE\\gate001.x",					// 門(扉)
		"data\\MODEL\\Kakejiku\\Kakejiku000.x",			// 掛け軸
		"data\\MODEL\\Pier\\Pier000.x",					// 橋
		"data\\MODEL\\Pillar\\WoodPillar000.x",			// 城に配置する形の柱
		"data\\MODEL\\Pillar\\WoodPillar001.x",			// ただ四角いだけの柱
		"data\\MODEL\\Stairs\\WoodStairs001.x",			// 階段
		"data\\MODEL\\B-Lattice\\BambooLattice001.x",	// 竹の格子
		"data\\MODEL\\RoofFloor\\FloorRooftop002.x",	// 骨組みみたいな屋根裏の木
		"data\\MODEL\\RoofFloor\\FloorRooftop003.x",	// レンガ柄風の屋根裏の木
		"data\\MODEL\\FUSUMA\\Fusuma000.x",				// ふすま(動く)
		"data\\MODEL\\FUSUMA\\Fusuma000.x",				// ふすま(動かない)
		"data\\MODEL\\FENCE\\fence000.x",				// フェンス00
		"data\\MODEL\\FENCE\\fence001.x",				// フェンス01
		"data\\MODEL\\FENCE\\fence002.x",				// フェンス02
		"data\\MODEL\\FENCE\\fence003.x",				// フェンス03
	};

	const char *SETUP_TXT = "data\\TXT\\actor.txt";	// セットアップテキスト相対パス
	const int PRIORITY = 4;	// アクターの優先順位
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(MODEL) == CActor::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
// 静的メンバ変数宣言
//************************************************************
CListManager<CActor>* CActor::m_pList = nullptr;		// リスト構造

//************************************************************
//	子クラス [CActor] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CActor::CActor() : CObjectModel(CObject::LABEL_ACTOR, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
m_type(TYPE_ROCK_S)		// 種類
{

}

//============================================================
//	デストラクタ
//============================================================
CActor::~CActor()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CActor::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CActor>::Create();

		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CActor::Uninit(void)
{
	// 当たり判定の消去処理
	ClearCollision();

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CActor::Update(const float fDeltaTime)
{
	// 遠距離判定
	bool bFar = useful::IsNearPosR(GetVec3Position());
	SetEnableDraw(bFar);
	if (!bFar) { return; }

	// 当たり判定のオフセット設定処理
	CollisionOffset();

	// オブジェクトモデルの更新
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CActor::Draw(CShader* pShader)
{
	// オブジェクトモデルの描画
	CObjectModel::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CActor* CActor::Create
( // 引数
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR3& rScale	// 拡大率
)
{
	// モデルUIの生成
	CActor* pActor = nullptr;
	if (type == TYPE_MASH_MOVE) // ふすまだけ子クラスを生成
	{
		pActor = new CMash(rPos);
	}
	else
	{
		pActor = new CActor;
	}

	// アクターの初期化
	if (FAILED(pActor->Init()))
	{ // 初期化に失敗した場合

		// アクターの破棄
		SAFE_DELETE(pActor);
		return nullptr;
	}

	// モデルの割り当て処理
	pActor->BindModel(MODEL[type]);

	// 向きを設定
	pActor->SetVec3Rotation(rRot);

	// 位置を設定
	pActor->SetVec3Position(rPos);

	// 拡大率を設定
	pActor->SetVec3Scaling(rScale);

	// 種類を設定
	pActor->m_type = type;

	// 当たり判定の割り当て処理
	pActor->BindCollision();

	// 確保したアドレスを返す
	return pActor;
}

//============================================================
// リスト構造の取得処理
//============================================================
CListManager<CActor>* CActor::GetList(void)
{
	// リスト構造を返す
	return m_pList;
}

//============================================================
// 当たり判定処理
//============================================================
void CActor::Collision
(
	D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rPosOld,		// 前回の位置
	const float fRadius,			// 半径
	const float fHeight,			// 高さ
	D3DXVECTOR3& rMove,				// 移動量
	bool& bJump						// ジャンプ状況
)
{
	// スフィア判定
	for (auto sphere : m_sphere)
	{
		// ヒット処理
		sphere->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);
	}

	// キューブ判定
	for (auto cube : m_cube)
	{
		// ヒット処理
		cube->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);
	}

	// シリンダー判定
	for (auto cylinder : m_cylinder)
	{
		// ヒット処理
		cylinder->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);
	}

	// ポリゴン判定
	for (auto polygon : m_polygon)
	{
		// ヒット処理
		polygon->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);
	}
}

//==========================================
//  判定を返すことのできる当たり判定
//==========================================
void CActor::Collision
(
	D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rPosOld,		// 前回の位置
	const float fRadius,			// 半径
	const float fHeight,			// 高さ
	D3DXVECTOR3& rMove,				// 移動量
	bool& bJump,					// ジャンプ状況
	bool& bHit						// 衝突判定
)
{
	// スフィア判定
	for (auto sphere : m_sphere)
	{
		// ヒット処理
		if (!sphere->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump)) { continue; }

		// 判定をtrueにする
		bHit = true;
	}

	// キューブ判定
	for (auto cube : m_cube)
	{
		// ヒット処理
		if (!cube->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump)) { continue; }

		// 判定をtrueにする
		bHit = true;
	}

	// シリンダー判定
	for (auto cylinder : m_cylinder)
	{
		// ヒット処理
		if (!cylinder->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump)) { continue; }

		// 判定をtrueにする
		bHit = true;
	}

	// ポリゴン判定
	for (auto polygon : m_polygon)
	{
		// ヒット処理
		if (!polygon->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump)) { continue; }

		// 判定をtrueにする
		bHit = true;
	}
}

//============================================================
// 当たり判定の消去処理
//============================================================
void CActor::ClearCollision(void)
{
	for (auto cube : m_cube)
	{
		// 終了処理
		cube->Uninit();
	}

	for (auto cylinder : m_cylinder)
	{
		// 終了処理
		cylinder->Uninit();
	}

	for (auto sphere : m_sphere)
	{
		// 終了処理
		sphere->Uninit();
	}

	for (auto polygon : m_polygon)
	{
		// 終了処理
		polygon->Uninit();
	}

	// クリア処理
	m_cube.clear();
	m_cylinder.clear();
	m_sphere.clear();
	m_polygon.clear();
}

//============================================================
// マトリックス計算処理
//============================================================
D3DXMATRIX CActor::CalcMatrix(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX   mtx;

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, GetVec3Scaling().x, GetVec3Scaling().y, GetVec3Scaling().z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetVec3Rotation().y, GetVec3Rotation().x, GetVec3Rotation().z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetVec3Position().x, GetVec3Position().y, GetVec3Position().z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// マトリックスの計算結果を返す
	return mtx;
}

//============================================================
// 当たり判定の割り当て処理
//============================================================
void CActor::BindCollision(void)
{
	// 当たり判定を取得
	CCollManager::SCollision coll = CScene::GetCollManager()->GetCollInfo(m_type);

	for (int nCnt = 0; nCnt < static_cast<int>(coll.m_cube.size()); nCnt++)
	{
		// キューブの情報を追加する
		m_cube.push_back(CCollisionCube::Create
		(
			GetVec3Position(),				// 位置
			coll.m_cube[nCnt].offset,		// オフセット座標
			coll.m_cube[nCnt].fWidth * GetVec3Scaling().x,		// 幅
			coll.m_cube[nCnt].fHeight * GetVec3Scaling().y,		// 高さ
			coll.m_cube[nCnt].fDepth * GetVec3Scaling().z,		// 奥行
			GetVec3Rotation().y				// 向き
		));
	}

	for (int nCnt = 0; nCnt < static_cast<int>(coll.m_cylinder.size()); nCnt++)
	{
		// シリンダーの情報を追加する
		m_cylinder.push_back(CCollisionCylinder::Create
		(
			GetVec3Position(),				// 位置
			coll.m_cylinder[nCnt].offset,	// オフセット座標
			coll.m_cylinder[nCnt].fRadius * GetVec3Scaling().x,	// 半径
			coll.m_cylinder[nCnt].fHeight * GetVec3Scaling().y	// 高さ
		));
	}

	for (int nCnt = 0; nCnt < static_cast<int>(coll.m_sphere.size()); nCnt++)
	{
		// スフィアの情報を追加する
		m_sphere.push_back(CCollisionSphere::Create
		(
			GetVec3Position(),				// 位置
			coll.m_sphere[nCnt].offset,		// オフセット座標
			coll.m_sphere[nCnt].fRadius * GetVec3Scaling().x		// 半径
		));
	}

	for (int nCnt = 0; nCnt < static_cast<int>(coll.m_polygon.size()); nCnt++)
	{
		// ポリゴンの情報を追加する
		m_polygon.push_back(CCollisionPolygon::Create
		(
			GetVec3Position(),				// 位置
			coll.m_polygon[nCnt].offset,	// オフセット座標
			D3DXVECTOR3(coll.m_polygon[nCnt].rot.x,GetVec3Rotation().y, coll.m_polygon[nCnt].rot.z),		// 向き
			D3DXVECTOR3(coll.m_polygon[nCnt].size.x * GetVec3Scaling().x, 0.0f, coll.m_polygon[nCnt].size.z * GetVec3Scaling().z)		// サイズ
		));
	}

	// 当たり判定の初期化処理
	CollisionOffset();
}

//============================================================
// 当たり判定のオフセット処理
//============================================================
void CActor::CollisionOffset(void)
{
	// マトリックスの計算結果を取得
	D3DXMATRIX mtx = CalcMatrix();

	for (auto cube : m_cube)
	{
		// 位置反映
		cube->OffSet(mtx);
	}

	for (auto cylinder : m_cylinder)
	{
		// 位置反映
		cylinder->OffSet(mtx);
	}

	for (auto sphere : m_sphere)
	{
		// 位置反映
		sphere->OffSet(mtx);
	}

	for (auto polygon : m_polygon)
	{
		// 位置反映
		polygon->OffSet(mtx);
	}
}

//============================================================
//	セットアップ処理
//============================================================
HRESULT CActor::LoadSetup(const char* pPass)
{
	int nType = 0;					// 種類の代入用
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	D3DXVECTOR3 scale = VEC3_ONE;	// 拡大率の代入用

	// ファイルを開く
	std::ifstream file(pPass);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "アクターセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// ファイルを読込
	std::string str;	// 読込文字列
	while (file >> str)
	{ // ファイルの終端ではない場合ループ

		if (str.front() == '#')
		{ // コメントアウトされている場合

			// 一行全て読み込む
			std::getline(file, str);
		}
		else if (str == "STAGE_ACTORSET")
		{
			do
			{ // END_STAGE_ACTORSETを読み込むまでループ

				// 文字列を読み込む
				file >> str;

				if (str.front() == '#')
				{ // コメントアウトされている場合

					// 一行全て読み込む
					std::getline(file, str);
				}
				else if (str == "ACTORSET")
				{
					do
					{ // END_ACTORSETを読み込むまでループ

						// 文字列を読み込む
						file >> str;

						if (str == "TYPE")
						{
							file >> str;	// ＝を読込

							// 種類を読込
							file >> nType;
						}
						else if (str == "POS")
						{
							file >> str;	// ＝を読込

							// 位置を読込
							file >> pos.x;
							file >> pos.y;
							file >> pos.z;
						}
						else if (str == "ROT")
						{
							file >> str;	// ＝を読込

							// 向きを読込
							file >> rot.x;
							file >> rot.y;
							file >> rot.z;
						}
						else if (str == "SCALE")
						{
							file >> str;	// ＝を読込

							// 拡大率を読込
							file >> scale.x;
							file >> scale.y;
							file >> scale.z;
						}
					} while (str != "END_ACTORSET");	// END_ACTORSETを読み込むまでループ

					// アクターオブジェクトの生成
					if (CActor::Create((EType)nType, pos, D3DXToRadian(rot), scale) == nullptr)
					{ // 確保に失敗した場合

						// 失敗を返す
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_ACTORSET");	// END_STAGE_ACTORSETを読み込むまでループ
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}
