# <div align="center">Beerator</div>
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

## Partie I - [partie "II" Clarisse]

## Planification du circuit, Création du PCB

Comme vu précédemment nous avons découpé l'électronique de notre robot en 4 blocs fonctionnels.

Nous devons maintenant créer le schéma électronique de notre carte en respectant cette répartition. Cela n’a pas d'effet sur l'électronique impliqué, mais cela aidera à la maintenabilité et à la lisibilité du projet.

De même, nous traitons chaque composant indépendamment. Ils seront reliés grâce à des labels dans le logiciel * KiCad *.

### ALIMENTATION

Nous avons décidé de connecter la batterie grâce à un JST, ainsi nous pourrons facilement la retirer pour la recharger.
Concernant les régulateurs, nous dimensionnons leurs capacités de découplage en accord avec leurs documentations respectives.

Nous avons ensuite décidé de rajouter des témoins d’alimentation sur la batterie et le 3.3V. Les leds que nous avons choisies consomment 10mA pour fournir une luminosité maximale.
Comme ce sont uniquement des témoins (Nous n’avons pas besoin de beaucoup de luminosité) et que le 3.3V ne peut pas fournir une trop grande intensité nous avons choisis de réduire le courant maximal à 1mA. 
Nous dimensionnons donc les résistances en conséquence, avec une loi d’ohm sachant que les LEDs sont en 3V. 

![img](Photos_PEL/PEL_1.png)

### CAPTEURS

Nous avons décidé de connecter tous nos capteurs grâce à des JST. 
Cela permettra une meilleure modularité physique des capteurs sur le robot.
Cela évite aussi de concevoir une grande carte ou plusieurs cartes, les capteurs disposant presque tous de leur propre carte.

Nous avons quand même dû ici respecter la géométrie des capteurs afin que les JST correspondent aux bonnes I/O des cartes électroniques des capteurs.

Des résistances de Pull-Up ont dû être ajoutées, pour l’I2C car cela est nécessaire pour que ce protocole fonctionne correctement.

![img](Photos_PEL/PEL_2.png)


### ACTIONNEURS

Nous avons 3 actionneurs - 2 MCC et 1 Servomoteur.

Les MCC sont commandés par des Drivers et leurs capacités de découplage.
Cette partie du projet est assez délicate. Nous décidons donc de mettre des Solder Jump afin de pouvoir tester nos signaux avant de les connecter aux moteurs.

Le Servo quant à lui suit un protocole particulier, il sera branché sur un port USART en mode Half-Duplex du uP nécessitant une résistance de Pull-Up.

![img](Photos_PEL/PEL_3.png)

### CONTRÔLEUR

Jusqu’ici tous les modèles électroniques et les placements de toutes les I/O étaient imposés par les constructeurs.

Nous modélisons le microcontrôleur avec ses 64 broches. 
Ensuite nous avons dû brancher tous nos composants à notre microcontrôleur.

Nous avons mis un point d’honneur à optimiser le choix des broches en suivant un protocole bien particulier: 
Regrouper au maximum les broches étant reliées à un même composant
Organiser les broches de manière à minimiser les croisements lors du routage
Segmenter les sous-circuits
Prendre en compte les problèmes de compatibilité électgromagnétique

Grâce au logiciel STM32CubeMx nous avons pu placer les broches sur des pins répondant aux besoins.

<img src="Photos_PEL/PEL_4.png" width="500">

Nous devons aussi alimenter le STM32 et il ne faut surtout pas oublier les capacités de découplage car l’alimentation doit être la plus stable possible.

Nous choisissons aussi de rajouter un Quartz externe, car l’horloge est plus précise.

Il ne faut pas oublier de rajouter, les branchements pour le STLink car c’est lui qui nous permettra de programmer le STM32.

![img](Photos_PEL/PEL_5.png)

Enfin nous réalisons une analyse électrique de notre circuit pour savoir s’il comporte des erreurs et nous les corrigeons jusqu’à ce qu’il n’y en ai plus.

<img src="Photos_PEL/PEL_6.png" width="500">


### Production du PCB

La prochaine étape est d’assigner les empreintes correspondants aux composants. Nous utiliserons des composants SMD 0603. Pour les connecteurs nous avons utilisé des connecteurs JST de type JST HX 2.54mm.
On peut remarquer que certains composants ne sont pas assignés, en effet ce sont les composants qui seront directement soudé au capteur donc ils ne doivent pas être présent sur notre PCB

<img src="Photos_PEL/PEL_7.png" width="500">

Nous travaillons sur un PCB 4 couches, on décide donc d’y placer 2 plans de potentiels séparés par 2 plans de masses pour d'obscures raisons de CEM.
On peut voir ci dessous le Stack-Up de notre carte

* <font color=red>5V (Rouge)</font>
* <font color=green>GND (Vert)</font>
* <font color=yellow>3,3V (Jaune)</font>
* <font color=blue>GND (Bleu)</font>

![img](Photos_PEL/PEL_9.png)
![img](Photos_PEL/PEL_10.png)
![img](Photos_PEL/PEL_11.png)
![img](Photos_PEL/PEL_12.png)

JLC PCB, le fabricant de nos carte nous impose aussi des contraintes de conception que nous devons renseigner.

<img src="Photos_PEL/PEL_13.png" width="400">

La prochaine est le placement des composants sur le PCB.
Nous essayons de regrouper les composants au maximum par groupe pour que le routage soit le plus simple possible

On décide aussi de placer le STM32 au centre car c’est lui qui possède le plus de connexion.

On voit ci dessous en vert tous les composants relatif au contrôle, en rouge tous le bloc actionneurs, en bleu le bloc alimentation et en jaune le bloc capteur

<img src="Photos_PEL/PEL_8.png" width="500">

Nous relions ensuite tous les composants électriquement. Nous veillerons aussi à utiliser les pistes les plus larges possible surtout pour la partie puissance. 
Cela permet de laisser passer le courant nécessaire et d’éviter des problèmes de CEM.

Quelques règles de CEM que nous nous efforçons de respecter : 
Pas d’angle à 90º
Les signaux à haute fréquence ne doivent pas se croiser, et les pistes doivent être le plus court possible
Il faut essayer de respecter une distance entre les pistes

Voici ce à quoi ressemblent nos deux plans avec des pistes.

<img src="Photos_PEL/PEL_14.png" width="500">
<img src="Photos_PEL/PEL_15.png" width="500">

Et enfin notre carte modélisé en 3D

<img src="Photos_PEL/PEL_16.png" width="500">


## Partie III - [partie "I" Nassim]

## Partie IV - [partie "IV" Adrien]
