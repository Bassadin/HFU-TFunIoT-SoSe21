//State management
enum GameState
{
    gameRunning = 0,
    hostingWebpageForHighscore = 1,
    countdown = 2,
    preGameInfo = 3
};
GameState currentGameState;

void changeGameState(GameState newGameState)
{
    //Break down old gamestate
    switch (currentGameState)
    {
    case gameRunning:
    {
        setNumberOfPowerMeterLEDsToLightUpStatic(0);
        break;
    }
    case countdown:
    {
        setNumberOfPowerMeterLEDsToLightUpStatic(0);
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
        goToDeepSleepTimer.detach();
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
        Serial.println("Going to sleep in 240 seconds");
        goToDeepSleepTimer.once(240, goToDeepSleep); //Go to sleep after 120 seconds/2 minutes
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