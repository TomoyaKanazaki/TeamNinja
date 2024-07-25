//============================================================
//
//	サウンドヘッダー [sound.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//************************************************************
//	クラス定義
//************************************************************
// サウンドクラス
class CSound
{
public:
	// コンストラクタ
	CSound();

	// デストラクタ
	~CSound();

	// サウンド列挙
	enum ELabel
	{
		LABEL_NONE = NONE_IDX,		// ラベル無し
		LABEL_BGM_GENERAL,			// BGM (汎用)
		LABEL_SE_SELECT_000,		// 選択操作音00
		LABEL_SE_SELECT_001,		// 選択操作音01
		LABEL_SE_DECISION_000,		// 決定音00
		LABEL_SE_DECISION_001,		// 決定音01
		LABEL_SE_DECISION_002,		// 決定音02
		LABEL_SE_LAND_S,			// 着地音 (小)
		LABEL_SE_LAND_B,			// 着地音 (大)
		LABEL_SE_STALKWALK_000,		// 追跡敵の歩く音
		LABEL_SE_CLONEATTACK_000,	// 分身を攻撃したときの音
		LABEL_SE_STALKATTACK_000,	// 追跡敵が攻撃したときの音
		LABEL_SE_STALKFOUND_000,	// 追跡敵が発見したときの音
		LABEL_SE_STALKUPSET_000,	// 追跡敵が動揺したときの音
		LABEL_SE_STALKSTANCE_000,	// 追跡敵が構えたときの音
		LABEL_SE_PLAYERWALK_000,	// プレイヤーの歩行音
		LABEL_SE_PLAYERJUMP_S,		// プレイヤーの小ジャンプ音
		LABEL_SE_PLAYERSTEP_000,	// プレイヤーの回避音
		LABEL_MAX					// この列挙型の総数
	};

	// サウンド情報構造体
	struct SSoundInfo
	{
		const char *pFilename;	// ファイル名
		int nCntLoop;			// ループカウント
	};

	// メンバ関数
	HRESULT Init(HWND hWnd);	// 初期化
	void Uninit(void);			// 終了
	HRESULT LoadAll(HWND hWnd);	// 全読込
	HRESULT Play(ELabel label);	// 再生
	void Stop(ELabel label);	// 停止
	void Stop(void);			// 全停止
	void SetVolume(const ELabel label, float fVolume);	// 音量設定
	float GetVolume(const ELabel label) const;			// 音量取得
	void SetFrequency(const ELabel label, float fFreq);	// 周波数設定
	float GetFrequency(const ELabel label) const;		// 周波数取得

	// 静的メンバ関数
	static CSound *Create(HWND hWnd);		// 生成
	static void Release(CSound *&pSound);	// 破棄

private:
	// メンバ関数
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);	// チャンクチェック
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	// チャンクデータ読込

	// メンバ変数
	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[LABEL_MAX];						// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX];						// オーディオデータサイズ
};

#endif	// _SOUND_H_
