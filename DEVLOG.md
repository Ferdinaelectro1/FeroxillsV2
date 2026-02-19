## 19 Février 2025

### Ce que j'ai fait 
- Correction du bug qui fesait que l'affichage en mode auto n'était pas stable (détection du trigger sur une copie d'échantillons)
- Correction du bug du signal de test random qui générait la valeur 0 simplement
- Implémentation de la possibilité de régler la tension maximale du signal via l'interface qml

### Ce qui bloque
- Le signal affiché dans la fenetre qml vibre
- À vitesse de reception élevé on constate des sauts d'image sur l'interface qml en mode continu(roll)

### Demain
- Implémenté la ligne de graduiation verticale qui va servir de repère pour les divisions verticales
- Implémenté la possibilité de régler la fréquence du signal de test depuis l'interface qml
