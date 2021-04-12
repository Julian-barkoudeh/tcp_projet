# tcp_projet

Comment utiliser le code?
1. Se placer dans le dossier du projet
2. Faire un make dans un premier terminal puis executé la ligne ./a.out
3. Ouvrir un 2e terminal et executer les deux commandes : 
    - gcc client.c -o client
    - ./client
Normalement la connection s'établie correctement.

Notre problème se situe après l'identification, cad au moment d'entrer le mot de passe puisque le serveur n'affiche pas les consignes (le message "Veuillez saisir votre mdp") (ligne 92 dans le fichier server.c)