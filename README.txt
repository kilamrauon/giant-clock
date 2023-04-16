A giant clock based on a wemos mini d1, a real time clock module ds1302, 
a mini keyboard KC11B04 (analogue output, it is necessary to check what each button returns
and adapt the code, to be connected to the 3.3 v output, otherwise the values
values may be wrong), and ws2812b leds (depending on the size,
I chose segments of 6 leds and the seconds are displayed by 4 leds inserted 
between the hours and minutes). The size of the segments is contained in a variable
as well as the size of the seconds separator, don't forget to modify the NUM_LEDS
(it is possible to change the program so that this is done automatically).
I do not use the wifi to seek the hour, the modification of the hour is done with the first button,
the modification of the minutes is done with the second button.
The third button changes the brightness and the fourth the colour (with a regular change of colour if you select
(with a regular change of colour if colour 6 is selected).
The LED segments can be associated with specific connectors as shown in photo 1chiffre.jpg
or be soldered. Each method has its advantages and disadvantages. 

Une horloge géante à base de wemos mini d1, un module horloge temps réél ds1302, 
d'un mini clavier KC11B04 (sortie analogique, il faut vérifier ce que renvoie 
chaque bouton et adapter le code, à brancher sur la sortie 3.3 v sinon les valeurs
renvoyées peuvent être erronées), et de leds ws2812b (en fonction de la taille,
j'ai choisi des segments de 6 leds et les secondes sont affichées par 4 leds insérées 
entre les heures et les minutes). La taille des segments est contenu dans une variable
ainsi que la taille du separateur des secondes, ne pas oublier de modifier la constante NUM_LEDS
(il est possible de changer le programme pour que cela se afsse automatiquement).
Je n'utilise pas le wifi pour chercher l'heure, la modification  de l'heure se fait avec le premier bouton,
la modification  des minutes se fait avec le second bouton.
Le troisème bouton modifie la luminosité et le quatrième la couleur (avec un changement
de couleur régulier si on sélectionne la couleur 6)
Les segments de led peuvent associés avec des connecteurs specifiques comme sur la photo 1chiffre.jpg
ou être soudés. Chaque méthode a ses avantages et ses inconvénients. 
