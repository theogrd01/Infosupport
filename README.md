# info- InfoSupportSupport - Outil de diagnostic poste

## Description

Cet outil permet de récupérer automatiquement les informations principales d'un poste utilisateur afin de faciliter les échanges avec un service informatique.


Cette  version a été développée en **C++** afin d'améliorer :
- Les performances de récupération des données
- Le temps d'affichage des informations
- La simplicité d'utilisation pour les utilisateurs finaux

---

## Fonctionnalités

L'application collecte automatiquement les informations suivantes :

### Informations utilisateur
- Nom du poste
- Nom de l'utilisateur connecté
- Nom du domaine Active Directory
- Version du système d'exploitation
- Temps depuis le dernier démarrage (Uptime)

### Informations réseau
- Adresse IP
- Adresse de la passerelle réseau
- Serveurs DNS configurés
- Statut DHCP (activé ou non)

### Informations matérielles
- Constructeur de la machine
- Modèle du poste
- Numéro de série
- Quantité de RAM disponible
- Espace occupé sur le lecteur `C:`

### Informations périphériques
- Imprimante par défaut

---

## Interface utilisateur

L'application propose une interface simple permettant :

- L'affichage rapide des informations du poste
- La lecture facile des données par un utilisateur non technique
- La copie complète des informations via un bouton dédié

La fonctionnalité de copie permet à l'utilisateur de récupérer toutes les informations affichées en un clic afin de les transmettre directement au service informatique en faisant un simple copier/coller (voir l'image à l'appui)


![alt text](image.png)
---

## Technologies utilisées

- **Langage :** C++
- **Système cible :** Windows
- **API utilisées :**
  - Windows API
  - WMI (Windows Management Instrumentation)
  - API réseau Windows

---

## Execution

Pour exécuter l'application :

- Windows 10 ou supérieur
- Droits utilisateur standards suffisants
- Poste connecté au réseau de l'entreprise(recommandé pour récupérer les informations Active Directory)





## Prérequis pour modification
- Visual Studio 2022
- CMake
- WiX Toolset 6

### Construction

```bash
cmake -S . -B build
cmake --build build --config Release
```

L'exécutable est généré dans :

```
build/Release/info-support InfoSupport.exe



```
> **Note :** il s'agit du même repertoire pour tout les pc Windows du groupe  InfoSupport connecté au serveur.





# Création du package MSI avec WiX 6

Le déploiement de **Info Support** est réalisé à l'aide de **WiX Toolset 6**, qui permet de générer un installateur Windows au format **MSI**.

Le fichier `Package.wxs` décrit l'ensemble du package d'installation : les informations du logiciel, les fichiers à installer ainsi que l'arborescence des dossiers créés sur le poste utilisateur.

---

# Structure du fichier Package.wxs

Le fichier est composé de plusieurs sections importantes.

## 1. Informations du package

```xml
<Package
    Name=" InfoSupport"
    Manufacturer="nom entreprise, Theo Garde"
    Version="1.0.1"
    UpgradeCode="{db21186f-0c97-4710-b9b4-33bea6f15317}">
```

Cette section contient les informations générales du logiciel.

| Élément | Description |
|---------|-------------|
| `Name` | Nom affiché dans Windows Installer et dans les programmes installés. |
| `Manufacturer` | Éditeur de l'application. |
| `Version` | Version actuelle du logiciel. |
| `UpgradeCode` | Identifiant permanent utilisé par Windows Installer pour reconnaître les différentes versions du logiciel. |

> **Important :** L'`UpgradeCode` ne doit jamais être modifié. Il permet aux futures versions du logiciel de remplacer automatiquement les anciennes.

---

## 2. Gestion des mises à jour

```xml
<MajorUpgrade
    DowngradeErrorMessage="Une version plus récente de  InfoSupport est déjà installée." />
```

Cette instruction indique à Windows Installer :

- d'autoriser les mises à jour du logiciel ;
- de désinstaller automatiquement l'ancienne version avant d'installer la nouvelle ;
- d'empêcher l'installation d'une version plus ancienne.

---

## 3. Compression des fichiers

```xml
<MediaTemplate EmbedCab="yes" />
```

L'option `EmbedCab="yes"` intègre tous les fichiers directement dans le fichier MSI.

Ainsi, un seul fichier (`Package.msi`) est nécessaire pour distribuer le logiciel.

---

## 4. Arborescence des dossiers

```xml
<StandardDirectory Id="TARGETDIR">

    <Directory Id=" InfoSupportFolder" Name=" InfoSupportAppli">

        <Directory Id="INSTALLFOLDER" Name="InfoSupport">

            <Directory Id="AssetsFolder" Name="assets"/>

        </Directory>

    </Directory>

</StandardDirectory>
```

Cette partie définit les dossiers créés pendant l'installation.

Une fois installé, le logiciel sera placé dans :

```text
C:\ InfoSupportAppli\InfoSupport
```

Les ressources graphiques seront placées dans :

```text
C:\ InfoSupportAppli\InfoSupport\assets
```

---

## 5. Déclaration des fichiers

Chaque fichier installé doit appartenir à un **Component**.

Exemple pour l'exécutable :

```xml
<Component Id="MainExecutable"
           Guid="{guid example}">

    <File
        Id="InfoSupportExe"
        Source="payload\info-support InfoSupport.exe"
        KeyPath="yes"/>

</Component>
```

Le paramètre :

```xml
KeyPath="yes"
```

indique à Windows Installer que ce fichier représente le composant.

Les deux logos sont déclarés exactement de la même manière dans le dossier `assets`.

---

## 6. Les GUID

Chaque composant possède un identifiant unique :

```xml
Guid="{exemple guid}"
```

Les GUID permettent à Windows Installer de savoir quels fichiers sont installés sur le poste.

### Important

- Ne jamais modifier un GUID déjà utilisé dans une version distribuée.
- Lorsqu'un nouveau composant est ajouté, il doit posséder un nouveau GUID.

---

## 7. Les Features

```xml
<Feature Id="MainFeature"
         Title=" InfoSupport Info Support"
         Level="1">
```

Une **Feature** est un regroupement de composants.

Dans ce projet, elle contient :

- l'exécutable ;
- le logo `.ico` ;
- le logo `.png`.

Tous ces composants seront installés ensemble.

---

# Le dossier payload

Le dossier `payload` contient tous les fichiers qui seront intégrés dans le MSI.

Exemple :

```text
payload/
│
├── info-support InfoSupport.exe
│
└── assets/
    ├── logoentreprise.ico
    └── logoentreprise.png
```

Avant chaque génération du package MSI, il est indispensable de remplacer l'ancien exécutable par la dernière version compilée.

---

# Génération du MSI

Depuis le dossier contenant `Package.wxs`, exécuter :

```developper powershell
wix build Package.wxs
```

Un fichier `Package.msi` sera généré.

---

# Création d'une nouvelle version

Pour publier une nouvelle version :

1. Compiler le projet C++.
2. Copier le nouvel exécutable dans le dossier `payload`.
3. Modifier le numéro de version :

```xml
Version="1.0.2"
```

4. Conserver le même `UpgradeCode`.
5. Générer le nouveau MSI :

```powershell
wix build Package.wxs
```

Windows Installer remplacera automatiquement l'ancienne version par la nouvelle.

---

# Bonnes pratiques

- Ne jamais modifier l'`UpgradeCode`.
- Ne pas modifier les GUID des composants existants.
- Incrémenter la version à chaque publication.
- Vérifier que le dossier `payload` contient bien la dernière version de l'exécutable.
- Tester le MSI sur une machine de test avant tout déploiement en production.

---

# Résumé de l'arborescence du projet

```text
 InfoSupportInstaller/
│
├── Package.wxs
├── Package.msi
│
└── payload/
    ├── info-support InfoSupport.exe
    │
    └── assets/
        ├── logoentreprise.ico
        └── logoentreprise.png
```

Le rôle de chaque élément est le suivant :

| Élément | Rôle |
|---------|------|
| `Package.wxs` | Décrit entièrement le package d'installation. |
| `payload` | Contient tous les fichiers à intégrer dans le MSI. |
| `info-support InfoSupport.exe` | Exécutable principal du logiciel. |
| `assets` | Ressources graphiques utilisées par l'application. |
| `Package.msi` | Installateur Windows généré par WiX 6. |

---

# Références

Documentation officielle WiX :

https://docs.firegiant.com/wix/whatsnew/

Documentation Windows Installer :

https://learn.microsoft.com/windows/win32/msi/windows-installer-portal


## Recommandations


Toutes personne souhaitant modifier ce logiciel doit être en mesure d'avoir au minimum des bases en C++ et connaitre le fonctionnement de WIX 6 pour la création de fichier msi (se reféré à la documentation très complète sur internet).

NE JAMAIS laisser faire à 100% du "vipecoding" et être totalement sûr des changements effectués sur le logiciel.
## auteur

- Théo Garde - étudiant à Epitech


projet datant du 10/07/2026
