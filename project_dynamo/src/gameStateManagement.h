//State management
enum GameState
{
    gameRunning = 0,
    hostingWebpageForHighscore = 1,
    countdown = 2,
    preGameInfo = 3
};
GameState currentGameState;

void goToDeepSleep()
{
    Serial.println("Going to sleep now.");
    esp_deep_sleep_start();
}

void changeGameState(GameState newGameState)
{
    //Break down old gamestate
    switch (currentGameState)
    {
    case gameRunning:
    {
        setNumberOfPowerMeterLEDsToLightUp(0);
        break;
    }
    case countdown:
    {
        setNumberOfPowerMeterLEDsToLightUp(0);
        break;
    }
    case preGameInfo:
    {
        changeStartLedState(false);
        break;
    }
    case hostingWebpageForHighscore:
    {
        changeEndLedState(false);
        break;
    }
    default:
        break;
    }

    Serial.println("------------------------------");
    Serial.print("Switching to new game State: ");
    Serial.println(newGameState);
    Serial.println("------------------------------");
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
        changeEndLedState(true);
        setupWiFiAndWebServer();
        Serial.println("Going to sleep in 120 seconds");
        goToDeepSleepTimer.once(120, goToDeepSleep); //Go to sleep after 120 seconds/2 minutes
        break;
    }
    case countdown:
    {
        gameCountdownTimer = millis();
        gameLEDCountdownCounter = ledPinsSize;
        player.playAsync(startMelody);
        break;
    }
    case preGameInfo:
    {
        preGameInfoTimer = millis();
        changeStartLedState(true);
        break;
    }
    default:
        break;
    }
}