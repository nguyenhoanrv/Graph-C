#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "GraphAPI/graph_jrb.h"
#include "libfdr/fields.h"
#include "libfdr/jrb.h"
void printListProduct(JRB products);
void inputProducts(JRB product, char *filename);
void inputHistoryOder(Graph g, JRB products, char *filename);
void chucnang4(Graph g, JRB products, int id);
int main()
{
  int selection;
  Graph g = createGraph();
  JRB products = make_jrb();
  char *filename = "products.txt";
  char *filenameOder = "oderhistory.txt";
  inputProducts(products, filename);
  while (selection != 6)
  {
    printf("---------------------MENU----------------------\n");
    printf("1. In danh sach san pham\n2. In danh sach giao dich\n3. Hien thi muc do lien quan giua hai san pham \n4. In danh sach cac san pham lien quan\n5. Hien thi moi lien he giua hai san pham\n6. Thoat\n");

    printf("----------------------END----------------------\n");
    printf("Nhap lua chon: \n");
    scanf("%d", &selection);
    switch (selection)
    {
    case 1:
      printListProduct(products);
      break;
    case 2:
      inputHistoryOder(g, products, filenameOder);
      break;
    case 3:
    {
      int ID1, ID2;
      printf("Nhap id1: ");
      scanf("%d", &ID1);
      printf("Nhap id2: ");
      scanf("%d", &ID2);
      double value = getEdgeValue(g, ID1, ID2);
      if (value == INFINITIVE_VALUE)
      {
        printf("-1\n");
      }
      else
      {
        printf("%g\n", value);
      }
    }

    break;
    case 4:
    {
      int id;
      printf("Nhap id: \n");
      scanf("%d", &id);
      chucnang4(g, products, id);
    }

    break;
    case 5:
    {
      int id1, id2;
      printf("Nhap id1: ");
      scanf("%d", &id1);
      printf("Nhap id2: ");
      scanf("%d", &id2);
      int path[1000];
      int cnt = DFS(g, id1, id2, path);
      if (cnt == 0)
      {
        printf("Hai san pham nay khong co lien he voi nhau.\n");
        break;
      }
      for (int i = 0; i < cnt; i++)
      {
        JRB node = jrb_find_int(products, path[i]);
        printf("%s", node->val.s);
        printf(" - ");
      }
      printf("\n");
    }
    break;
    case 6:
      break;
    default:
      printf("Sai lua chon!\n");
      break;
    }
  }

  return 0;
}

void inputProducts(JRB products, char *filename)
{
  IS is = new_inputstruct(filename);
  if (is == NULL)
  {
    printf("File fail!\n");
    exit(1);
  }
  while ((get_line(is)) > 0)
  {
    jrb_insert_int(products, atoi(is->fields[0]), new_jval_s(strdup(is->fields[1])));
  }
}

void printListProduct(JRB products)
{
  JRB node;
  jrb_traverse(node, products)
  {
    printf("Ma san pham: %d\n", jval_i(node->key));
    printf("Ten san pham: %s\n", jval_s(node->val));
  }
}

void inputHistoryOder(Graph g, JRB products, char *filename)
{
  JRB node;
  IS is = new_inputstruct(filename);
  if (is == NULL)
  {
    printf("File fail!\n");
  }
  while (get_line(is) > 0)
  {
    for (int i = 0; i < is->NF; i++)
    {
      node = jrb_find_int(products, atoi(is->fields[i]));
      printf("%s ", jval_s(node->val));
    }
    printf("\n");

    for (int i = 0; i < is->NF - 1; i++)
    {
      int id1 = atoi(is->fields[i]), id2;
      for (int j = i + 1; j < is->NF; j++)
      {
        id2 = atoi(is->fields[j]);
        if (getEdgeValue(g, id1, id2) == INFINITIVE_VALUE)
          addEdge(g, id1, id2, 1);
        else
        {
          node = jrb_find_int(g.edges, id1);
          JRB tree = (JRB)jval_v(node->val);
          node = jrb_find_int(tree, id2);
          node->val.d++;

          node = jrb_find_int(g.edges, id2);
          tree = (JRB)jval_v(node->val);
          node = jrb_find_int(tree, id1);
          node->val.d++;
        }
      }
    }
  }
}

void chucnang4(Graph g, JRB products, int id)
{
  JRB node, tree;
  tree = make_jrb();
  int dinhke[100];
  int sodinhke = outdegree(g, id, dinhke);
  printf("Cac san pham lien quan: ");

  for (int i = 0; i < sodinhke; i++)
  {
    jrb_insert_dbl(tree, getEdgeValue(g, id, dinhke[i]), new_jval_i(dinhke[i]));
  }
  jrb_rtraverse(node, tree)
  {
    JRB find = jrb_find_int(products, node->val.i);
    printf("%s ", find->val.s);
  }

  printf("\n");
}