//============================================================
//
//	ギミック処理 [gimmick.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "gimmick.h"

#include "gimmick_jumptable.h"
#include "gimmick_step.h"
#include "gimmick_post.h"
#include "gimmick_multi.h"
#include "gimmick_jumpoff.h"
#include "gimmick_heavydoor.h"
#include "gimmick_bridge.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 2;				// ギミック範囲ポリゴンの優先順位
	const int INIT_NUM_ACTIVE = -1;		// 発動可能人数の初期値
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CGimmick>* CGimmick::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CGimmick] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGimmick::CGimmick() : CObject3D(CObject::LABEL_GIMMICK, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
m_type(TYPE_JUMPTABLE),			// 種類
m_nNumActive(INIT_NUM_ACTIVE)	// 発動可能な分身の数
{

}

//============================================================
//	デストラクタ
//============================================================
CGimmick::~CGimmick()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGimmick::Init(void)
{
	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CGimmick>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// デバッグの時以外は描画しない
#ifndef _DEBUG
	SetEnableDraw(false);
#endif
	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGimmick::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクト3Dの終了
	CObject3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CGimmick::Update(const float fDeltaTime)
{
	// オブジェクト3Dの更新
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CGimmick::Draw(CShader* pShader)
{
	// オブジェクト3Dの描画
	CObject3D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CGimmick* CGimmick::Create
(
	const D3DXVECTOR3& rPos,		// 位置
	const EAngle& eAngle,			// 角度
	const D3DXVECTOR3& rSize,		// サイズ
	const EType type,				// 種類
	const int nNumActive			// 発動可能な分身の数
)
{
	// 分身のUIの生成
	CGimmick* pGimmick = nullptr;

	switch (type)
	{
	case CGimmick::TYPE_JUMPTABLE:		// ジャンプ台

		pGimmick = new CGimmickJumpTable;

		break;

	case CGimmick::TYPE_STEP:			// 踏み台

		pGimmick = new CGimmickStep;

		break;

	case CGimmick::TYPE_POST:			// 設置

		pGimmick = new CGimmickPost;

		break;

	case CGimmick::TYPE_JUMPOFF:		// 飛び降り

		pGimmick = new CGimmickJumpOff;

		break;

	case CGimmick::TYPE_HEAVYDOOR:		// 重い扉

		pGimmick = new CGimmickHeavyDoor;

		break;

	case CGimmick::TYPE_BRIDGE:		// 橋

		pGimmick = new CGimmickBridge;

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	if (pGimmick == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 分身のUIの初期化
		if (FAILED(pGimmick->Init()))
		{ // 初期化に失敗した場合

			// 分身のUIの破棄
			SAFE_DELETE(pGimmick);
			return nullptr;
		}

		// 種類を設定
		pGimmick->m_type = type;

		// 発動可能な分身の数を設定
		pGimmick->m_nNumActive = nNumActive;

		// 向きを設定
		pGimmick->m_eAngle = eAngle;

		// 位置を設定
		pGimmick->SetVec3Position(rPos + D3DXVECTOR3(0.0f, 0.01f, 0.0f));

		// サイズを設定
		pGimmick->SetVec3Sizing(rSize);

		// 自身を描画しない
		//pGimmick->SetEnableDraw(false);

		// 確保したアドレスを返す
		return pGimmick;
	}
}

//============================================================
// リスト取得
//============================================================
CListManager<CGimmick>* CGimmick::GetList(void)
{
	// リスト構造を返す
	return m_pList;
}

//============================================================
// セットアップ処理
//============================================================
HRESULT CGimmick::LoadSetup(const char* pPass)
{
	D3DXVECTOR3 pos		= VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 size	= VEC3_ZERO;	// 大きさの代入用
	int nAngle	= 0;	// 向きの代入用
	int nType	= 0;	// 種類の代入用
	int nNumAct	= 0;	// 必要人数の代入用

	// ファイルを開く
	std::ifstream file(pPass);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ギミックセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

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
		else if (str == "STAGE_GIMMICKSET")
		{
			do { // END_STAGE_GIMMICKSETを読み込むまでループ

				// 文字列を読み込む
				file >> str;

				if (str.front() == '#')
				{ // コメントアウトされている場合

					// 一行全て読み込む
					std::getline(file, str);
				}

				// 通常ギミックの読込
				else if (str == "GIMMICKSET")
				{
					do { // END_GIMMICKSETを読み込むまでループ

						// 文字列を読み込む
						file >> str;

						if (str == "POS")
						{
							file >> str;	// ＝を読込

							// 位置を読込
							file >> pos.x;
							file >> pos.y;
							file >> pos.z;
						}
						else if (str == "ANGLE")
						{
							file >> str;	// ＝を読込
							file >> nAngle;	// 向きを読込
						}
						else if (str == "SIZE")
						{
							file >> str;	// ＝を読込

							// 大きさを読込
							file >> size.x;
							file >> size.y;
							file >> size.z;
						}
						else if (str == "TYPE")
						{
							file >> str;	// ＝を読込
							file >> nType;	// 種類を読込
						}
						else if (str == "NUMACT")
						{
							file >> str;		// ＝を読込
							file >> nNumAct;	// 必要人数を読込
						}
					} while (str != "END_GIMMICKSET");	// END_GIMMICKSETを読み込むまでループ

					// ギミックの生成
					if (CGimmick::Create(pos, (EAngle)nAngle, size, (CGimmick::EType)nType, nNumAct) == nullptr)
					{ // 確保に失敗した場合

						// 失敗を返す
						assert(false);
						return E_FAIL;
					}
				}

				// ボタンギミックの読込
				else if (str == "BUTTON_GIMMICKSET")
				{
					// 座標と向きの変数
					D3DXVECTOR3 posMulch = VEC3_ZERO;
					int angle = ANGLE_0;

					// 文字列を読み込む
					file >> str;

					if (str.front() == '#')
					{ // コメントアウトされている場合

						// 一行全て読み込む
						std::getline(file, str);
					}
					else if (str == "MULCHSET")
					{
						do
						{ // END_MULCHSETを読み込むまでループ

							// 文字列を読み込む
							file >> str;

							if (str == "POS")
							{
								file >> str;	// ＝を読込

								// 位置を読込
								file >> posMulch.x;
								file >> posMulch.y;
								file >> posMulch.z;
							}
							else if (str == "ANGLE")
							{
								file >> str;	// ＝を読込

								// 向きを読込
								file >> angle;
							}
						} while (str != "END_MULCHSET");	// END_MULCHSETを読み込むまでループ
					}

					std::vector<CGimmickMulti::SButton> vecButton;	// ボタン情報配列
					do { // END_BUTTON_GIMMICKSETを読み込むまでループ

						// 文字列を読み込む
						file >> str;

						if (str.front() == '#')
						{ // コメントアウトされている場合

							// 一行全て読み込む
							std::getline(file, str);
						}
						else if (str == "BUTTONSET")
						{
							do { // END_BUTTONSETを読み込むまでループ

								// 文字列を読み込む
								file >> str;

								if (str == "POS")
								{
									file >> str;	// ＝を読込

									// 位置を読込
									file >> pos.x;
									file >> pos.y;
									file >> pos.z;
								}
								else if (str == "SIZE")
								{
									file >> str;	// ＝を読込

									// 大きさを読込
									file >> size.x;
									file >> size.y;
									file >> size.z;
								}
							} while (str != "END_BUTTONSET");	// END_BUTTONSETを読み込むまでループ

							// ボタン情報を最後尾に追加
							vecButton.push_back(CGimmickMulti::SButton(pos, size));
						}
					} while (str != "END_BUTTON_GIMMICKSET");	// END_BUTTON_GIMMICKSETを読み込むまでループ

					// ギミックボタン統括クラスの生成
					if (CGimmickMulti::Create(posMulch, (EAngle)angle, vecButton) == nullptr)
					{ // 確保に失敗した場合

						// 失敗を返す
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_GIMMICKSET");	// END_STAGE_GIMMICKSETを読み込むまでループ
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}
