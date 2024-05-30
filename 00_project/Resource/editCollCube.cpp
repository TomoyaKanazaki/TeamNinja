//#if 1
////============================================================
////
////	エディットコリジョンキューブ処理 [editCollisionCube.cpp]
////	Author：小原立暉
////
////============================================================
////************************************************************
////	インクルードファイル
////************************************************************
//#include "manager.h"
//#include "editManager.h"
//#include "editCollCube.h"
//
//#include "objectMeshCube.h"
//
////************************************************************
////	マクロ定義
////************************************************************
//#define KEY_CREATE		(DIK_0)	// 生成キー
//#define NAME_CREATE		("0")	// 生成表示
//#define KEY_RELEASE		(DIK_9)	// 破棄キー
//#define NAME_RELEASE	("9")	// 破棄表示
//#define KEY_TYPE		(DIK_3)	// 種類変更キー
//#define NAME_TYPE		("3")	// 種類変更表示
//
//#define KEY_UP_SIZE_X		(DIK_T)	// X軸拡大キー
//#define NAME_UP_SIZE_X		("T")	// X軸拡大表示
//#define KEY_DOWN_SIZE_X		(DIK_G)	// X軸縮小キー
//#define NAME_DOWN_SIZE_X	("G")	// X軸縮小表示
//#define KEY_UP_SIZE_Y		(DIK_Y)	// Y軸拡大キー
//#define NAME_UP_SIZE_Y		("Y")	// Y軸拡大表示
//#define KEY_DOWN_SIZE_Y		(DIK_H)	// Y軸縮小キー
//#define NAME_DOWN_SIZE_Y	("H")	// Y軸縮小表示
//#define KEY_UP_SIZE_Z		(DIK_U)	// Z軸拡大キー
//#define NAME_UP_SIZE_Z		("U")	// Z軸拡大表示
//#define KEY_DOWN_SIZE_Z		(DIK_J)	// Z軸縮小キー
//#define NAME_DOWN_SIZE_Z	("J")	// Z軸縮小表示
//
////************************************************************
////	定数宣言
////************************************************************
//namespace
//{
//	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_collisioncube.txt";	// セーブテキストパス
//	const char* ANGLE_NAME[] =	// 向きの名前
//	{
//		"0度",		// 角度：0度
//		"90度",		// 角度：90度
//		"180度",	// 角度：180度
//		"270度",	// 角度：270度
//	};
//
//	const D3DXVECTOR3 INIT_SIZE = D3DXVECTOR3(30.0f, 30.0f, 30.0f);			// サイズ
//	const float	INIT_ALPHA = 0.5f;	// 配置前のα値
//	const int DIGIT_FLOAT = 2;		// 小数点以下の桁数
//}
//
////************************************************************
////	スタティックアサート
////************************************************************
//static_assert(NUM_ARRAY(ANGLE_NAME) == CEditor::ANGLE_MAX, "ERROR : Angle Count Mismatch");
//
////************************************************************
////	親クラス [CEditCollisionCube] のメンバ関数
////************************************************************
////============================================================
////	コンストラクタ
////============================================================
//CEditCollisionCube::CEditCollisionCube()
//{
//#if _DEBUG
//
//	// メンバ変数をクリア
//	m_pCube = nullptr;							// キューブ情報
//	m_infoCreate.angle = CEditor::ANGLE_0;		// フィールド配置情報
//	m_infoCreate.size = INIT_SIZE;				// サイズ
//	m_infoCreate.fLength = 0.0f;				// 長さ
//	m_infoCreate.fAngle = 0.0f;					// 方向
//	m_bSave = false;							// 保存状況
//
//#endif	// _DEBUG
//}
//
////============================================================
////	デストラクタ
////============================================================
//CEditCollisionCube::~CEditCollisionCube()
//{
//#if _DEBUG
//#endif	// _DEBUG
//}
//
////============================================================
////	初期化処理
////============================================================
//HRESULT CEditCollisionCube::Init(void)
//{
//#if _DEBUG
//
//	// メンバ変数を初期化
//	m_pCube = nullptr;							// キューブ情報
//	m_infoCreate.angle = CEditor::ANGLE_0;		// フィールド配置情報
//	m_infoCreate.size = INIT_SIZE;				// サイズ
//	m_infoCreate.fLength = 0.0f;				// 長さ
//	m_infoCreate.fAngle = 0.0f;					// 方向
//	m_bSave = false;							// 保存状況
//
//	// 親クラスの初期化
//	if (FAILED(CEditorObject::Init()))
//	{ // 初期化に失敗した場合
//
//		// 失敗を返す
//		assert(false);
//		return E_FAIL;
//	}
//
//	// キューブの生成
//	D3DXCOLOR colField = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// 地面色
//	m_pCube = CCollisionCube::Create
//	( // 引数
//		VEC3_ZERO,
//		m_infoCreate.fLength,
//		m_infoCreate.fAngle,
//		m_infoCreate.size.x,
//		m_infoCreate.size.y,
//		m_infoCreate.size.z
//	);
//	if (m_pCube == nullptr)
//	{ // 生成に失敗した場合
//
//		// 失敗を返す
//		assert(false);
//		return E_FAIL;
//}
//	// 成功を返す
//	return S_OK;
//
//#else	// NDEBUG
//
//	// 成功を返す
//	return S_OK;
//
//#endif	// _DEBUG
//}
//
////============================================================
////	終了処理
////============================================================
//void CEditCollisionCube::Uninit(void)
//{
//#if _DEBUG
//
//	// 親クラスの終了
//	CEditorObject::Uninit();
//
//	// キューブの色の全初期化
//	InitAllColorCube();
//
//	// キューブの終了
//	SAFE_UNINIT(m_pCube);
//
//#endif	// _DEBUG
//}
//
////============================================================
////	更新処理
////============================================================
//void CEditCollisionCube::Update(void)
//{
//#if _DEBUG
//
//	// 親クラスの更新
//	CEditorObject::Update();
//
//	// 大きさの更新
//	UpdateSizing();
//
//	// キューブの生成
//	CreateCube();
//
//	// キューブの破棄
//	ReleaseCube();
//
//	// 位置を反映
//	m_pCube->GetCube()->SetVec3Position(GetVec3Position());
//
//#endif	// _DEBUG
//}
//
////============================================================
////	保存状況取得処理
////============================================================
//bool CEditCollisionCube::IsSave(void)
//{
//#if _DEBUG
//
//	// 保存状況を返す
//	return m_bSave;
//
//#else	// NDEBUG
//
//	// falseを返す
//	return false;
//
//#endif	// _DEBUG
//}
//
////============================================================
////	情報保存処理
////============================================================
//void CEditCollisionCube::SaveInfo(void)
//{
//#if _DEBUG
//
//	// 現在の情報を保存
//	//m_save = m_infoCreate;
//
//#endif	// _DEBUG
//}
//
////============================================================
////	情報読込処理
////============================================================
//void CEditCollisionCube::LoadInfo(void)
//{
//#if _DEBUG
//
//	// 保存情報を設定
//	//m_infoCreate = m_save;
//
//#endif	// _DEBUG
//}
//
////============================================================
////	操作表示の描画処理
////============================================================
//void CEditCollisionCube::DrawDebugControl(void)
//{
//#if _DEBUG
//
//	// 操作表示の描画
//	CEditorObject::DrawDebugControl();
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, "大きさ：[%s/%s/%s/%s/%s/%s+%s]\n", NAME_UP_SIZE_X, NAME_DOWN_SIZE_X, NAME_UP_SIZE_Y, NAME_DOWN_SIZE_Y, NAME_UP_SIZE_Z, NAME_DOWN_SIZE_Z, NAME_TRIGGER);
//	DebugProc::Print(DebugProc::POINT_RIGHT, "種類変更：[%s]\n", NAME_TYPE);
//	DebugProc::Print(DebugProc::POINT_RIGHT, "削除：[%s]\n", NAME_RELEASE);
//	DebugProc::Print(DebugProc::POINT_RIGHT, "設置：[%s]\n", NAME_CREATE);
//
//#endif	// _DEBUG
//}
//
////============================================================
////	情報表示の描画処理
////============================================================
//void CEditCollisionCube::DrawDebugInfo(void)
//{
//#if _DEBUG
//
//	// 情報表示の描画
//	CEditorObject::DrawDebugInfo();
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, "%s：[向きの種類]\n", ANGLE_NAME[m_infoCreate.angle]);
//	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f：[大きさ]\n", m_infoCreate.size.x, m_infoCreate.size.y * 2, m_infoCreate.size.y);
//
//#endif	// _DEBUG
//}
//
////============================================================
//// 位置更新
////============================================================
//void CEditCollisionCube::UpdatePosition(void)
//{
//	// 位置の更新
//	CEditorObject::UpdatePosition();
//
//	// 位置を反映
//	m_pCube->GetCube()->SetVec3Position(GetVec3Position());
//}
//
////============================================================
////	向き更新処理
////============================================================
//void CEditCollisionCube::UpdateRotation(void)
//{
//	// 向きの更新
//	CEditorObject::UpdateRotation();
//
//	// 向きを反映
//	m_pCube->GetCube()->SetVec3Rotation(GetVec3Rotation());
//}
//
////============================================================
////	大きさの更新処理
////============================================================
//void CEditCollisionCube::UpdateSizing(void)
//{
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
//
//	// 大きさを変更
//	if (!pKeyboard->IsPress(KEY_TRIGGER))
//	{
//		if (pKeyboard->IsPress(KEY_UP_SIZE_X))
//		{
//			m_infoCreate.size.x += INIT_SIZE.x;
//		}
//		if (pKeyboard->IsPress(KEY_DOWN_SIZE_X))
//		{
//			m_infoCreate.size.x -= INIT_SIZE.x;
//		}
//		if (pKeyboard->IsPress(KEY_UP_SIZE_Y))
//		{
//			m_infoCreate.size.y += INIT_SIZE.y;
//		}
//		if (pKeyboard->IsPress(KEY_DOWN_SIZE_Y))
//		{
//			m_infoCreate.size.y -= INIT_SIZE.y;
//		}
//		if (pKeyboard->IsPress(KEY_UP_SIZE_Z))
//		{
//			m_infoCreate.size.z += INIT_SIZE.z;
//		}
//		if (pKeyboard->IsPress(KEY_DOWN_SIZE_Z))
//		{
//			m_infoCreate.size.z -= INIT_SIZE.z;
//		}
//	}
//	else
//	{
//		if (pKeyboard->IsTrigger(KEY_UP_SIZE_X))
//		{
//			m_infoCreate.size.x += INIT_SIZE.x;
//		}
//		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_X))
//		{
//			m_infoCreate.size.x -= INIT_SIZE.x;
//		}
//		if (pKeyboard->IsTrigger(KEY_UP_SIZE_Y))
//		{
//			m_infoCreate.size.y += INIT_SIZE.y;
//		}
//		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_Y))
//		{
//			m_infoCreate.size.y -= INIT_SIZE.y;
//		}
//		if (pKeyboard->IsTrigger(KEY_UP_SIZE_Z))
//		{
//			m_infoCreate.size.z += INIT_SIZE.z;
//		}
//		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_Z))
//		{
//			m_infoCreate.size.z -= INIT_SIZE.z;
//		}
//	}
//
//	// 大きさを補正
//	useful::LimitMinNum(m_infoCreate.size.x, INIT_SIZE.x);
//	useful::LimitMinNum(m_infoCreate.size.y, INIT_SIZE.y);
//	useful::LimitMinNum(m_infoCreate.size.z, INIT_SIZE.z);
//
//	// 大きさを反映
//	m_pCube->GetCube()->SetVec3Sizing(m_infoCreate.size);
//}
//
////============================================================
////	キューブの生成処理
////============================================================
//void CEditCollisionCube::CreateCube(void)
//{
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
//	D3DXVECTOR3 posEdit = GetVec3Position();	// エディットの位置
//	D3DXCOLOR colField = XCOL_WHITE;			// 色保存用
//
//	// キューブを配置
//	if (pKeyboard->IsTrigger(KEY_CREATE))
//	{
//		//----------------------------------------------------
//		//	キューブの情報を配置用に変更
//		//----------------------------------------------------
//		// 自動更新・自動描画をONにする
//		m_pCube->GetCube()->SetEnableUpdate(true);
//		m_pCube->GetCube()->SetEnableDraw(true);
//
//		// 色を設定
//		colField = m_pCube->GetCube()->GetCubeColor();	// 元の色を取得
//		m_pCube->GetCube()->SetCubeColor(D3DXCOLOR(colField.r, colField.g, colField.b, 1.0f));
//
//		// 未保存を設定
//		m_bSave = false;
//
//		//----------------------------------------------------
//		//	新しいキューブの生成
//		//----------------------------------------------------
//		// キューブの生成
//		m_pCube = CCollisionCube::Create
//		( // 引数
//			GetVec3Position(),
//			m_infoCreate.fLength,
//			m_infoCreate.fAngle,
//			m_infoCreate.size.x,
//			m_infoCreate.size.y,
//			m_infoCreate.size.z
//		);
//		assert(m_pCube != nullptr);
//
//		// 色を設定
//		colField = m_pCube->GetCube()->GetCubeColor();	// 元の色を取得
//		m_pCube->GetCube()->SetCubeColor(D3DXCOLOR(colField.r, colField.g, colField.b, INIT_ALPHA));
//	}
//}
//
////============================================================
////	キューブの破棄処理
////============================================================
//void CEditCollisionCube::ReleaseCube(void)
//{
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
//	bool bRelease = false;	// 破棄状況
//
//	// キューブを削除
//	if (pKeyboard->IsTrigger(KEY_RELEASE))
//	{
//		// 破棄する状態を設定
//		bRelease = true;
//	}
//
//	// キューブの削除判定
//	DeleteCollisionCube(bRelease);
//}
//
////============================================================
////	キューブの削除判定
////============================================================
//void CEditCollisionCube::DeleteCollisionCube(const bool bRelase)
//{
//	CListManager<CField>* pListManager = CField::GetList();	// キューブリストマネージャー
//	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
//	std::list<CField*> listField = pListManager->GetList();	// キューブリスト情報
//
//	D3DXVECTOR3 posEdit = GetVec3Position();	// エディットの位置
//	for (auto& rList : listField)
//	{ // キューブ数分繰り返す
//
//		// 同じアドレスだった場合次へ
//		if (rList == m_pCube) { continue; }
//
//		D3DXVECTOR3 posOther = rList->GetVec3Position();	// 対象の地面位置
//		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// 自身の大きさ
//		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// 対象の大きさ
//
//		// 自身の大きさを設定
//		D3DXVECTOR2 sizeThisField = m_pCube->GetVec2Sizing();	// 自身の地面の大きさ
//		sizeThis.x = sizeThisField.x;	// 判定サイズXを設定
//		sizeThis.y = editstage::SIZE;	// 判定サイズYを設定
//		sizeThis.z = sizeThisField.y;	// 判定サイズZを設定
//		sizeThis *= 0.5f;				// 判定サイズを半分に
//
//		// 対象の大きさを設定
//		D3DXVECTOR2 sizeOtherField = rList->GetVec2Sizing();	// 対象の地面の大きさ
//		sizeOther.x = sizeOtherField.x;	// 判定サイズXを設定
//		sizeOther.y = editstage::SIZE;	// 判定サイズYを設定
//		sizeOther.z = sizeOtherField.y;	// 判定サイズZを設定
//		sizeOther *= 0.5f;				// 判定サイズを半分に
//
//		// 矩形の当たり判定
//		if (collision::Box3D
//		( // 引数
//			posEdit,	// 判定位置
//			posOther,	// 判定目標位置
//			sizeThis,	// 判定サイズ(右・上・後)
//			sizeThis,	// 判定サイズ(左・下・前)
//			sizeOther,	// 判定目標サイズ(右・上・後)
//			sizeOther	// 判定目標サイズ(左・下・前)
//		))
//		{ // 判定内だった場合
//
//			if (bRelase)
//			{ // 破棄する場合
//
//				// 終了処理
//				rList->Uninit();
//
//				// 未保存を設定
//				m_bSave = false;
//			}
//			else
//			{ // 破棄しない場合
//
//				// 赤を設定
//				rList->SetColor(XCOL_RED);
//			}
//		}
//		else
//		{ // 判定外だった場合
//
//			// 通常色を設定
//			rList->SetColor(XCOL_WHITE);
//		}
//	}
//}
//
////============================================================
////	キューブの色の全初期化処理
////============================================================
//void CEditCollisionCube::InitAllColorCube(void)
//{
//	CListManager<CField>* pListManager = CField::GetList();	// キューブリストマネージャー
//	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
//	std::list<CField*> listField = pListManager->GetList();	// キューブリスト情報
//
//	for (auto& rList : listField)
//	{ // キューブ数分繰り返す
//
//		// 同じアドレスだった場合次へ
//		if (rList == m_pCube) { continue; }
//
//		// 通常色を設定
//		rList->SetColor(XCOL_WHITE);
//	}
//}
//
////============================================================
////	保存処理
////============================================================
//HRESULT CEditCollisionCube::Save(void)
//{
//#if _DEBUG
//
//	// 地面のリストを取得
//	CListManager<CField>* pListManager = CField::GetList();	// リストマネージャー
//	std::list<CField*> listField;	// 地面リスト
//	if (pListManager != nullptr)
//	{ // リストマネージャーが生成されている場合
//
//		// リストを取得
//		listField = pListManager->GetList();
//	}
//
//	// ファイルを開く
//	std::ofstream  file(SAVE_PASS);	// ファイルストリーム
//	if (file.fail())
//	{ // ファイルが開けなかった場合
//
//		// エラーメッセージボックス
//		MessageBox(nullptr, "ステージ地面配置の書き出しに失敗！", "警告！", MB_ICONWARNING);
//
//		// 失敗を返す
//		return E_FAIL;
//	}
//
//	// 見出しを書き出し
//	file << "#==============================================================================" << std::endl;
//	file << "#" << std::endl;
//	file << "#	ステージ地面配置のセーブデータ [save_collisioncube.txt]" << std::endl;
//	file << "#	Author : 藤田 勇一" << std::endl;
//	file << "#" << std::endl;
//	file << "#==============================================================================" << std::endl;
//	file << "# この行から下をコピーし [stage.txt] に張り付け\n" << std::endl;
//
//	// キューブの色の全初期化
//	InitAllColorCube();
//
//	// 小数点書き出しの方法を指定
//	file << std::fixed << std::setprecision(DIGIT_FLOAT);
//
//	// 読み込み開始文字列を書き出し
//	file << "STAGE_CUBESET\n" << std::endl;
//
//	for (const auto& rList : listField)
//	{ // 地面の総数分繰り返す
//
//		// 同じアドレスだった場合次へ
//		if (rList == m_pCube) { continue; }
//
//		// 書き出す情報を取得
//		CField::EType type = rList->GetType();			// 種類
//		D3DXVECTOR3 pos = rList->GetVec3Position();	// 位置
//		D3DXVECTOR3 rot = rList->GetVec3Rotation();	// 向き
//		D3DXVECTOR2 size = rList->GetVec2Sizing();	// 大きさ
//		D3DXCOLOR col = rList->GetColor();		// 色
//		POSGRID2 part = rList->GetPattern();		// 分割数
//		POSGRID2 texPart = rList->GetTexPattern();	// テクスチャ分割数
//
//		// 向きを360度に変換
//		D3DXToDegree(rot);
//
//		// 情報を書き出し
//		file << "	CUBESET" << std::endl;
//		file << "		TYPE	= " << type << std::endl;
//		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
//		file << "		ROT		= " << rot.x << " " << rot.y << " " << rot.z << std::endl;
//		file << "		SIZE	= " << size.x << " " << size.y << std::endl;
//		file << "		COL		= " << col.r << " " << col.g << " " << col.b << " " << col.a << std::endl;
//		file << "		PART	= " << part.x << " " << part.y << std::endl;
//		file << "		TEXPART	= " << texPart.x << " " << texPart.y << std::endl;
//		file << "	END_CUBESET\n" << std::endl;
//	}
//
//	// 読み込み終了文字列を書き出し
//	file << "END_STAGE_CUBESET" << std::endl;
//
//	// キューブの削除判定
//	DeleteCollisionCube(false);
//
//	// 保存済みにする
//	m_bSave = true;
//
//	// 成功を返す
//	return S_OK;
//
//#else	// NDEBUG
//
//	// 成功を返す
//	return S_OK;
//
//#endif	// _DEBUG
//}
//#endif
