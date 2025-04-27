#pragma once

#include <xaudio2.h>

// サウンドファイル
typedef enum
{
	//每次加新音乐后也要在这里加
	GAMEBGM = 0,
	TITLEBGM,
	OK,
	BOOMSE,
	DASH,
	DEATH,
	GOAL,
	HOLD,
	JUMP,
	JUMPDOWN,
	JUMPDOWNEXTRA,
	SHOOT,
	TAKEHIT,
	TAKEWALL,
	STARUP,
	STAROVER,
	BACK,

	SOUND_LABEL_MAX,
} SOUND_LABEL;

class Sound {
private:
	// パラメータ構造体
	typedef struct
	{
		LPCSTR filename;	// 音声ファイルまでのパスを設定
		bool bLoop;			// trueでループ。通常BGMはture、SEはfalse。
	} PARAM;

	PARAM m_param[SOUND_LABEL_MAX] =
	{
		{"res/bgm/gamebgm.wav", true},	// サンプルBGM（ループさせるのでtrue設定）
		{ "res/bgm/titlebgm.wav", true },
		{"res/se/titlescene/OK.wav", false},
		{"res/se/gamescene/boom2.wav", false},
		{"res/se/gamescene/dash3.wav", false},
		{"res/se/gamescene/death.wav", false},
		{"res/se/gamescene/goal2.wav", false},
		{"res/se/gamescene/hold.wav", false},
		{"res/se/gamescene/jump2.wav", false},
		{"res/se/gamescene/jumpdown.wav", false},
		{"res/se/gamescene/jumpdown.wav", false},
		{"res/se/gamescene/shoot.wav", false},
		{"res/se/gamescene/takehit.wav", false},
		{"res/se/gamescene/takewall.wav", false},
		{"res/se/gameclearscene/starup2.wav", false},
		{"res/se/gameclearscene/starover.wav", false},
		{"res/se/back.wav", false}
	};

	IXAudio2* m_pXAudio2 = NULL;
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;
	IXAudio2SourceVoice* m_pSourceVoice[SOUND_LABEL_MAX];
	WAVEFORMATEXTENSIBLE m_wfx[SOUND_LABEL_MAX]; // WAVフォーマット
	XAUDIO2_BUFFER m_buffer[SOUND_LABEL_MAX];
	BYTE* m_DataBuffer[SOUND_LABEL_MAX];

	HRESULT FindChunk(HANDLE, DWORD, DWORD&, DWORD&);
	HRESULT ReadChunkData(HANDLE, void*, DWORD, DWORD);

public:
	// ゲームループ開始前に呼び出すサウンドの初期化処理
	HRESULT Init(void);

	// ゲームループ終了後に呼び出すサウンドの解放処理
	void Uninit(void);

	// 引数で指定したサウンドを再生する
	void Play(SOUND_LABEL label);

	// 引数で指定したサウンドを停止する
	void Stop(SOUND_LABEL label);

	// 引数で指定したサウンドの再生を再開する
	void Resume(SOUND_LABEL label);

};