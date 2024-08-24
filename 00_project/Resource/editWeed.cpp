//===========================================
//
//  草エディター(editWeed.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "editWeed.h"
#include "editManager.h"
#include "manager.h"
#include "collision.h"
#include "stage.h"
#include "objectMeshCylinder.h"

//===========================================
//  定数宣言
//===========================================
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_weed.txt"; // セーブテキストパス

	const float	INIT_ALPHA = 0.5f; // 配置前のα値
	const int DIGIT_FLOAT = 2; // 小数点以下の桁数

	const int KEY_CREATE		= DIK_0; // 生成キー
	const int KEY_RELEASE		= DIK_9; // 破棄キー
	const int KEY_UP_SIZE		= DIK_T; // 拡大キー
	const int KEY_DOWN_SIZE		= DIK_G; // 縮小キー
	const int KEY_UP_TIME		= DIK_Y; // 拡大キー
	const int KEY_DOWN_TIME		= DIK_H; // 縮小キー
	const int KEY_UP_NUM		= DIK_U; // 拡大キー
	const int KEY_DOWN_NUM		= DIK_J; // 縮小キー

	const char* NAME_CREATE			= "0"; // 生成表示
	const char* NAME_RELEASE		= "9"; // 破棄表示
	const char* NAME_UP_SIZE		= "T"; // 拡大表示
	const char* NAME_DOWN_SIZE		= "G"; // 縮小表示
	const char* NAME_UP_TIME		= "Y"; // 拡大表示
	const char* NAME_DOWN_TIME		= "H"; // 縮小表示
	const char* NAME_UP_NUM			= "U"; // 拡大表示
	const char* NAME_DOWN_NUM		= "J"; // 縮小表示

	int CREATE_TIME = 30; // 生成頻度
	const int INIT_NUM = 3; // 初期生成数
	const float PERMISSION = 0.05f; // 重なりの許容範囲

}

//===========================================
//	コンストラクタ
//===========================================
CEditWeed::CEditWeed(CEditStage* pEditor) : CEditorObject(pEditor),
m_pCylinder(nullptr),
m_bSave(false),
m_nCoolTime(0),
m_nNum(0)
{
	// メンバ変数をクリア
	memset(&m_infoCreate, 0, sizeof(m_infoCreate)); // 配置情報
}

//===========================================
//	デストラクタ
//===========================================
CEditWeed::~CEditWeed()
{
}

//===========================================
//	初期化処理
//===========================================
HRESULT CEditWeed::Init(void)
{
#if _DEBUG

	// 親クラスの初期化
	if (FAILED(CEditorObject::Init())) { assert(false); return E_FAIL; }

	// オブジェクトの生成
	m_pCylinder = CObjectMeshCylinder::Create
	(
		GetVec3Position(),					// 位置
		VEC3_ZERO,							// 向き
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),	// 色
		POSGRID2(16, 1),					// 分割数
		POSGRID2(1, 1),						// テクスチャ分割数
		m_infoCreate.fSize,					// 半径
		25.0f								// 高さ
	);
	m_pCylinder->GetRenderState()->SetCulling(D3DCULL_NONE);	// カリングオフ

	// 生成情報の初期化
	m_nCoolTime = CREATE_TIME;
	m_nNum = INIT_NUM;

	//生成に失敗した場合関数を抜ける
	if (m_pCylinder == nullptr) { assert(false); return E_FAIL; }

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
void CEditWeed::Uninit(void)
{
#if _DEBUG

	// 親クラスの終了
	CEditorObject::Uninit();

	// オブジェクトの終了
	SAFE_UNINIT(m_pCylinder);

#endif	// _DEBUG
}

//===========================================
//	更新処理
//===========================================
void CEditWeed::Update(void)
{
#if _DEBUG

	// 親クラスの更新
	CEditorObject::Update();

	// 大きさの更新
	UpdateSize();

	// 生成頻度の更新
	UpdateTime();

	// 生成数の更新
	UpdateNum();

	// 生成
	Create();

	// 破棄
	Release();

	// 位置を反映
	m_pCylinder->SetVec3Position(GetVec3Position());

	// 向きを反映
	m_pCylinder->SetVec3Rotation(GetVec3Rotation());

#endif	// _DEBUG
}

//===========================================
//	保存状況取得処理
//===========================================
bool CEditWeed::IsSave(void)
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
void CEditWeed::SaveInfo(void)
{
#if _DEBUG

	// 現在の情報を保存
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//===========================================
//	情報読込処理
//===========================================
void CEditWeed::LoadInfo(void)
{
#if _DEBUG

	// 保存情報を設定
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//===========================================
//	操作表示の描画処理
//===========================================
void CEditWeed::DrawDebugControl(void)
{
#if _DEBUG

	// 操作表示の描画
	CEditorObject::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "生成範囲：[ %s / %s+%s ]\n",
		NAME_UP_SIZE, NAME_DOWN_SIZE, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "生成頻度：[ %s / %s+%s ]\n",
		NAME_UP_TIME, NAME_DOWN_TIME, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "生成数  ：[ %s / %s ]\n",
		NAME_UP_NUM, NAME_DOWN_NUM);
	DebugProc::Print(DebugProc::POINT_RIGHT, "削除：[ %s ]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "設置：[ %s ]\n", NAME_CREATE);

#endif	// _DEBUG
}

//===========================================
//	情報表示の描画処理
//===========================================
void CEditWeed::DrawDebugInfo(void)
{
#if _DEBUG

	// 情報表示の描画
	CEditorObject::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f：[ 生成範囲 ]\n", m_infoCreate.fSize);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d：[ 生成頻度 ]\n", CREATE_TIME);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d：  [ 生成数 ]\n", m_nNum);

#endif	// _DEBUG
}

//===========================================
//	大きさの更新処理
//===========================================
void CEditWeed::UpdateSize(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 大きさを変更
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_SIZE))
		{
			m_infoCreate.fSize += editstage::SIZE * 0.1f;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE))
		{
			m_infoCreate.fSize -= editstage::SIZE * 0.1f;
		}
	}
	else
	{
		if (pKeyboard->IsTrigger(KEY_UP_SIZE))
		{
			m_infoCreate.fSize += editstage::SIZE * 0.1f;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE))
		{
			m_infoCreate.fSize -= editstage::SIZE * 0.1f;
		}
	}

	// 大きさを補正
	useful::LimitMinNum(m_infoCreate.fSize, editstage::SIZE);

	// 大きさを反映
	m_pCylinder->SetRadius(m_infoCreate.fSize);
}

//==========================================
//  生成頻度の更新処理
//==========================================
void CEditWeed::UpdateTime(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 生成頻度の変更
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_TIME))
		{
			CREATE_TIME += 1;
		}
		if (pKeyboard->IsPress(KEY_DOWN_TIME))
		{
			CREATE_TIME -= 1;
		}
	}
	else
	{
		if (pKeyboard->IsTrigger(KEY_UP_TIME))
		{
			CREATE_TIME += 1;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_TIME))
		{
			CREATE_TIME -= 1;
		}
	}

	// 生成頻度の補正
	if (CREATE_TIME < 1)
	{
		CREATE_TIME = 1;
	}
}

//==========================================
//  生成数の更新処理
//==========================================
void CEditWeed::UpdateNum(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 生成数の変更
	if (pKeyboard->IsTrigger(KEY_UP_NUM))
	{
		m_nNum += 1;
	}
	if (pKeyboard->IsTrigger(KEY_DOWN_NUM))
	{
		m_nNum -= 1;
	}

	// 生成数の補正
	if (m_nNum < 1)
	{
		m_nNum = 1;
	}
}

//===========================================
//	生成処理
//===========================================
void CEditWeed::Create(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 配置
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		// 未保存を設定
		m_bSave = false;

		// 生成
		D3DXVECTOR3 posCent = GetVec3Position();	// 生成中心位置
		D3DXVECTOR3 posSet;	// 位置設定用
		D3DXVECTOR3 rotSet;	// 向き設定用
		for (int i = 0; i < m_nNum; ++i)
		{
			// 生成位置を設定
			posSet = posCent;
			posSet.x += (float)(rand() % ((int)m_infoCreate.fSize * 2) - (int)m_infoCreate.fSize + 1);
			posSet.y += 0.0f;
			posSet.z += (float)(rand() % ((int)m_infoCreate.fSize * 2) - (int)m_infoCreate.fSize + 1);
			float fDistance = (float)(rand() % 100 + 1) * 0.01f;

			// 生成位置を設定する
			D3DXVECTOR3 pos = GetVec3Position();
			pos.x += sinf(fTheta) * m_pCylinder->GetRadius() * fDistance;
			pos.z += cosf(fTheta) * m_pCylinder->GetRadius() * fDistance;

			// 生成向きを設定
			rotSet = D3DXVECTOR3(0.0f, (float)(rand() % 628 + 1) * 0.01f, 0.0f);

			// 草オブジェクトの生成
			CWeed::Create(posSet, rotSet);
		}
	}
}

//===========================================
//	破棄処理
//===========================================
void CEditWeed::Release(void)
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
void CEditWeed::DeleteCollision(const bool bRelase)
{
	// リストを取得
	CListManager<CWeed> *pListManager = CWeed::GetList();
	if (pListManager == nullptr) { return; }
	std::list<CWeed*> listWeed = pListManager->GetList();

	// リスト内を全て確認する
	for (auto& rList : listWeed)
	{
		D3DXVECTOR3 posOther = rList->GetVec3Position();	// 対象の地面位置

		// 円形の当たり判定
		if (collision::Circle3D
		( // 引数
			GetVec3Position(),	// 判定位置
			posOther,			// 判定目標位置
			m_pCylinder->GetRadius(),	// 判定半径
			50.0f
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
				rList->SetColor(XCOL_RED);
			}
		}
		else
		{ // 判定外だった場合

			// 通常色を設定
			rList->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//===========================================
//	保存処理
//===========================================
HRESULT CEditWeed::Save(void)
{
#if _DEBUG

	// 地面のリストを取得
	CListManager<CWeed> *pListManager = CWeed::GetList();	// リストマネージャー
	std::list<CWeed*> listChanger;	// 地面リスト
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
		MessageBox(nullptr, "草の書き出しに失敗^^", "エラー乙www", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 見出しを書き出し
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	草のセーブデータw [save_weed.txt]" << std::endl;
	file << "#	Author : 金崎 朋弥" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# この行から下をコピーし [stage.txt] に張り付け\n" << std::endl;

	// 小数点書き出しの方法を指定
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// 読み込み開始文字列を書き出し
	file << "STAGE_WEEDSET\n" << std::endl;

	for (const auto& rList : listChanger)
	{ // 草の総数分繰り返す

		// 書き出す情報を取得
		D3DXVECTOR3 pos = rList->GetVec3Position(); // 位置
		D3DXVECTOR3 rot = rList->GetVec3Rotation(); // 向き

		// 情報を書き出し
		file << "	WEEDSET" << std::endl;
		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		file << "		ROT		= " << rot.x << " " << rot.y << " " << rot.z << std::endl;
		file << "	END_WEEDSET\n" << std::endl;
	}

	// 読み込み終了文字列を書き出し
	file << "END_STAGE_WEEDSET" << std::endl;

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
