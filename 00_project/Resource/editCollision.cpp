//============================================================
//
//	エディット当たり判定処理 [editCollision.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "editCollision.h"
#include "actor.h"
#include "editManager.h"
#include "collision.h"
#include "effect3D.h"

#include "editCollCube.h"
#include "editCollCylinder.h"
#include "editCollSphere.h"
#include "editCollPolygon.h"

#include "gameManager.h"
#include "sceneGame.h"
#include "collManager.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_CREATE					(DIK_0)	// 生成キー
#define NAME_CREATE					("0")	// 生成表示
#define KEY_RELEASE					(DIK_9)	// 破棄キー
#define NAME_RELEASE				("9")	// 破棄表示
#define KEY_CHANGE_OBJECT_FRONT		(DIK_2)	// オブジェクトタイプ前進変更キー
#define NAME_CHANGE_OBJECT_FRONT	("2")	// オブジェクトタイプ前進変更表示
#define KEY_CHANGE_OBJECT_BACK		(DIK_3)	// オブジェクトタイプ後進変更キー
#define NAME_CHANGE_OBJECT_BACK		("3")	// オブジェクトタイプ後進変更表示
#define KEY_CHANGE_ACTOR			(DIK_4)	// アクターの種類変更キー
#define NAME_CHANGE_ACTOR			("4")	// アクターの種類変更表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_collision.txt";	// セーブテキストパス

	const char* TYPE_NAME[] =	// オブジェクトタイプ名
	{
		"キューブ",			// キューブ
		"シリンダー",		// シリンダー
		"スフィア",			// スフィア
		"ポリゴン",			// ポリゴン
	};
	const int DIGIT_FLOAT = 2;			// 小数点以下の桁数
	const float DELETE_RANGE = 30.0f;	// 消去する範囲
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);			// 色
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TYPE_NAME) == CEditCollision::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	親クラス [CEditCollision] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditCollision::CEditCollision(CEditManager* pEditManager) : CEditor(pEditManager)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pEditor = nullptr;				// エディター情報
	m_pActor = nullptr;					// アクターの情報
	m_type = TYPE_CUBE;					// オブジェクトタイプ
	m_actorType = CActor::TYPE_ROCK_S;	// アクタータイプ
	m_cube = {};						// キューブの可変長配列
	m_cylinder = {};					// シリンダーの可変長配列
	m_sphere = {};						// スフィアの可変長配列
	m_polygon = {};						// ポリゴンの可変長配列

	// オブジェクトの描画関係処理
	DispObject(false);

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditCollision::~CEditCollision()
{
#if _DEBUG

	// オブジェクトの描画関係処理
	DispObject(true);

#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditCollision::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pEditor = nullptr;				// エディター情報
	m_pActor = nullptr;					// アクターの情報
	m_type = TYPE_CUBE;					// オブジェクトタイプ
	m_actorType = CActor::TYPE_ROCK_S;	// アクタータイプ
	m_cube = {};						// キューブの可変長配列
	m_cylinder = {};					// シリンダーの可変長配列
	m_sphere = {};						// スフィアの可変長配列
	m_polygon = {};						// ポリゴンの可変長配列

	// エディター情報の生成
	m_pEditor = CEditorCollShape::Create(m_type, 0);
	if (m_pEditor == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// アクターの生成
	m_pActor = CActor::Create(m_actorType, VEC3_ZERO, VEC3_ZERO, VEC3_ONE);
	m_pActor->ClearCollision();

	// 成功を返す
	return S_OK;

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	終了処理
//============================================================
void CEditCollision::Uninit(void)
{
#if _DEBUG

	for (auto& rCube : m_cube)
	{ // コリジョンキューブ数分繰り返す

		// 終了処理
		rCube.Uninit();
	}

	for (auto& rCylinder : m_cylinder)
	{ // コリジョンシリンダー数分繰り返す

		// 終了処理
		rCylinder.Uninit();
	}

	for (auto& rSphere : m_sphere)
	{ // コリジョンスフィア数分繰り返す

		// 終了処理
		rSphere.Uninit();
	}

	for (auto& rPolygon : m_polygon)
	{ // コリジョンポリゴン数分繰り返す

		// 終了処理
		rPolygon.Uninit();
	}

	// 配列を全消去する
	m_cube.clear();
	m_cylinder.clear();
	m_sphere.clear();
	m_polygon.clear();

	// エディター情報の破棄
	SAFE_REF_RELEASE(m_pEditor);

	// アクターの破棄
	SAFE_UNINIT(m_pActor);

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditCollision::Update(void)
{
#if _DEBUG

	// オブジェクトタイプの変更
	ChangeObjectType();

	// アクターの種類変更
	ChangeActorType();

	// エディター情報の更新
	assert(m_pEditor != nullptr);
	m_pEditor->Update();

	// 生成処理
	Create();

	// 消去処理
	Delete();

#endif	// _DEBUG
}

//============================================================
//	保存処理
//============================================================
HRESULT CEditCollision::Save(void)
{
#if _DEBUG

	// マネージャーのセーブ処理
	ManagerSave();

	// ファイルを開く
	std::ofstream  file(SAVE_PASS);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "当たり判定配置の書き出しに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
}

	// 見出しを書き出し
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	当たり判定配置のセーブデータ [save_collision.txt]" << std::endl;
	file << "#	Author : 藤田 勇一" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# この行から下をコピーし [Collision.txt] に張り付け\n" << std::endl;

	// 小数点書き出しの方法を指定
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// 当たり判定の情報
	CCollManager::SCollision coll = {};

	for (int nCntSave = 0; nCntSave < CActor::TYPE_MAX; nCntSave++)
	{
		// 当たり判定の情報を取得する
		coll = CSceneGame::GetCollManager()->GetCollInfo((CActor::EType)nCntSave);

		// 読み込み開始文字列を書き出し
		file << "#=================================================================================" << std::endl;
		file << "ACTOR_TYPE = " << nCntSave << std::endl;

		// 読み込み開始文字列を書き出し
		file << "STAGE_COLLSET\n" << std::endl;

//============================================================
//	キューブの当たり判定
//============================================================

		// 読み込み開始文字列を書き出し
		file << "CUBESET" << std::endl;

		// 当たり判定の数を書き出す
		file << "	NUM	= " << coll.m_cube.size() << "\n" << std::endl;

		for (const auto& rCube : coll.m_cube)
		{ // キューブの総数分繰り返す

			// 情報を書き出し
			file << "	COLLSET" << std::endl;
			file << "		OFFSET	= " << rCube.offset.x << " " << rCube.offset.y << " " << rCube.offset.z << std::endl;
			file << "		WIDTH	= " << rCube.fWidth << std::endl;
			file << "		HEIGHT	= " << rCube.fHeight << std::endl;
			file << "		DEPTH	= " << rCube.fDepth << std::endl;
			file << "	END_COLLSET" << std::endl;
		}

		// 読み込み終了文字列を書き出し
		file << "END_CUBESET\n" << std::endl;

//============================================================
//	シリンダーの当たり判定
//============================================================

		// 読み込み開始文字列を書き出し
		file << "CYLINDERSET" << std::endl;

		// 当たり判定の数を書き出す
		file << "	NUM	= " << coll.m_cylinder.size() << std::endl;

		for (const auto& rCylinder : coll.m_cylinder)
		{ // シリンダーの総数分繰り返す

			// 情報を書き出し
			file << "	COLLSET" << std::endl;
			file << "		OFFSET	= " << rCylinder.offset.x << " " << rCylinder.offset.y << " " << rCylinder.offset.z << std::endl;
			file << "		RADIUS	= " << rCylinder.fRadius << std::endl;
			file << "		HEIGHT	= " << rCylinder.fHeight << std::endl;
			file << "	END_COLLSET" << std::endl;
		}

		// 読み込み終了文字列を書き出し
		file << "END_CYLINDERSET\n" << std::endl;

//============================================================
//	スフィアの当たり判定
//============================================================

		// 読み込み開始文字列を書き出し
		file << "SPHERESET" << std::endl;

		// 当たり判定の数を書き出す
		file << "	NUM	= " << coll.m_sphere.size() << std::endl;

		for (const auto& rSphere : coll.m_sphere)
		{ // スフィアの総数分繰り返す

			// 情報を書き出し
			file << "	COLLSET" << std::endl;
			file << "		OFFSET	= " << rSphere.offset.x << " " << rSphere.offset.y << " " << rSphere.offset.z << std::endl;
			file << "		RADIUS	= " << rSphere.fRadius << std::endl;
			file << "	END_COLLSET" << std::endl;
		}

		// 読み込み終了文字列を書き出し
		file << "END_SPHERESET\n" << std::endl;

//============================================================
//	ポリゴンの当たり判定
//============================================================

		// 読み込み開始文字列を書き出し
		file << "POLYGONSET" << std::endl;

		// 当たり判定の数を書き出す
		file << "	NUM	= " << coll.m_polygon.size() << std::endl;

		for (const auto& rPolygon : coll.m_polygon)
		{ // ポリゴンの総数分繰り返す

			// 情報を書き出し
			file << "	COLLSET" << std::endl;
			file << "		OFFSET	= " << rPolygon.offset.x << " " << rPolygon.offset.y << " " << rPolygon.offset.z << std::endl;
			file << "		ROT	= "		<< rPolygon.rot.x << " " << rPolygon.rot.y << " " << rPolygon.rot.z << std::endl;
			file << "		WIDTH	= " << rPolygon.size.x << std::endl;
			file << "		DEPTH	= " << rPolygon.size.z << std::endl;
			file << "	END_COLLSET" << std::endl;
		}

		// 読み込み終了文字列を書き出し
		file << "END_POLYGONSET\n" << std::endl;

		// 読み込み終了文字列を書き出し
		file << "END_STAGE_COLLSET" << std::endl;
		file << "#=================================================================================\n\n" << std::endl;
	}

	// 成功を返す
	return S_OK;

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	保存状況取得処理
//============================================================
bool CEditCollision::IsSave(void)
{
#if _DEBUG

	// エディター情報の保存
	assert(m_pEditor != nullptr);
	return m_pEditor->IsSave();

#else	// NDEBUG

	// falseを返す
	return false;

#endif	// _DEBUG
}

//============================================================
//	情報保存処理
//============================================================
void CEditCollision::SaveInfo(void)
{
#if _DEBUG

	// エディター情報の情報保存
	assert(m_pEditor != nullptr);
	m_pEditor->SaveInfo();

#endif	// _DEBUG
}

//============================================================
//	情報読込処理
//============================================================
void CEditCollision::LoadInfo(void)
{
#if _DEBUG

	// エディター情報の情報読込
	assert(m_pEditor != nullptr);
	m_pEditor->LoadInfo();

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditCollision::DrawDebugControl(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "エディットステージタイプ前進変更：[%s]\nエディットステージタイプ後進変更：[%s]\n", NAME_CHANGE_OBJECT_FRONT, NAME_CHANGE_OBJECT_BACK);

	// エディター情報の操作表示
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugControl();

#endif	// _DEBUG
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditCollision::DrawDebugInfo(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "%s：[エディットコリジョンタイプ]\n", TYPE_NAME[m_type]);

	// エディター情報の情報表示
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugInfo();

#endif	// _DEBUG
}

//============================================================
// オブジェクトの描画関係処理
//============================================================
void CEditCollision::DispObject(const bool bDisp)
{
	for (int nCntDim = 0; nCntDim < CObject::DIM_MAX; nCntDim++)
	{ // 次元の総数分繰り返す

		for (int nCntPri = 0; nCntPri < object::MAX_PRIO; nCntPri++)
		{ // 優先順位の総数分繰り返す

			// オブジェクトの先頭を代入
			CObject* pObject = CObject::GetTop(static_cast<CObject::EDim>(nCntDim), nCntPri);

			while (pObject != nullptr)
			{ // オブジェクトが使用されている場合繰り返す

				// 次のオブジェクトを代入
				CObject* pObjectNext = pObject->GetNext();

				if (pObject->GetLabel() == CObject::LABEL_NONE)
				{ // 自動破棄しないラベルの場合

					// 次のオブジェクトへのポインタを代入
					pObject = pObjectNext;
					continue;
				}

				if (pObject->IsDeath())
				{ // 死亡している場合

					// 次のオブジェクトへのポインタを代入
					pObject = pObjectNext;
					continue;
				}

				if (pObject->GetLabel() == CObject::LABEL_PLAYER)
				{ // プレイヤーの場合

					// 更新状況を変化する
					pObject->SetEnableUpdate(bDisp);
				}

#ifdef _DEBUG

				if (pObject->GetLabel() != CObject::LABEL_DEBUG &&
					pObject->GetLabel() != CObject::LABEL_FIELD)
				{ // デバッグラベル、フィールドラベル以外

					// オブジェクトを見えなくする
					pObject->SetEnableDebugDisp(bDisp);
				}

#endif // _DEBUG

				// 次のオブジェクトへのポインタを代入
				pObject = pObjectNext;
			}
		}
	}
}

//============================================================
//	オブジェクトタイプの変更処理
//============================================================
void CEditCollision::ChangeObjectType(void)
{
	// オブジェクトタイプの変更
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
	if (pKeyboard->IsTrigger(KEY_CHANGE_OBJECT_FRONT))
	{
		// エディター情報の破棄
		SAFE_REF_RELEASE(m_pEditor);

		// オブジェクトタイプの変更
		m_type = (CEditCollision::EType)((m_type + 1) % CEditCollision::TYPE_MAX);

		if (m_pEditor == nullptr)
		{ // エディターが NULL の場合

			// インデックス
			int nIdx = NONE_IDX;

			// インデックスを代入
			switch (m_type)
			{
			case CEditCollision::TYPE_CUBE:
				nIdx = m_cube.size();
				break;
			case CEditCollision::TYPE_CYLINDER:
				nIdx = m_cylinder.size();
				break;
			case CEditCollision::TYPE_SPHERE:
				nIdx = m_sphere.size();
				break;
			case CEditCollision::TYPE_POLYGON:
				nIdx = m_polygon.size();
				break;
			default:		// 例外処理
				assert(false);
				break;
			}

			// エディター情報の生成
			m_pEditor = CEditorCollShape::Create(m_type, nIdx);
			assert(m_pEditor != nullptr);	// 生成失敗
		}
	}
	else if (pKeyboard->IsTrigger(KEY_CHANGE_OBJECT_BACK))
	{
		// エディター情報の破棄
		SAFE_REF_RELEASE(m_pEditor);

		// オブジェクトタイプの変更
		m_type = (CEditCollision::EType)((m_type + (CEditCollision::TYPE_MAX - 1)) % CEditCollision::TYPE_MAX);

		if (m_pEditor == nullptr)
		{ // エディターが NULL の場合

			// インデックス
			int nIdx = NONE_IDX;

			// インデックスを代入
			switch (m_type)
			{
			case CEditCollision::TYPE_CUBE:
				nIdx = m_cube.size();
				break;
			case CEditCollision::TYPE_CYLINDER:
				nIdx = m_cylinder.size();
				break;
			case CEditCollision::TYPE_SPHERE:
				nIdx = m_sphere.size();
				break;
			case CEditCollision::TYPE_POLYGON:
				nIdx = m_polygon.size();
				break;
			default:		// 例外処理
				assert(false);
				break;
			}

			// エディター情報の生成
			m_pEditor = CEditorCollShape::Create(m_type, nIdx);
			assert(m_pEditor != nullptr);	// 生成失敗
		}
	}
}

//============================================================
//	アクター変更処理
//============================================================
void CEditCollision::ChangeActorType(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 種類を変更
	if (pKeyboard->IsTrigger(KEY_CHANGE_ACTOR))
	{
		m_actorType = (CActor::EType)((m_actorType + 1) % CActor::TYPE_MAX);

		// モデルを生成し直す
		m_pActor->Uninit();
		m_pActor = CActor::Create(m_actorType, VEC3_ZERO);
		m_pActor->ClearCollision();
		for (auto& rCube : m_cube)
		{ // コリジョンキューブ数分繰り返す

			// 終了処理
			rCube.Uninit();
		}
		for (auto& rCylinder : m_cylinder)
		{ // コリジョンシリンダー数分繰り返す

			// 終了処理
			rCylinder.Uninit();
		}
		for (auto& rSphere : m_sphere)
		{ // コリジョンスフィア数分繰り返す

			// 終了処理
			rSphere.Uninit();
		}
		for (auto& rPolygon : m_polygon)
		{ // コリジョンポリゴン数分繰り返す

			// 終了処理
			rPolygon.Uninit();
		}
		// 配列を全消去する
		m_cube.clear();
		m_cylinder.clear();
		m_sphere.clear();
		m_polygon.clear();
	}
}

//============================================================
// 生成処理
//============================================================
void CEditCollision::Create(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_CREATE))
	{ // 生成キーを押した場合

		CEditCollCube* pCube = nullptr;
		CEditCollCylinder* pCylinder = nullptr;
		CEditCollSphere* pSphere = nullptr;
		CEditCollPolygon* pPolygon = nullptr;

		switch (m_type)
		{
		case CEditCollision::TYPE_CUBE:

			pCube = dynamic_cast<CEditCollCube*>(m_pEditor);

			if (pCube != nullptr)
			{ // キューブが NULL じゃない場合

				// 追加処理
				m_cube.push_back(*pCube);
			}

			break;
		case CEditCollision::TYPE_CYLINDER:

			pCylinder = dynamic_cast<CEditCollCylinder*>(m_pEditor);

			if (pCylinder != nullptr)
			{ // シリンダーが NULL じゃない場合

				// 追加処理
				m_cylinder.push_back(*pCylinder);
			}

			break;
		case CEditCollision::TYPE_SPHERE:

			pSphere = dynamic_cast<CEditCollSphere*>(m_pEditor);

			if (pSphere != nullptr)
			{ // スフィアが NULL じゃない場合

				// 追加処理
				m_sphere.push_back(*pSphere);
			}

			break;
		case CEditCollision::TYPE_POLYGON:

			pPolygon = dynamic_cast<CEditCollPolygon*>(m_pEditor);

			if (pPolygon != nullptr)
			{ // ポリゴンが NULL じゃない場合

				// 追加処理
				m_polygon.push_back(*pPolygon);
			}

			break;
		default:		// 例外処理
			assert(false);
			break;
		}

		// 生成処理
		m_pEditor->Create();
	}
}

//============================================================
// 消去処理
//============================================================
void CEditCollision::Delete(void)
{
	bool bRelease = false;

	if (GET_INPUTKEY->IsTrigger(KEY_RELEASE))
	{ // 消去キーを押した場合

		// 消去判定を通す
		bRelease = true;
	}

	switch (m_type)
	{
	case CEditCollision::TYPE_CUBE:

		// キューブの消去処理
		DeleteCollCube(bRelease);

		break;

	case CEditCollision::TYPE_CYLINDER:

		// シリンダーの消去処理
		DeleteCollCylinder(bRelease);

		break;

	case CEditCollision::TYPE_SPHERE:

		// スフィアの消去処理
		DeleteCollSphere(bRelease);

		break;

	case CEditCollision::TYPE_POLYGON:

		// ポリゴンの消去処理
		DeleteCollPolygon(bRelease);

		break;

	default:		// 例外処理
		assert(false);
		break;
	}
}

//============================================================
// マネージャーのセーブ処理
//============================================================
void CEditCollision::ManagerSave(void)
{
	// ローカル変数宣言
	std::vector<CCollManager::SCollCube> saveCube;			// キューブの情報配列
	std::vector<CCollManager::SCollCylinder> saveCylinder;	// シリンダーの情報配列
	std::vector<CCollManager::SCollSphere> saveSphere;		// スフィアの情報配列
	std::vector<CCollManager::SCollPolygon> savePolygon;	// ポリゴンの情報配列
	CCollManager::SCollCube cube;				// キューブ
	CCollManager::SCollCylinder cylinder;		// シリンダー
	CCollManager::SCollSphere sphere;			// スフィア
	CCollManager::SCollPolygon polygon;			// ポリゴン

	for (const auto& rCube : m_cube)
	{
		// 情報を取得
		cube.offset = rCube.GetVec3OffSet();		// オフセット座標
		cube.fWidth = rCube.GetInfo().fWidth;		// 幅
		cube.fHeight = rCube.GetInfo().fHeight;		// 高さ
		cube.fDepth = rCube.GetInfo().fDepth;		// 奥行

		// 情報を設定
		saveCube.push_back(cube);
	}

	for (const auto& rCylinder : m_cylinder)
	{
		// 情報を取得
		cylinder.offset = rCylinder.GetVec3OffSet();		// オフセット座標
		cylinder.fRadius = rCylinder.GetInfo().fRadius;		// 半径
		cylinder.fHeight = rCylinder.GetInfo().fHeight;		// 高さ

		// 情報を設定
		saveCylinder.push_back(cylinder);
	}

	for (const auto& rSphere : m_sphere)
	{
		// 情報を取得
		sphere.offset = rSphere.GetVec3OffSet();		// オフセット座標
		sphere.fRadius = rSphere.GetInfo().fRadius;		// 半径

		// 情報を設定
		saveSphere.push_back(sphere);
	}

	for (const auto& rPolygon : m_polygon)
	{
		// 情報を取得
		polygon.offset = rPolygon.GetVec3OffSet();		// オフセット座標
		polygon.rot = rPolygon.GetInfo().rot;			// 向き
		polygon.size = rPolygon.GetInfo().size;			// サイズ

		// 情報を設定
		savePolygon.push_back(polygon);
	}

	// 当たり判定マネージャーに設定
	CSceneGame::GetCollManager()->SetCollInfo(m_actorType, CCollManager::SCollision(saveCube, saveCylinder, saveSphere, savePolygon));
}

//============================================================
//	コリジョンキューブの色の全初期化処理
//============================================================
void CEditCollision::InitAllColorCollCube(void)
{
#ifdef _DEBUG

	for (auto& rCube : m_cube)
	{ // コリジョンキューブ数分繰り返す

		// 通常色を設定
		rCube.GetCube()->GetCube()->SetCubeColor(COL);
	}

#endif // _DEBUG
}

//============================================================
// シリンダーの色全初期化
//============================================================
void CEditCollision::InitAllColorCollCylinder(void)
{
#ifdef _DEBUG

	for (auto& rCylinder : m_cylinder)
	{ // コリジョンシリンダー数分繰り返す

		// 通常色を設定
		rCylinder.GetCylinder()->GetTube()->SetColor(COL);
	}

#endif // _DEBUG
}

//============================================================
// スフィアの色全初期化
//============================================================
void CEditCollision::InitAllColorCollSphere(void)
{
#ifdef _DEBUG

	for (auto& rSphere : m_sphere)
	{ // コリジョンスフィア数分繰り返す

		// 通常色を設定
		rSphere.GetSphere()->GetSphere()->SetColor(COL);
	}

#endif // _DEBUG
}

//============================================================
// ポリゴンの色全初期化
//============================================================
void CEditCollision::InitAllColorCollPolygon(void)
{
#ifdef _DEBUG

	for (auto& rPolygon : m_polygon)
	{ // コリジョンスフィア数分繰り返す

		// 通常色を設定
		rPolygon.GetPolygon()->GetPolygon()->SetColor(COL);
	}

#endif // _DEBUG
}

//============================================================
// キューブの消去処理
//============================================================
void CEditCollision::DeleteCollCube(const bool bRelease)
{
	D3DXVECTOR3 posEdit = m_pEditor->GetVec3OffSet();	// エディットの位置
	CEffect3D::Create(posEdit, DELETE_RANGE);			// エフェクトを生成

	for (auto cube = m_cube.begin(); cube != m_cube.end(); cube++)
	{ // フィールド数分繰り返す

		D3DXVECTOR3 posOther = cube->GetVec3OffSet();	// 対象の地面位置
		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// 自身の大きさ
		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// 対象の大きさ

		// 自身の大きさを設定
		sizeThis.x = DELETE_RANGE;		// 判定サイズXを設定
		sizeThis.y = DELETE_RANGE;		// 判定サイズYを設定
		sizeThis.z = DELETE_RANGE;		// 判定サイズZを設定
		sizeThis *= 0.5f;				// 判定サイズを半分に

		// 対象の大きさを設定
		sizeOther.x = cube->GetCube()->GetWidth();	// 判定サイズXを設定
		sizeOther.y = cube->GetCube()->GetHeight();	// 判定サイズYを設定
		sizeOther.z = cube->GetCube()->GetDepth();	// 判定サイズZを設定
		sizeOther *= 0.5f;				// 判定サイズを半分に

		// 矩形の当たり判定
		if (collision::Box3D
		( // 引数
			posEdit,	// 判定位置
			posOther,	// 判定目標位置
			sizeThis,	// 判定サイズ(右・上・後)
			sizeThis,	// 判定サイズ(左・下・前)
			sizeOther,	// 判定目標サイズ(右・上・後)
			sizeOther	// 判定目標サイズ(左・下・前)
		))
		{ // 判定内だった場合

			if (bRelease)
			{ // 破棄する場合

				// 終了処理
				cube->Uninit();

				// 消去処理
				m_cube.erase(cube);

				// 抜け出す
				break;
			}
			else
			{ // 破棄しない場合

#ifdef _DEBUG

				// 青を設定
				cube->GetCube()->GetCube()->SetCubeColor(XCOL_BLUE);

#endif // _DEBUG

				// 抜け出す
				break;
			}
		}
		else
		{ // 判定外だった場合

#ifdef _DEBUG

			// 通常色を設定
			cube->GetCube()->GetCube()->SetCubeColor(COL);

#endif // _DEBUG
		}
	}
}

//============================================================
// シリンダーの消去処理
//============================================================
void CEditCollision::DeleteCollCylinder(const bool bRelease)
{
	D3DXVECTOR3 posEdit = m_pEditor->GetVec3OffSet();	// エディットの位置
	CEffect3D::Create(posEdit, DELETE_RANGE);			// エフェクトを生成

	for (auto cylinder = m_cylinder.begin(); cylinder != m_cylinder.end(); cylinder++)
	{ // フィールド数分繰り返す

		D3DXVECTOR3 posOther = cylinder->GetVec3OffSet();	// 対象の地面位置
		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// 自身の大きさ
		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// 対象の大きさ

		// 自身の大きさを設定
		sizeThis.x = DELETE_RANGE;		// 判定サイズXを設定
		sizeThis.y = DELETE_RANGE;		// 判定サイズYを設定
		sizeThis.z = DELETE_RANGE;		// 判定サイズZを設定
		sizeThis *= 0.5f;				// 判定サイズを半分に

		// 対象の大きさを設定
		sizeOther.x = cylinder->GetCylinder()->GetRadius();	// 判定サイズXを設定
		sizeOther.y = cylinder->GetCylinder()->GetHeight();	// 判定サイズYを設定
		sizeOther.z = cylinder->GetCylinder()->GetRadius();	// 判定サイズZを設定
		sizeOther *= 0.5f;				// 判定サイズを半分に

		// 矩形の当たり判定
		if (collision::Box3D
		( // 引数
			posEdit,	// 判定位置
			posOther,	// 判定目標位置
			sizeThis,	// 判定サイズ(右・上・後)
			sizeThis,	// 判定サイズ(左・下・前)
			sizeOther,	// 判定目標サイズ(右・上・後)
			sizeOther	// 判定目標サイズ(左・下・前)
		))
		{ // 判定内だった場合

			if (bRelease)
			{ // 破棄する場合

				// 終了処理
				cylinder->Uninit();

				// 消去処理
				m_cylinder.erase(cylinder);

				// 抜け出す
				break;
			}
			else
			{ // 破棄しない場合

#ifdef _DEBUG

				// 青を設定
				cylinder->GetCylinder()->GetTube()->SetColor(XCOL_BLUE);

#endif // _DEBUG

				// 抜け出す
				break;
			}
		}
		else
		{ // 判定外だった場合

#ifdef _DEBUG

			// 通常色を設定
			cylinder->GetCylinder()->GetTube()->SetColor(COL);

#endif // _DEBUG
		}
	}
}

//============================================================
// スフィアの消去処理
//============================================================
void CEditCollision::DeleteCollSphere(const bool bRelease)
{
	D3DXVECTOR3 posEdit = m_pEditor->GetVec3OffSet();	// エディットの位置
	CEffect3D::Create(posEdit, DELETE_RANGE);			// エフェクトを生成

	for (auto sphere = m_sphere.begin(); sphere != m_sphere.end(); sphere++)
	{ // フィールド数分繰り返す

		D3DXVECTOR3 posOther = sphere->GetVec3OffSet();	// 対象の地面位置
		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// 自身の大きさ
		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// 対象の大きさ

		// 自身の大きさを設定
		sizeThis.x = DELETE_RANGE;		// 判定サイズXを設定
		sizeThis.y = DELETE_RANGE;		// 判定サイズYを設定
		sizeThis.z = DELETE_RANGE;		// 判定サイズZを設定
		sizeThis *= 0.5f;				// 判定サイズを半分に

		// 対象の大きさを設定
		sizeOther.x = sphere->GetSphere()->GetRadius();	// 判定サイズXを設定
		sizeOther.y = sphere->GetSphere()->GetRadius();	// 判定サイズYを設定
		sizeOther.z = sphere->GetSphere()->GetRadius();	// 判定サイズZを設定
		sizeOther *= 0.5f;				// 判定サイズを半分に

		// 矩形の当たり判定
		if (collision::Box3D
		( // 引数
			posEdit,	// 判定位置
			posOther,	// 判定目標位置
			sizeThis,	// 判定サイズ(右・上・後)
			sizeThis,	// 判定サイズ(左・下・前)
			sizeOther,	// 判定目標サイズ(右・上・後)
			sizeOther	// 判定目標サイズ(左・下・前)
		))
		{ // 判定内だった場合

			if (bRelease)
			{ // 破棄する場合

				// 終了処理
				sphere->Uninit();

				// 消去処理
				m_sphere.erase(sphere);

				// 抜け出す
				break;
			}
			else
			{ // 破棄しない場合

#ifdef _DEBUG

				// 青を設定
				sphere->GetSphere()->GetSphere()->SetColor(XCOL_BLUE);

#endif // _DEBUG

				// 抜け出す
				break;
			}
		}
		else
		{ // 判定外だった場合

#ifdef _DEBUG

			// 通常色を設定
			sphere->GetSphere()->GetSphere()->SetColor(COL);

#endif // _DEBUG
		}
	}
}

//============================================================
// ポリゴンの消去処理
//============================================================
void CEditCollision::DeleteCollPolygon(const bool bRelease)
{
	D3DXVECTOR3 posEdit = m_pEditor->GetVec3OffSet();	// エディットの位置
	CEffect3D::Create(posEdit, DELETE_RANGE);			// エフェクトを生成

	for (auto polygon = m_polygon.begin(); polygon != m_polygon.end(); polygon++)
	{ // フィールド数分繰り返す

		D3DXVECTOR3 posOther = polygon->GetVec3OffSet();	// 対象の地面位置
		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// 自身の大きさ
		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// 対象の大きさ

		// 自身の大きさを設定
		sizeThis.x = DELETE_RANGE;		// 判定サイズXを設定
		sizeThis.y = DELETE_RANGE;		// 判定サイズYを設定
		sizeThis.z = DELETE_RANGE;		// 判定サイズZを設定
		sizeThis *= 0.5f;				// 判定サイズを半分に

		// 対象の大きさを設定
		sizeOther = polygon->GetInfo().size * 0.5f;	// 判定サイズを設定

		// 矩形の当たり判定
		if (collision::Box3D
		( // 引数
			posEdit,	// 判定位置
			posOther,	// 判定目標位置
			sizeThis,	// 判定サイズ(右・上・後)
			sizeThis,	// 判定サイズ(左・下・前)
			sizeOther,	// 判定目標サイズ(右・上・後)
			sizeOther	// 判定目標サイズ(左・下・前)
		))
		{ // 判定内だった場合

			if (bRelease)
			{ // 破棄する場合

				// 終了処理
				polygon->Uninit();

				// 消去処理
				m_polygon.erase(polygon);

				// 抜け出す
				break;
			}
			else
			{ // 破棄しない場合

#ifdef _DEBUG

				// 青を設定
				polygon->GetPolygon()->GetPolygon()->SetColor(XCOL_BLUE);

#endif // _DEBUG

				// 抜け出す
				break;
			}
		}
		else
		{ // 判定外だった場合

#ifdef _DEBUG

			// 通常色を設定
			polygon->GetPolygon()->GetPolygon()->SetColor(COL);

#endif // _DEBUG
		}
	}
}
