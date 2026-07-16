# info-HCCSupport - Outil de diagnostic poste

## Description

Cet outil permet de récupérer automatiquement les informations principales d'un poste utilisateur afin de faciliter les échanges avec le service informatique.

L'objectif est de remplacer l'ancienne application développée en Python, devenue indisponible et présentant des lenteurs lors de la collecte des informations.

Cette nouvelle version a été développée en **C++** afin d'améliorer :
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

La fonctionnalité de copie permet à l'utilisateur de récupérer toutes les informations affichées en un clic afin de les transmettre directement au service informatique en faisant un simple copier/coller
---

## Technologies utilisées

- **Langage :** C++
- **Système cible :** Windows
- **API utilisées :**
  - Windows API
  - WMI (Windows Management Instrumentation)
  - API réseau Windows

---

## Prérequis

Pour exécuter l'application :

- Windows 10 ou supérieur
- Droits utilisateur standards suffisants
- Poste connecté au réseau de l'hopital (recommandé pour récupérer les informations Active Directory)


   note: il s'agit d'une première version du logiciel


## auteur

- Théo Garde


projet datant du 10/07/2026