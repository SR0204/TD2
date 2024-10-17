#include <Novice.h>
#include <time.h>

const char kWindowTitle[] = "LE2C_12_スズキ_レオ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	int windowSizeX = 750;
	int windowSizeY = 500;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, windowSizeX, windowSizeY);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};



	//乱数の初期化
	unsigned int currentTime = static_cast<unsigned int>(time(nullptr));
	srand(currentTime);

	//ゲームが始まったか
	bool isGameStart = false;
	//ゲームのフレーム
	int gameFrame = 0;
	//ゲームが終わるフレーム
	const int kMaxGameFrame = 60 * 12;
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

	
	int black = 0x00000000;

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


		//スペースキーを押すと...
		if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0 && isGameStart == false) {
			//ゲーム開始
			isGameStart = true;

			//合図が出るフレームを決定
			signalAppearFrame = rand() % 60 + 420;
			//偽合図が出るフレームを決定
			fakeSignalAppearFrame = rand() % 60 + 300;

			//敵が押すフレームを決定
			enemyPressedFrame = rand() % 10 + 10;
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

					//勝ち負けの判定
					if (playerPressedFrame < enemyPressedFrame) {
						isPlayerWin = true;
						isPlayerLose = false;
						isPlayerDraw = false;
					}
					else if (playerPressedFrame > enemyPressedFrame) {
						isPlayerWin = false;
						isPlayerLose = true;
						isPlayerDraw = false;
					}
					else if (playerPressedFrame == enemyPressedFrame) {
						isPlayerWin = false;
						isPlayerLose = false;
						isPlayerDraw = true;
					}
				}
			}

			//フレームが最大値になったら
			if (gameFrame == kMaxGameFrame)
			{
				//ゲーム終了！
				isGameStart = false;
			}
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawBox(0, 0, windowSizeX, windowSizeY, 0.0f, black, kFillModeSolid);

		Novice::ScreenPrintf(0, 0, "GameStart[%d] Frame[%4d] SignalFrame[%4d] SignalAppear[%d] FakeSignalFrame[%4d]", isGameStart, gameFrame, signalAppearFrame, isSignalAppear, fakeSignalAppearFrame);
		Novice::ScreenPrintf(0, 20, "PlayerPressedFrame[%4d] EnemyPressedFrame[%4d]", playerPressedFrame, enemyPressedFrame);
		Novice::ScreenPrintf(0, 40, "win[%d] lose[%d] draw[%d]", isPlayerWin, isPlayerLose, isPlayerDraw);

		if (isGameStart == true) {
			//合図
			if (gameFrame >= signalAppearFrame && gameFrame <= signalAppearFrame + 6) {
				Novice::DrawBox(0, 0, windowSizeX, windowSizeY, 0.0f, 0xFFFFFFFF, kFillModeSolid);
			}

			//偽の合図
			if (gameFrame >= fakeSignalAppearFrame && gameFrame <= fakeSignalAppearFrame + 6) {
				Novice::DrawBox(0, 0, windowSizeX, windowSizeY, 0.0f, 0xFFFFFF11, kFillModeSolid);
			}

			//勝ち負けの判定
			if (isPlayerWin == true) {
				Novice::DrawEllipse(400, 400, 40, 40, 0.0f, RED, kFillModeSolid);
			}
			if (isPlayerLose == true) {
				Novice::DrawEllipse(400, 400, 40, 40, 0.0f, BLUE, kFillModeSolid);
			}
			if (isPlayerDraw == true) {
				Novice::DrawEllipse(400, 400, 40, 40, 0.0f, WHITE, kFillModeSolid);
			}
		}



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
