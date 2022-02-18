#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "graph.h"

/***************************************************************************/
void edge_to_string(const edge_t *edge, char *str, unsigned size) /* GOOD */
{
  assert(str != NULL);
  assert(size > 0);

  /* <tail> "->" <head> "("<weight>")" */

  unsigned edge_tail = edge->tail;
  unsigned edge_head = edge->head;
  unsigned edge_weight = edge->weight;

  snprintf(str, size, "%2d -> %2d (%02d)", edge_tail, edge_head, edge_weight);
}

/***************************************************************************/
bool list_is_empty(const adjacency_list_t *list) /* GOOD */
{
  assert(list != NULL);

  if (list->first == NULL)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/***************************************************************************/
unsigned list_size(const adjacency_list_t *list) /* GOOD */
{
  assert(list != NULL);

  int list_length = 1;
  edge_t *tmp = list->first;

  while (tmp->next != NULL)
  {
    list_length += 1;
    tmp = tmp->next;
  }
  return list_length;
}

/***************************************************************************/
void list_prepend(adjacency_list_t *list, edge_t *edge) /* GOOD */
{
  assert(list != NULL);
  assert(edge != NULL);

  edge_t *prev = list->first;
  list->first = edge;
  edge->next = prev;
}

/***************************************************************************/
bool list_contains(const adjacency_list_t *list, unsigned tail, unsigned head) /* GOOD */
{
  assert(list != NULL);

  if (list->first->head == head && list->first->tail == tail)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/***************************************************************************/
bool graph_initialise(graph_t *graph, unsigned vertex_count) /* GOOD */
{

  assert(graph != NULL);

  graph->vertex_count = vertex_count;
  graph->adjacency_lists = (struct adjacency_list_s *)malloc(vertex_count * sizeof(adjacency_list_t));

  if (graph->adjacency_lists == NULL)
  {
    return false;
  }
  else
  {
    return true;
  }
}

/***************************************************************************/
void graph_print(const graph_t *graph) /* GOOD */
{
  assert(graph != NULL);

  /* edge_to_string(&edge, str, sizeof(str)); */

  unsigned number_of_vertices = graph->vertex_count;
  unsigned number_of_edges = graph->edge_count;
  char str[20] = "";

  printf("Graph with %d vertices and %d edges:\n", number_of_vertices, number_of_edges);
  for (int i = 0; i < number_of_vertices; i++)
  {
    printf("vertex %i:\n", i);
    if (graph->adjacency_lists[i].first != NULL)
    {
      edge_t *edge = graph->adjacency_lists[i].first;
      while (edge->next != NULL)
      {
        edge_to_string(edge, str, sizeof(str));
        printf("  %s\n", str);
        edge = edge->next;
      }
      edge_to_string(edge, str, sizeof(str));
      printf("  %s\n", str);
    }
  }
}

/***************************************************************************/
void graph_release(graph_t *graph)
{
  assert(graph != NULL);

  /*  - Al het geheugen van lijsten en edges freeen
      - Loop alle lijsten, alle edges en free
      - free(graph -> adjacency_lists);
      - Waarden in graph resetten
  */
}

/***************************************************************************/
bool graph_connect(graph_t *graph, unsigned tail, unsigned head, unsigned weight) /* GOOD */
{
  assert(graph != NULL);

  edge_t *edge = (struct edge_s *)malloc(sizeof(edge_t));
  if (edge == NULL)
  {
    return false;
  }
  else
  {
    edge->tail = tail;
    edge->head = head;
    edge->weight = weight;

    if (graph->adjacency_lists[tail].first == NULL)
    {
      graph->adjacency_lists[tail].first = edge;
      edge->next = NULL;
    }
    else
    {
      edge->next = graph->adjacency_lists[tail].first;
      graph->adjacency_lists[tail].first = edge;
    }
  }

  return true;
}

/***************************************************************************/
void graph_disconnect(graph_t *graph, unsigned tail, unsigned head)
{
  assert(graph != NULL);

  edge_t *edge = graph->adjacency_lists[tail].first;
  edge_t *prev = graph->adjacency_lists[tail].first;

  if (edge->next == NULL && edge->head == head)
  {
    free(edge);
    graph->edge_count -= 1;
  }

  else
  {
    while (edge != NULL)
    {
      edge = edge->next;
      if (prev->head == head)
      {
        free(prev);
        graph->edge_count -= 1;
      }
      prev = edge;
      edge = edge->next;
    }
  }
}
/***************************************************************************/
unsigned graph_indegree(const graph_t *graph, unsigned id) /* GOOD */
{
  assert(graph != NULL);

  unsigned result = 0;

  if (id < graph->vertex_count)
  {
    for (size_t i = 0; i < graph->vertex_count; i++)
    {
      const adjacency_list_t *list = &graph->adjacency_lists[i];

      for (edge_t *edge = list->first; edge != NULL; edge = edge->next)
      {
        if (edge->head == id)
        {
          result++;
        }
      }
    }
  }

  return result;
}

/***************************************************************************/
unsigned graph_outdegree(const graph_t *graph, unsigned id) /* GOOD */
{
  assert(graph != NULL);

  unsigned number_of_vertices = graph->vertex_count;

  if (id > number_of_vertices)
  {
    return 0;
  }
  else
  {
    unsigned length = list_size(&graph->adjacency_lists[id]);
    return length;
  }
}

/***************************************************************************/
void graph_build_from_file(graph_t *graph, const char *pathname)
{
  assert(graph != NULL);
  assert(pathname != NULL);

  FILE *fp = fopen(pathname, "r");
  if (fp != NULL)
  {
    unsigned vertex_count;

    if (fscanf(fp, "%u", &vertex_count) == 1)
    {
      if (graph_initialise(graph, vertex_count))
      {
        while (!feof(fp))
        {
          unsigned tail;
          unsigned head;
          unsigned weight;

          int n = fscanf(fp, "%u %u %u", &tail, &head, &weight);

          if (!feof(fp))
          {
            if (n == 3)
            {
              if (!graph_connect(graph, tail, head, weight))
              {
                fprintf(stderr, "Failed to connect: %d->%d\n", tail, head);
              }
            }
            else
            {
              /* Bad format */
            }
          }
        }
      }
    }
    else
    {
      /* Bad format */
    }

    (void)fclose(fp);
  }
}

/***************************************************************************/
void graph_to_dot(const graph_t *graph, const char *pathname)
{
  assert(graph != NULL);
  assert(pathname != NULL);

  FILE *fp = fopen(pathname, "w");
  if (fp != NULL)
  {
    fprintf(fp, "digraph {\n");

    for (size_t i = 0; i < graph->vertex_count; i++)
    {
      const adjacency_list_t *list = &graph->adjacency_lists[i];

      for (edge_t *edge = list->first; edge != NULL; edge = edge->next)
      {
        fprintf(fp, "%d -> %d\n", edge->tail, edge->head);
      }
    }

    fprintf(fp, "}\n");
  }
}
