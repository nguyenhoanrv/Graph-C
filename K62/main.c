#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfdr/fields.h"
#include "libfdr/jrb.h"
#include "GraphAPI/graph_jrb.h"
int readFile(Graph g, char *filename)
{
  IS is = new_inputstruct(filename);
  if (is == NULL)
  {
    printf("File fail!\n");
    exit(1);
  }
  get_line(is);
  int sothanh = atoi(is->fields[0]);
  int soduongdi = atoi(is->fields[1]);
  while (get_line(is) > 0)
  {
    addEdge(g, atoi(is->fields[0]), atoi(is->fields[1]), atoi(is->fields[2]));
  }
  return sothanh;
}

void readFile1(Graph g1, char *filename)
{
  IS is = new_inputstruct(filename);
  if (is == NULL)
  {
    printf("File fail!\n");
    exit(1);
  }
  get_line(is);
  int sothanh = atoi(is->fields[0]);
  int soduongdi = atoi(is->fields[1]);
  while (get_line(is) > 0)
  {
    if (atoi(is->fields[2]) >= 50)
      addEdge(g1, atoi(is->fields[0]), atoi(is->fields[1]), atoi(is->fields[2]));
  }
}

void printMatrix(Graph g, int sothanh)
{
  int sz = sothanh * sothanh;
  JRB node1, node2;
  int *a = (int *)malloc(sz * sizeof(int));

  jrb_traverse(node1, g.edges)
  {
    jrb_traverse(node2, g.edges)
    {
      if (hasEdge(g, node1->key.i, node2->key.i))
      {
        a[(node1->key.i - 1) * sothanh + node2->key.i - 1] = 1;
      }
      else
      {
        a[(node1->key.i - 1) * sothanh + node2->key.i - 1] = 0;
      }
    }
  }
  for (int i = 0; i < sz; i++)
  {
    printf("%d ", a[i]);
    if ((i + 1) % sothanh == 0)
    {
      printf("\n");
    }
  }
}

void chucnang4(Graph g)
{
  int s, t;
  printf("Nhap thanh tri xuat phat: ");
  scanf("%d", &s);
  printf("Nhap thanh tri dich: ");
  scanf("%d", &t);
  int path[100];
  int numVertices;
  int time = (int)shortestPath(g, s, t, path, &numVertices);
  if (time == INFINITIVE_VALUE)
  {
    printf("ROUTE NOT FOUND\n");
  }
  else
  {
    printf("Thoi gian ngan nhat: %d\n", time);
    printf("Thu tu cac thanh tri phai di qua: ");
    for (int i = 0; i < numVertices; i++)
    {
      printf("%d ", path[i]);
    }
    printf("\n");
  }
}
int main()
{
  Graph g = createGraph();
  Graph g1 = createGraph();
  int sothanh = readFile(g, "dothi.txt");
  readFile1(g1, "dothi.txt");
  int selection;
  while (selection != 6)
  {
    printf("--------------------Menu-----------------\n");
    printf("1. In ma tran ke cua do thi\n2. In danh sach ke moi thanh tri\n3. Dua ra danh sach cac thanh tri theo yeu cau\n4. Tim duong di ngan nhat giua 2 thanh tri\n5. Tim duong di bo ngan nhat giua 2 thanh tri \n");
    printf("--------------------End-----------------\n");
    printf("---> Chon:\n");
    scanf("%d", &selection);
    switch (selection)
    {
    case 1:
      printMatrix(g, sothanh);
      break;
    case 2:
    {
      JRB node;

      int output[100];
      jrb_traverse(node, g.edges)
      {
        int cnt = outdegree(g, node->key.i, output);
        printf("-  Castle %d: ", node->key.i);
        for (int i = 0; i < cnt; i++)
        {
          printf("%d ", output[i]);
        }
        printf("\n");
      }
    }
    break;
    case 3:
    {
      int selection1;
      while (selection1 != 3)
      {
        JRB node;
        int output[100];
        int cnt;
        printf("1. Danh sach cac thanh tri chi co the den truc tiep bang cach di bo\n2. Danh sach cac thanh tri co nhieu duong di noi truc tiep\n3. Thoat\n");
        printf("----> Chon: ");
        scanf("%d", &selection1);

        if (selection1 == 1)
        {
          printf("Danh sach thanh la: ");
          jrb_traverse(node, g.edges)
          {
            cnt = outdegree(g, node->key.i, output);
            for (int i = 0; i < cnt; i++)
            {
              if (getEdgeValue(g, node->key.i, output[i]) < 50)
              {
                break;
              }
              if (i == cnt - 1)
                printf("%d ", node->key.i);
            }
          }
          printf("\n");
        }
        else if (selection1 == 2)
        {
          printf("Danh sach thanh la: ");
          int max = 0;
          int rs[sothanh];
          int i = 0;
          jrb_traverse(node, g.edges)
          {
            cnt = outdegree(g, node->key.i, output);
            if (cnt > max)
            {
              max = cnt;
            }
          }
          jrb_traverse(node, g.edges)
          {
            cnt = outdegree(g, node->key.i, output);
            if (cnt == max)
            {
              rs[i] = node->key.i;
              i++;
            }
          }

          while (i--)
          {
            printf("%d ", rs[i]);
          }
          printf("\n");
        }
        else
        {
          break;
        }
      }
    }
    case 4:
      chucnang4(g);
      break;
    case 5:
      chucnang4(g1);
      break;
    default:
      break;
    }
  }
}
