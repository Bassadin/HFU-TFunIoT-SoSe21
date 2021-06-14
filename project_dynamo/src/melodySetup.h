//Melody player settings
#include <melody_player.h>
#include <melody_factory.h>
MelodyPlayer player(BUZZER_PIN, 0, LOW);

const char victoryMelodyString[] = "victory_jingle:d=4,o=5,b=110:16c5,16f5,16a5,16c6,16p,16a5,2c6";
const char startMelodyString[] = "start_shot:d=4,o=5,b=90:4c5,4p,4c5,4p,4c5,4p,2c6";

Melody victoryMelody, startMelody;

void loadMelodies() {
    victoryMelody = MelodyFactory.loadRtttlString(victoryMelodyString);
    startMelody = MelodyFactory.loadRtttlString(startMelodyString);
}