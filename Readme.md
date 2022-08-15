SAE 02 C Program

The main purpose of this project is to find and implement the best data structure for easy or fast read access on a large data.

The text below is a note of brainstorming and thought process to solve the problem.

#For that, we can use a HashTable.

To minimize collision, for permutation names like "abcd", "bcda", "dcab" which will return the same hash key, we define a constant. We chose a prime number = 31 which we will multily every character ASCII (nowadays known as UNICODE). This way, the hash output is affected by the position of the character and hash("abcd") & hash("bcde") will be different.

Optimization: To reduce collision and %Table giving too many zero value, we can choose an odd sized table or table size equal a prime number. (Reference Maths Discrètes on ring)

#Fichier à traiter: dpt2020.csv

Question à se poser:
    -Les entrées (Noms) sont ils majuscule miniscule?

On réalise vite que la lecture prend énormement de temps pour la première fois
Alors il faudra stocker les éléments qui nous interessent pour éviter la relecture
mais quelle structure de données choisir?


A voir les tables de hachage: 
    hash function knuth

Structure possible ["ALAIN",[0,2,5]] ou chaque indice l'année Ex:1900,1920

Dans "dpt2020.csv", on voit que les données (prénoms) sont triés par ordre alphabétique alors on peut soit
utilisé un tableau trié (un tableau de struct qui contient le sexe, le prenom et année) et rechercher un data par dichotomie ou utiliser une structure de donnée équivalent qui est l'arbre binaire de recherche
Le plus est bien sûr d'utiliser un tableau mais le problème avec un tableau est la taille qui doit être prédéfini si l'utilisateur rentre un csv qui dépasse la taille du tableau alors il y aura un problème
