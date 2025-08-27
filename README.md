# Space Invaders Klon in C++ mit SFML von Henri Heidemann, Haissam Sleiman und Hanno Winter

Dieses Project implementiert im Rahmen des Kurses "Einführung in C/C++ SoSe 2025" den finalen Task nach Aufgabenstellung.
Hierzu haben wir einen Space Invaders Klon in C++ implementiert und diesen im Vergleich zum Original um einige Features erweitert.
Als Bibliothek für Grafik und Sound haben wir [SFML](https://github.com/SFML/SFML) verwendet.

## Spielablauf

Der Spieler startet als Raumschiff am unteren Rand des Spielfeldes, während sich am oberen Rand ein großer Schwarm von Aliens befindet.
Dieser bewegt sich als Einheit hin und her und sinkt dabei langsam herab.
Der Spieler kann sich durch Drücken der Pfeiltasten LEFT und RIGHT bewegen und durch Drücken von SPACE Projektile abfeuern.
Ziel ist es, durch geschicktes abfeuern der Projektile die Aliens zu treffen und so den gesamten Schwarm zu vernichten.
Dies wird dem Spieler dadurch erschwert, dass die Aliens ebenfalls Projektive abfeuern.
Diesen muss der Spieler ausweichen, da ihm jeder Treffer ein Leben kostet.
Hat der Spieler keine Leben mehr, ist das Spiel verloren und muss erneut begonnen werden.

Im Gegensatz zum Original können einzelne Aliens nun auch aus dem Schwarm ausbrechen und den Spieler direkt attackieren.
Zur Abhilfe tauchen gelegentlich Items auf, die der Spieler einsammeln und auf die Aliens abfeuern kann.
Mit jedem Level steigert sich die Intensität und die Aliens bewegen sich und feuern Projektive schneller ab.

## Bauen und ausführen im JupyterHub: 

1. Ins Terminal navigieren, z. B. in der Konsole mit "cd cpp-sommersemester-2025/Abgabe_Vorlage"
2. Nur beim ersten Mal: Die Befehle "make getSFML", "make getFLAC" und "make getGTest" ausführen, um Ressourcen herunterzuladen.
3. Das Projekt und die Tests bauen: "make game" bzw. "make tests"
4. Den Pfad im Terminal anpassen: "export LD_LIBRARY_PATH=./SFML/lib:./libFLAC:$LD_LIBRARY_PATH"
5. Das Spiel oder die Test ausführen (in der Konsolen-Anwendung, nicht VSCode!): "./game" oder "./tests"

## Credits

- https://opengameart.org/content/assets-for-a-space-invader-like-game
- https://opengameart.org/content/pixel-explosion-12-frames
- Rest ist Eigenanfertigung (Sounds in https://sfxr.me/ erstellt.)