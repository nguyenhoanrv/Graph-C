#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libfdr/fields.h"
#include "libfdr/jrb.h"
#include "GraphAPI/graph_jrb.h"

void readFile(Graph g, char *filename)
{
  IS is = new_inputstruct(filename);
  if (is == NULL)
  {
    printf("File fail!\n");
    exit(1);
  }
  get_line(is);
  int n, m;
  n = atoi(is->fields[0]);
  m = atoi(is->fields[1]);

  for (int i = 0; i < n; i++)
  {
    get_line(is);
    addVertex(g, atoi(is->fields[1]), strdup(is->fields[0]));
  }
  for (int i = 0; i < m; i++)
  {
    get_line(is);
    addEdge(g, atoi(is->fields[0]), atoi(is->fields[1]), 0);
  }
}
int checkArgv(int argc, char *argv[])
{
  if (argc == 2 && (strcmp(argv[1], "help") == 0))
    return 1;
  if (argc == 2 && (strcmp(argv[1], "about") == 0))
    return 2;
  if (argc == 4 && (strcmp(argv[1], "i2n") == 0))
    return 3;
  if (argc == 4 && (strcmp(argv[1], "n2i") == 0))
    return 4;
  if (argc == 5 && (strcmp(argv[1], "cc") == 0))
    return 5;
  if (argc == 5 && (strcmp(argv[1], "cr") == 0))
    return 6;
  if (argc == 5 && (strcmp(argv[1], "odg") == 0))
    return 7;
  if (argc == 5 && (strcmp(argv[1], "rel") == 0))
    return 8;
  return 0;
}

int getVertexId(Graph g, char *name)
{
  JRB node;
  jrb_traverse(node, g.vertices)
  {
    if (strcmp(node->val.s, name) == 0)
    {
      return node->key.i;
    }
  }
  return NULL;
}
int main(int argc, char *argv[])
{
  Graph g = createGraph();
  char *filename = "g.txt";
  readFile(g, filename);
  switch (checkArgv(argc, argv))
  {
  case 1:
    printf("Supported command: help, i2n, n2i, cc, cr, odg, rel.\n");
    break;
  case 2:
    printf("C advanced 20142 final exam.\n");
    break;
  case 3:
  {
    int id = atoi(argv[3]);
    printf("Name: %s\n", (char *)getVertex(g, id));
  }
  break;
  case 4:
  {
    char *name = argv[3];
    int id = getVertexId(g, name);
    if (id == NULL)
    {
      printf("Khong ton tai!\n");
    }
    else
      printf("Id: %d\n", id);
  }
  break;
  case 5:
  {
    JRB node;
    int id1 = atoi(argv[3]);
    int id2 = atoi(argv[4]);
    jrb_traverse(node, g.vertices)
    {
      if (hasEdge(g, id1, node->key.i) == 1 && hasEdge(g, id2, node->key.i) == 1)
      {
        printf("%s\n", node->val.s);
      }
    }
  }
  break;
  case 6:
  {
    JRB node;
    int id1 = atoi(argv[3]);
    int id2 = atoi(argv[4]);
    jrb_traverse(node, g.vertices)
    {
      if (hasEdge(g, node->key.i, id1) == 1 && hasEdge(g, node->key.i, id2) == 1)
      {
        printf("%s\n", node->val.s);
      }
    }
  }
  break;
  default:
    printf("Command fail!\n");
    break;
  }
}