/* −−−−−−−−−−−−−−−− INCLUSIONS DES BIBLIOTHEQUES NECESSAIRES −−−−−−−−−−−−−−−−−−− */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* −−−−−−−−−−−−−−−− DECLARATIONS DES CONSTANTES PARAMETRIQUES −−−−−−−−−−−−−−−−−− */
#define SCENE_NB_ROW_MIN            (8)
#define SCENE_NB_ROW_MAX            (20)
#define SCENE_NB_COL_MIN            (10)
#define SCENE_NB_COL_MAX            (24)
#define SCENE_MINE_PERCENT_MIN      (13)
#define SCENE_MINE_PERCENT_MAX      (50)

#define SCENE_CELL_VOID_VALUE       (0)
#define SCENE_CELL_VOID_CHAR        (' ')
#define SCENE_MINE_VALUE            (9)
#define SCENE_MINE_CHAR             ('*')
#define SCENE_CELL_MASK_OFFSET      (10)

#define BUFFER_MAX_SIZE             (256)

//#define DEBUG                       (1)

/* −−−−−−−−−−−−−−−− PROTOTYPES DES FONCTIONS −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−− */
void AskFieldVariables(int *pa_pINbRow, int *pa_pINbCol, int *pa_pIPercent);
void SceneInit(int pa_sceneArray[][SCENE_NB_COL_MAX], int pa_nbRow, int pa_nbCol, int pa_percent);
void SceneDisplay(int pa_sceneArray[][SCENE_NB_COL_MAX], int pa_nbRow, int pa_nbCol);
void BorderDisplay(int pa_nbCol);
void ColumnDisplay(int pa_nbCol);
void ColumnDisplayX10(int pa_nbCol);

/* −−−−−−−−−−−−−−−− CORPS DU PROGRAMME : FONCTION MAIN −−−−−−−−−−−−−−−−−−−−−−−−− */
int main(int argc, char *argv[]) {
     int iScene[SCENE_NB_ROW_MAX][SCENE_NB_COL_MAX];
     int iSceneNbRow, iSceneNbCol, iSceneMinePercent;
    
    //Scene initiale / Affichage formaté de la scème de jeu
    AskFieldVariables(&iSceneNbRow, &iSceneNbCol, &iSceneMinePercent);
    SceneInit(iScene, iSceneNbRow, iSceneNbCol, iSceneMinePercent);
    SceneDisplay(iScene, iSceneNbRow, iSceneNbCol);

    return EXIT_SUCCESS;
}

/* −−−−−−−−−−−−−−−− IMPLEMENTATIONS DES FONCTIONS −−−−−−−−−−−−−−−−−−−−−−−−−−−−−− */
void AskFieldVariables(int *pa_pINbRow, int *pa_pINbCol, int *pa_pIPercent) {

    char cBuffer[BUFFER_MAX_SIZE];
    int iNbFgetsArgs;

    iNbFgetsArgs = 0;
    do {
        printf("Please enter field dimensions (row col minePercent).\r\n\r\n");
        printf("Notes : \r\n%3d <= nbRow(s)\t\t<= %3d\r\n%3d <= nbCol\t\t<= %3d\r\n%3d <= minePercentage\t<= %3d\r\n  -> ", SCENE_NB_ROW_MIN, SCENE_NB_ROW_MAX, SCENE_NB_COL_MIN, SCENE_NB_COL_MAX, SCENE_MINE_PERCENT_MIN, SCENE_MINE_PERCENT_MAX);
        
        fgets(cBuffer, sizeof(cBuffer), stdin);
        iNbFgetsArgs = sscanf(cBuffer, "%d %d %d", pa_pINbRow, pa_pINbCol, pa_pIPercent);

    } while(*pa_pINbRow < SCENE_NB_ROW_MIN || *pa_pINbRow > SCENE_NB_ROW_MAX ||
            *pa_pINbCol < SCENE_NB_COL_MIN || *pa_pINbCol > SCENE_NB_COL_MAX ||
            *pa_pIPercent < SCENE_MINE_PERCENT_MIN || *pa_pIPercent > SCENE_MINE_PERCENT_MAX ||
            iNbFgetsArgs < 3);
}
void SceneInit(int pa_sceneArray[][SCENE_NB_COL_MAX], int pa_nbRow, int pa_nbCol, int pa_percent) {
    int iNbMines;
    int iMineRow, iMineCol;

    //Initialisation des cases du tableau à la valeur 0
    for(int i = 0; i < pa_nbRow; i++) {
        for(int j = 0; j < pa_nbCol; j++) {
            pa_sceneArray[i][j] = SCENE_CELL_VOID_VALUE;
        }
    }

    //Placement aléatoire des mines
    iNbMines = (pa_nbRow * pa_nbCol * pa_percent) / 100;

    srand((unsigned int)time(NULL));
    for(int i = iNbMines; i > 0; i--) {
        iMineRow = rand() % pa_nbRow;
        iMineCol = rand() % pa_nbCol;
        if(pa_sceneArray[iMineRow][iMineCol] != SCENE_MINE_VALUE) {
            pa_sceneArray[iMineRow][iMineCol] = SCENE_MINE_VALUE;
        }
        
        //Incrémentation de l’indice des cellules autour de la mine
        for(int k = -1; k <= 1; k++) {
            for(int m = -1; m <= 1; m++) {
                if( iMineRow + k >= 0 && iMineRow + k < pa_nbRow &&
                    iMineCol + m >= 0 && iMineCol + m < pa_nbCol &&
                    pa_sceneArray[iMineRow + k][iMineCol + m] != SCENE_MINE_VALUE) {
                    
                    pa_sceneArray[iMineRow + k][iMineCol + m]++;
                }
            }
        }
    }
    
    //Masquage de toutes les cases
    for(int i = 0; i < pa_nbRow; i++) {
        for(int j = 0; j < pa_nbCol; j++) {
            pa_sceneArray[i][j] += SCENE_CELL_MASK_OFFSET;
        }
    }
}
void SceneDisplay(int pa_sceneArray[][SCENE_NB_COL_MAX], int pa_nbRow, int pa_nbCol) {
    
    ColumnDisplayX10(pa_nbCol);
    ColumnDisplay(pa_nbCol);
    BorderDisplay(pa_nbCol);
    
    for(int i = 0; i < pa_nbRow; i++) {
        printf("%2d |", i);
        for(int j = 0; j < pa_nbCol; j++) {

#ifdef DEBUG                
            printf(" %d ", pa_sceneArray[i][j]);

#else
            if(pa_sceneArray[i][j] >= 30) {
                printf(" ? ");
            }
            else if(pa_sceneArray[i][j] >= 20) {
                printf(" M ");
            }
            else if(pa_sceneArray[i][j] >= 10) {
                printf(" . ");
            }
            else {
                switch(pa_sceneArray[i][j]) {
                    case SCENE_CELL_VOID_VALUE :
                        printf(" %c ", SCENE_CELL_VOID_CHAR);
                        break;
                    case SCENE_MINE_VALUE :
                        printf(" %c ", SCENE_MINE_CHAR);
                        break;
                    default :
                    printf(" %d ", pa_sceneArray[i][j]); 
                }
            }
#endif
        }
        printf("| %d", i);
        printf("\r\n");
    }
    
    BorderDisplay(pa_nbCol);
    ColumnDisplay(pa_nbCol);
    ColumnDisplayX10(pa_nbCol);
    
}
void BorderDisplay(int pa_nbCol) {
    
    printf("    ");
        for(int i = 0; i < pa_nbCol; i++) {
        printf("---");
    }
    printf("\r\n");
}
void ColumnDisplay(int pa_nbCol) {
    
    printf("   ");
    for(int i = 0; i < pa_nbCol; i++) {
        printf("%3d", i % 10);
    }
    printf("\r\n");
}
void ColumnDisplayX10(int pa_nbCol) {
    int i = 0;
    printf("   ");
    while(i < pa_nbCol) {
        (!(i % 10)) ? printf("%3d", i / 10) : printf("   ");
        i++;
    }
    printf("\r\n");
}
