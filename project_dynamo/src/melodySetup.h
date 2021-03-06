//Melody player settings
#include <melody_player.h>
#include <melody_factory.h>
MelodyPlayer player(BUZZER_PIN, 3, LOW); //Needs to be a PWM channel (second paramter) other than 0 to not conflict with JLED

const char victoryMelodyString[] = "victory_jingle:d=4,o=5,b=110:16c5,16f5,16a5,16c6,16p,16a5,2c6";
const char startMelodyString[] = "start_shot:d=4,o=5,b=90:4c5,4p,4c5,4p,4c5,4p,2c6";
const char defeatMelodyString[] = "defeat:d=4,o=5,b=100:8c6,16p,8b5,16p,8a#5,16p,2a5";
const char silverMelodyString[] = "silver:d=4,o=5,b=120:16f5,16f5,16a5,16c6";

Melody victoryMelody, startMelody, defeatMelody, silverMelody;

void loadMelodies() {
    victoryMelody = MelodyFactory.loadRtttlString(victoryMelodyString);
    startMelody = MelodyFactory.loadRtttlString(startMelodyString);
    defeatMelody = MelodyFactory.loadRtttlString(defeatMelodyString);
    silverMelody = MelodyFactory.loadRtttlString(silverMelodyString);
    Serial.println("Melodies loaded.");
}