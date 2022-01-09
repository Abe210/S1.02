Question à se poser:
    -Les entrées (Noms) sont ils majuscule miniscule
    -

On réalise vite fait que la lecture prend énormement de temps pour la première fois
Alors il faudra stocker les éléments qui nous interessent pour éviter la relecture
mais quelle structure de données choisir?


A voir les tables de hachage: 
    hash function knuth

Structure possible ["ALAIN",[0,2,5]] ou chaque indice l'année Ex:1900,1920

on voit que les données (prénoms) sont triés par ordre alphabétique alors on peut soit
utilisé un tableau trié (un tableau de struct qui contient le sexe, le prenom et année) et rechercher un data par dichotomie ou utiliser une structure de 
donnée équivalent qui est l'arbre binaire de recherche
Le plus est bien sûr d'utiliser un tableau mais le problème avec un tableau est la taille qui doit être prédéfini si l'utilisateur rentre un csv qui dépasse la taille du tableau alors il y aura un problème