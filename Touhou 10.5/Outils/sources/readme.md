# Sources des outils

Ce dossier contient le code source de i32.dll et des 4 .exe dans le dossier parent.
Vous ne devriez pas avoir besoin de regarder ce dossier, sauf si vous comptez modifier le fonctionnement interne du patch.
Si vous désirez malgré tout continuer, lisez le fichier cc_GDI32.bat (concernant i32.dll), ou extract.c et repack.c (concernant extract.exe et repack.exe).

Pour compiler cv0_to_txt.exe et txt_to_cv0.exe :
gcc cv0_to_txt.c encoding.c -Wall -Wextra -liconv -o cv0_to_txt.exe
gcc txt_to_cv0.c encoding.c -Wall -Wextra -liconv -o txt_to_cv0.exe