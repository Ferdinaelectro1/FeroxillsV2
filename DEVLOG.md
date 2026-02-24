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

## 20 Février 2025

### Ce que j'ai fait 
- Implémentation de la ligne de repère vertical Y dans l'interface graphique

### Demain
- Implémenté la possibilité de régler la fréquence du signal de test depuis l'interface qml

## 21 Février 2025

### Ce que j'ai fait
- Implémentation de la possibilité de régler la fréquence du signal de test depuis l'interface qml
- Amélioration de certaines parties de l'interface qml
- Création d'un dossier qui va habriter les composants custom utile à l'interface qml

### Demain
- Nommer le rôle de chaque slider permettant de régler les paramètres du générateur de signal de texte sur l'interface qml

## 22 Février 2025

### Ce que j'ai fait
- Nommage du rôle de chaque slider permettant de régler les paramètres du générateur de signal de texte sur l'interface qml

### Demain
- Résoudre le problème de l'affichage qui saute en mode continue quand la vitesse d'arrivé des données augmente


## 23 Février 2025

### Ce que j'ai fait
- Génération d'une pulsation à un instant t depuis de le générateur de signal pour simuler un évènement qui survient sur signal.
- Détection et affichage lorsqu'un trigger est détecter , en mode affichage trigger

### Demain
- Implémenter un pop pup permettant au user , de saisir la valeur du trigger de déclanchement en mode d'affichage trigger
- Résoudre le problème de l'affichage qui saute en mode continue quand la vitesse d'arrivé des données augmente

## 24 Février 2025

### Ce que j'ai fait
- Implémenté un pop pup permettant au user , de saisir la valeur du trigger de déclanchement en mode d'affichage trigger
- Implémenter une classe EventBus permettant de faire communiquer n'importe quel classe avec l'interface qml sans forcément créer d'ojet dans le backend

### Demain
- Optimiser le projet
- Résoudre le problème de l'affichage qui saute en mode continue quand la vitesse d'arrivé des données augmente