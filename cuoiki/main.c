#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfdr/fields.h"
#include "GraphAPI/graph_jrb.h"

int readfile1(Graph g, char *filename)
{
  IS is = new_inputstruct(filename);
  if (is == NULL)
  {
    printf("File fail!\n");
    exit(1);
  }
  get_line(is);
  int cntpage = atoi(is->fields[0]);
  while ((get_line(is) > 0))
  {
    int id = atoi(is->fields[1]);
    char *name = strdup(is->fields[0]);
    addVertex(g, id, name);
  }
  return cntpage;
}

void readfile2(Graph g, char *filename)
{
  IS is = new_inputstruct(filename);
  if (is == NULL)
  {
    printf("File fail!\n");
    exit(1);
  }
  get_line(is);
  int cnt = atoi(is->fields[0]);
  while ((get_line(is) > 0))
  {
    for (int i = 1; i < is->NF; i++)
    {
      addEdge(g, atoi(is->fields[0]), atoi(is->fields[i]), 1);
    }
  }
}

void printArray(Graph g, int arr[], int sz)
{
  for (int i = 0; i < sz; i++)
  {
    printf("%s %d \n", getVertex(g, arr[i]), arr[i]);
  }
}
int main()
{
  Graph g = createGraph();
  int selection;
  int cntpage;
  while (selection != 6)
  {
    printf("--------------------Menu-----------------\n");
    printf("1. Chuc nang B \n2. Chuc nang C\n3. Chuc nang D\n4. Chuc nang E\n5. Chuc nang F\n6. Thoat\n");
    printf("--------------------End-----------------\n");
    printf("---> Chon:\n");
    scanf("%d", &selection);
    switch (selection)
    {
    case 1:
    {
      JRB node;
      int i, temp1[10], temp2[10];
      int max = 0;
      int output[100];
      int id = 1001;
      cntpage = readfile1(g, "webpages.txt");
      readfile2(g, "pageConnections.txt");
      int min = indegree(g, id, output);

      printf("So luong webpage: %d\n", cntpage);
      jrb_traverse(node, g.vertices)
      {
        i = indegree(g, node->key.i, output);
        if (max < i)
        {
          max = i;
        }
        if (min > i)
        {
          min = i;
        }
        printf("%s %d\n", node->val.s, node->key.i);
      }
      int j = 0, k = 0;
      jrb_traverse(node, g.vertices)
      {
        i = indegree(g, node->key.i, output);
        if (i == max)
        {
          temp1[j] = node->key.i;
          j++;
        }
        if (i == min)
        {
          temp2[k] = node->key.i;
          k++;
        }
      }
      printf("1. Webpage co nhieu lien ket nhat: ");
      printArray(g, temp1, j);
      printf("2. Webpage co it lien ket nhat: ");
      printArray(g, temp2, k);
    }
    break;
    case 2:
    {
      // JRB node;
      // int i, o;
      // int output[100];
      // jrb_traverse(node, g.vertices)
      // {
      //   i = indegree(g, node->key.i, output);
      //   o = outegree(g, node->key.i, output);
      // }
      printf("em chua lam duoc\n");
    }
    break;
    case 3:
      printf("em chua lam duoc\n");
      break;
    case 4:
    {
      JRB node;
      int output[100];
      int count1 = 0, count2 = 0;
      jrb_traverse(node, g.vertices)
      {
        int a = indegree(g, node->key.i, output);
        int b = outdegree(g, node->key.i, output);
        if (a > 0 && b == 0)
          count1++;
        if (a == 0 && b > 0)
          count2++;
      }
      printf("1. So webpage chi co lien ket den la: %d\n", count1);
      printf("2. So spambot: %d\n", count2);
    }
    break;
    case 5:
    {
      int id1, id2;
      int path[100];
      printf("Nhap id1: ");
      scanf("%d", &id1);
      printf("Nhap id2: ");
      scanf("%d", &id2);
      int cnt = shortest_noWeight_path(g, id1, id2, path);
      if (cnt == 1)
        printf("-1\n");
      else
        printf("Khoang cac nho nhat giua 2 webpage la: %d\n", cnt - 1);
    }
    break;
    case 6:
      break;
    default:
      printf("Sai lua chon:");
      break;
    }
  }
  dropGraph(g);
  return 0;
}
