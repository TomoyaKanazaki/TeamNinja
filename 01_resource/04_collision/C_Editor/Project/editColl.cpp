//============================================================
//
//	エディット当たり判定処理 [editColl.cpp]
//	Author：藤田勇一
//
//============================================================
#if _DEBUG 

//************************************************************
//	インクルードファイル
//************************************************************
#include "editColl.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "collSphere.h"
#include "debugCollSphere.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *SAVE_TXT = "data\\TXT\\save_collision.txt";	// 判定保存テキスト

	const int	SUB_DRAWINFO	= 2;		// 判定情報表示の開始ID減算量
	const int	NUM_DRAWINFO	= 5;		// 判定情報表示の総数
	const float INIT_COLL_RAD	= 20.0f;	// 生成時の判定半径

	const float MOVE_OFFSET	= 2.5f;		// オフセット移動量
	const float MOVE_RAD	= 1.0f;		// 半径変更量
	const float MIN_RAD		= 1.0f;		// 最小半径
	const float MAX_RAD		= 1000.0f;	// 最大半径
}

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_DOUBLE		(DIK_LCONTROL)	// 二重化キー
#define NAME_DOUBLE		("LCTRL")		// 二重化表示
#define KEY_TRIGGER		(DIK_LSHIFT)	// トリガー化キー
#define NAME_TRIGGER	("LSHIFT")		// トリガー化表示

#define KEY_SAVE	(DIK_F5)	// 保存キー
#define NAME_SAVE	("F5")		// 保存表示
#define KEY_LOAD	(DIK_F6)	// 読込キー
#define NAME_LOAD	("F6")		// 読込表示

#define KEY_ADD_COLL	(DIK_0)		// 判定追加キー
#define NAME_ADD_COLL	("0")		// 判定追加表示
#define KEY_SUB_COLL	(DIK_9)		// 判定削除キー
#define NAME_SUB_COLL	("9")		// 判定削除表示
#define KEY_UP_PARTS	(DIK_UP)	// パーツ変更キー
#define NAME_UP_PARTS	("↑")		// パーツ変更表示
#define KEY_DOWN_PARTS	(DIK_DOWN)	// パーツ変更キー
#define NAME_DOWN_PARTS	("↓")		// パーツ変更表示
#define KEY_UP_COLL		(DIK_LEFT)	// 判定変更キー
#define NAME_UP_COLL	("←")		// 判定変更表示
#define KEY_DOWN_COLL	(DIK_RIGHT)	// 判定変更キー
#define NAME_DOWN_COLL	("→")		// 判定変更表示

#define KEY_FAR		(DIK_W)	// 奥移動キー
#define NAME_FAR	("W")	// 奥移動表示
#define KEY_NEAR	(DIK_S)	// 手前移動キー
#define NAME_NEAR	("S")	// 手前移動表示
#define KEY_RIGHT	(DIK_D)	// 右移動キー
#define NAME_RIGHT	("D")	// 右移動表示
#define KEY_LEFT	(DIK_A)	// 左移動キー
#define NAME_LEFT	("A")	// 左移動表示
#define KEY_UP		(DIK_E)	// 上移動キー
#define NAME_UP		("E")	// 上移動表示
#define KEY_DOWN	(DIK_Q)	// 下移動キー
#define NAME_DOWN	("Q")	// 下移動表示

#define KEY_RAD_UP		(DIK_R)	// 判定拡大キー
#define NAME_RAD_UP		("R")	// 判定拡大表示
#define KEY_RAD_DOWN	(DIK_F)	// 判定縮小キー
#define NAME_RAD_DOWN	("F")	// 判定縮小表示

//************************************************************
//	子クラス [CEditColl] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditColl::CEditColl() :
	m_nCurSelectParts	(0),	// 現在の選択パーツ
	m_nOldSelectParts	(0),	// 前回の選択パーツ
	m_nCurSelectColl	(0),	// 現在の選択判定
	m_nOldSelectColl	(0)		// 前回の選択判定

{

}

//============================================================
//	デストラクタ
//============================================================
CEditColl::~CEditColl()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditColl::Init(void)
{
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報

	// メンバ変数を初期化
	m_nCurSelectParts = 0;	// 現在の選択パーツ
	m_nOldSelectParts = 0;	// 前回の選択パーツ
	m_nCurSelectColl = 0;	// 現在の選択判定
	m_nOldSelectColl = 0;	// 前回の選択判定

	// エディットの初期化
	if (FAILED(CEdit::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キーを初期化 (Tポーズにする)
	pPlayer->GetMotion()->Reset();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEditColl::Uninit(void)
{
	// マテリアルを再設定
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報
	pPlayer->ResetMaterial();

	// 前回の選択判定の色を元に戻す
	CCollSphere *pOldColl = pPlayer->GetCollision(m_nOldSelectParts);	// 前回の円判定情報
	std::vector<CCollSphere::SInfo> oldVector = pOldColl->GetVector();	// 前回の円判定配列
	if (oldVector.size() > 0)
	{ // 前回パーツの配列が存在する場合

		CCollSphere::SInfo oldInfo = pOldColl->GetInfo(m_nOldSelectColl);	// 前回の判定情報
		oldInfo.pVisual->SetColor(debugCollSphere::DEFAULT_COL);	// 色を設定
		pOldColl->SetInfo(oldInfo, m_nCurSelectColl);				// 判定情報を反映
	}

	// エディットの終了
	CEdit::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEditColl::Update(void)
{
	// パーツ選択の更新
	SelectParts();

	// 判定追加の更新
	AddColl();

	// 判定選択の更新
	SelectColl();

	// 判定削除の更新
	SubColl();

	// 判定オフセットの更新
	UpdateCollOffset();

	// 判定半径の更新
	UpdateCollRadius();

	// 判定情報の更新
	UpdateCollInfo();

	// 判定保存
	SaveColl();

	// 判定読込
	LoadColl();

	// エディットの更新
	CEdit::Update();
}

//============================================================
//	描画処理
//============================================================
void CEditColl::Draw(void)
{
	// エディットの描画
	CEdit::Draw();
}

//============================================================
//	エディット操作表示
//============================================================
void CEditColl::DrawEditControl(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグの情報

	// エディット操作表示
	CEdit::DrawEditControl();

	pDebug->Print(CDebugProc::POINT_RIGHT, "保存：[%s]\n", NAME_SAVE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "読込：[%s+%s]\n", NAME_LOAD, NAME_DOUBLE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "パーツ変更：[%s/%s]\n", NAME_UP_PARTS, NAME_DOWN_PARTS);
	pDebug->Print(CDebugProc::POINT_RIGHT, "判定変更：[%s/%s]\n", NAME_UP_COLL, NAME_DOWN_COLL);
	pDebug->Print(CDebugProc::POINT_RIGHT, "判定追加/削除：[%s/%s]\n", NAME_ADD_COLL, NAME_SUB_COLL);
	pDebug->Print(CDebugProc::POINT_RIGHT, "オフセット移動：[%s/%s/%s/%s/%s/%s+%s]\n", NAME_FAR, NAME_LEFT, NAME_NEAR, NAME_RIGHT, NAME_UP, NAME_DOWN, NAME_TRIGGER);
	pDebug->Print(CDebugProc::POINT_RIGHT, "半径拡縮：[%s/%s+%s]\n", NAME_RAD_UP, NAME_RAD_DOWN, NAME_TRIGGER);
}

//============================================================
//	エディット情報表示
//============================================================
void CEditColl::DrawEditData(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグの情報
	CPlayer *pPlayer = CScene::GetPlayer();							// プレイヤー情報
	CCollSphere *pColl = pPlayer->GetCollision(m_nCurSelectParts);	// 円判定情報
	std::vector<CCollSphere::SInfo> vector = pColl->GetVector();	// 円判定配列

	// エディット情報表示
	CEdit::DrawEditData();

	pDebug->Print(CDebugProc::POINT_RIGHT, "%d：[総パーツ数]\n", pPlayer->GetNumModel());
	pDebug->Print(CDebugProc::POINT_RIGHT, "%d：[選択パーツ]\n", m_nCurSelectParts);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%d：[総判定数]\n", vector.size());
	pDebug->Print(CDebugProc::POINT_RIGHT, "%d：[選択判定]\n", m_nCurSelectColl);
	pDebug->Print(CDebugProc::POINT_RIGHT, "\n");

	if (vector.size() > 0)
	{ // 配列が存在する場合

		// 配列の先頭イテレーター
		auto info = vector.begin();

		// 判定情報表示の先頭インデックス
		int nDrawColl = m_nCurSelectColl - SUB_DRAWINFO;
		useful::LimitMinNum(nDrawColl, 0);	// 0未満の場合補正

		// イテレーターを表示インデックス分進める
		info += nDrawColl;

		for (int nCntColl = 0; nCntColl < NUM_DRAWINFO; nCntColl++)
		{ // 判定数分繰り返す

			// 配列の最後尾の場合抜ける
			if (info == vector.end()) { break; }

			// 判定番号
			pDebug->Print(CDebugProc::POINT_RIGHT, "********************* [判定番号：%d] ", nCntColl + nDrawColl);
			pDebug->Print(CDebugProc::POINT_RIGHT, (nCntColl + nDrawColl == m_nCurSelectColl) ? "<>\n" : "**\n");	// 選択中の判定なら強調表示

			// 判定情報
			pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f %f：[オフセット]\n", info->offset.x, info->offset.y, info->offset.z);
			pDebug->Print(CDebugProc::POINT_RIGHT, "%f：[半径]\n", info->fRadius);

			// イテレーターを進める
			info++;
		}
	}
}

//============================================================
//	パーツ選択の更新処理
//============================================================
void CEditColl::SelectParts(void)
{
	CInputKeyboard *pKey = GET_INPUTKEY;	// キーボード情報
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報
	CCollSphere *pOldColl = pPlayer->GetCollision(m_nOldSelectParts);	// 前回の円判定情報
	std::vector<CCollSphere::SInfo> oldVector = pOldColl->GetVector();	// 前回の円判定配列
	CCollSphere *pCurColl = pPlayer->GetCollision(m_nCurSelectParts);	// 現在の円判定情報
	std::vector<CCollSphere::SInfo> curVector = pCurColl->GetVector();	// 現在の円判定配列

	// 前回の選択パーツを設定
	m_nOldSelectParts = m_nCurSelectParts;

	// 選択パーツ変更
	if (pKey->IsTrigger(KEY_UP_PARTS))
	{
		int nNumParts = pPlayer->GetNumModel();	// パーツの総数
		m_nCurSelectParts = (m_nCurSelectParts + (nNumParts - 1)) % nNumParts;
	}
	else if (pKey->IsTrigger(KEY_DOWN_PARTS))
	{
		int nNumParts = pPlayer->GetNumModel();	// パーツの総数
		m_nCurSelectParts = (m_nCurSelectParts + 1) % nNumParts;
	}

	// 前回の選択を補正
	int nOldMaxSelect = (int)pOldColl->GetVector().size() - 1;	// 最高選択可能インデックス
	useful::LimitMinNum(nOldMaxSelect, 0);						// 0より小さい場合補正
	useful::LimitNum(m_nOldSelectColl, 0, nOldMaxSelect);		// 前回選択の補正

	// 現在の選択を補正
	int nCurMaxSelect = (int)pCurColl->GetVector().size() - 1;	// 最高選択可能インデックス
	useful::LimitMinNum(nCurMaxSelect, 0);						// 0より小さい場合補正
	useful::LimitNum(m_nCurSelectColl, 0, nCurMaxSelect);		// 現在選択の補正

	// マテリアルを再設定
	pPlayer->ResetMaterial();

	// 選択パーツのマテリアルを変更
	pPlayer->SetPartsMaterial(material::GlowGreen(), m_nCurSelectParts);
}

//============================================================
//	判定追加の更新処理
//============================================================
void CEditColl::AddColl(void)
{
	CInputKeyboard *pKey = GET_INPUTKEY;	// キーボード情報
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報

	// 判定追加
	if (pKey->IsTrigger(KEY_ADD_COLL))
	{
		CCollSphere *pColl = pPlayer->GetCollision(m_nCurSelectParts);	// 判定情報
		pColl->AddColl(VEC3_ZERO, INIT_COLL_RAD);
	}
}

//============================================================
//	判定削除の更新処理
//============================================================
void CEditColl::SubColl(void)
{
	CInputKeyboard *pKey = GET_INPUTKEY;	// キーボード情報
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報
	CCollSphere *pColl = pPlayer->GetCollision(m_nCurSelectParts);	// 円判定情報
	std::vector<CCollSphere::SInfo> vector = pColl->GetVector();	// 円判定配列

	// 判定削除
	if (vector.size() > 0)
	{ // 配列が存在する場合

		if (pKey->IsTrigger(KEY_SUB_COLL))
		{
			// 選択中の判定を削除
			pColl->SubColl(m_nCurSelectColl);

			// 削除後の配列の要素数より大きい値の場合補正
			int nMaxSelect = (int)pColl->GetVector().size() - 1;	// 最高選択可能インデックス
			useful::LimitMinNum(nMaxSelect, 0);						// 0より小さい場合補正
			useful::LimitNum(m_nCurSelectColl, 0, nMaxSelect);		// 選択中の要素番号を補正
		}
	}
}

//============================================================
//	判定選択の更新処理
//============================================================
void CEditColl::SelectColl(void)
{
	CInputKeyboard *pKey = GET_INPUTKEY;	// キーボード情報
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報

	// 前回の選択判定を設定
	m_nOldSelectColl = m_nCurSelectColl;

	CCollSphere *pOldColl = pPlayer->GetCollision(m_nOldSelectParts);	// 前回の円判定情報
	std::vector<CCollSphere::SInfo> oldVector = pOldColl->GetVector();	// 前回の円判定配列
	if (oldVector.size() > 0)
	{ // 前回パーツの配列が存在する場合

		// 前回の選択を補正
		int nOldMaxSelect = (int)pOldColl->GetVector().size() - 1;	// 最高選択可能インデックス
		useful::LimitMinNum(nOldMaxSelect, 0);						// 0より小さい場合補正
		useful::LimitNum(m_nOldSelectColl, 0, nOldMaxSelect);		// 前回選択の補正

		// 前回の選択判定の色を元に戻す
		CCollSphere::SInfo oldInfo = pOldColl->GetInfo(m_nOldSelectColl);	// 前回の判定情報
		oldInfo.pVisual->SetColor(debugCollSphere::DEFAULT_COL);	// 色を設定
		pOldColl->SetInfo(oldInfo, m_nOldSelectColl);				// 判定情報を反映
	}

	CCollSphere *pCurColl = pPlayer->GetCollision(m_nCurSelectParts);	// 現在の円判定情報
	std::vector<CCollSphere::SInfo> curVector = pCurColl->GetVector();	// 現在の円判定配列
	if (curVector.size() > 0)
	{ // 現在パーツの配列が存在する場合

		// 選択判定変更
		if (pKey->IsTrigger(KEY_UP_COLL))
		{
			int nNumColl = curVector.size();	// 判定の総数
			m_nCurSelectColl = (m_nCurSelectColl + (nNumColl - 1)) % nNumColl;
		}
		else if (pKey->IsTrigger(KEY_DOWN_COLL))
		{
			int nNumColl = curVector.size();	// 判定の総数
			m_nCurSelectColl = (m_nCurSelectColl + 1) % nNumColl;
		}

		// 現在の選択を補正
		int nCurMaxSelect = (int)pCurColl->GetVector().size() - 1;	// 最高選択可能インデックス
		useful::LimitMinNum(nCurMaxSelect, 0);						// 0より小さい場合補正
		useful::LimitNum(m_nCurSelectColl, 0, nCurMaxSelect);		// 現在選択の補正

		// 現在の選択判定の色を設定
		CCollSphere::SInfo curInfo = pCurColl->GetInfo(m_nCurSelectColl);	// 現在の判定情報
		curInfo.pVisual->SetColor(debugCollSphere::CHOICE_COL);	// 色を設定
		pCurColl->SetInfo(curInfo, m_nCurSelectColl);			// 判定情報を反映
	}
}

//============================================================
//	判定オフセットの更新処理
//============================================================
void CEditColl::UpdateCollOffset(void)
{
	CInputKeyboard *pKey = GET_INPUTKEY;	// キーボード情報
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報
	CCollSphere *pColl = pPlayer->GetCollision(m_nCurSelectParts);	// 円判定情報
	std::vector<CCollSphere::SInfo> vector = pColl->GetVector();	// 円判定配列

	// 選択判定変更
	if (vector.size() > 0)
	{ // 配列が存在する場合

		CCollSphere::SInfo curInfo = pColl->GetInfo(m_nCurSelectColl);	// 判定情報
		bool bTrigger = pKey->IsPress(KEY_TRIGGER);	// トリガー化

		if ((bTrigger) ? pKey->IsTrigger(KEY_FAR) : pKey->IsPress(KEY_FAR))
		{
			curInfo.offset.z += MOVE_OFFSET;
		}
		else if ((bTrigger) ? pKey->IsTrigger(KEY_NEAR) : pKey->IsPress(KEY_NEAR))
		{
			curInfo.offset.z -= MOVE_OFFSET;
		}
		else if ((bTrigger) ? pKey->IsTrigger(KEY_RIGHT) : pKey->IsPress(KEY_RIGHT))
		{
			curInfo.offset.x += MOVE_OFFSET;
		}
		else if ((bTrigger) ? pKey->IsTrigger(KEY_LEFT) : pKey->IsPress(KEY_LEFT))
		{
			curInfo.offset.x -= MOVE_OFFSET;
		}
		else if ((bTrigger) ? pKey->IsTrigger(KEY_UP) : pKey->IsPress(KEY_UP))
		{
			curInfo.offset.y += MOVE_OFFSET;
		}
		else if ((bTrigger) ? pKey->IsTrigger(KEY_DOWN) : pKey->IsPress(KEY_DOWN))
		{
			curInfo.offset.y -= MOVE_OFFSET;
		}

		// 判定情報を反映
		pColl->SetInfo(curInfo, m_nCurSelectColl);
	}
}

//============================================================
//	判定半径の更新処理
//============================================================
void CEditColl::UpdateCollRadius(void)
{
	CInputKeyboard *pKey = GET_INPUTKEY;	// キーボード情報
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報
	CCollSphere *pColl = pPlayer->GetCollision(m_nCurSelectParts);	// 円判定情報
	std::vector<CCollSphere::SInfo> vector = pColl->GetVector();	// 円判定配列

	// 選択判定変更
	if (vector.size() > 0)
	{ // 配列が存在する場合

		CCollSphere::SInfo curInfo = pColl->GetInfo(m_nCurSelectColl);	// 判定情報
		bool bTrigger = pKey->IsPress(KEY_TRIGGER);	// トリガー化

		if ((bTrigger) ? pKey->IsTrigger(KEY_RAD_UP) : pKey->IsPress(KEY_RAD_UP))
		{
			curInfo.fRadius += MOVE_RAD;
			useful::LimitMaxNum(curInfo.fRadius, MAX_RAD);	// 半径を補正
		}
		else if ((bTrigger) ? pKey->IsTrigger(KEY_RAD_DOWN) : pKey->IsPress(KEY_RAD_DOWN))
		{
			curInfo.fRadius -= MOVE_RAD;
			useful::LimitMinNum(curInfo.fRadius, MIN_RAD);	// 半径を補正
		}

		// 判定情報を反映
		pColl->SetInfo(curInfo, m_nCurSelectColl);
	}
}

//============================================================
//	判定情報の更新処理
//============================================================
void CEditColl::UpdateCollInfo(void)
{
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報
	for (int nCntParts = 0; nCntParts < pPlayer->GetNumModel(); nCntParts++)
	{ // パーツの最大数分繰り返す

		// 当たり判定の更新
		pPlayer->GetCollision(nCntParts)->Update();
	}
}

//============================================================
//	判定保存処理
//============================================================
void CEditColl::SaveColl(void)
{
	// セーブ操作されていない場合抜ける
	if (!GET_INPUTKEY->IsTrigger(KEY_SAVE)) { return; }

	// ファイルを書き出し形式で開く
	FILE *pFile = fopen(SAVE_TXT, "w");
	if (pFile != nullptr)
	{ // ファイルが開けた場合

		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#	判定セーブテキスト [save_collison.txt]\n");
		fprintf(pFile, "#	Author : you\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "---------->--<---------- ここから下を コピーし貼り付け ---------->--<----------\n\n");

		fprintf(pFile, "COLLISIONSET\n\n");

		CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報
		for (int nCntParts = 0; nCntParts < pPlayer->GetNumModel(); nCntParts++)
		{ // パーツの最大数分繰り返す

			CCollSphere *pColl = pPlayer->GetCollision(nCntParts);			// 円判定情報
			std::vector<CCollSphere::SInfo> vector = pColl->GetVector();	// 円判定配列
			if (vector.size() <= 0) { continue; }	// 判定が一つもない場合次のループへ

			fprintf(pFile, "	COLLSET\n\n");

			fprintf(pFile, "		PARTS = %d\n", nCntParts);
			fprintf(pFile, "		NUMCOLL = %d\n\n", (int)vector.size());

			for (auto info : vector)
			{ // 判定数分繰り返す

				fprintf(pFile, "		COLL\n");
				fprintf(pFile, "			OFFSET = %.2f %.2f %.2f\n", info.offset.x, info.offset.y, info.offset.z);
				fprintf(pFile, "			RADIUS = %.2f\n", info.fRadius);
				fprintf(pFile, "		END_COLL\n\n");
			}

			fprintf(pFile, "	END_COLLSET\n\n");
		}

		fprintf(pFile, "END_COLLISIONSET\n\n");

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "判定保存ファイルの書き出しに失敗！", "警告！", MB_ICONWARNING);
	}
}

//============================================================
//	判定読込処理
//============================================================
void CEditColl::LoadColl(void)
{
	// ロード操作されていない場合抜ける
	if (!GET_INPUTKEY->IsPress(KEY_DOUBLE)) { return; }
	if (!GET_INPUTKEY->IsTrigger(KEY_LOAD)) { return; }

	// ファイルを読み込み形式で開く
	FILE *pFile = fopen(SAVE_TXT, "r");
	if (pFile != nullptr)
	{ // ファイルが開けた場合

		CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報
		CCollSphere *pColl = nullptr;			// 円判定情報
		std::vector<CCollSphere::SInfo> vector;	// 円判定配列

		CCollSphere::SInfo info;	// 判定情報
		int nParts = 0;				// パーツ番号
		int nEnd = 0;				// テキスト読み込み終了の確認用
		char aString[MAX_STRING];	// テキストの文字列の代入用

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "COLLISIONSET") == 0)
			{ // 読み込んだ文字列が COLLISIONSET の場合

				do
				{ // 読み込んだ文字列が END_COLLISIONSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "COLLSET") == 0)
					{ // 読み込んだ文字列が COLLSET の場合

						do
						{ // 読み込んだ文字列が END_COLLSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "PARTS") == 0)
							{ // 読み込んだ文字列が PARTS の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nParts);		// パーツ番号を読み込む

								// 円判定情報を設定
								pColl = pPlayer->GetCollision(nParts);

								// 円判定配列を設定
								vector = pColl->GetVector();
								pColl->Uninit();	// 判定情報のクリア・メッシュ破棄
							}
							else if (strcmp(&aString[0], "COLL") == 0)
							{ // 読み込んだ文字列が COLL の場合

								do
								{ // 読み込んだ文字列が END_COLL ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "OFFSET") == 0)
									{ // 読み込んだ文字列が OFFSET の場合

										fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
										fscanf(pFile, "%f", &info.offset.x);	// Xオフセットを読み込む
										fscanf(pFile, "%f", &info.offset.y);	// Yオフセットを読み込む
										fscanf(pFile, "%f", &info.offset.z);	// Zオフセットを読み込む
									}
									else if (strcmp(&aString[0], "RADIUS") == 0)
									{ // 読み込んだ文字列が RADIUS の場合

										fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
										fscanf(pFile, "%f", &info.fRadius);	// 半径を読み込む
									}
								} while (strcmp(&aString[0], "END_COLL") != 0);	// 読み込んだ文字列が END_COLL ではない場合ループ

								if (pColl != nullptr)
								{
									// 判定を追加
									pColl->AddColl(info.offset, info.fRadius);
								}
							}
						} while (strcmp(&aString[0], "END_COLLSET") != 0);	// 読み込んだ文字列が END_COLLSET ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_COLLISIONSET") != 0);		// 読み込んだ文字列が END_COLLISIONSET ではない場合ループ
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "判定保存ファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}

#endif	// _DEBUG
