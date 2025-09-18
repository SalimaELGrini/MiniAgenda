#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_CONTACTS 100
#define MAX_NAME 50
#define MAX_PHONE 15
#define FILENAME "contacts.txt"

typedef struct {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
} Contact;

Contact agenda[MAX_CONTACTS];
int n = 0;

// Validation nom
int validerNom(char *nom) {
    int i = 0;
    while (nom[i]) {
        if (!isalpha(nom[i]) && nom[i] != ' ') return 0;
        i++;
    }
    return 1;
}

// Validation téléphone marocain
int validerTelephoneMaroc(char *tel) {
    int len = strlen(tel);
    if (len != 10) return 0;
    if (!(tel[0]=='0' && (tel[1]=='6' || tel[1]=='7'))) return 0;
    for(int i=0;i<10;i++) if(!isdigit(tel[i])) return 0;
    return 1;
}

// Lire string avec fgets et enlever \n
void lireString(char *str, int taille) {
    fgets(str, taille, stdin);
    int len = strlen(str);
    if (len > 0 && str[len-1]=='\n') str[len-1]='\0';
}

// Charger contacts
void chargerContacts() {
    FILE *f = fopen(FILENAME, "r");
    if (!f) return;
    n = 0;
    while (fscanf(f, " %[^\n]s %[^\n]s", agenda[n].name, agenda[n].phone) == 2 && n < MAX_CONTACTS)
        n++;
    fclose(f);
}

// Sauvegarder contacts
void sauvegarderContacts() {
    FILE *f = fopen(FILENAME, "w");
    if (!f) { printf("Erreur sauvegarde!\n"); return; }
    for (int i = 0; i < n; i++)
        fprintf(f, "%s\n%s\n", agenda[i].name, agenda[i].phone);
    fclose(f);
}

// Trier contacts
void trierContacts() {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (strcmp(agenda[j].name, agenda[j+1].name) > 0) {
                Contact temp = agenda[j];
                agenda[j] = agenda[j+1];
                agenda[j+1] = temp;
            }
}

// Afficher contacts
void afficherContacts() {
    if (n == 0) { printf("Aucun contact.\n"); return; }
    trierContacts();
    printf("\n--- Liste des contacts ---\n");
    for (int i = 0; i < n; i++)
        printf("%d. %s - %s\n", i+1, agenda[i].name, agenda[i].phone);
}

// Ajouter contact
void ajouterContact() {
    if (n >= MAX_CONTACTS) { printf("Agenda plein!\n"); return; }

    char nom[MAX_NAME], tel[MAX_PHONE];

    // consommer \n résiduel
    getchar();

    do {
        printf("Nom: ");
        lireString(nom, MAX_NAME);
        if (!validerNom(nom)) printf("Nom invalide! Lettres et espaces seulement.\n");
    } while (!validerNom(nom));

    do {
        printf("Telephone (06xxxxxxxx ou 07xxxxxxxx): ");
        lireString(tel, MAX_PHONE);
        if (!validerTelephoneMaroc(tel)) printf("Telephone invalide! Numero marocain valide requis.\n");
    } while (!validerTelephoneMaroc(tel));

    strcpy(agenda[n].name, nom);
    strcpy(agenda[n].phone, tel);
    n++;
    sauvegarderContacts();
    printf("Contact ajoute avec succes!\n");
}

// Chercher contacts
int chercherContacts(int indices[]) {
    char nomCherche[MAX_NAME];
    getchar(); // consommer \n
    printf("Entrez le nom ou partie du nom à chercher: ");
    lireString(nomCherche, MAX_NAME);

    int count = 0;
    for (int i = 0; i < n; i++) {
        if (strstr(agenda[i].name, nomCherche)) {
            indices[count++] = i;
        }
    }
    return count;
}

// Afficher résultats recherche
void afficherRecherche(int indices[], int count) {
    printf("\n--- Resultats de recherche ---\n");
    for (int i = 0; i < count; i++)
        printf("%d. %s - %s\n", i+1, agenda[indices[i]].name, agenda[indices[i]].phone);
}

// Modifier contact
void modifierContact() {
    if (n == 0) { printf("Aucun contact a modifier.\n"); return; }
    afficherContacts();

    int choix;
    printf("Entrez le numero du contact a modifier: ");
    scanf("%d", &choix);
    getchar(); // consommer \n

    if (choix < 1 || choix > n) { printf("Numero invalide!\n"); return; }

    int idx = choix-1;
    char nom[MAX_NAME], tel[MAX_PHONE];

    do {
        printf("Nouveau nom: ");
        lireString(nom, MAX_NAME);
        if (!validerNom(nom)) printf("Nom invalide!\n");
    } while (!validerNom(nom));

    do {
        printf("Nouveau telephone (06xxxxxxxx ou 07xxxxxxxx): ");
        lireString(tel, MAX_PHONE);
        if (!validerTelephoneMaroc(tel)) printf("Telephone invalide!\n");
    } while (!validerTelephoneMaroc(tel));

    strcpy(agenda[idx].name, nom);
    strcpy(agenda[idx].phone, tel);
    sauvegarderContacts();
    printf("Contact modifie avec succes!\n");
}

// Supprimer contact
void supprimerContact() {
    if (n == 0) { printf("Aucun contact a supprimer.\n"); return; }
    afficherContacts();

    int choix;
    printf("Entrez le numero du contact a supprimer: ");
    scanf("%d", &choix);
    getchar(); // consommer \n

    if (choix < 1 || choix > n) { printf("Numero invalide!\n"); return; }

    int idx = choix-1;
    char confirm;
    printf("Confirmez suppression de %s? (o/n): ", agenda[idx].name);
    scanf(" %c", &confirm);
    getchar(); // consommer \n

    if (confirm == 'o' || confirm == 'O') {
        for (int i = idx; i < n-1; i++) agenda[i] = agenda[i+1];
        n--;
        sauvegarderContacts();
        printf("Contact supprime avec succes!\n");
    } else {
        printf("Suppression annulee.\n");
    }
}

// Menu principal
int main() {
    chargerContacts();
    int choix;
    do {
        printf("\n--- Mini Agenda Stable ---\n");
        printf("1. Ajouter un contact\n2. Afficher tous les contacts\n3. Chercher un contact\n");
        printf("4. Modifier un contact\n5. Supprimer un contact\n0. Quitter\nChoix: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: ajouterContact(); break;
            case 2: afficherContacts(); break;
            case 3: {
                int indices[MAX_CONTACTS];
                int count = chercherContacts(indices);
                if(count>0) afficherRecherche(indices, count);
                else printf("Aucun contact trouve.\n");
                break;
            }
            case 4: modifierContact(); break;
            case 5: supprimerContact(); break;
            case 0: printf("Au revoir!\n"); break;
            default: printf("Choix invalide.\n");
        }
    } while (choix != 0);

    return 0;
}

