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

//==========================================
//  定数定義
//==========================================
namespace
{
	const char* PARAM_FILE = "data\\TXT\\CheckPoint.txt"; // パラメータが保存されたパス
}

//==========================================
//  静的メンバ変数宣言
//==========================================
int CCheckPoint::m_nNumAll = 0;

//==========================================
//  コンストラクタ
//==========================================
CCheckPoint::CCheckPoint():
	m_bSave(false),
	m_fRadius(0.0f),
	m_nSaveTension(0),
	m_fRotSpeed(0.0f)
{
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
	m_bSave = false;
	m_fRadius = 0.0f;
	m_nSaveTension = 0;
	m_fRotSpeed = 0.0f;

	// 親クラスの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルを割り当て
	BindModel("data\\MODEL\\FONT\\name_boss000.x");

	// 自身のラベルを設定
	SetLabel(LABEL_CHECKPOINT);

	// 定数パラメータの読み込み
	Load();

	// サイズを調整
	SetVec3Scaling(D3DXVECTOR3(0.2f, 0.2f, 0.2f));

	// マテリアルの設定処理
	SetAllMaterial(material::GlowCyan());

	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void CCheckPoint::Uninit(void)
{
	// 親クラスの終了
	CObjectModel::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CCheckPoint::Update(const float fDeltaTime)
{
	// プレイヤーとの当たり判定
	CollisionPlayer();

	// くるくるしてみる
	D3DXVECTOR3 rot = GetVec3Rotation();
	rot.y += m_fRotSpeed;
	SetVec3Rotation(rot);

	// 親クラスの更新
	CObjectModel::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CCheckPoint::Draw(CShader* pShader)
{
	// 親クラスの描画
	CObjectModel::Draw();
}

//==========================================
//  生成処理
//==========================================
CCheckPoint* CCheckPoint::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
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
	pSavePoint->SetVec3Position(rPos);

	// 向きを設定
	pSavePoint->SetVec3Rotation(rRot);

	// 確保したアドレスを返す
	return pSavePoint;
}

//==========================================
//  プレイヤーとの当たり判定
//==========================================
void CCheckPoint::CollisionPlayer(void)
{
	// セーブフラグがオンなら関数を抜ける
	if (m_bSave) { return; }

	//　自身の位置を取得
	D3DXVECTOR3 pos = GetVec3Position();

	// プレイヤーの情報を取得
	CPlayer* Player = GET_PLAYER; // 座標

	// 当たっていない場合関数を抜ける
	if (!collision::CirclePillar(pos, Player->GetVec3Position(), m_fRadius, Player->GetRadius()))
	{ return; }

	// プレイヤーを回復する
	Player->RecoverCheckPoint();

	// 士気力を保存する
	m_nSaveTension = Player->GetTension();

	// セーブフラグをオンにする
	m_bSave = true;
}

//==========================================
//  外部情報の読み込み
//==========================================
void CCheckPoint::Load()
{
	//ローカル変数宣言
	FILE* pFile; // ファイルポインタ

	//ファイルを読み取り専用で開く
	pFile = fopen(PARAM_FILE, "r");

	// ファイルが開けなかった場合
	if (pFile == NULL) { assert(false); return; }

	// 情報の読み込み
	while (1)
	{
		// 文字列の記録用
		char aStr[256];

		// 文字列読み込み
		fscanf(pFile, "%s", &aStr[0]);

		// 条件分岐
		if (strcmp(&aStr[0], "RADIUS") == 0) // 当たり判定の半径の設定
		{
			// データを格納
			fscanf(pFile, "%f", &m_fRadius);
		}
		if (strcmp(&aStr[0], "ROT_SPEED") == 0) // 回る速度
		{
			// データを格納
			fscanf(pFile, "%f", &m_fRotSpeed);
		}
		if (strcmp(&aStr[0], "END_OF_FILE") == 0) // 読み込み終了
		{
			break;
		}
	}
}
