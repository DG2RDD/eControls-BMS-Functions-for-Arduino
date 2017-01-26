# eControls-BMS_Fr et BMS_En
Bibliothèque de fonctions Arduino pour la GTB et la régulation industrielle 

Avez-vous déjà tenté d'écrire un PID en code Arduino à partir de rien ? Votre arduino a-t-il apris à voler en cours de route ?

OK, Maintenant reprenons notre calme et jetons un coup d'oeil à cette Repo. Ma camarades et moi-même utilisons ces fonctions de contrôle-commande depuis un peu plus d'un quart de siècle (tu peux me traiter de dinosaure si çà te fait plaisir, çà m'est égal, mais garde en tête que je codais probablement alors que tu n'étais probablement même pas encore inscrit sur le grand mur de la Vie, de l'Univers, et du reste ...)

Alors, y'a quoi là-dedans, vieux machin ?
Un paquet de fonctions C développées à l'origine dans un environnement propriétaire apparenté. Nous les avons traduites en C Arduino récemment. en fait, nous les utilisons actuellement dans une version industrialisée de l'arduino appelée Industruino, mais elles peuvent pour la plupart fonctionner sur un Arduino. La compatibilité matérielle est listée dans le fichier .cpp, dans l'en-tête de chacune des fonctions.
Une fois combinées, ces fonctions vous permettront de réaliser des applications d'automatisme et de régulation industrielle complexes pour lesquelles le volume de code à produire sera réduit au minimum.

L'initiale de chaque nom de fonction l'apparente à l'une des quatre familles suivantes :
- "C" : fonctions dédiées à la commande de marche et d'arrêt des équipements
- "R" : fonctions de régulation PID et traitements des signaux autour du PID
- "S" : fonctions d'information et de calcul de variables système
- "T" : fonctions de calcul logique ou analogique. Cette famille est un peu mon sac à malices 

Parmi les problèmes récurrents dans le contrôle et régulation, le façage et le retour à un fonctionnement automatique reveint fréquemment. Toutes nos fonctions sont compatibles avec un principe unique que j'appelle l'ISF (Interface Standard de Forçage).
Vous allez découvrir que la plupart reçoivent un ordre de forçage à travers un argument integer appelé CFG :
- CFG =-1 signifie une action forcée à Off ou à 0
- CFG = 0 signifie aucune action (la fonction est neutralisée ou "débrayée")
- CFG = 1 signifie un fonctionnement en auto (fonction active)
- CFG = 2 signifie un forçage à On ou à une valeur prédéfinie (forçage conditionnel, ou en position de repli)
- CFG = 3 signifie un forçage à On ou à l'action maximum (forçage inconditionnel, ou en position maxi). 

Evidemment, vous pouvez calculer une valeur de CFG selon des états et des conditions. Il y a une fonction pour çà : C_COMMUT

Eh, toto. Maintenant que tu as fini de causer, si on parlait de code ?

"C" - La famille des Commandes auxquelles on obéit
- C_CDELEM : Produit une commande booléenne à partir d'une demande et d'un défaut booléens. Compatible ISF
- C_TPOCDE : Comme C_CDELEM, mais avec des délais d'aéctivation et de désactivation. Compatible ISF
- C_COMMUT : Calcule l'integer CFG de forçage des autres fonctions via leur commande ISF
- C_DISCOR : Différentiel temporisé entre deux booléens ou deux floats vers un booléen. Compatible ISF
- C_TFONCT : Calcul de temps de fonctionnement. Compatible ISF

"R" - La famille du PID et de ses proches collaborateurs
- R_ANA3PL : Traduit une action 0-100% en trois-points à travers 2 booleans. Compatible ISF
- R_ANA3PT : Traduit une action 0-100% en trois-points à travers 2 sorties binaires. Compatible ISF
- R_ANATOL : Traduit une action 0-100% en PWM à travers un boolean. Compatible ISF
- R_ANATOR : Traduit une action 0-100% en PWM à travers une sortie binaire. Compatible ISF
- R_PIDITR : Régulateur PID parallèle intératif digital. Compatible ISF

"S" - La famille qui exploite le Système
- S_SCANTS : Calcule en temps réel du temps de cycle CPU. Compatible ISF
- S_TIMERS : Active et désactive des booléens à des intervalles fixes dans le temps. Compatible ISF

"T" - Le sac à malices
- T_APPRLN : Traduit un Float en un autre via une approximation polylinéaire. Compatible ISF
- T_CLINEB : Traduit un Float en un autre via une conversion linéaire bornée. Compatible ISF
- T_COMULT : Multiplexe un tableau de 16 booléens en un Unsigned Integer. Compatible ISF
- T_DEMULT : Démultiplexe un unsigned integer en un tableau de 16 booléens. Compatible ISF
- T_DMULTC : Démultiplexe 5 contacts secs depuis une entrée en tension (câblage à faire). Compatible ISF
- T_HYSTPO : Commande M/A temporisée d'un booléen à partir d'un Float et de seuils bas et haut. ISF adaptée
- T_LIMVAR : Limiteur de vitesse de variation d'un float. Compatible ISF
- T_MOYDYN : Moyenne dynamique (filtre du 1er ordre) sur un float. Compatible ISF
- T_MULSEC : Commandes M/A selon le nombre d'équipements demandé, les défauts, avec priorités et secours mutuel. Compatible ISF
- T_SEQCSR : Séquenceur de charges multiples à partir d'une commande analogique. Compatible ISF
- T_SEQDEM : Séquenceur de charges multiples à partir de dépassements de seuils sur une commande analogique. Compatible ISF
