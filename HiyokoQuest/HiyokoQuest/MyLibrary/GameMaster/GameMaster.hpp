#ifndef __GAME_MASTER_HPP__
#define __GAME_MASTER_HPP__

#include <GL/freeglut.h>

#include <iostream>
#include <ctime>

#include "./RougeLikeMap/RougeLikeMap.hpp"
#include "./RougeLikeMap/MapSet.hpp"
#include "./GameKey.hpp"

#include "./Character/Player/Player.hpp"

class GameMaster
{
	/* ゲームの処理ステップ */
	enum class GAME_STEP : int
	{
		INIT,        /* 初期化: 初めの1回のみ */
		CREATE_MAP,  /* マップ生成 */
		TURN_START,  /* ターン開始 */
		PLAYER_TURN, /* プレイヤーフェーズ */
		ITEM_TURN,   /* アイテムフェーズ */
		ENEMY_TURN,  /* エネミーフェーズ */
		STATUS_TURN, /* ステータスフェーズ */
		TURN_END,    /* ターン終了 */
	};

	/* ゲーム処理 */
	GAME_STEP game_step; /* ゲーム進行管理 */
	bool game_over_flag; /* ゲームオーバ管理 */

	/* 乱数処理 */
	int random_seed, cnt_seed;     /* 乱数シード */

	/* マップ処理 */
	using MAP_TYPE = unsigned char; /* マップ型を節約 */
	RougeLikeMap<MAP_TYPE>* game_map; /* ゲームマップ */
	int floor_number, turn_number, room_number; /* 階層, 経過ターン, 部屋数 */
	const int width = 120, height = 100; /* マップの広さ(固定) */

	/* プレイヤー処理 */
	Character* player;

	/* エネミー処理 */

	/* アイテム処理 */

private:
	/* ターン処理 */
	void Init();       /* 初期化 */
	void CreateMap();  /* マップ生成 */
	void TurnStart();  /* ターン開始 */
	void PlayerTurn(); /* プレイヤーターン */
	void ItemTurn();   /* アイテムターン */
	void EnemyTurn();  /* エネミーターン */
	void StatusTurn(); /* ステータスターン */
	void TurnEnd();    /* ターン終了 */

	/* 描画処理 */
	void DrawMap();    /* マップ描画 */
	void DrawStatus(); /* ステータス描画 */

public:
		GameMaster();
		~GameMaster();

		/* ゲーム更新 */
		void Update();

		/* キー入力仲介処理 */
		void KeyInput(bool *key_on, bool *key_off);

		/* ゲッタ */
		inline bool IsGameOver() { return game_over_flag; }
};

#endif
