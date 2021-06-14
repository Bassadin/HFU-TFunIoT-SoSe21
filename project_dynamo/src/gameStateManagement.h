//State management
enum GameState
{
    gameRunning = 0,
    hostingWebpageForHighscore = 1,
    countdown = 2
};
GameState currentGameState;

void goToDeepSleep()
{
    esp_deep_sleep_start();
}

void changeGameState(GameState newGameState)
{
    Serial.print("Switching to new game State: ");
    Serial.println(newGameState);
    currentGameState = newGameState;

    switch (newGameState)
    {
    case gameRunning:
    {
        break;
    }
    case hostingWebpageForHighscore:
    {
        setupWiFiAndWebServer();
        goToDeepSleepTimer.once(120, goToDeepSleep); //Go to sleep after 120 seconds/2 minutes
        break;
    }
    case countdown:
    {
        player.playAsync(startMelody);
        break;
    }
    default:
        break;
    }
}