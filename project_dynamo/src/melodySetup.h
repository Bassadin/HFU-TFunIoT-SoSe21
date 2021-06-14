//Melody player settings
#include <melody_player.h>
#include <melody_factory.h>
MelodyPlayer player(BUZZER_PIN, 0, LOW);

String notes[] = {"C3", "F3", "A3", "C4", "A3", "C4", "C4", "C4"};
const char melodyString[] = "ff6_victory:d=4,o=5,b=140:32d6,32p,32d6,32p,32d6,32p,d6,a#,c6,16d6,8p,16c6,2d6";

Melody melody, melody2;

void loadMelodies() {
    melody = MelodyFactory.load("Victory tune", 140, notes, 8);
    melody2 = MelodyFactory.loadRtttlString(melodyString);
}