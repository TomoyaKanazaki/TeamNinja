//=========================================
//
//  落とし穴ギミック (field_fall.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "field_fall.h"
#include "manager.h"
#include "texture.h"
#include "player_clone.h"
#include "player.h"

#include "objectMeshCube.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const int FALL_NUM = 3; // 上に乗ることのできる分身の数

	// 床関連
	namespace floor
	{
		const float HEIGHT = 30.0f;				// 高さ
		const float NONE_ROT = D3DX_PI * 0.5f;	// 通常状態の向き
	}
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
		m_apFloor[nCnt] = CObjectMeshCube::Create
		(
			VEC3_ZERO,
			VEC3_ZERO,
			VEC3_ZERO,
			XCOL_WHITE,
			XCOL_ABLACK,
			CObjectMeshCube::BORDER_OFF,
			0.0f,
			CObjectMeshCube::TEXSTATE_ONE,
			CObjectMeshCube::SFaceTex(-1),
			CObjectMeshCube::ORIGIN_UP
		);
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

	// 床の処理
	Floor();
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
// 位置の設定処理
//===========================================
void CGimmickFall::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 位置を設定する
	CField::SetVec3Position(rPos);

	for (int nCnt = 0; nCnt < NUM_FLOOR; nCnt++)
	{
		// 床が NULL の場合、抜ける
		if (m_apFloor[nCnt] == nullptr) { continue; }

		// 床の位置を設定する
		m_apFloor[nCnt]->SetVec3Position(rPos);
	}
}

//===========================================
// 向きの設定処理
//===========================================
void CGimmickFall::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 向きを設定する
	CField::SetVec3Rotation(rRot);

	for (int nCnt = 0; nCnt < NUM_FLOOR; nCnt++)
	{
		// 床が NULL の場合、抜ける
		if (m_apFloor[nCnt] == nullptr) { continue; }

		// 床の向きを設定する
		m_apFloor[nCnt]->SetVec3Rotation(rRot);
	}
}

//===========================================
// サイズの設定処理
//===========================================
void CGimmickFall::SetVec2Sizing(const D3DXVECTOR2& rSize)
{
	// サイズを設定する
	CField::SetVec2Sizing(rSize);

	for (int nCnt = 0; nCnt < NUM_FLOOR; nCnt++)
	{
		// 床が NULL の場合、抜ける
		if (m_apFloor[nCnt] == nullptr) { continue; }

		// 床のサイズを設定する
		m_apFloor[nCnt]->SetVec3Sizing(D3DXVECTOR3(rSize.x, floor::HEIGHT, rSize.y));
	}
}

//===========================================
// 種類の設定処理
//===========================================
void CGimmickFall::SetType(const EType type)
{
	// 種類を設定する
	CField::SetType(type);

	for (int nCnt = 0; nCnt < NUM_FLOOR; nCnt++)
	{		
		// 床が NULL の場合、抜ける
		if (m_apFloor[nCnt] == nullptr) { continue; }

		// 床のテクスチャを設定する
		m_apFloor[nCnt]->BindTexture(GetTextureIndex());
	}
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

//===========================================
// 床の処理
//===========================================
void CGimmickFall::Floor(void)
{
	// 位置・向き・サイズを取得
	D3DXVECTOR3 pos = GetVec3Position();
	D3DXVECTOR3 rot = GetVec3Rotation();
	D3DXVECTOR2 size = GetVec2Sizing();
	D3DXVECTOR3 posFloor = VEC3_ZERO;
	D3DXVECTOR3 rotFloor = VEC3_ZERO;
	D3DXVECTOR3 sizeFloor = VEC3_ZERO;

	// 床が無かった場合、停止
	if (m_apFloor[0] == nullptr || m_apFloor[1] == nullptr) { assert(false); return; }

	// 床の位置を設定する
	posFloor.x = pos.x + sinf(rot.y) * -(size.x * 0.5f);
	posFloor.y = pos.y - floor::HEIGHT * 0.5f;
	posFloor.z = pos.z + cosf(rot.y) * -(size.y * 0.5f);
	m_apFloor[0]->SetVec3Position(posFloor);

	// 床の位置を設定する
	posFloor.x = pos.x + sinf(rot.y) * (size.x * 0.5f);
	posFloor.y = pos.y - floor::HEIGHT * 0.5f;
	posFloor.z = pos.z + cosf(rot.y) * (size.y * 0.5f);
	m_apFloor[1]->SetVec3Position(posFloor);

	// 向きを設定する
	rotFloor = rot;
	rotFloor.x = -floor::NONE_ROT;
	m_apFloor[0]->SetVec3Rotation(rotFloor);
	rotFloor.x = floor::NONE_ROT;
	m_apFloor[1]->SetVec3Rotation(rotFloor);

	// サイズを設定する
	sizeFloor.x = size.x * 0.5f;
	sizeFloor.y = size.y * 0.5f;
	sizeFloor.z = floor::HEIGHT;
	m_apFloor[0]->SetVec3Sizing(sizeFloor);
	m_apFloor[1]->SetVec3Sizing(sizeFloor);
}
