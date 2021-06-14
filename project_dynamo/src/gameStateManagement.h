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
    //Break down old gamestate
    switch (currentGameState)
    {
    case gameRunning:
    {
        setNumberOfLEDsToLightUp(0);
        break;
    }
    case countdown:
    {
        setNumberOfLEDsToLightUp(0);
        break;
    }
    default:
        break;
    }

    Serial.print("Switching to new game State: ");
    Serial.println(newGameState);
    currentGameState = newGameState;

    //Setup for new Gamestate
    switch (newGameState)
    {
    case gameRunning:
    {
        lastGameStartTime = millis();
        break;
    }
    case hostingWebpageForHighscore:
    {
        setupWiFiAndWebServer();
        Serial.println("Going to sleep...");
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