# Projet Cours de Réseau
## Julian BARKOUDEH et Camille DOUZET EI2I3 II

### Objectif du projet
L'objectif du projet est d'implémenter une application permettant de gérer des comptes bancaires en utilisant une architecture client-serveur.

### Rappels des différentes fonctionnalités
Chaque client peut avoir un ou plusieurs comptes bancaires (jusqu'à 3 dans notre cas)
Tous les comptes sont déjà crées au préalable.
Vous pouvez consulter la liste des clients et comptes actifs un peu plus bas.
Vous pouvez effectuer 4 types d'opérations :
    1. Faire un **ajout** : Ajouter une somme dans le compte que vous aurez choisi
    2. Faire un **retrait** : Debiter un montant dans le compte voulu
    3. Obtenir le **solde** de votre compte.
    4. Obtenir les 10 dernières **opérations** du client
### Veuillez saisir le numéro de l'opération dans la case de message destiné au serveur afin que votre demande soit prise en compte. 
Par exemple, ecrire "3", pour afficher votre solde. 
Le serveur vous renverra les réponses à vos requêtes.

### Comment est organisé le projet ?
Il y a 5 scripts principaux :
- **server.c** : Contient le code utile pour la création des sockets et de la communication avec le protocole TCP
- **client.c** : Contient le code nécessaire pour établir la connexion avec le serveur avec le protocole TCP
- **serverUDP.c** : Contient le code utile pour la création des sockets et de la communication avec le protocole UDP
- **clientUDP.c** : Contient le code nécessaire pour établir la connexion avec le serveur avec le protocole UDP
- **communication.c** : Fonctions utiles pour le script server.c
- **communication.h** : Structures et Prototypes des fonctions utilisées dans server.c
Il y a aussi un **makefile** pour facilité la compilation du projet

### Comment exécuter le code ?
1. Se placer dans le dossier du projet
2. Faire un `make` dans un premier terminal puis executé la ligne `./tcp.out` pour executer les codes pour protocole en TCP 
                                                                  `./udp.out` pour executer les codes pour protocole en UDP 
3. Ouvrir un 2e terminal et executer les deux commandes : 
    - `gcc client.c -o client`
    - `./client`
Normalement la connection s'établie correctement.

### Récapitulatif des clients et comptes existants
- client 1 :
identifiant : julian
mot de passe : 1234
état des comptes :
    - compte 1 : 100
    - compte 2 : 3000
    - compte 3 : ...

- client 2 :
identifiant : camille
mot de passe : 1234
état des comptes :
    - compte 1 : 500
    - compte 2 : ...
    - compte 3 : ...

- client 3 :
identifiant : maria
mot de passe : 1234
état des comptes :
    - compte 1 : 10000
    - compte 2 : ...
    - compte 3 : ...

### Les étapes à suivre pour effectuer une action 
1. Envoyer un premier message pour initialiser la communication
2. S'identifier : La banque vous demande d'abord votre identifiant puis votre mot de passe
3. Choisissez l'opération que vous souhaitez réaliser
4. Si tout se passe bien vous pouvez choisir une nouvelle action
5. ENvoyez **exit** lorsque vous voulez terminer la communication

#### A bientot chez la BarkouDouzetBanque !