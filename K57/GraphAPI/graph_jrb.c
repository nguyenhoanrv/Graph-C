#include "graph_jrb.h"
#include <stdlib.h>
#include <string.h>

Graph createGraph()
{
    Graph graph;
    graph.edges = make_jrb();
    graph.vertices = make_jrb();
    return graph;
}

void dropGraph(Graph graph)
{
    JRB node, tree;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB)jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}

void addVertex(Graph graph, int id, char *name)
{
    JRB node = jrb_find_int(graph.vertices, id);
    if (node == NULL)
        jrb_insert_int(graph.vertices, id, new_jval_s(strdup(name)));
}

void *getVertex(Graph graph, int id)
{
    JRB node = jrb_find_int(graph.vertices, id);
    if (node == NULL)
        return NULL;
    else
        return jval_s(node->val);
}

int getNumOfV(Graph graph)
{
    int count = 0;
    JRB node;
    jrb_traverse(node, graph.vertices)
        count++;
    return count;
}

void addEdge(Graph graph, int v1, int v2, double weight)
{
    JRB node, tree;
    if (getEdgeValue(graph, v1, v2) == INFINITIVE_VALUE)
    {
        node = jrb_find_int(graph.edges, v1);
        if (node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        }
        else
            tree = (JRB)jval_v(node->val);

        jrb_insert_int(tree, v2, new_jval_d(weight));
    }

    //Neu 2 chieu !
    // if (getEdgeValue(graph, v2, v1) == INFINITIVE_VALUE)
    //     addEdge(graph, v2, v1, weight);
}

double getEdgeValue(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
        return INFINITIVE_VALUE;
    tree = (JRB)jval_v(node->val);
    node = jrb_find_int(tree, v2);
    if (node == NULL)
        return INFINITIVE_VALUE;
    else
        return jval_d(node->val);
}

int hasEdge(Graph graph, int v1, int v2)
{
    JRB node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
        return 0;
    JRB tree = (JRB)jval_v(node->val);
    if (jrb_find_int(tree, v2) == NULL)
        return 0;
    else
        return 1;
}

int getNumOfEdge(Graph graph)
{
    JRB node, node2;
    int cnt = 0;
    jrb_traverse(node, graph.vertices)
    {
        jrb_traverse(node2, graph.vertices) if (hasEdge(graph, jval_i(node->key), jval_i(node2->key)))
            cnt++;
    }
    return cnt;
}

int indegree(Graph graph, int v, int *output)
{
    JRB tree, node;
    int total = 0;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB)jval_v(node->val);
        if (jrb_find_int(tree, v))
        {
            output[total] = jval_i(node->key);
            total++;
        }
    }
    return total;
}

int outdegree(Graph graph, int v, int *output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph.edges, v);
    if (node == NULL)
        return 0;
    tree = (JRB)jval_v(node->val);
    total = 0;
    jrb_traverse(node, tree)
    {
        output[total] = jval_i(node->key);
        total++;
    }
    return total;
}

int DAG(Graph graph) // Khong cho chu trinh return 1
{
    int start, notCycles;
    JRB node;

    jrb_traverse(node, graph.vertices)
    {
        start = jval_i(node->key);
        // printf("Goi DFS xuat phat tu dinh %d\n", start);
        notCycles = DFS_DAG(graph, start);
        if (notCycles == 0)
            return 0;
    }
    return 1;
}

int DFS_DAG(Graph graph, int start)
{
    int visited[1000] = {};
    int output[100];
    Dllist node, stack;

    stack = new_dllist();
    dll_append(stack, new_jval_i(start));

    while (!dll_empty(stack))
    {
        node = dll_last(stack);
        int u = jval_i(node->val);
        dll_delete_node(node);

        if (!visited[u])
        {
            visited[u] = 1;
            int n = outdegree(graph, u, output);
            for (int i = 0; i < n; i++)
            {
                int v = output[i];
                if (v == start)
                    return 0;
                if (!visited[v])
                    dll_append(stack, new_jval_i(v));
            }
        }
    }
    return 1;
}

void swapArray(int arr[], int cnt)
{
    for (int i = 0; i < cnt / 2; i++)
    {
        int c = arr[i];
        arr[i] = arr[cnt - i - 1];
        arr[cnt - i - 1] = c;
    }
}

int topologicalSort(Graph graph, int *output)
{
    int indeg[1000], tmp[100];
    Dllist queue, node;
    queue = new_dllist();

    JRB vertex;
    jrb_traverse(vertex, graph.vertices)
    {
        int u = jval_i(vertex->key);
        indeg[u] = indegree(graph, u, tmp);
        if (indeg[u] == 0)
            dll_append(queue, new_jval_i(u));
    }

    int total = 0;
    while (!dll_empty(queue))
    {
        node = dll_first(queue);
        int u = jval_i(node->val);
        dll_delete_node(node);
        output[total++] = u;

        int m = outdegree(graph, u, tmp);
        for (int i = 0; i < m; i++)
        {
            int v = tmp[i];
            indeg[v]--;
            if (indeg[v] == 0)
                dll_append(queue, new_jval_i(v));
        }
    }
    swapArray(output, total);
    return total;
}

int DFS(Graph graph, int start, int stop, int *path)
{
    int visited[1000] = {};
    int output[100];
    int cnt = 0, u;
    int save[1000];
    Dllist stack = new_dllist();

    dll_append(stack, new_jval_i(start));

    while (!dll_empty(stack))
    {
        Dllist node = dll_last(stack);
        u = jval_i(node->val);
        dll_delete_node(node);

        if (!visited[u])
        {
            visited[u] = 1;
            if (u == stop)
                break;
            int n = outdegree(graph, u, output);
            for (int i = 0; i < n; i++)
            {
                int v = output[i];
                if (!visited[v])
                {
                    save[v] = u;
                    dll_append(stack, new_jval_i(v));
                }
            }
        }
    }
    if (u != stop)
        return 0;
    else
    {
        int i = stop;
        path[cnt++] = i;
        while (i != start)
        {
            i = save[i];
            path[cnt++] = i;
        }
        swapArray(path, cnt);
    }
    return cnt;
}

int shortest_noWeight_path(Graph graph, int start, int stop, int *path)
{
    JRB dist = make_jrb();
    Dllist node;

    Dllist queue = new_dllist();
    dll_append(queue, new_jval_i(start));

    Dllist trace = new_dllist();
    dll_append(trace, new_jval_i(start));
    jrb_insert_int(dist, start, new_jval_v(trace));

    int u;
    while (!dll_empty(queue))
    {
        node = dll_first(queue);
        u = jval_i(node->val);
        dll_delete_node(node);

        Dllist u_find_path = jval_v(jrb_find_int(dist, u)->val);
        if (u == stop)
        {
            int cnt = 0;
            Dllist tmp;
            dll_traverse(tmp, u_find_path)
            {
                cnt++;
                if (path != NULL)
                    path[cnt - 1] = jval_i(tmp->val);
            }
            return cnt;
        }

        int output[1000];
        int num = outdegree(graph, u, output);

        for (int i = 0; i < num; ++i)
        {
            JRB v_find = jrb_find_int(dist, output[i]);
            if (v_find == NULL)
            {
                Dllist current_path = new_dllist();
                Dllist node;
                dll_traverse(node, u_find_path)
                    dll_append(current_path, node->val);

                dll_append(current_path, new_jval_i(output[i]));
                jrb_insert_int(dist, output[i], new_jval_v(current_path));
                dll_append(queue, new_jval_i(output[i]));
            }
        }
    }
    if (u != stop)
        return 0;
}

double shortestPath(Graph graph, int start, int stop, int *path, int *numVertices)
{
    double distance[1000];
    int previous[1000], u, visit[1000];

    for (int i = 0; i < 1000; i++)
    {
        distance[i] = INFINITIVE_VALUE;
        visit[i] = 0;
        previous[i] = 0;
    }
    distance[start] = 0;
    previous[start] = start;
    visit[start] = 1;

    Dllist ptr, queue, node;
    queue = new_dllist();
    dll_append(queue, new_jval_i(start));

    while (!dll_empty(queue))
    {
        double min = INFINITIVE_VALUE;
        dll_traverse(ptr, queue)
        {
            u = jval_i(ptr->val);
            if (min > distance[u])
            {
                min = distance[u];
                node = ptr;
            }
        }

        u = jval_i(node->val);
        visit[u] = 1;
        dll_delete_node(node);
        if (u == stop)
            break;

        int output[1000];
        int n = outdegree(graph, u, output);

        for (int i = 0; i < n; i++)
        {
            int v = output[i];
            double w = getEdgeValue(graph, u, v);
            if (distance[v] > distance[u] + w)
            {
                distance[v] = distance[u] + w;
                previous[v] = u;
            }
            if (visit[v] == 0)
            {
                dll_append(queue, new_jval_i(v));
            }
        }
    }

    double distance_s_t = distance[stop];
    int count = 0;
    if (distance[stop] != INFINITIVE_VALUE)
    {
        path[count++] = stop;
        while (stop != start)
        {
            stop = previous[stop];
            path[count++] = stop;
        }
        *numVertices = count;
    }
    swapArray(path, count);
    return distance_s_t;
}

// void BFS_connected(Graph g, int start, int *id, int lt)
// {
//     int n, output[100];
//     Dllist node, queue;

//     queue = new_dllist();
//     dll_append(queue, new_jval_i(start));

//     while (!dll_empty(queue))
//     {
//         node = dll_first(queue);
//         int u = jval_i(node->val);
//         dll_delete_node(node);

//         if (id[u] == -1)
//         {
//             id[u] = lt;
//             n = getAdjacentVertices(g, u, output);
//             for (int i = 0; i < n; i++)
//             {
//                 int v = output[i];
//                 if (id[v] == -1)
//                     dll_append(queue, new_jval_i(v));
//             }
//         }
//     }
// }

// void getComponents(Graph g)
// {
//     int lt = 0;
//     int id[1000];
//     int n = getNumOfV(g);
//     for (int i = 0; i < n; i++)
//         id[i] = -1;
//     for (int i = 0; i < n; i++)
//         if (id[i] == -1)
//         {
//             lt++;
//             BFS_connected(g, i, id, lt);
//         }
//     if (lt == 1)
//         printf("Cac dinh da cho lien thong!\n");
//     else
//     {
//         printf("Do thi vo huong da cho khong lien thong!\n");
//         printf("So thanh phan lien thong cua do thi la: %d\n", lt);
//         for (int i = 1; i <= lt; i++)
//         {
//             printf("\nCac dinh thuoc thanh phan lien thong thu %d la: ", i);
//             for (int j = 0; j < n; j++)
//                 if (id[j] == i)
//                     printf("%d ", j);
//             printf("\n");
//         }
//     }
// }
