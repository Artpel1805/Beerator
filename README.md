<div align="center"># Beerator
## Introduction

Le but du projet est de réaliser intégralement un robot capable de se déplacer, de chercher et d’attraper une canette (de bière ? 😶), relever sa couleur, puis de la déposer dans la zone de couleur correspondante.
Partant d’une liste de tous les composants principaux à employer et d’une proposition de châssis, toutes les étapes sont à réaliser de l’aspect matériel (Modélisation sous logiciel de CAO, soudage des composants) jusqu’à l’asservissement logiciel, en passant par la planification et la réalisation du PCB.

Voici un model 3D de ce à quoi pourrais ressembler le robot:

![img](Beerator3D.png)

Le travail fut réparti entre les quatre membres de l’équipe, et les responsabilités suivantes furent attribuées:
* Chef de projet / Responsable intégration : Arthur Pellegrin
* Responsable Software : Nassim Rami
* Responsable Hardware : Adrien Lenoir
* Responsable Livrables : Clarisse Cheng

À l’écriture de ses lignes, nous avons planifié tout le projet, réalisé le PCB, designé et imprimé le châssis. Les composants sont en cours de soudage. Il reste ainsi à réaliser l’assemblage des pièces, l’algorithme de fonctionnement du robot, la commande des différents actionneurs, les calculs d’odométrie, et l’asservissement logiciel.

## General Informations

Vous trouverez dans ce repo bon nombre de dossier.

- Nous avons dans un premier temps rassembler les contraintes et les fonctionnalités du robot dans un cahier des charges: [Cahier des Charges](Cahier%20des%20Charges)

- Nous avons ensuite listé toutes les documentations des composants que nous utiliserons pour répondre à notre cahier des charges dans le dossier [Documentation](Documentation)

- Après cela nous avons architecturer la partie électronique du projet dans le dossier [Diagramme](Diagramme)

- Une fois l'architecture satisfaisante nous avons modéliser la carte électronique sous KiCaD vous trouverez le projet dans le dossier [KiCad/Beerator](KiCad/Beerator). Vous pourrez aussi trouver la BOM dans le fichier

- Nous avons en parallele esssayé d'améliorer le modele 3D du robot. Des choix ont été fait afin d'amméliorer la répartition des masses dans le robots et avoir une meilleure disposition des capteurs.

Les différents travaux effectués sont détaillés dans les parties qui suivent.

