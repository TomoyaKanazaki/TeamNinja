//#if 1
////============================================================
////
////	エディット攻撃敵処理 [editEnemyAttack.cpp]
////	Author：小原立暉
////
////============================================================
////************************************************************
////	インクルードファイル
////************************************************************
//#include "editEnemyAttack.h"
//#include "editManager.h"
//#include "manager.h"
//#include "collision.h"
//#include "useful.h"
//#include "stage.h"
//
//#include "enemyNavigation.h"
//#include "enemyNavRandom.h"
//
////************************************************************
////	マクロ定義
////************************************************************
//#define KEY_CREATE		(DIK_0)	// 生成キー
//#define NAME_CREATE		("0")	// 生成表示
//#define KEY_RELEASE		(DIK_9)	// 破棄キー
//#define NAME_RELEASE	("9")	// 破棄表示
//#define KEY_TYPE		(DIK_4)	// 種類変更キー
//#define NAME_TYPE		("4")	// 種類変更表示
//
//#define KEY_ROTA_RIGHT	(DIK_Z)	// 右回転キー
//#define NAME_ROTA_RIGHT	("Z")	// 右回転表示
//#define KEY_ROTA_LEFT	(DIK_C)	// 左回転キー
//#define NAME_ROTA_LEFT	("C")	// 左回転表示
//
////************************************************************
////	定数宣言
////************************************************************
//namespace
//{
//	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_enemyAttack.txt";	// セーブテキストパス
//
//	const float	INIT_ALPHA = 0.5f;	// 配置前のα値
//	const int DIGIT_FLOAT = 2;		// 小数点以下の桁数
//
//	const float INIT_MOVE_WIDTH = 30.0f;		// 移動範囲の初期幅
//	const float INIT_MOVE_DEPTH = 30.0f;		// 移動範囲の初期奥行
//	const float INIT_CHASE_WIDTH = 30.0f;		// 追跡範囲の初期幅
//	const float INIT_CHASE_DEPTH = 30.0f;		// 追跡範囲の初期奥行
//
//	const float ROT_MOVE = 0.02f;	// 向きの移動量
//}
//
////************************************************************
////	親クラス [CEditEnemyAttack] のメンバ関数
////************************************************************
////============================================================
////	コンストラクタ
////============================================================
//CEditEnemyAttack::CEditEnemyAttack(CEditStage* pEditor) : CEditorObject(pEditor)
//{
//#if _DEBUG
//
//	// メンバ変数をクリア
//	m_pEnemy = nullptr;	// 攻撃敵情報
//	m_bSave = false;	// 保存状況
//	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// 攻撃敵配置情報
//
//#endif	// _DEBUG
//}
//
////============================================================
////	デストラクタ
////============================================================
//CEditEnemyAttack::~CEditEnemyAttack()
//{
//#if _DEBUG
//#endif	// _DEBUG
//}
//
////============================================================
////	初期化処理
////============================================================
//HRESULT CEditEnemyAttack::Init(void)
//{
//#if _DEBUG
//
//	// メンバ変数を初期化
//	m_pEnemy = nullptr;								// 情報
//	m_infoCreate.type = (CEnemyAttack::EType)0;		// 種類
//	m_infoCreate.fMoveWidth = INIT_MOVE_WIDTH;		// 移動範囲幅
//	m_infoCreate.fMoveDepth = INIT_MOVE_DEPTH;		// 移動範囲奥行
//	m_infoCreate.fChaseWidth = INIT_CHASE_WIDTH;	// 追跡範囲幅
//	m_infoCreate.fChaseDepth = INIT_CHASE_DEPTH;	// 追跡範囲奥行
//	m_bSave = false;								// 保存状況
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
//	// 攻撃敵の生成
//	D3DXCOLOR colActor = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// 地面色
//	m_pEnemy = CEnemyAttack::Create
//	( // 引数
//		GetVec3Position(),			// 位置
//		GetVec3Rotation(),			// 向き
//		m_infoCreate.type,			// 種類
//		m_infoCreate.fMoveWidth,	// 移動幅
//		m_infoCreate.fMoveDepth,	// 移動奥行
//		m_infoCreate.fChaseWidth,	// 追跡幅
//		m_infoCreate.fChaseDepth	// 追跡奥行
//	);
//	if (m_pEnemy == nullptr)
//	{ // 生成に失敗した場合
//
//		// 失敗を返す
//		assert(false);
//		return E_FAIL;
//	}
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
//
////============================================================
////	終了処理
////============================================================
//void CEditEnemyAttack::Uninit(void)
//{
//#if _DEBUG
//
//	// 親クラスの終了
//	CEditorObject::Uninit();
//
//	// 攻撃敵の色の全初期化
//	InitAllColorActor();
//
//	// 攻撃敵の終了
//	SAFE_UNINIT(m_pEnemy);
//
//#endif	// _DEBUG
//}
//
////============================================================
////	更新処理
////============================================================
//void CEditEnemyAttack::Update(void)
//{
//#if _DEBUG
//
//	// 親クラスの更新
//	CEditorObject::Update();
//
//	// 種類の変更
//	ChangeType();
//
//	// 攻撃敵の生成
//	CreateActor();
//
//	// 攻撃敵の破棄
//	ReleaseActor();
//
//	// 位置を反映
//	m_pEnemy->SetVec3Position(GetVec3Position());
//
//#endif	// _DEBUG
//}
//
////============================================================
////	保存状況取得処理
////============================================================
//bool CEditEnemyAttack::IsSave(void)
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
//void CEditEnemyAttack::SaveInfo(void)
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
//void CEditEnemyAttack::LoadInfo(void)
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
//void CEditEnemyAttack::DrawDebugControl(void)
//{
//#if _DEBUG
//
//	// 操作表示の描画
//	CEditorObject::DrawDebugControl();
//
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
//void CEditEnemyAttack::DrawDebugInfo(void)
//{
//#if _DEBUG
//
//	// 情報表示の描画
//	CEditorObject::DrawDebugInfo();
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, "%d：[種類]\n", m_infoCreate.type);
//
//#endif	// _DEBUG
//}
//
////============================================================
////	種類変更の更新処理
////============================================================
//void CEditEnemyAttack::ChangeType(void)
//{
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
//
//	// 種類を変更
//	if (pKeyboard->IsTrigger(KEY_TYPE))
//	{
//		m_infoCreate.type = (CEnemyAttack::EType)((m_infoCreate.type + 1) % CEnemyAttack::TYPE_MAX);
//
//		// モデルを生成し直す
//		m_pEnemy->Uninit();
//		m_pEnemy = CEnemyAttack::Create
//		(
//			GetVec3Position(),			// 位置
//			GetVec3Rotation(),			// 向き
//			m_infoCreate.type,			// 種類
//			m_infoCreate.fMoveWidth,	// 移動幅
//			m_infoCreate.fMoveDepth,	// 移動奥行
//			m_infoCreate.fChaseWidth,	// 追跡幅
//			m_infoCreate.fChaseDepth	// 追跡奥行
//		);
//	}
//}
//
////============================================================
////	攻撃敵の生成処理
////============================================================
//void CEditEnemyAttack::CreateActor(void)
//{
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
//
//	// 攻撃敵を配置
//	if (pKeyboard->IsTrigger(KEY_CREATE))
//	{
//		//----------------------------------------------------
//		//	攻撃敵の情報を配置用に変更
//		//----------------------------------------------------
//		// 自動更新・自動描画をONにする
//		m_pEnemy->SetEnableUpdate(true);
//		m_pEnemy->SetEnableDraw(true);
//
//		// 未保存を設定
//		m_bSave = false;
//
//		//----------------------------------------------------
//		//	新しい攻撃敵の生成
//		//----------------------------------------------------
//		// 攻撃敵の生成
//		m_pEnemy = CEnemyAttack::Create
//		( // 引数
//			GetVec3Position(),			// 位置
//			GetVec3Rotation(),			// 向き
//			m_infoCreate.type,			// 種類
//			m_infoCreate.fMoveWidth,	// 移動幅
//			m_infoCreate.fMoveDepth,	// 移動奥行
//			m_infoCreate.fChaseWidth,	// 追跡幅
//			m_infoCreate.fChaseDepth	// 追跡奥行
//		);
//		assert(m_pEnemy != nullptr);
//	}
//}
//
////============================================================
////	攻撃敵の破棄処理
////============================================================
//void CEditEnemyAttack::ReleaseActor(void)
//{
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
//	bool bRelease = false;	// 破棄状況
//
//	// 攻撃敵を削除
//	if (pKeyboard->IsTrigger(KEY_RELEASE))
//	{
//		// 破棄する状態を設定
//		bRelease = true;
//	}
//
//	// 攻撃敵の削除判定
//	DeleteCollisionActor(bRelease);
//}
//
////============================================================
////	攻撃敵の削除判定
////============================================================
//void CEditEnemyAttack::DeleteCollisionActor(const bool bRelase)
//{
//	CListManager<CEnemyAttack>* pListManager = CEnemyAttack::GetList();	// 攻撃敵リストマネージャー
//	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
//	std::list<CEnemyAttack*> listActor = pListManager->GetList();	// 攻撃敵リスト情報
//
//	D3DXVECTOR3 posEdit = GetVec3Position();	// エディットの位置
//	for (auto& rList : listActor)
//	{ // 攻撃敵数分繰り返す
//
//		// 同じアドレスだった場合次へ
//		if (rList == m_pEnemy) { continue; }
//
//		D3DXVECTOR3 posOther = rList->GetVec3Position();	// 対象の地面位置
//		D3DXVECTOR3 scaleThis = VEC3_ZERO;	// 自身の大きさ
//		D3DXVECTOR3 scaleOther = VEC3_ZERO;	// 対象の大きさ
//
//		// 自身の大きさを設定
//		D3DXVECTOR3 scaleThisActor = m_pEnemy->GetVec3Scaling();	// 自身の地面の大きさ
//		scaleThis.x = scaleThisActor.x;	// 判定サイズXを設定
//		scaleThis.y = scaleThisActor.y;	// 判定サイズYを設定
//		scaleThis.z = scaleThisActor.z;	// 判定サイズZを設定
//		scaleThis *= 0.5f;				// 判定サイズを半分に
//
//		// 対象の大きさを設定
//		D3DXVECTOR3 scaleOtherActor = rList->GetVec3Scaling();		// 対象の地面の大きさ
//		scaleOther.x = scaleOtherActor.x;	// 判定サイズXを設定
//		scaleOther.y = scaleOtherActor.y;	// 判定サイズYを設定
//		scaleOther.z = scaleOtherActor.z;	// 判定サイズZを設定
//		scaleOther *= 0.5f;				// 判定サイズを半分に
//
//		// 矩形の当たり判定
//		if (collision::Box3D
//		( // 引数
//			posEdit,	// 判定位置
//			posOther,	// 判定目標位置
//			scaleThis,	// 判定サイズ(右・上・後)
//			scaleThis,	// 判定サイズ(左・下・前)
//			scaleOther,	// 判定目標サイズ(右・上・後)
//			scaleOther	// 判定目標サイズ(左・下・前)
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
//				rList->SetAllMaterial(material::Red());
//			}
//		}
//		else
//		{ // 判定外だった場合
//
//			// 通常色を設定
//			rList->ResetMaterial();
//		}
//	}
//}
//
////============================================================
////	攻撃敵の色の全初期化処理
////============================================================
//void CEditEnemyAttack::InitAllColorActor(void)
//{
//	CListManager<CEnemyAttack>* pListManager = CEnemyAttack::GetList();	// 攻撃敵リストマネージャー
//	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
//	std::list<CEnemyAttack*> listActor = pListManager->GetList();	// 攻撃敵リスト情報
//
//	for (auto& rList : listActor)
//	{ // 攻撃敵数分繰り返す
//
//		// 同じアドレスだった場合次へ
//		if (rList == m_pEnemy) { continue; }
//
//		// 通常色を設定
//		rList->ResetMaterial();
//	}
//}
//
////============================================================
////	保存処理
////============================================================
//HRESULT CEditEnemyAttack::Save(void)
//{
//#if _DEBUG
//
//	// 地面のリストを取得
//	CListManager<CEnemyAttack>* pListManager = CEnemyAttack::GetList();	// リストマネージャー
//	std::list<CEnemyAttack*> listActor;	// 地面リスト
//	if (pListManager != nullptr)
//	{ // リストマネージャーが生成されている場合
//
//		// リストを取得
//		listActor = pListManager->GetList();
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
//	file << "#	攻撃敵配置のセーブデータ [save_actor.txt]" << std::endl;
//	file << "#	Author : 藤田 勇一" << std::endl;
//	file << "#" << std::endl;
//	file << "#==============================================================================" << std::endl;
//	file << "# この行から下をコピーし [enemyAttack.txt] に張り付け\n" << std::endl;
//
//	// 攻撃敵の色の全初期化
//	InitAllColorActor();
//
//	// 小数点書き出しの方法を指定
//	file << std::fixed << std::setprecision(DIGIT_FLOAT);
//
//	// 読み込み開始文字列を書き出し
//	file << "STAGE_ENEMYATTCKSET\n" << std::endl;
//
//	for (const auto& rList : listActor)
//	{ // 地面の総数分繰り返す
//
//		// 同じアドレスだった場合次へ
//		if (rList == m_pEnemy) { continue; }
//
//		// 書き出す情報を取得
//		ENavType navType;
//		CEnemyAttack::EType type = rList->GetType();	// 種類
//		D3DXVECTOR3 pos = rList->GetVec3Position();		// 位置
//		D3DXVECTOR3 rot = rList->GetVec3Rotation();		// 向き
//		D3DXVECTOR3 scale = rList->GetVec3Scaling();	// 大きさ
//		float fMoveWidth = rList->GetNavigation();
//
//		if (typeid(*rList->GetNavigation()) == typeid(CEnemyNavRandom))
//		{ // ランダムナビの場合
//
//			// ナビの種類を設定
//			navType = NAVTYPE_RANDOM;
//		}
//		else
//		{ // 上記以外
//
//			// ナビの種類を設定
//			navType = NAVTYPE_STREET;
//		}
//
//		// 向きを360度に変換
//		rot = D3DXToDegree(rot);
//
//			m_infoCreate.fMoveWidth,	// 移動幅
//			m_infoCreate.fMoveDepth,	// 移動奥行
//			m_infoCreate.fChaseWidth,	// 追跡幅
//			m_infoCreate.fChaseDepth	// 追跡奥行
//
//		// 情報を書き出し
//		file << "	ENEMYATTCKSET" << std::endl;
//		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
//		file << "		ROT		= " << rot.x << " " << rot.y << " " << rot.z << std::endl;
//		file << "		TYPE	= " << type << std::endl;
//		file << "		SCALE	= " << scale.x << " " << scale.y << " " << scale.z << std::endl;
//		file << "	END_ENEMYATTCKSET\n" << std::endl;
//	}
//
//	// 読み込み終了文字列を書き出し
//	file << "END_STAGE_ENEMYATTCKSET" << std::endl;
//
//	// 攻撃敵の削除判定
//	DeleteCollisionActor(false);
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
