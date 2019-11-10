#ifndef __GAME_MASTER_HPP__
#define __GAME_MASTER_HPP__

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <ctime>
#include <list>
#include <map>
#include <functional>
#include <random>

#include "./RougeLikeMap/RougeLikeMap.hpp"
#include "./RougeLikeMap/MapSet.hpp"
#include "./GameKey.hpp"

#include "./Character/Player/Player.hpp"
#include "./Character/Stair/Stair.hpp"
#include "./Character/Enemy/Enemy.hpp"

#include "./Character/Enemy/EnemyGenerator.hpp"

#include "..//DrawGame/DrawGame.hpp"

class GameMaster
{
	/* ゲームの処理ステップ */
	enum class GAME_STEP : int
	{
		INIT,        /* 初期化: 初めの1回のみ */
		CREATE_MAP,  /* マップ生成 */
		DISPLAY_INFO,/* ダンジョン情報表示 */
		TURN_START,  /* ターン開始 */
		PLAYER_TURN, /* プレイヤーフェーズ */
		ITEM_TURN,   /* アイテムフェーズ */
		ENEMY_TURN,  /* エネミーフェーズ */
		STATUS_TURN, /* ステータスフェーズ */
		TURN_END,    /* ターン終了 */
		STAIR_TURN,  /* 階段フェーズ */
		GAME_END,    /* ゲーム終了 */
	};

	/* ゲーム処理 */
	GAME_STEP game_step; /* ゲーム進行管理 */
	bool game_over_flag; /* ゲームオーバ管理 */
	
	/* キー処理 */
	BUTTON_MASK key_pos;   /* キー1文字分のバッファ */
	bool all_key_release;  /* 全キー入力監視 */

	/* 乱数処理 */
	int random_seed, cnt_seed;     /* 乱数シード */
	std::mt19937 mt_rnd;

	/* マップ処理 */
	RougeLikeMap<MAP_TYPE>* game_map; /* ゲームマップ */
	int floor_number, turn_number, room_number; /* 階層, 経過ターン, 部屋数 */
	const int width = 70, height = 50; /* マップの広さ(固定) */

	/* プレイヤー処理 */
	Player* player;

	/* 階段処理 */
	Stair* stair;

	/* エネミー処理 */
	std::list<Enemy*> enemy_list;

	/* アイテム処理 */

	/* 描画処理 */
	DrawGame draw_manager; /* 描画管理 */
	const float BASE_FPS = 60.0f; /* 基礎フレーム(1秒) */

	/* テーブル管理 */
	std::map<GAME_STEP, void(GameMaster::*)()> manage_turn_process; /* ターン処理 */

private:
	/* 生成処理 */
	void InitDungeon();
	void InitEnemy();
	Enemy* EnemyGenerate();

	/* ターン処理 */
	void TurnProcess(); /* ターン統括処理 */
	void Init();       /* 初期化 */
	void CreateMap();  /* マップ生成 */
	void DispInfo();   /* ダンジョン情報表示 */
	void TurnStart();  /* ターン開始 */
	void PlayerTurn(); /* プレイヤーターン */
	void StairTurn();  /* 階層ターン */
	void ItemTurn();   /* アイテムターン */
	void EnemyTurn();  /* エネミーターン */
	void StatusTurn(); /* ステータスターン */
	void TurnEnd();    /* ターン終了 */
	void GameEnd();    /* ゲーム終了 */

	/* キャラクター処理 */
	void CalcDirectionToPos(POS_TYPE *x,POS_TYPE *y,DIRECTION direct); /* 進行方向の座標を取得 */
	bool IsPosMove(const int x,const int y); /* 座標(x,y)地点は, 進行可能か判定 */
	MAPSET::DATA IsPosAttack(const int& x, const int& y);
	bool CharacterMove(Character *ch_data, const DIRECTION &direct); /* 汎用移動処理 */
	bool CharacterAttack(Character *ch_data); /* 汎用攻撃処理 */
	bool AnimationUpdate(); /* アニメーション処理 */

	/* StairTurn専用処理 */
	void DiposeFloor();
	void DiposeEnemy();

	/* EnemyTurn専用処理 */
	/* iteratorは破壊対策のため, 解放時に更新される */
	bool EnemyDeath(std::list<Enemy*>::iterator &enemy_itr);

	/* 描画処理 */
	void CameraPos();
	void DrawMap();    /* マップ描画 */
	void DrawStatus(); /* ステータス描画 */
	void DrawAll();

	/* テーブル管理 */
	void InitTurnProcessMap();

public:
		GameMaster();
		~GameMaster();

		/* ゲーム更新 */
		void Update();

		/* キー入力仲介処理 */
		void KeyInput(const bool *key_on,const bool *key_off);

		/* ゲッタ */
		inline bool IsGameOver() { return game_over_flag; }
};

#endif
