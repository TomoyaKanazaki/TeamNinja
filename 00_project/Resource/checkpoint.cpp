//==========================================
//
//  チェックポイント(checkpoint.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "checkpoint.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "collision.h"
#include "popupUI.h"
#include "sound.h"
#include "goal.h"
#include "scene.h"
#include "sceneGame.h"
#include "gameManager.h"

//==========================================
//  定数定義
//==========================================
namespace
{

	const float RADIUS = 50.0f;			// 半径
	const D3DXVECTOR3 OFFSET = D3DXVECTOR3(0.0f, 8.0f, 0.0f);//エフェクト用オフセット
	const D3DXVECTOR3 OFFSET_CHECKEFFECT = D3DXVECTOR3(0.0f, 80.0f, 0.0f);//チェックエフェクト用オフセット
	const int SAVE_MOTION_BLEND = 8;	// セーブモーションのブレンドカウント
}

//==========================================
//  静的メンバ変数宣言
//==========================================
int CCheckPoint::m_nNumAll = 0;
CListManager<CCheckPoint>* CCheckPoint::m_pList = nullptr;	// オブジェクトリスト

//==========================================
//  コンストラクタ
//==========================================
CCheckPoint::CCheckPoint():
	m_pos(VEC3_ZERO),
	m_bSave(false),
	m_nSaveTension(0)
{
	m_pEffectdata = NULL;
	// 総数を加算
	++m_nNumAll;
}

//==========================================
//  デストラクタ
//==========================================
CCheckPoint::~CCheckPoint()
{
	// 総数を減算
	--m_nNumAll;
}

//==========================================
//  初期化処理
//==========================================
HRESULT CCheckPoint::Init(void)
{
	// 値の初期化
	m_pos = VEC3_ZERO;
	m_bSave = false;
	m_nSaveTension = 0;

	// 自身のラベルを設定
	SetLabel(LABEL_CHECKPOINT);

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CCheckPoint>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void CCheckPoint::Uninit(void)
{

	//現在のエフェクトを削除
	if (m_pEffectdata != NULL)
	{
		delete m_pEffectdata;
		m_pEffectdata = NULL;
	}

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトを破棄
	Release();
}

//==========================================
//  更新処理
//==========================================
void CCheckPoint::Update(const float fDeltaTime)
{
#ifdef _DEBUG

	// ※エディターの時に動かすのでデバッグのみ

	// エフェクトの位置を設定する
	m_pEffectdata->m_pos = m_pos;

#endif // _DEBUG

	// プレイヤーとの当たり判定
	CollisionPlayer();
}

//==========================================
//  描画処理
//==========================================
void CCheckPoint::Draw(CShader* pShader)
{

}

//==========================================
// 半径取得処理
//==========================================
float CCheckPoint::GetRadius(void) const
{
	// 半径を返す
	return RADIUS;
}

//==========================================
//  生成処理
//==========================================
CCheckPoint* CCheckPoint::Create(const D3DXVECTOR3& rPos)
{
	// ポインタを宣言
	CCheckPoint* pSavePoint = new CCheckPoint;	// セーブポイント生成用

	// メモリの確保に失敗していた場合nullを返す
	if (pSavePoint == nullptr) { assert(false); return nullptr; }

	// セーブポイントの初期化
	if (FAILED(pSavePoint->Init()))
	{
		// メモリ開放
		delete pSavePoint;

		// nullを返す
		return nullptr;
	}

	// 位置を設定
	pSavePoint->m_pos = rPos;

	// チェックポイントエフェクトを出す
	pSavePoint->m_pEffectdata = GET_EFFECT->Create("data\\EFFEKSEER\\checkpoint_red.efkefc", rPos + OFFSET, VEC3_ZERO, VEC3_ZERO, 50.0f, true,false);

	// 確保したアドレスを返す
	return pSavePoint;
}

//==========================================
// リスト取得
//==========================================
CListManager<CCheckPoint>* CCheckPoint::GetList(void)
{
	// リスト構造を返す
	return m_pList;
}

//==========================================
//  プレイヤーとの当たり判定
//==========================================
void CCheckPoint::CollisionPlayer(void)
{
	// プレイヤーの情報を取得
	CPlayer* Player = GET_PLAYER;

	// プレイヤーが未生成なら関数を抜ける
	if (Player == nullptr) { return; }

	// セーブフラグがオンなら関数を抜ける
	if (m_bSave) { return; }

	// 当たっていない場合関数を抜ける
	if (!collision::CirclePillar(m_pos, Player->GetVec3Position(), RADIUS, Player->GetRadius()))
	{ return; }

	// プレイヤーを回復する
	Player->RecoverCheckPoint();

	// セーブポイントの番号を渡す
	GET_GAMEMANAGER->SetSave(m_pList->GetIndex(this));

	// セーブ音を鳴らす
	PLAY_SOUND(CSound::LABEL_SE_SAVE_000);

	// セーブモーションを設定する
	Player->SetState(CPlayer::STATE_SAVE);

	//現在のエフェクトを削除
	std::vector<CEffekseer::CEffectData*> vEffect = GET_EFFECT->GetList();
	if (!vEffect.empty())
	{
		delete m_pEffectdata;
		m_pEffectdata = NULL;
	}
	
	// チェックポイントエフェクトを出す
	m_pEffectdata = GET_EFFECT->Create("data\\EFFEKSEER\\checkpoint_blue.efkefc", m_pos + OFFSET, VEC3_ZERO, VEC3_ZERO, 50.0f, true, false);
	GET_EFFECT->Create("data\\EFFEKSEER\\check.efkefc", m_pos + OFFSET_CHECKEFFECT, VEC3_ZERO, VEC3_ZERO, 30.0f);

	// セーブフラグをオンにする
	m_bSave = true;
}

//============================================================
//	セットアップ処理
//============================================================
HRESULT CCheckPoint::LoadSetup(const char* pPass)
{
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用

	// ファイルを開く
	std::ifstream file(pPass);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "チェックポイントセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

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
		else if (str == "STAGE_CHECKSET")
		{
			do
			{ // END_STAGE_CHECKSETを読み込むまでループ

				// 文字列を読み込む
				file >> str;

				if (str.front() == '#')
				{ // コメントアウトされている場合

					// 一行全て読み込む
					std::getline(file, str);
				}
				else if (str == "CHECKSET")
				{
					do
					{ // END_CHECKSETを読み込むまでループ

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
					} while (str != "END_CHECKSET");	// END_CHECKSETを読み込むまでループ

					// チェックポイントの生成
					if (CCheckPoint::Create(pos) == nullptr)
					{ // 確保に失敗した場合

						// 失敗を返す
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_CHECKSET");	// END_STAGE_CHECKSETを読み込むまでループ
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}
