Ich hatte anfangs ständige Aussetzer bei etwas schlechterer Internetverbindung (habe sie absichtlich verschlechtert, um das zu testen). Das liegt aber nicht am ESP32 Audio, sondern am verwendeten Script. Dort sind die Buffer viel zu klein dimensioniert. Doch mit 8 statt üblichen 4 Mb müssen wir an dieser Stelle nicht sparen. Dafür müssen wir in der Datei Audio.h folgende Werte ändern:
m_buffSizeRAM      = 16000
m_resBuffSizeRAM  = 3200
m_maxBlockSize    = 3200
Zusätzlich fügen wir in den Sketch folgende Zeile ein:
audio.setConnectionTimeout(330,3200);

Mit dieser Änderung läuft es nicht nur gut, sondern perfekt. Während ein Technisat Digitalradio 3 IR nur rumstotterte, lief der ESP sauber ohne Aussetzer durch. Das habe ich über mehrere Stunden getestet. Auch der Klang ist glasklar. Das ist kein Wunder, findet die Digital/Analog Wandlung quasi erst im Lautsprecher statt. Auch mit einer leeren MP3 Datei (ohne Audiosignal) gab es bei voller Lautstärke keinerlei Rauschen. Zwischen einem VS1053B und dem ESP32 Audio liegen klangliche Welten. Nachteilig ist allerdings, dass der ESP nur einen Mono Verstärker hat. Dieser erreicht mit einem 4 Ohm Lautsprecher bei 5 Volt etwa 3 Watt, was für kleinere bis mittlere Radios völlig ausreicht.

Mit dem 3 Band Equalizer kann man den Klang noch etwas auf den Lautsprecher abstimmen.
setTone(int8_t gainLowPass, int8_t gainBandPass, int8_t gainHighPass); // Werte von jeweils -40 bis +6

Neben den beiden Anschlüssen für den Lautsprecher befindet sich ein weiterer Anschluss mit der Bezeichnung GAIN am Board. Mit diesem kann man zur Anpassung des Objektes die Verstärkung senken oder erhöhen. Dazu gibt es folgende Möglichkeiten:
15dB > GAIN mit 100 kOhm nach GND
12db > GAIN direkt mit GND verbinden
9dB > nichts anschließen
6dB > GAIN direkt mit Vin verbinden
3dB > GAIN mit 100 kOhm nach Vin

Natürlich lassen sich auch alle Senderinformationen (Sender, Titel, Interpret...) abgreifen, um diese auf einem Display darzustellen.
Weitere Informationen gibt es hier:
https://github.com/schreibfaul1/ESP32-audioI2S/wiki
