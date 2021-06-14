//State management
enum GameState
{
    gameRunning = 0,
    hostingWebpageForHighscore = 1,
    warmingUp = 2
};
GameState currentGameState = warmingUp;

void goToDeepSleep()
{
    esp_deep_sleep_start();
}

void handleEndGame()
{
    setupWiFiAndWebServer();
    goToDeepSleepTimer.once(120, goToDeepSleep); //Go to sleep after 120 seconds/2 minutes
}

void changeGameState(GameState newGameState)
{
    Serial.print("Switching to new game State: ");
    Serial.println(newGameState);
    currentGameState = newGameState;
    //TODO place actions on game state change to happen once herei
}