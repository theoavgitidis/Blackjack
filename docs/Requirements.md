# Requirements

# 1. Systembeschreibung

Das System ist ein in **C++** entwickeltes **Konsolenspiel**, das das Kartenspiel **Blackjack** für einen Benutzer spielbar macht.  
Der Benutzer interagiert über Texteingaben und Konsolenausgaben mit dem Spiel. Das System verwaltet die Spielregeln, das Kartendeck, die Kartenverteilung, die Spielzüge des Spielers und des Dealers sowie die Ermittlung des Spielergebnisses.

## 1.1 Systemziel

Das System muss es einem Benutzer ermöglichen, **Blackjack in der Konsole gegen einen vom System gesteuerten Dealer** zu spielen.

## 1.2 Systemgrenze

### Zum System gehören:
- Konsolenein- und -ausgabe
- Spiellogik für Blackjack
- Karten- und Deckverwaltung
- Spieler- und Dealerzüge
- Gewinnerermittlung
- Neustart oder Beenden des Spiels

### Nicht zum System gehören:
- grafische Benutzeroberfläche
- Online-Mehrspieler
- Netzwerkkommunikation
- Benutzerkonten
- Echtgeldfunktionen; Einsätze und Kontostand werden ausschließlich als Spielgeld behandelt

---

# 2. Stakeholder

## 2.1 Spieler
Der Spieler nutzt das Konsolenspiel und erwartet eine korrekte, verständliche und bedienbare Blackjack-Implementierung.

## 2.2 Entwicklerteam
Das Entwicklerteam implementiert, testet und wartet das System. Es benötigt klare und saubere Anforderungen.
## 2.3 Dozent / Prüfer
Der Dozent oder Prüfer bewertet, ob das Projekt funktional korrekt, strukturiert und nachvollziehbar umgesetzt wurde.

---

# 3. Funktionale Anforderungen

## REQ-F-001 – Spielstart
Das System muss dem Benutzer ermöglichen, eine Blackjack-Partie über die Konsole zu starten.

- **Priorität:** Muss
- **Akzeptanzkriterium:** Nach Programmstart kann der Benutzer ein neues Spiel beginnen.

## REQ-F-002 – Kartendeck bereitstellen
Das System muss für eine Spielsitzung ein gültiges Blackjack-Kartendeck bereitstellen und dieses über mehrere Runden verwenden. Wenn nur noch wenige Karten im Deck verbleiben, muss das System automatisch ein neues vollständiges Deck bereitstellen und mischen.

- **Priorität:** Muss
- **Akzeptanzkriterium:** Zu Beginn einer Spielsitzung steht ein vollständiges Deck mit zulässigen Kartenwerten zur Verfügung. Gezogene Karten bleiben aus dem Deck entfernt, bis das System bei niedrigem Kartenbestand ein neues vollständiges Deck mischt.

## REQ-F-003 – Anfangskarten austeilen
Das System muss zu Beginn einer Partie dem Spieler und dem Dealer die initialen Karten gemäß den festgelegten Blackjack-Regeln zuteilen.

- **Priorität:** Muss
- **Akzeptanzkriterium:** Nach Start einer Partie haben Spieler und Dealer die vorgesehene Anzahl an Startkarten.

## REQ-F-004 – Hand des Spielers anzeigen
Das System muss dem Benutzer seine aktuellen Karten und den aktuellen Punktestand in der Konsole anzeigen.

- **Priorität:** Muss
- **Akzeptanzkriterium:** Nach jeder relevanten Spielaktion wird die Hand des Spielers inklusive Punktwert ausgegeben.

## REQ-F-005 – Dealerinformationen anzeigen
Das System muss dem Benutzer die sichtbaren Karten des Dealers gemäß den festgelegten Spielregeln anzeigen.

- **Priorität:** Muss
- **Akzeptanzkriterium:** Während der Partie werden nur die freigegebenen Dealerinformationen angezeigt.

## REQ-F-006 – Spieleraktion auswählen
Das System muss dem Benutzer ermöglichen, im eigenen Zug eine zulässige Aktion auszuwählen.

- **Priorität:** Muss
- **Akzeptanzkriterium:** Während des Spielerzugs kann der Benutzer mindestens zwischen zulässigen Aktionen wählen.

## REQ-F-007 – Zusätzliche Karte ziehen
Das System muss dem Spieler nach Auswahl von „Hit“ eine weitere Karte zuteilen.

- **Priorität:** Muss
- **Akzeptanzkriterium:** Nach Eingabe der Aktion „Hit“ wird die Spielerhand um genau eine Karte erweitert.

## REQ-F-008 – Zug beenden
Das System muss dem Spieler ermöglichen, seinen Zug mit „Stand“ zu beenden.

- **Priorität:** Muss
- **Akzeptanzkriterium:** Nach Auswahl von „Stand“ zieht der Spieler keine weiteren Karten mehr.

## REQ-F-009 – Bust erkennen
Das System muss erkennen, wenn der Punktestand des Spielers oder Dealers den zulässigen Grenzwert überschreitet.

- **Priorität:** Muss
- **Akzeptanzkriterium:** Überschreitet eine Hand den Wert 21, wird dies sofort als Bust behandelt.

## REQ-F-010 – Dealerzug ausführen
Das System muss nach Abschluss des Spielerzugs den Dealerzug automatisch gemäß den definierten Blackjack-Regeln ausführen.

- **Priorität:** Muss
- **Akzeptanzkriterium:** Nach Ende des Spielerzugs führt das System den Dealerzug regelkonform ohne weitere Benutzereingabe aus.

## REQ-F-011 – Gewinner ermitteln
Das System muss nach Ende einer Partie das Spielergebnis korrekt bestimmen und ausgeben.

- **Priorität:** Muss
- **Akzeptanzkriterium:** Das System gibt eindeutig aus, ob der Spieler gewinnt, verliert oder unentschieden spielt.

## REQ-F-012 – Spielende behandeln
Das System muss erkennen, wenn eine Partie durch Blackjack, Bust, Stand oder Regelabschluss beendet ist.

- **Priorität:** Muss
- **Akzeptanzkriterium:** Eine Partie endet automatisch, sobald eine Endbedingung erreicht ist.

## REQ-F-013 – Neues Spiel oder Beenden
Das System muss dem Benutzer nach Abschluss einer Partie die Möglichkeit geben, eine neue Partie zu starten oder das Programm zu beenden.

- **Priorität:** Muss
- **Akzeptanzkriterium:** Nach Spielende kann der Benutzer zwischen Neustart und Programmende wählen.

## REQ-F-014 – Ungültige Eingaben behandeln
Das System muss ungültige Benutzereingaben erkennen und den Benutzer zu einer erneuten Eingabe auffordern.

- **Priorität:** Muss
- **Akzeptanzkriterium:** Bei ungültiger Eingabe stürzt das Programm nicht ab und fordert eine neue Eingabe an.

---

# 4. Nichtfunktionale Anforderungen

## REQ-NF-001 – Konsolenbasierte Bedienung
Das System muss vollständig über die Konsole bedienbar sein.

## REQ-NF-002 – Verständliche Ausgaben
Das System muss Spielzustände, Eingabeoptionen und Ergebnisse in verständlicher Textform ausgeben.

## REQ-NF-003 – Robuste Eingabeverarbeitung
Das System muss fehlerhafte oder unerwartete Benutzereingaben behandeln, ohne abzustürzen.

## REQ-NF-004 – Wartbare Struktur
Das System muss in C++ modular aufgebaut sein, sodass zentrale Bestandteile getrennt implementiert werden können, insbesondere:
- Kartenlogik
- Spiellogik
- Eingabe-/Ausgabelogik

## REQ-NF-005 – Erweiterbarkeit
Das System soll so strukturiert sein, dass zusätzliche Spieloptionen wie z. B. „Double Down“ oder „Split“ später ergänzt werden können.

## REQ-NF-006 – Regelkonformität
Das System muss die im Projekt festgelegten Blackjack-Regeln konsistent anwenden.

## REQ-NF-007 – Portabilität
Das System soll mit einer üblichen C++-Entwicklungsumgebung ohne spezielle proprietäre Abhängigkeiten kompilierbar sein.

---

# 5. Randbedingungen

## REQ-C-001 – Programmiersprache
Das System muss in **C++** implementiert werden.

## REQ-C-002 – Benutzerschnittstelle
Das System muss als **Konsolenanwendung** umgesetzt werden.

## REQ-C-003 – Lokale Ausführung
Das System muss lokal auf einem Rechner ohne Netzwerkverbindung ausführbar sein.

## REQ-C-004 – Keine grafische Oberfläche
Das System darf keine grafische Benutzeroberfläche voraussetzen.

## REQ-C-005 – Standardkonforme Umsetzung
Das System soll möglichst mit Standard-C++ und der Standardbibliothek umgesetzt werden.

---

# 6. Festgelegte Regeldefinitionen

Für die aktuelle Umsetzung gelten folgende Blackjack-Regeln:

- Es wird mit einem Standarddeck aus 52 Karten gespielt.
- Das Deck wird zu Beginn der Spielsitzung gemischt und über mehrere Runden verwendet.
- Wenn nur noch wenige Karten im Deck verbleiben, wird automatisch ein neues vollständiges Deck erzeugt und gemischt.
- Zahlenkarten zählen ihren Zahlenwert.
- Bube, Dame und König zählen jeweils 10 Punkte.
- Ein Ass zählt zunächst 11 Punkte und wird bei Bedarf als 1 Punkt gewertet, um einen Bust zu vermeiden.
- Spieler und Dealer erhalten zu Beginn einer Runde jeweils zwei Karten.
- Während des Spielerzugs ist die zweite Dealerkarte verdeckt.
- Der Spieler kann „Hit“, „Stand“ oder „Double Down“ wählen.
- Bei „Hit“ erhält der Spieler genau eine zusätzliche Karte.
- Bei „Stand“ beendet der Spieler seinen Zug.
- Bei „Double Down“ wird der Einsatz mit Spielgeld verdoppelt, der Spieler erhält genau eine weitere Karte und beendet danach automatisch seinen Zug.
- Split, Insurance und Surrender gehören nicht zur aktuellen Umsetzung.
- Der Dealer zieht automatisch weitere Karten, solange seine Punktzahl kleiner als 17 ist.
- Der Dealer bleibt ab einer Punktzahl von 17 stehen.
- Überschreitet eine Hand 21 Punkte, gilt sie als Bust.
- Hat der Spieler oder Dealer direkt mit den ersten zwei Karten Blackjack, endet die Runde sofort.
- Haben Spieler und Dealer gleichzeitig Blackjack, endet die Runde unentschieden.
- Bei gleicher Punktzahl nach Abschluss der Züge endet die Runde unentschieden (Push).
- Es spielt genau ein Spieler gegen einen vom System gesteuerten Dealer.
- Einsätze, Auszahlungen und Kontostand sind ausschließlich Spielgeld und haben keinen Echtgeldbezug.

---

# 7. Schreibschema für alle weiteren Requirements

Für jede weitere Requirement verwenden wir dieses Schema:

## Beispieltemplate

### REQ-F-XXX – Titel
**Beschreibung:** Das System muss ...

- **Begründung:** ...
- **Quelle:** ...
- **Priorität:** Muss / Sollte / Kann
- **Akzeptanzkriterium:** ...
- **Abhängigkeiten:** ...
