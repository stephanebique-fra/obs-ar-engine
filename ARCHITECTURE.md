# OBS AR Engine - Architecture

## Vision

OBS AR Engine est un moteur de réalité augmentée open source destiné à la diffusion sportive.

Le moteur est indépendant d'OBS Studio et des API graphiques (SDL, Metal, OpenGL...).

OBS Studio est une application hôte, pas le cœur du moteur.

---

## Principes

- Une responsabilité par module.
- Le moteur ne dépend d'aucune API graphique.
- Les backends graphiques dépendent du moteur, jamais l'inverse.
- Les performances temps réel sont une priorité.
- Le projet est 100 % open source.

---

## Modules

### Core
Types fondamentaux, temps, logs, utilitaires.

### Math
Vecteurs, matrices, géométrie, transformations.

### Vision
Détection des lignes, intersections, tracking, IA.

### Camera
Caméra, calibration, distorsion, pose.

### Sports
Description géométrique des terrains (basket, volley, handball...).

### Projection
Projection 2D/3D entre le monde réel et l'image.

### Assets
Images, polices, modèles et autres ressources.

### Overlay
Descriptions des éléments graphiques à afficher.

### Rendering
Interface de rendu indépendante des API graphiques.

Backends prévus :
- SDL
- Metal
- OpenGL
- OBS

### Runtime
Pipeline temps réel et orchestration des modules.

### Application
Outils de démonstration et interface utilisateur.

---

## Pipeline

Camera
↓
Vision
↓
Calibration
↓
Projection
↓
Overlay
↓
Rendering

---

## Objectif

Construire un moteur de réalité augmentée sportif, performant, modulaire et indépendant de toute technologie de rendu.