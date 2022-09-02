void buzzer(int time = 500)
{
    tone(47, 220, 500 * 1000);
    delay(time);
    noTone(47);
}
