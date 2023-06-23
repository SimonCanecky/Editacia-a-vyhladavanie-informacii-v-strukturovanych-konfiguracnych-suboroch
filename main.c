#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]){
    int opt;
    int u = 0;
    char* optstring = ":k:s:u:";
    char text[1500];
    char strRiadok[51];
    char* pArgument;
    int enter = 0;
    int prepinac = 0;


    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
            case 'k':
                pArgument = optarg;
                prepinac = 1;
                break;
            case 's':
                pArgument = optarg;
                prepinac = 2;
                break;
            case 'u':
                pArgument = optarg;
                u = 1;
                prepinac = 3;
                break;
            case '?':
                printf("E1");
                return 0;
            case ':':
                printf("E2");
                return 0;
        }
    }
    if (u == 1 && optind >= argc){
        printf("E3");
        return 0;
    }

    while (fgets(strRiadok, sizeof(strRiadok), stdin) != NULL) {
        if (strcmp(strRiadok, "\n") == 0) {
            enter++;
            if (enter == 2)
                break;
        }
        else
            enter = 0;
        strcat(text, strRiadok);
    }
    
    if (prepinac == 0) //ziadny prepinac
        printf("%s", text);

    else if (prepinac == 1){ //-k
        char* kluc;
        char* nazovSekcia;
        if (strchr(pArgument, '.') != NULL){
            kluc = strchr(pArgument, '.') + 1;
            nazovSekcia = strtok(pArgument, ".");
            if (strstr(text, nazovSekcia) != NULL){
                char* sekcia = strstr(text, nazovSekcia) + sizeof(nazovSekcia)+1;
                sekcia = strtok(sekcia, "\n");
                while (sekcia != NULL){
                    if (strchr(sekcia, '[') != NULL)
                        break;
                    if (strstr(sekcia, kluc) != NULL) {
                        if (strchr(sekcia, '=') != NULL) {
                            sekcia = strchr(sekcia, '=') + 1;
                            int i = 0;
                            while (isspace(sekcia[i]))
                                i++;
                            printf("%s\n",sekcia + i);
                        }
                    }
                    sekcia = strtok(NULL,"\n");
                }
            }
        }
        else {
            char *riadok = strtok(text, "\n");
            char *hodnota;
            while (riadok != NULL) {
                if (strstr(riadok, pArgument) != NULL) {
                    if (strchr(riadok, '=') != NULL) {
                        hodnota = strchr(riadok, '=') + 1;
                        int i = 0;
                        while (isspace(hodnota[i]))
                            i++;
                        printf("%s\n",hodnota + i);
                    }
                }
                riadok = strtok(NULL, "\n");
            }
        }
    }

    else if (prepinac == 2) { //-s
        char *sekcia;
        char strNazovSekcia[50];
        sekcia = strstr(text, pArgument);
        if (strcmp(pArgument, "*") == 0) {
            char* riadok = strtok(text,"\n");
            while (riadok != NULL){
                if (strchr(riadok,'[') != NULL) {
                    char* endNazovSekcia = strchr(riadok, ']');
                    int dlzka = endNazovSekcia - riadok;
                    char nazovSekcia[dlzka + 1];
                    strncpy(nazovSekcia, riadok, dlzka);
                    nazovSekcia[dlzka] = '\0';
                    strcpy(strNazovSekcia, nazovSekcia+1);
                }

                if (strchr(riadok,'=') != NULL){
                    char* rovnasa = strchr(riadok, '=');
                    int dlzka = rovnasa - riadok;
                    char kluc[dlzka + 1];
                    strncpy(kluc, riadok, dlzka);
                    kluc[dlzka] = '\0';
                    int i = 0;
                    while (isspace(kluc[i])) {
                        i++;
                    }
                    printf("%s.%s\n", strNazovSekcia, kluc + i);
                }
                riadok = strtok(NULL,"\n");
            }
        }

       else if (sekcia != NULL){
            sekcia += strlen(pArgument) + 2;
            sekcia = strtok(sekcia, "\n");
            while (sekcia != NULL){
                if (strchr(sekcia, '[') != NULL)
                    break;
                if (strchr(sekcia, '=') != NULL) {
                    char* strZaRovnasa = strchr(sekcia, '=');
                    int dlzka = strZaRovnasa - sekcia;
                    char kluc[dlzka + 1];
                    strncpy(kluc, sekcia, dlzka);
                    kluc[dlzka] = '\0';
                    int i = 0;
                    while (isspace(kluc[i]))
                        i++;
                    printf("%s.%s\n", pArgument, kluc + i);
                }
                sekcia = strtok(NULL,"\n");
            }
        }
    }

    else if(prepinac == 3){ //-u
        char* kluc;
        char* nazovSekcia;
        int control = 0;
        int control2 = 0;
        int control3 = 0;
        int tabulator = 0;
        if (strchr(pArgument, '.') != NULL) {
            kluc = strchr(pArgument, '.') + 1;
            nazovSekcia = strtok(pArgument, ".");
            if(strstr(text,nazovSekcia) == NULL) {
                /*int length = strlen(text);
                text[length - 1] = '\0';*/
                printf("%s", text);
                printf("[%s]\n%s=%s", nazovSekcia, kluc, argv[optind]);
            }
            else {
                for (int i=0; text[i] != '\0'; i++) {
                    if (text[i] == '\n') {
                        memmove(text + i + 2, text + i + 1, strlen(text + i + 1) + 1);
                        text[i+1] = '\t';
                        i++;
                    }
                }
                char* riadok = strtok(text,"\n");
                char strsekcia[] = "";
                while (riadok != NULL){
                    if (strchr(riadok,'[') != NULL) {
                        if (control3 == 1 && control2 == 0){
                            control2 = 1;
                            printf("%s=%s\n",kluc,argv[optind]);
                        }
                        strcat(strsekcia, strchr(riadok, '['));
                        int length = strlen(strsekcia);
                        strsekcia[length-1] = '\0';
                    }
                    if (strstr(strsekcia+1,nazovSekcia) != NULL){  //+1
                        control3 = 1;
                        if (strstr(riadok,kluc) != NULL && control2 == 0){
                            control2 = 1;
                            printf("\n%s=%s",kluc,argv[optind]);
                            riadok = strtok(NULL,"\n");
                            continue;
                        }
                    }
                    if (strchr(riadok,'\t') != NULL && control == 1)
                        printf("\n");
                    if (tabulator == 0) {
                        printf("%s", riadok);//+1
                        tabulator = 1;
                    }
                    else
                        printf("%s",riadok+1);//+1
                    control = 1;
                    riadok = strtok(NULL,"\n");
                }
            }
        }
        else {
            if (strstr(text,pArgument) == NULL){
                /*int length = strlen(text);
                text[length-1] = '\0';*/
                printf("%s",text);
                printf("[My section]\n%s=%s",pArgument,argv[optind]);
            }
            else {
                for (int i=0; text[i] != '\0'; i++) {
                    if (text[i] == '\n') {
                        memmove(text + i + 2, text + i + 1, strlen(text + i + 1) + 1);
                        text[i+1] = '\t';
                        i++;
                    }
                }
                char* riadok = strtok(text,"\n");
                while (riadok != NULL){
                    if (strstr(riadok,pArgument) != NULL){
                        printf("\n%s=%s",pArgument,argv[optind]);
                        riadok = strtok(NULL,"\n");
                        continue;
                    }
                    if (strchr(riadok,'\t') != NULL && control == 1)
                        printf("\n");
                    if (tabulator == 0) {
                        printf("%s", riadok);//+1
                        tabulator = 1;
                    }
                    else
                        printf("%s",riadok+1);//+1
                    control = 1;
                    riadok = strtok(NULL,"\n");
                }
            }
        }
    }
    return 0;
}