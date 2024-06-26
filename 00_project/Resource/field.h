//============================================================
//
//	地面ヘッダー [field.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FIELD_H_
#define _FIELD_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectMeshField.h"

//************************************************************
//  前方宣言
//************************************************************
class CPlayerClone;	// プレイヤーの分身クラス

//************************************************************
//	クラス定義
//************************************************************
// 地面クラス
class CField : public CObjectMeshField
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_SOIL = 0,	// 土テクスチャ
		TYPE_WEED_SOIL,	// 草土テクスチャ
		TYPE_WEED,		// 草テクスチャ
		TYPE_FLOOR,		// 床テクスチャ
		TYPE_GRAVEL,	// 砂利道
		TYPE_BOOB,		// 泥
		TYPE_CLEEN,		// 掃除床
		TYPE_FALL,		// 落とし穴
		TYPE_DECAYED,	// 朽ちた床
		TYPE_WATER,		// 水
		TYPE_GUTTER,	// ドブ
		TYPE_BRIDGE,	// 橋
		TYPE_MAX,		// この列挙型の総数
		TYPE_NONE		// 存在しない種類
	};

	// 地形列挙
	enum ETerrain
	{
		TERRAIN_120x120 = 0,	// 120x120分割数の地形
		TERRAIN_MAX				// この列挙型の総数
	};

	// フィールド基準
	enum EZ
	{
		Z_MIDDLE = 0, // 中心
		Z_FRONT, // 手前
		Z_BACK, // 奥
		Z_MAX
	};

	// コンストラクタ
	CField();

	// デストラクタ
	~CField() override;

	// 地形情報構造体
	struct STerrainInfo
	{
		D3DXVECTOR3 *pPosGap;	// 頂点座標のずれ量
		POSGRID2 part;			// 分割数
	};

	// 仮想関数
	virtual void Hit(CPlayerClone* pClone);		// 分身に当たっていた時の処理
	virtual void Miss(CPlayerClone* pClone);	// 分身に当たっていない時の処理

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static void LoadSetup(void);	// セットアップ
	static CField *Create	// 生成
	( // 引数
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR2& rSize,	// 大きさ
		const D3DXCOLOR& rCol,		// 色
		const POSGRID2& rPart,		// 分割数
		const POSGRID2& rTexPart	// テクスチャ分割数
	);
	static CListManager<CField> *GetList(void);	// リスト取得

	// メンバ関数
	void SetTerrain(const ETerrain terrain);	// 地形設定
	void SetType(const EType type);				// 種類設定
	ETerrain GetTerrain(void) const	{ return m_terrain; }	// 地形取得
	EType GetType(void) const		{ return m_type; }		// 種類取得
	virtual bool IsFall() const { return bool(); }			// 落下判定の取得
	const char GetFlag() const;								// フラグ取得

	// 静的メンバ関数
	static EZ CalcNearLine(); // プレイヤーに最も近い基準線の算出

	// 静的メンバ関数
	static const char GetFlag(EType type);		// フラグ取得

private:

	// 静的メンバ変数
	static CListManager<CField> *m_pList;	// オブジェクトリスト
	static STerrainInfo m_aTerrainInfo[TERRAIN_MAX];	// 地形情報
	static EZ m_eNear; // 最も近い基準線

	// メンバ変数
	CListManager<CField>::AIterator m_iterator;	// イテレーター
	ETerrain m_terrain;	// 地形
	EType m_type;		// 種類
};

#endif	// _FIELD_H_
