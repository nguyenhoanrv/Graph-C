#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GraphAPI/graph_jrb.h"
#include "libfdr/fields.h"
#include "libfdr/jrb.h"

void readFile(Graph g, char *filename, JRB name_ID);
char *findName(JRB name_ID, int i);

int main()
{
    Graph g = createGraph();
    char filename[] = "Flight.txt";

    JRB name_ID = make_jrb();
    readFile(g, filename, name_ID);
    int selection;
    do
    {
        printf("\n1. In thong tin chuyen bay!\n"
               "2. So diem bay & chuyen bay\n"
               "3. Chuyen bay thang\n"
               "4. So duong bay giam dan\n"
               "5. Duong di giua 2 dinh\n"
               "6. Thanh phan lien thong\n"
               "7. Gia ve toi uu !\n"
               "0. Thoat!\n"
               "---> Chon: ");
        scanf("%d", &selection);

        switch (selection)
        {
        case 1:
        {
            JRB node1, node2;
            jrb_traverse(node1, g.vertices)
            {
                int id1 = jval_i(node1->key);
                jrb_traverse(node2, g.vertices)
                {
                    int id2 = jval_i(node2->key);
                    if (hasEdge(g, node1->key.i, node2->key.i))
                        printf("%s %s %.1f\n", findName(name_ID, id1), findName(name_ID, id2), getEdgeValue(g, id1, id2));
                }
            }
        }
        break;
        case 2:
            printf("So diem bay: %d\n", getNumOfV(g));
            printf("So chuyen bay: %d\n", getNumOfEdge(g));
            break;
        case 3:
        {
            printf("Nhap ten dinh: ");
            char dinh[30];
            rewind(stdin);
            gets(dinh);

            JRB node;
            jrb_traverse(node, g.vertices)
            {
                if (strcmp(dinh, jval_s(node->val)) == 0)
                {
                    int ID = jval_i(node->key);
                    int output[100];
                    int indee = indegree(g, ID, output);
                    printf("Cac tinh co chuyen bay thang toi dinh nay la: ");
                    for (int i = 0; i < indee; i++)
                    {
                        printf("\n%s", getVertex(g, output[i]));
                    }
                }
            }
        }
        break;

        case 4:
        {
            printf("Danh sach ten dinh co so duong di nhieu nhat (giam dan)!\n");
            JRB node;
            jrb_traverse(node, g.vertices)
            {
                int ID = jval_i(node->key);
                int output[100], output2[100];
                int so_luong_duong_di = outdegree(g, ID, output) + indegree(g, ID, output2);
            }
        }
        break;
        case 5:
        {
            int path[1000], ID1, ID2;
            char dinh1[30], dinh2[30];
            printf("Nhap dinh 1: ");
            rewind(stdin);
            gets(dinh1);
            JRB node;
            jrb_traverse(node, g.vertices)
            {
                if (strcmp(dinh1, jval_s(node->val)) == 0)
                    ID1 = node->key.i;
            }
            printf("Nhap dinh 2: ");
            rewind(stdin);
            gets(dinh2);
            jrb_traverse(node, g.vertices)
            {
                if (strcmp(dinh2, jval_s(node->val)) == 0)
                    ID2 = node->key.i;
            }
            int value = shortest_noWeight_path(g, ID1, ID2, path);
            if (value == 0)
                printf("Khong co duong di\n");
            else
            {
                for (int i = 0; i < value; i++)
                {
                    printf("%s", getVertex(g, path[i]));
                    if (i < value - 1)
                        printf(" - ");
                    else
                        printf("\n");
                }
            }
        }
        break;

        case 7:
        {
            int path[1000], ID1, ID2;
            char dinh1[30], dinh2[30];
            printf("Nhap dinh 1: ");
            rewind(stdin);
            gets(dinh1);
            JRB node;
            jrb_traverse(node, g.vertices)
            {
                if (strcmp(dinh1, jval_s(node->val)) == 0)
                    ID1 = node->key.i;
            }
            printf("Nhap dinh 2: ");
            rewind(stdin);
            gets(dinh2);
            jrb_traverse(node, g.vertices)
            {
                if (strcmp(dinh2, jval_s(node->val)) == 0)
                    ID2 = node->key.i;
            }
            int value;
            double tien = shortestPath(g, ID1, ID2, path, &value);
            if (value == 0)
                printf("Khong co duong di\n");
            else
            {
                printf("So tien la: %.1f\n", tien);
                for (int i = 0; i < value; i++)
                {
                    printf("%s", getVertex(g, path[i]));
                    if (i < value - 1)
                        printf(" - ");
                    else
                        printf("\n");
                }
            }
        }
        break;
        case 0:
            break;
        default:
            printf("---> Nhap lai!\n");
            break;
        }
    } while (selection != 0);

    dropGraph(g);
    return 0;
}

char *findName(JRB name_ID, int i)
{
    JRB node;
    jrb_traverse(node, name_ID)
    {
        if (node->val.i == i)
            return jval_s(node->key);
    }
}

char *fixName(char *s)
{
    int n = strlen(s);
    char *s1 = (char *)malloc(sizeof(char) * n);
    for (int i = 0; i < n - 3; i++)
    {
        s1[i] = s[i + 1];
    }
    s1[n - 3] = '\0';
    return s1;
}

void readFile(Graph g, char *filename, JRB name_ID)
{
    IS is = new_inputstruct(filename);
    if (is == NULL)
    {
        printf("Error: File Fail!\n");
        exit(1);
    }

    int i = 0;
    const char s[2] = "=";
    char *nameID = (char *)malloc(sizeof(char) * 10);
    char *name = (char *)malloc(sizeof(char) * 30);

    while (get_line(is) > 0)
    {
        char *check = strchr(is->text1, '=');
        if (check != NULL)
        {
            nameID = strtok(is->text1, s);
            name = strtok(NULL, s);

            JRB find = jrb_find_str(name_ID, nameID);
            if (find == NULL)
            {
                i++;
                jrb_insert_str(name_ID, strdup(nameID), new_jval_i(i));
                addVertex(g, i, fixName(name));
            }
        }
        else
        {
            char ID1[5], ID2[5];
            strcpy(ID1, is->fields[0]);
            strcpy(ID2, is->fields[1]);
            double value = atof(is->fields[2]);
            JRB find1 = jrb_find_str(name_ID, ID1);
            JRB find2 = jrb_find_str(name_ID, ID2);
            addEdge(g, find1->val.i, find2->val.i, value);
        }
    }
}
