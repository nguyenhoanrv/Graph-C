#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfdr/fields.h"
#include "GraphAPI/graph_jrb.h"
#include "libfdr/jrb.h"
int inputProducts(JRB products, char *filename)
{
  IS is = new_inputstruct(filename);
  if (is == NULL)
  {
    printf("File fail!\n");
    exit(1);
  }
  get_line(is);
  int sosanpham = atoi(is->fields[0]);
  while (get_line(is) > 0)
  {
    jrb_insert_int(products, atoi(is->fields[1]), new_jval_s(strdup(is->fields[0])));
  }
  return sosanpham;
}
void *inputKhohang(Graph khohang, JRB products, char *filename, int sosanpham)
{
  IS is = new_inputstruct(filename);
  if (is == NULL)
  {
    printf("File fail!");
    exit(1);
  }
  get_line(is);
  int sokhohang = atoi(is->fields[0]);
  JRB danhsach = make_jrb();

  for (int i = 0; i < sokhohang; i++)
  {
    get_line(is);
    JRB thanhphankhohang = make_jrb();
    int ID = atoi(is->fields[1]);
    addVertex(khohang, ID, strdup(is->fields[0]));
    for (int j = 0; j < sosanpham; j++)
    {
      get_line(is);
      jrb_insert_int(thanhphankhohang, atoi(is->fields[0]), new_jval_i(atoi(is->fields[1])));
    }
    jrb_insert_int(danhsach, ID, new_jval_v(thanhphankhohang));
  }
  get_line(is);
  while (get_line(is) > 0)
  {
    int v1 = atoi(is->fields[0]);
    int v2 = atoi(is->fields[1]);
    double weight = atof(is->fields[2]);
    addEdge(khohang, v1, v2, weight);
  }
  return danhsach;
}

void chucnang5(Graph khohang, JRB products, JRB danhsach, int idsp, int idkh)
{
  JRB node1, node3;
  node1 = jrb_find_int(danhsach, idkh);
  node3 = (JRB)(node1->val.v);
  printf("%s\n", jrb_find_int(khohang.vertices, idkh)->val.s);
  printf("%s %d\n", jrb_find_int(products, idsp)->val.s, jrb_find_int(node3, idsp)->val.i);
}

int checkArgc(int argc, char *argv[])
{
  if (argc == 2 && strcmp(argv[1], "-t") == 0)
    return 1;
  if (argc == 3 && strcmp(argv[1], "-s") == 0)
    return 2;
  if (argc == 5 && strcmp(argv[1], "-w") == 0)
    return 3;
  if (argc == 4 && strcmp(argv[1], "-a") == 0)
    return 4;
  if (argc == 6 && strcmp(argv[1], "-h") == 0)
    return 5;
  if (argc == 8 && strcmp(argv[1], "-g") == 0)
    return 6;
  return 0;
}
int main(int argc, char *argv[])
{
  JRB products = make_jrb();
  JRB node;
  Graph khohang = createGraph();
  int sosanpham = inputProducts(products, "sanpham.txt");
  JRB danhsach = inputKhohang(khohang, products, "khohang.txt", sosanpham);
  switch (checkArgc(argc, argv))
  {
  case 1:
    printf("C-Advanced, HK20192\n");
    break;
  case 2:
    jrb_traverse(node, products)
    {
      printf("%s %d", node->val.s, node->key.i);
      printf("\n");
    }
    break;
  case 3:
  {
    double value = getEdgeValue(khohang, atoi(argv[3]), atoi(argv[4]));
    if (value == INFINITIVE_VALUE)
      printf("-1\n");
    else
      printf("%g\n", value);
  }
  break;
  case 4:
  {
    jrb_traverse(node, danhsach)
    {
      JRB node1;
      JRB tree = (JRB)(node->val.v);
      int id = node->key.i;
      node1 = jrb_find_int(khohang.vertices, id);
      printf("%s\n", node1->val.s);
      jrb_traverse(node1, tree)
      {
        printf("%s %d\n", jrb_find_int(products, node1->key.i)->val.s, node1->val.i);
      }
      printf("-----\n");
    }
    break;
  }
  case 5:
  {
    int idsp = atoi(argv[4]);
    int idkh = atoi(argv[5]);
    chucnang5(khohang, products, danhsach, idsp, idkh);
    printf("---Cac kho ke la: \n");
    int ouput[10];
    int sodinhke = outdegree(khohang, idkh, ouput);
    for (int i = 0; i < sodinhke; i++)
    {
      idkh = ouput[i];
      chucnang5(khohang, products, danhsach, idsp, idkh);
    }
  }
  break;
  case 6:
  {
    int idsp = atoi(argv[4]);
    int slsp = atoi(argv[5]);
    int idkh1 = atoi(argv[6]);
    int idkh2 = atoi(argv[7]);
    int path[100];
    int numVertices[100];
    JRB node1, node2;
    node1 = jrb_find_int(danhsach, idkh1);
    node2 = (JRB)(node1->val.v);
    if (jrb_find_int(node2, idsp)->val.i >= slsp)
    {
      printf("Dat hang thanh cong, thoi gian giao hang la 30 phut.\n");
      break;
    }
    else
    {
      node1 = jrb_find_int(danhsach, idkh2);
      node2 = (JRB)(node1->val.v);
      if (jrb_find_int(node2, idsp)->val.i >= slsp)
      {

        int s = (int)shortestPath(khohang, idkh2, idkh1, path, numVertices);
        int time = (s * 2 + 30);
        int h = time / 60;
        int m = time % 60;
        printf("Dat hang thanh cong, thoi gian giao hang la %d gio %d phut.\n", h, m);
        break;
      }
    }
  }
    printf("Dat hang khong thanh cong.\n");
    break;
  default:
    printf("Sai cau lenh!\n");
    break;
  }
  dropGraph(khohang);
}