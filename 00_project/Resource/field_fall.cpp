//=========================================
//
//  落とし穴ギミック (field_fall.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "field_fall.h"
#include "manager.h"
#include "player_clone.h"
#include "player.h"

#include "objectMeshCube.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const int FALL_NUM = 3; // 上に乗ることのできる分身の数
}

//==========================================
//  コンストラクタ
//==========================================
CGimmickFall::CGimmickFall() : CField(),
m_bFall(false) // 落下フラグ
{

}

//==========================================
//  デストラクタ
//==========================================
CGimmickFall::~CGimmickFall()
{

}

//=========================================
//  初期化処理
//=========================================
HRESULT CGimmickFall::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CField::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < NUM_FLOOR; nCnt++)
	{
		// TODO：ここクリエイト!!
	}

	// 成功を返す
	return S_OK;
}

//=========================================
//  終了処理
//=========================================
void CGimmickFall::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_FLOOR; nCnt++)
	{
		// 床の終了処理
		SAFE_UNINIT(m_apFloor[nCnt]);
	}

	// 親クラスの終了
	CField::Uninit();
}

//=========================================
//  更新処理
//=========================================
void CGimmickFall::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CField::Update(fDeltaTime);
}

//=========================================
//  描画処理
//=========================================
void CGimmickFall::Draw(CShader* pShader)
{
	// 親クラスの描画
	CField::Draw(pShader);
}

//===========================================
//  当たっていた場合の処理
//===========================================
void CGimmickFall::Hit(CPlayerClone* pClone)
{
	// 分身に文字列を渡す
	pClone->AddFrags(GetFlag());

	// 分身に自身の情報を渡す
	pClone->SetField(this);

	// 分身数をカウント
	Count();
}

//==========================================
//  当たっていない場合の処理
//==========================================
void CGimmickFall::Miss(CPlayerClone* pClone)
{
	// 分身からフラグを削除する
	pClone->SabFrags(GetFlag());

	// 分身からフィールドを削除する
	pClone->DeleteField(this);
}

//===========================================
//  当たっていた場合の処理
//===========================================
void CGimmickFall::Hit(CPlayer* pPlayer)
{
	// 分身に文字列を渡す
	pPlayer->AddFrags(GetFlag());

	// 分身数をカウント
	Count();
}

//==========================================
//  当たっていない場合の処理
//==========================================
void CGimmickFall::Miss(CPlayer* pPlayer)
{
	// 分身からフラグを削除する
	pPlayer->SabFrags(GetFlag());
}

//===========================================
//  乗っているキャラクター総数の計算処理
//===========================================
void CGimmickFall::Count()
{
	// 分身のリスト構造が無ければ抜ける
	if (CPlayerClone::GetList() == nullptr) { return; }

	// リストを取得
	std::list<CPlayerClone*> list = CPlayerClone::GetList()->GetList();

	// 自身の上にいる分身の数をカウント
	int nNum = 0; // 乗っている分身数
	for (CPlayerClone* clone : list)
	{
		// 分身が所持しているフィールドが自身でない場合次に進む
		if (clone->GetField() != this) { continue; }

		// 自身の上の分身数を加算
		++nNum;
	}

	// 分身の数が最大数未満の場合関数を抜ける
	if (nNum < FALL_NUM) { m_bFall = false; return; }

	// 落下フラグを立てる
	m_bFall = true;
}
