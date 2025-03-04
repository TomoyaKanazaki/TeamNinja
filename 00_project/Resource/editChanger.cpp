//===========================================
//
//  カメラ情報変更地点エディター(editChanger.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "editChanger.h"
#include "editManager.h"
#include "manager.h"
#include "collision.h"
#include "stage.h"

//===========================================
//  定数宣言
//===========================================
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_changer.txt"; // セーブテキストパス

	const D3DXVECTOR3 INIT_SIZE = D3DXVECTOR3(editstage::SIZE, editstage::SIZE, editstage::SIZE); // 大きさ
	const float	INIT_ALPHA = 0.5f; // 配置前のα値
	const int DIGIT_FLOAT = 2; // 小数点以下の桁数

	const int KEY_CREATE		= DIK_0; // 生成キー
	const int KEY_RELEASE		= DIK_9; // 破棄キー
	const int KEY_DIRECTION		= DIK_4; // 方向変更キー
	const int KEY_ROTATION		= DIK_5; // 角度変更キー
	const int KEY_UP_SIZE_X		= DIK_T; // X軸拡大キー
	const int KEY_DOWN_SIZE_X	= DIK_G; // X軸縮小キー
	const int KEY_UP_SIZE_Y		= DIK_Y; // Y軸拡大キー
	const int KEY_DOWN_SIZE_Y	= DIK_H; // Y軸縮小キー
	const int KEY_UP_SIZE_Z		= DIK_U; // Z軸拡大キー
	const int KEY_DOWN_SIZE_Z	= DIK_J; // Z軸縮小キー

	const char* NAME_CREATE			= "0"; // 生成表示
	const char* NAME_RELEASE		= "9"; // 破棄表示
	const char* NAME_DIRECTION		= "4"; // 方向変更表示
	const char* NAME_ROTATION		= "5"; // 角度変更表示
	const char* NAME_UP_SIZE_X		= "T"; // X軸拡大表示
	const char* NAME_DOWN_SIZE_X	= "G"; // X軸縮小表示
	const char* NAME_UP_SIZE_Y		= "Y"; // Y軸拡大表示
	const char* NAME_DOWN_SIZE_Y	= "H"; // Y軸縮小表示
	const char* NAME_UP_SIZE_Z		= "U"; // Z軸拡大表示
	const char* NAME_DOWN_SIZE_Z	= "J"; // Z軸縮小表示

	const char* NAME_DIR[CCameraChanger::DIRECTION_MAX] = // 方向
	{
		"正面から", // 正面
		"後方から", // 後方
		"左から", // 左
		"右から", // 右
	};

	const char* NAME_ROT[CCameraChanger::ROTATION_MAX] = // 角度
	{
		"デフォルト", // デフォルト
		"上から", // 上
		"下から" // 下
	};
}

//===========================================
//	コンストラクタ
//===========================================
CEditChanger::CEditChanger(CEditStage* pEditor) : CEditorObject(pEditor),
m_pChanger(nullptr),
m_bSave(false)
{
	// メンバ変数をクリア
	memset(&m_infoCreate, 0, sizeof(m_infoCreate)); // 配置情報
}

//===========================================
//	デストラクタ
//===========================================
CEditChanger::~CEditChanger()
{
}

//===========================================
//	初期化処理
//===========================================
HRESULT CEditChanger::Init(void)
{
#if _DEBUG

	// 親クラスの初期化
	if (FAILED(CEditorObject::Init())) { assert(false); return E_FAIL; }

	// オブジェクトの生成
	m_pChanger = CCameraChanger::Create
	(
		GetVec3Position(), // 位置
		m_infoCreate.size, // 大きさ
		m_infoCreate.dir, // 方向
		m_infoCreate.rot // 角度
	);

	//生成に失敗した場合関数を抜ける
	if (m_pChanger == nullptr) { assert(false); return E_FAIL; }

	// 成功を返す
	return S_OK;

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//===========================================
//	終了処理
//===========================================
void CEditChanger::Uninit(void)
{
#if _DEBUG

	// 親クラスの終了
	CEditorObject::Uninit();

	// オブジェクトの終了
	SAFE_UNINIT(m_pChanger);

#endif	// _DEBUG
}

//===========================================
//	更新処理
//===========================================
void CEditChanger::Update(void)
{
#if _DEBUG

	// 親クラスの更新
	CEditorObject::Update();

	// 大きさの更新
	UpdateSize();

	// 方向の変更
	ChangeDirection();

	// 角度の変更
	ChangeRotation();

	// 生成
	Create();

	// 破棄
	Release();

	// 位置を反映
	m_pChanger->SetVec3Position(GetVec3Position());

	// 向きを反映
	m_pChanger->SetVec3Rotation(GetVec3Rotation());

#endif	// _DEBUG
}

//===========================================
//	保存状況取得処理
//===========================================
bool CEditChanger::IsSave(void)
{
#if _DEBUG

	// 保存状況を返す
	return m_bSave;

#else	// NDEBUG

	// falseを返す
	return false;

#endif	// _DEBUG
}

//===========================================
//	情報保存処理
//===========================================
void CEditChanger::SaveInfo(void)
{
#if _DEBUG

	// 現在の情報を保存
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//===========================================
//	情報読込処理
//===========================================
void CEditChanger::LoadInfo(void)
{
#if _DEBUG

	// 保存情報を設定
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//===========================================
//	操作表示の描画処理
//===========================================
void CEditChanger::DrawDebugControl(void)
{
#if _DEBUG

	// 操作表示の描画
	CEditorObject::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "大きさ：[%s/%s+%s/%s/%s+%s/%s/%s+%s]\n",
		NAME_UP_SIZE_X, NAME_DOWN_SIZE_X, NAME_TRIGGER,
		NAME_UP_SIZE_Y, NAME_DOWN_SIZE_Y, NAME_TRIGGER,
		NAME_UP_SIZE_Z, NAME_DOWN_SIZE_Z, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "方向変更：[%s]\n", NAME_DIRECTION);
	DebugProc::Print(DebugProc::POINT_RIGHT, "角度変更：[%s]\n", NAME_ROTATION);
	DebugProc::Print(DebugProc::POINT_RIGHT, "削除：[%s]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "設置：[%s]\n", NAME_CREATE);

#endif	// _DEBUG
}

//===========================================
//	情報表示の描画処理
//===========================================
void CEditChanger::DrawDebugInfo(void)
{
#if _DEBUG

	// 情報表示の描画
	CEditorObject::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f：[大きさ]\n", m_infoCreate.size.x, m_infoCreate.size.y);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%s[方向]\n", NAME_DIR[m_infoCreate.dir]);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%s[角度]\n", NAME_ROT[m_infoCreate.rot]);

#endif	// _DEBUG
}

//===========================================
//	向き更新処理
//===========================================
void CEditChanger::UpdateRotation(void)
{
	// 向きの更新
	CEditorObject::UpdateRotation();

	// 向きを反映
	m_pChanger->SetVec3Rotation(GetVec3Rotation());
}

//===========================================
//	大きさの更新処理
//===========================================
void CEditChanger::UpdateSize(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 大きさを変更
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_SIZE_X))
		{
			m_infoCreate.size.x += INIT_SIZE.x;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE_X))
		{
			m_infoCreate.size.x -= INIT_SIZE.x;
		}
		if (pKeyboard->IsPress(KEY_UP_SIZE_Y))
		{
			m_infoCreate.size.y += INIT_SIZE.y;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE_Y))
		{
			m_infoCreate.size.y -= INIT_SIZE.y;
		}
		if (pKeyboard->IsPress(KEY_UP_SIZE_Z))
		{
			m_infoCreate.size.z += INIT_SIZE.z;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE_Z))
		{
			m_infoCreate.size.z -= INIT_SIZE.z;
		}
	}
	else
	{
		if (pKeyboard->IsTrigger(KEY_UP_SIZE_X))
		{
			m_infoCreate.size.x += INIT_SIZE.x * 0.5f;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_X))
		{
			m_infoCreate.size.x -= INIT_SIZE.x * 0.5f;
		}
		if (pKeyboard->IsTrigger(KEY_UP_SIZE_Y))
		{
			m_infoCreate.size.y += INIT_SIZE.y * 0.5f;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_Y))
		{
			m_infoCreate.size.y -= INIT_SIZE.y * 0.5f;
		}
		if (pKeyboard->IsTrigger(KEY_UP_SIZE_Z))
		{
			m_infoCreate.size.z += INIT_SIZE.z * 0.5f;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_Z))
		{
			m_infoCreate.size.z -= INIT_SIZE.z * 0.5f;
		}
	}

	// 大きさを補正
	useful::LimitMinNum(m_infoCreate.size.x, INIT_SIZE.x);
	useful::LimitMinNum(m_infoCreate.size.y, INIT_SIZE.y);
	useful::LimitMinNum(m_infoCreate.size.z, INIT_SIZE.z);

	// 大きさを反映
	m_pChanger->SetVec3Sizing(m_infoCreate.size);
}

//===========================================
//  方向変更の更新処理
//===========================================
void CEditChanger::ChangeDirection(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 種類を変更
	if (pKeyboard->IsTrigger(KEY_DIRECTION))
	{
		m_infoCreate.dir = (CCameraChanger::EDirection)((m_infoCreate.dir + 1) % CCameraChanger::DIRECTION_MAX);
	}

	// 種類を反映
	m_pChanger->SetDirection(m_infoCreate.dir);
}

//===========================================
//  角度変更の更新処理
//===========================================
void CEditChanger::ChangeRotation(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 種類を変更
	if (pKeyboard->IsTrigger(KEY_ROTATION))
	{
		m_infoCreate.rot = (CCameraChanger::ERotation)((m_infoCreate.rot + 1) % CCameraChanger::ROTATION_MAX);
	}

	// 種類を反映
	m_pChanger->SetRotation(m_infoCreate.rot);
}

//===========================================
//	生成処理
//===========================================
void CEditChanger::Create(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 配置
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	情報を配置用に変更
		//----------------------------------------------------
		// 自動更新・自動描画をONにする
		m_pChanger->SetEnableUpdate(true);
		m_pChanger->SetEnableDraw(true);

		// 未保存を設定
		m_bSave = false;

		// オブジェクトの生成
		m_pChanger = CCameraChanger::Create
		(
			GetVec3Position(), // 位置
			m_infoCreate.size, // 大きさ
			m_infoCreate.dir, // 方向
			m_infoCreate.rot // 角度
		);
		assert(m_pChanger != nullptr);

		InitAllColor();
	}
}

//===========================================
//	破棄処理
//===========================================
void CEditChanger::Release(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報
	bool bRelease = false;	// 破棄状況

	// 削除
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// 破棄する状態を設定
		bRelease = true;
	}

	// 削除判定
	DeleteCollision(bRelease);
}

//===========================================
//	削除判定
//===========================================
void CEditChanger::DeleteCollision(const bool bRelase)
{
	// リストを取得
	CListManager<CCameraChanger> *pListManager = CCameraChanger::GetList();
	if (pListManager == nullptr) { return; }
	std::list<CCameraChanger*> listChanger = pListManager->GetList();

	// リスト内を全て確認する
	for (auto& rList : listChanger)
	{
		// 同じアドレスだった場合次へ
		if (rList == m_pChanger) { continue; }

		D3DXVECTOR3 posOther = rList->GetVec3Position();	// 対象の地面位置
		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// 自身の大きさ
		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// 対象の大きさ

		// 自身の大きさを設定
		D3DXVECTOR3 sizeThisChanger = m_pChanger->GetVec3Sizing();	// 自身の地面の大きさ
		sizeThis.x = sizeThisChanger.x;	// 判定サイズXを設定
		sizeThis.y = sizeThisChanger.y;	// 判定サイズYを設定
		sizeThis.z = sizeThisChanger.z;	// 判定サイズZを設定
		sizeThis *= 0.5f;				// 判定サイズを半分に

		// 対象の大きさを設定
		D3DXVECTOR3 sizeOtherChanger = rList->GetVec3Sizing();	// 対象の地面の大きさ
		sizeOther.x = sizeOtherChanger.x;	// 判定サイズXを設定
		sizeOther.y = sizeOtherChanger.y;	// 判定サイズYを設定
		sizeOther.z = sizeOtherChanger.z;	// 判定サイズZを設定
		sizeOther *= 0.5f;				// 判定サイズを半分に

		// 矩形の当たり判定
		if (collision::Box3D
		( // 引数
			GetVec3Position(),	// 判定位置
			posOther,	// 判定目標位置
			sizeThis,	// 判定サイズ(右・上・後)
			sizeThis,	// 判定サイズ(左・下・前)
			sizeOther,	// 判定目標サイズ(右・上・後)
			sizeOther,	// 判定目標サイズ(左・下・前)
			useful::RotToFourDire(GetVec3Rotation().y),			// 判定方向列挙
			useful::RotToFourDire(rList->GetVec3Rotation().y)	// 判定目標方向列挙
		))
		{ // 判定内だった場合

			if (bRelase)
			{ // 破棄する場合

				// 終了処理
				rList->Uninit();

				// 未保存を設定
				m_bSave = false;
			}
			else
			{ // 破棄しない場合

				// 赤を設定
				rList->SetCubeColor(XCOL_RED);
			}
		}
		else
		{ // 判定外だった場合

			// 通常色を設定
			rList->SetCubeColor(XCOL_ACYAN);
		}
	}
}

//===========================================
//  色全初期化
//===========================================
void CEditChanger::InitAllColor(void)
{
	CListManager<CCameraChanger>* pListManager = CCameraChanger::GetList(); // リストマネージャー
	if (pListManager == nullptr) { return; } // リスト未使用の場合抜ける
	std::list<CCameraChanger*> listField = pListManager->GetList(); // リスト情報

	for (auto& rList : listField)
	{ // 数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pChanger) { continue; }

		// 通常色を設定
		rList->SetCubeColor(XCOL_ACYAN);
	}
}

//===========================================
//	保存処理
//===========================================
HRESULT CEditChanger::Save(void)
{
#if _DEBUG

	// 地面のリストを取得
	CListManager<CCameraChanger> *pListManager = CCameraChanger::GetList();	// リストマネージャー
	std::list<CCameraChanger*> listChanger;	// 地面リスト
	if (pListManager != nullptr)
	{ // リストマネージャーが生成されている場合

		// リストを取得
		listChanger = pListManager->GetList();
	}

	// ファイルを開く
	std::ofstream  file(SAVE_PASS);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "カメラ変更地点配置の書き出しに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 見出しを書き出し
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	カメラ変更地点配置のセーブデータ [save_Changer.txt]" << std::endl;
	file << "#	Author : 金崎 朋弥" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# この行から下をコピーし [stage.txt] に張り付け\n" << std::endl;

	// 小数点書き出しの方法を指定
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// 読み込み開始文字列を書き出し
	file << "STAGE_CHANGERSET\n" << std::endl;

	for (const auto& rList : listChanger)
	{ // 地面の総数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pChanger) { continue; }

		// 書き出す情報を取得
		CCameraChanger::EDirection dir = rList->GetDir(); // 方向
		CCameraChanger::ERotation rot = rList->GetRot(); // 角度
		D3DXVECTOR3 pos = rList->GetVec3Position(); // 位置
		D3DXVECTOR3 size = rList->GetVec3Sizing(); // 大きさ

		// 情報を書き出し
		file << "	CHANGERSET" << std::endl;
		file << "		DIR		= " << dir << std::endl;
		file << "		ROT		= " << rot << std::endl;
		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		file << "		SIZE	= " << size.x << " " << size.y << " " << size.z << std::endl;
		file << "	END_CHANGERSET\n" << std::endl;
	}

	// 読み込み終了文字列を書き出し
	file << "END_STAGE_CHANGERSET" << std::endl;

	// 削除判定
	DeleteCollision(false);

	// 保存済みにする
	m_bSave = true;

	// 成功を返す
	return S_OK;

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}
