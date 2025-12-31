# 🚗 Smart City Traffic Core Simulator

**Sous-projet : Traffic Core**  
**Cours : Programmation Orientée Objet en C++**  
**Professeure : Pr. Ikram BEN ABDEL OUAHAB**  
**Année académique : 2025/2026**  
**Licence IDAI – FST Tanger**

## Description

Simulation interactive de circulation routière dans une **Smart City**, développée en **C++** avec la bibliothèque **Raylib** pour la visualisation 3D.

Le projet propose une **interface graphique intégrée** avec un menu à l’écran permettant de contrôler la simulation en temps réel.

## Membres de l'équipe

- **Ouarda Ait El Fakih** – [@ouarda-aitelafkih](https://github.com/ouarda-aitelafkih)
- **Fatima Zahra El Bouhssini** – [@fatimazahraeelbouhssini4-arch](https://github.com/fatimazahraeelbouhssini4-arch)
- **Halima Achabbak** – [@achabbakhalima-ai](https://github.com/achabbakhalima-ai)
- **Fatima Boumehaout** – [@Fatimaboumehaout](https://github.com/Fatimaboumehaout)

## Fonctionnalités principales

- Circulation dynamique de plusieurs véhicules en 3D (modèles Kenney et Sketchfab)
- Mouvement réaliste avec accélération, freinage et animation des roues
- Menu interactif affiché à l’écran avec :
  - Nombre de véhicules
  - Temps écoulé
  - État de la simulation (RUNNING / PAUSED)
  - Position et mode de la caméra
- Plusieurs modes de caméra :
  - Mode 1 : Orbital (RTS)
  - Mode 2 : Free Fly (FPS)
  - Mode 3 : Follow Vehicle
- Contrôles clavier complets :
  - **SPACE** : Pause / Reprise
  - **V / K** : Ajouter / Supprimer un véhicule
  - **R** : Réinitialiser la caméra
  - **C** : Mode cinématique (Auto-Orbit)
  - **1 / 2 / 3** : Changer de mode caméra
  - **TAB** : Changer le véhicule suivi
  - **SHIFT** : Boost de vitesse
  - **WASD / ZQSD** + flèches : Déplacement et rotation caméra
  - **Q / D** : Zoom

## Technologies utilisées

- **Langage** : C++17
- **Bibliothèque graphique** : Raylib (visualisation 3D)
- **Modèles 3D** : Kenney.nl et Sketchfab

## Compilation et exécution

### Prérequis
- Compilateur C++ (MinGW recommandé sous Windows)
- Raylib installé
- CMake ≥ 3.10

### Étapes

```bash
git clone https://github.com/fatimazahraeelbouhssini4-arch/Traffic-core-.git
cd Traffic-core-
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build . --config Release
./SmartCity.exe
La simulation :
https://github.com/fatimazahraeelbouhssini4-arch/Traffic-Core/blob/main/simulation/simulation.mp4
