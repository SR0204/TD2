#include <Novice.h>
#include <time.h>
#include"Vector2.h"

const char kWindowTitle[] = "見切り";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	int windowSizeX = 750;
	int windowSizeY = 500;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, windowSizeX, windowSizeY);


	enum GameScene {

		TITLE,
		RULE,
		PLAY,
		GAMECLEAR,
		GAMEOVER,
		DRAW
	};

	GameScene scene = TITLE;


	//難易度
	enum GameDifiicult {
		easy,
		normal,
		pro
	};




	GameDifiicult gameDifficult = normal;


	//乱数の初期化
	unsigned int currentTime = static_cast<unsigned int>(time(nullptr));
	srand(currentTime);

	//ゲームが始まったか
	bool isGameStart = false;
	//ゲームのフレーム
	int gameFrame = 0;
	//ゲームが終わるフレーム
	const int kMaxGameFrame = 60 * 10;

	//ゲームの延長
	//int EndGameFrame = 60 * 12;

	//合図が出るフレーム
	int signalAppearFrame = 0;
	//合図が出ているか
	bool isSignalAppear = false;
	//偽の合図が出るフレーム
	int fakeSignalAppearFrame = 0;

	//勝ち負けのフラグ
	bool isPlayerWin = false;
	bool isPlayerLose = false;
	bool isPlayerDraw = false;

	//プレイヤーがキーを押したフレーム
	int playerPressedFrame = 0;
	//敵がキーを押したフレーム
	int enemyPressedFrame = 0;


	//プレイヤー
	Vector2 PlayerPosition{

		PlayerPosition.x = 100,
		PlayerPosition.y = 400,

	};

	float PlayerRad = 50;


	//敵
	Vector2 EnemyPosition{

		EnemyPosition.x = 600,
		EnemyPosition.y = 400,

	};

	float EnemyRad = 50;

	int RuleGH = Novice::LoadTexture("./Resource/Rule.png");

	int GameClearGH = Novice::LoadTexture("./Resource/GameClear.png");

	int GameOverGH = Novice::LoadTexture("./Resource/GameOver.png");

	int DrawGH = Novice::LoadTexture("./Resource/Draw.png");

	int slashSE = Novice::LoadAudio("./Resource/slash.mp3");
	int bgm = Novice::LoadAudio("./Resource/bgm.mp3");

	int haikeiGH = Novice::LoadTexture("./Resource/haikei.png");

	int titleGH = Novice::LoadTexture("./Resource/title.png");

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///


		switch (scene)
		{
		case TITLE:

			Novice::DrawSprite(0, 0, titleGH, 1.0f, 1.0f, 0.0f, WHITE);

			//スペースキーを押すと...
			if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0 && isGameStart == false) {
				//ゲーム開始
				isGameStart = true;


				//srand(currentTime);

				//合図が出るフレームを決定
				signalAppearFrame = rand() % 60 + 420;


				//偽合図が出るフレームを決定
				//fakeSignalAppearFrame = rand() % 60 + 300;//ノーマル


				scene = RULE;
			}
			break;

		case RULE:
			//Novice::DrawBox(0, 0, 750, 500, 0.0f, BLACK, kFillModeSolid);

			if (preKeys[DIK_1] == 0 && keys[DIK_1] != 0) {

				gameDifficult = easy;
			}

			if (preKeys[DIK_2] == 0 && keys[DIK_2] != 0) {

				gameDifficult = normal;

				//偽合図が出るフレームを決定
				fakeSignalAppearFrame = rand() % 60 + 300;//ノーマル
			}

			if (preKeys[DIK_3] == 0 && keys[DIK_3] != 0) {

				gameDifficult = pro;

				//偽合図が出るフレームを決定
				fakeSignalAppearFrame = rand() % 30 + 390;//ハード
			}

			Novice::ScreenPrintf(0, 0, "モード %d", gameDifficult);

			switch (gameDifficult)
			{
			case easy:

				//敵が押すフレームを決定
				enemyPressedFrame = rand() % 31 + 30;

				break;
			case normal:

				//敵が押すフレームを決定
				enemyPressedFrame = rand() % 5 + 13;

				break;
			case pro:

				//敵が押すフレームを決定
				enemyPressedFrame = rand() % 7 + 7;

				break;

			}

			Novice::DrawSprite(0, 0, (int)RuleGH, 1.0f, 1.0f, 0.0f, WHITE);

			if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
			{
				Novice::PlayAudio(bgm, true, 0.5f);

				scene = PLAY;
			}

			break;

		case PLAY:

			Novice::ScreenPrintf(280, 20, "your Frame[%4d]", playerPressedFrame);

			Novice::DrawSprite(0, 0, haikeiGH, 1.0f, 1.0f, 0.0f, WHITE);

			if (isGameStart == true) {
				//合図
				if (gameFrame >= signalAppearFrame && gameFrame <= signalAppearFrame + 6) {
					Novice::DrawBox(0, 0, windowSizeX, windowSizeY, 0.0f, 0xFFFFFFFF, kFillModeSolid);
				}

				//偽の合図
				if (gameDifficult == normal || gameDifficult == pro) {
					if (gameFrame >= fakeSignalAppearFrame && gameFrame <= fakeSignalAppearFrame + 6) {
						Novice::DrawBox(0, 0, windowSizeX, windowSizeY, 0.0f, RED, kFillModeSolid);
					}
				}


				//勝ち負けの判定
				/*if (isPlayerWin == true) {
					Novice::DrawEllipse(400, 400, 40, 40, 0.0f, RED, kFillModeSolid);
				}
				if (isPlayerLose == true) {
					Novice::DrawEllipse(400, 400, 40, 40, 0.0f, BLUE, kFillModeSolid);
				}
				if (isPlayerDraw == true) {
					Novice::DrawEllipse(400, 400, 40, 40, 0.0f, WHITE, kFillModeSolid);
				}*/
			}

			//ゲーム中に...
			if (isGameStart == true) {

				//ゲーム中はフレームを加算し続ける
				gameFrame++;

				//ゲームのフレームが合図を出すフレームになったら...
				if (gameFrame == signalAppearFrame) {
					//合図が出ているフラグをtrueに
					isSignalAppear = true;
				}

				//合図が出ている間...
				if (isSignalAppear == true) {
					//プレイヤーが押すまでのフレームを加算
					playerPressedFrame++;

					//キーを押したら合図を消す＆押した瞬間のフレームを決定！
					if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0) {
						isSignalAppear = false;

						//斬撃のSEを再生
						Novice::PlayAudio(slashSE, false, 0.5f);

					}


				}
				else if (isSignalAppear == false && preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
				{
					scene = GAMEOVER;
				}




				//プレイヤー
				Novice::DrawEllipse((int)PlayerPosition.x, (int)PlayerPosition.y, (int)PlayerRad, (int)PlayerRad, 0.0f, WHITE, kFillModeSolid);

				//敵
				Novice::DrawEllipse((int)EnemyPosition.x, (int)EnemyPosition.y, (int)EnemyRad, (int)EnemyRad, 0.0f, BLUE, kFillModeSolid);


				//フレームが最大値になったら
				if (gameFrame == kMaxGameFrame)
				{
					//斬撃のSEを止める
					Novice::StopAudio(slashSE);

					//勝ち負けの判定
					if (playerPressedFrame < enemyPressedFrame) {

						isPlayerWin = true;
						isPlayerLose = false;
						isPlayerDraw = false;

						scene = GAMECLEAR;

					}
					else if (playerPressedFrame > enemyPressedFrame) {
						isPlayerWin = false;
						isPlayerLose = true;
						isPlayerDraw = false;

						scene = GAMEOVER;

					}
					else if (playerPressedFrame == enemyPressedFrame) {
						isPlayerWin = false;
						isPlayerLose = false;
						isPlayerDraw = true;

						scene = DRAW;

					}

					//bgmを止める
					Novice::StopAudio(bgm);

					//ゲーム終了！
					isGameStart = false;
				}

			}


			/*if (gameFrame > kMaxGameFrame) {

				gameFrame++;

				if (gameFrame == EndGameFrame) {



				}
			}*/



			break;
		case GAMECLEAR:
			//Novice::DrawBox(0, 0, 750, 500, 0.0f, RED, kFillModeSolid);

			Novice::ScreenPrintf(280, 200, "your Frame[%4d]", playerPressedFrame);

			Novice::ScreenPrintf(280, 220, "enemy Frame[%4d]", enemyPressedFrame);

			Novice::DrawSprite(0, 0, GameClearGH, 1.0f, 1.0f, 0.0f, WHITE);

			if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
			{




				//ゲームが始まったか
				isGameStart = false;
				//ゲームのフレーム
				gameFrame = 0;

				//合図が出るフレーム
				signalAppearFrame = 0;
				//合図が出ているか
				isSignalAppear = false;
				//偽の合図が出るフレーム
				fakeSignalAppearFrame = 0;

				//勝ち負けのフラグ
				isPlayerWin = false;
				isPlayerLose = false;
				isPlayerDraw = false;

				//プレイヤーがキーを押したフレーム
				playerPressedFrame = 0;
				//敵がキーを押したフレーム
				enemyPressedFrame = 0;


				scene = TITLE;
			}


			break;
		case GAMEOVER:
			//Novice::DrawBox(0, 0, 750, 500, 0.0f, GREEN, kFillModeSolid);

			Novice::ScreenPrintf(280, 200, "your Frame[%4d]", playerPressedFrame);

			Novice::ScreenPrintf(280, 220, "enemy Frame[%4d]", enemyPressedFrame);

			Novice::DrawSprite(0, 0, GameOverGH, 1.0f, 1.0f, 0.0f, WHITE);


			if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
			{



				//ゲームが始まったか
				isGameStart = false;
				//ゲームのフレーム
				gameFrame = 0;

				//合図が出るフレーム
				signalAppearFrame = 0;
				//合図が出ているか
				isSignalAppear = false;
				//偽の合図が出るフレーム
				fakeSignalAppearFrame = 0;

				//勝ち負けのフラグ
				isPlayerWin = false;
				isPlayerLose = false;
				isPlayerDraw = false;

				//プレイヤーがキーを押したフレーム
				playerPressedFrame = 0;
				//敵がキーを押したフレーム
				enemyPressedFrame = 0;



				scene = TITLE;

			}


			break;

		case DRAW:
			Novice::ScreenPrintf(280, 20, "your Frame[%4d]", playerPressedFrame);

			Novice::ScreenPrintf(280, 35, "enemy Frame[%4d]", enemyPressedFrame);

			//Novice::DrawBox(0, 0, 750, 500, 0.0f, BLUE, kFillModeSolid);

			Novice::DrawSprite(0, 0, DrawGH, 1.0f, 1.0f, 0.0f, WHITE);

			if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
			{

				//ゲームが始まったか
				isGameStart = false;
				//ゲームのフレーム
				gameFrame = 0;

				//合図が出るフレーム
				signalAppearFrame = 0;
				//合図が出ているか
				isSignalAppear = false;
				//偽の合図が出るフレーム
				fakeSignalAppearFrame = 0;

				//勝ち負けのフラグ
				isPlayerWin = false;
				isPlayerLose = false;
				isPlayerDraw = false;

				//プレイヤーがキーを押したフレーム
				playerPressedFrame = 0;
				//敵がキーを押したフレーム
				enemyPressedFrame = 0;

				scene = TITLE;

			}

		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//Novice::ScreenPrintf(0, 0, "GameStart[%d] Frame[%4d] SignalFrame[%4d] SignalAppear[%d] FakeSignalFrame[%4d]", isGameStart, gameFrame, signalAppearFrame, isSignalAppear, fakeSignalAppearFrame);

		//Novice::ScreenPrintf(0, 40, "win[%d] lose[%d] draw[%d]", isPlayerWin, isPlayerLose, isPlayerDraw);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}