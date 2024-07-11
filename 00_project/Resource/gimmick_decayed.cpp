//=========================================
//
//  朽ちた床ギミック (gimmick_decayed.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_decayed.h"
#include "manager.h"
#include "player_clone.h"
#include "player.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const int FALL_NUM = 3; // 上に乗ることのできる分身の数
}

//=========================================
//  コンストラクタ
//=========================================
CGimmickDecayed::CGimmickDecayed() : CField(),
m_bFall(false) // 落ちるフラグ
{

}

//=========================================
//  デストラクタ
//=========================================
CGimmickDecayed::~CGimmickDecayed()
{

}

//=========================================
//  初期化処理
//=========================================
HRESULT CGimmickDecayed::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CField::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//=========================================
//  終了処理
//=========================================
void CGimmickDecayed::Uninit(void)
{
	// 親クラスの終了
	CField::Uninit();
}

//=========================================
//  更新処理
//=========================================
void CGimmickDecayed::Update(const float fDeltaTime)
{
	if (m_bFall) { DebugProc::Print(DebugProc::POINT_CENTER, "落ちるon\n"); }

	// 親クラスの更新
	CField::Update(fDeltaTime);
}

//=========================================
//  描画処理
//=========================================
void CGimmickDecayed::Draw(CShader* pShader)
{
	// 親クラスの描画
	CField::Draw(pShader);
}

//===========================================
//  当たっていた場合の処理
//===========================================
void CGimmickDecayed::Hit(CPlayerClone* pClone)
{
	// 分身に文字列を渡す
	pClone->AddFrags(GetFlag());

	// 分身に自身の情報を渡す
	pClone->SetField(this);

	// 落下フラグがoffの場合分身の数を計算
	if (!m_bFall) { Count(); }
}

//==========================================
//  当たっていない場合の処理
//==========================================
void CGimmickDecayed::Miss(CPlayerClone* pClone)
{
	// 分身からフラグを削除する
	pClone->SabFrags(GetFlag());

	// 分身からフィールドを削除する
	pClone->DeleteField(this);
}

//===========================================
//  当たっていた場合の処理
//===========================================
void CGimmickDecayed::Hit(CPlayer* pPlayer)
{
	// プレイヤーに文字列を渡す
	pPlayer->AddFrags(GetFlag());

	// 落下フラグがoffの場合分身の数を計算
	if (!m_bFall) { Count(); }
}

//==========================================
//  当たっていない場合の処理
//==========================================
void CGimmickDecayed::Miss(CPlayer* pPlayer)
{
	// プレイヤーからフラグを削除する
	pPlayer->SabFrags(GetFlag());
}

//===========================================
//  乗っているキャラクター総数の計算処理
//===========================================
void CGimmickDecayed::Count()
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
	if (nNum < FALL_NUM) { return; }

	// 落下フラグを立てる
	m_bFall = true;
}
