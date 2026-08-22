# Feroxills

Oscilloscope logiciel multi-source, écrit en C++/Qt6 (QML).

Il permet de visualiser des signaux provenant de différentes sources (générateur interne, port série, ...) avec plusieurs modes d'affichage (continu, trigger, auto).

![Aperçu de l'interface](docs/assets/screenshoot.png)

## Fonctionnalités actuelles

- Affichage de signaux en mode Continu, Trigger et Auto
- Génération de signaux de test (fréquence, tension réglables)
- Détection de ports série disponibles
- Interface QML personnalisée

## Prérequis

- Qt 6.5+ (modules `Quick`, `QuickControls2`, `SerialPort`, `Test`)
- CMake 3.16+
- Compilateur C++17

## Compilation

```bash
cmake -B build -S .
cmake --build build
```

## Statut

Projet en développement actif. Voir `Issues.txt` et `features.txt` pour les travaux en cours et à venir.