# calculator

Une calculatrice dans un terminal.

Elle est capable de gerer :
- Addition (+)
- soustraction (-)
- multiplication (*)
- division (/)
- puissances entieres (^)

elle est capable de manipuler des floats actuellement, peut etre je la joindrait au projet MathsLib un jour.

Defaut d'implementation (a regler peut etre un jour) :
Les donnees sont separees dans deux tableaux differents, un pour les operations, un deusieme pour les nombres en tant que tel, la difficultee etant de faire la correspondance entre les deux.
Une meilleure implemetation gererait ca dans le meme tableau, avec une classe mere dont herite chaque type d'implementation.
