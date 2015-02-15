# Outils

Ce dossier contient une version légèrement modifiée du fichier th105e.dll du patch anglais, et une DLL faite maison.
Ces 2 fichiers permettent de :
- charger une police modifiée, permettant d'afficher des caractères japonais
- modifier les chaînes en dur dans le jeu.

Pour les utiliser, mettez th105e.dll et i32.dll dans le dossier du jeu, et mettez MonoSpatialModSWR.ttf dans th105e.dat (pour ce faire, vouz pouvez par exemple ouvrir th105e.dll avec WinRAR).

Ce dossier contient aussi 2 programmes (extract.exe et repack.exe), qui permettent d'extraire des informations des fichiers .dat décrivant les images (par exemple data_menu_config_Config.dat), puis de mettre dans les fichiers .dat les informations modifiées.
Pour les utiliser, faites glisser le fichier .dat concerné sur le programme correspondant (extract.exe pour extraire les informations, ou repack.exe pour insérer les changements dans les fichiers .dat).
