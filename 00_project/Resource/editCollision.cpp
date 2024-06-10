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

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_CREATE			(DIK_0)	// 生成キー
#define NAME_CREATE			("0")	// 生成表示
#define KEY_RELEASE			(DIK_9)	// 破棄キー
#define NAME_RELEASE		("9")	// 破棄表示
#define KEY_CHANGE_OBJECT	(DIK_2)	// オブジェクトタイプ変更キー
#define NAME_CHANGE_OBJECT	("2")	// オブジェクトタイプ変更表示

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

	// 配列を全消去する
	m_cube.clear();
	m_cylinder.clear();
	m_sphere.clear();

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
	file << "# この行から下をコピーし [stage.txt] に張り付け\n" << std::endl;

	// 小数点書き出しの方法を指定
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// 読み込み開始文字列を書き出し
	file << "STAGE_COLLISIONSET\n" << std::endl;

	// アクターの種類を書き出し
	file << "ACTOR_TYPE = " << m_actorType << std::endl;

//============================================================
//	キューブの当たり判定
//============================================================

	// 読み込み開始文字列を書き出し
	file << "CUBE_COLLISIONSET" << std::endl;

	// 当たり判定の数を書き出す
	file << "\nCUBE_NUM	= " << m_cube.size() << "\n" << std::endl;

	for (const auto& rCube : m_cube)
	{ // キューブの総数分繰り返す

		// 情報を書き出し
		file << "	COLLISIONSET" << std::endl;
		file << "		OFFSET	= " << rCube.GetVec3OffSet().x << " " << rCube.GetVec3OffSet().y << " " << rCube.GetVec3OffSet().z << std::endl;
		file << "		WIDTH	= " << rCube.GetCube()->GetWidth() << std::endl;
		file << "		HEIGHT	= " << rCube.GetCube()->GetHeight() << std::endl;
		file << "		DEPTH	= " << rCube.GetCube()->GetDepth() << std::endl;
		file << "	END_COLLISIONSET\n" << std::endl;
	}

	// 読み込み終了文字列を書き出し
	file << "END_CUBE_COLLISIONSET\n" << std::endl;

//============================================================
//	シリンダーの当たり判定
//============================================================

	// 読み込み開始文字列を書き出し
	file << "CYLINDER_COLLISIONSET" << std::endl;

	// 当たり判定の数を書き出す
	file << "\nCYLINDER_NUM	= " << m_cylinder.size() << "\n" << std::endl;

	for (const auto& rCylinder : m_cylinder)
	{ // シリンダーの総数分繰り返す

		// 情報を書き出し
		file << "	COLLISIONSET" << std::endl;
		file << "		OFFSET	= " << rCylinder.GetVec3OffSet().x << " " << rCylinder.GetVec3OffSet().y << " " << rCylinder.GetVec3OffSet().z << std::endl;
		file << "		RADIUS	= " << rCylinder.GetCylinder()->GetRadius() << std::endl;
		file << "		HEIGHT	= " << rCylinder.GetCylinder()->GetHeight() << std::endl;
		file << "	END_COLLISIONSET\n" << std::endl;
	}

	// 読み込み終了文字列を書き出し
	file << "END_CYLINDER_COLLISIONSET\n" << std::endl;

//============================================================
//	スフィアの当たり判定
//============================================================

	// 読み込み開始文字列を書き出し
	file << "SPHERE_COLLISIONSET" << std::endl;

	// 当たり判定の数を書き出す
	file << "\nSPHERE_NUM	= " << m_sphere.size() << "\n" << std::endl;

	for (const auto& rSphere : m_sphere)
	{ // スフィアの総数分繰り返す

		// 情報を書き出し
		file << "	COLLISIONSET" << std::endl;
		file << "		OFFSET	= " << rSphere.GetVec3OffSet().x << " " << rSphere.GetVec3OffSet().y << " " << rSphere.GetVec3OffSet().z << std::endl;
		file << "		RADIUS	= " << rSphere.GetSphere()->GetRadius() << std::endl;
		file << "	END_COLLISIONSET\n" << std::endl;
	}

	// 読み込み終了文字列を書き出し
	file << "END_SPHERE_COLLISIONSET\n" << std::endl;

	// 読み込み終了文字列を書き出し
	file << "END_STAGE_COLLISIONSET" << std::endl;

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

	DebugProc::Print(DebugProc::POINT_RIGHT, "エディットステージタイプ変更：[%s]\n", NAME_CHANGE_OBJECT);

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

				if (pObject->GetLabel() != CObject::LABEL_DEBUG &&
					pObject->GetLabel() != CObject::LABEL_FIELD)
				{ // デバッグラベル、フィールドラベル以外

					// オブジェクトを見えなくする
					pObject->SetEnableDebugDisp(bDisp);
				}

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
	if (pKeyboard->IsTrigger(KEY_CHANGE_OBJECT))
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
// 生成処理
//============================================================
void CEditCollision::Create(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_CREATE))
	{ // 生成キーを押した場合

		CEditCollCube* pCube = nullptr;
		CEditCollCylinder* pCylinder = nullptr;
		CEditCollSphere* pSphere = nullptr;

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
			{ // キューブが NULL じゃない場合

				// 追加処理
				m_cylinder.push_back(*pCylinder);
			}

			break;
		case CEditCollision::TYPE_SPHERE:

			pSphere = dynamic_cast<CEditCollSphere*>(m_pEditor);

			if (pSphere != nullptr)
			{ // キューブが NULL じゃない場合

				// 追加処理
				m_sphere.push_back(*pSphere);
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

	default:		// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	コリジョンキューブの色の全初期化処理
//============================================================
void CEditCollision::InitAllColorCollCube(void)
{
	for (auto& rCube : m_cube)
	{ // コリジョンキューブ数分繰り返す

#ifdef _DEBUG

		// 通常色を設定
		rCube.GetCube()->GetCube()->SetCubeColor(COL);

#endif // _DEBUG
	}
}

//============================================================
// シリンダーの色全初期化
//============================================================
void CEditCollision::InitAllColorCollCylinder(void)
{
	for (auto& rCylinder : m_cylinder)
	{ // コリジョンシリンダー数分繰り返す

#ifdef _DEBUG

		// 通常色を設定
		rCylinder.GetCylinder()->GetTube()->SetColor(COL);

#endif // _DEBUG
	}
}

//============================================================
// スフィアの色全初期化
//============================================================
void CEditCollision::InitAllColorCollSphere(void)
{
	for (auto& rSphere : m_sphere)
	{ // コリジョンスフィア数分繰り返す

#ifdef _DEBUG

		// 通常色を設定
		rSphere.GetSphere()->GetSphere()->SetColor(COL);

#endif // _DEBUG
	}
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
