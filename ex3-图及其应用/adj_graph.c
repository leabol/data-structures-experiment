#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define MAX_VERTEX_NUM   20
typedef int InfoType;
typedef int VertexType;

typedef struct ArcNode{
    int             adjvex;
    struct ArcNode *nextarc;
    InfoType       *info;
}ArcNode;

typedef struct VNode{
   VertexType       data;
   ArcNode         *firstarc; 
}VNode, AdjList[MAX_VERTEX_NUM];

typedef struct{
    AdjList         vertices;//顶点数组
    int             vexnum;
    int             arcnum;
}ALGraph;

int init_ALGraph(ALGraph *G)
{
    if (!G || G->vexnum > MAX_VERTEX_NUM) return -1;

    G->arcnum = 0;
    G->vexnum = 0;

    for (int i = 0; i < MAX_VERTEX_NUM; i++){
        G->vertices[i].firstarc = NULL;
    }

    return 0;
}

static int LocateVex(const ALGraph *G, VertexType u)
{
    if (!G) return -1; 

    int idx = -1;
    for (int i = 0; i < G->vexnum; i++){
        if (G->vertices[i].data == u) idx = i;
    }

    return idx;
}
int AddVertex(ALGraph *G, VertexType data)
{
    if (!G || G->vexnum >= MAX_VERTEX_NUM) return -1;

    G->vertices[G->vexnum].data = data;
    G->vexnum++;    
    return 1;
}

int AddEdge(ALGraph *G, VertexType from, VertexType to, InfoType *info)
{
    if (!G) return -1;

    int from_idx = LocateVex(G, from);
    int to_idx = LocateVex(G, to);
    
    ArcNode *new_arcnode = (ArcNode*)malloc(sizeof(ArcNode));
    if (!new_arcnode) return -1;

    new_arcnode->nextarc = G->vertices[from_idx].firstarc;
    G->vertices[from_idx].firstarc = new_arcnode;
    new_arcnode->adjvex = to_idx;
    new_arcnode->info   = info;

    G->arcnum++;
    return 1;
}

int Ud_AddEdge(ALGraph *G, VertexType from, VertexType to, InfoType *info)
{
    if (AddEdge(G, from, to, info) == -1) return -1;
    if (AddEdge(G, to, from, info) == -1) return -1;
    return 1;
}

int DestroyALGraph(ALGraph *G)
{
    if (!G) return -1;

    for (int i = 0; i < G->vexnum; i++){
        ArcNode *tmp = G->vertices[i].firstarc;
        while (tmp){
            ArcNode *next = tmp->nextarc;
            free(tmp);
            tmp = next;
        }
    }
    free(G);
    return 1;
}

int FirstAdjVex(const ALGraph *G, valueType v)
{
    if (!G) return -1;

    int v_idx = LocateVex(G, v);
    if (G->vertices[v_idx].firstarc){
        return G->vertices[v_idx].firstarc->adjvex;
    }
    return -1;
}
int NextAdjVex(const ALGraph *G, valueType v, valueType w)
{
    if (!G) return -1;

    int v_idx = LocateVex(G, v);
    int w_idx = LocateVex(G, w);
    const ArcNode *tmp = G->vertices[v_idx].firstarc;

    while (tmp && tmp->adjvex != w_idx){
        tmp = tmp->nextarc;
    }

    if (tmp && tmp->nextarc){
        return tmp->nextarc->adjvex;
    }
    return -1;
}

static int visited[MAX_VERTEX_NUM];

void (*visitFunc) (VNode*);

static void DFS(ALGraph *G, int node_idx)
{
    if (node_idx < 0 || node_idx >= G->vexnum) return;

    visited[node_idx] = 1;

    visitFunc(&(G->vertices[node_idx]));

    ArcNode *tmp = G->vertices[node_idx].firstarc;
    while (tmp){
        if (!visited[tmp->adjvex]){
            //do something
            DFS(G, tmp->adjvex);
        }
        tmp = tmp->nextarc;
    }
    // for (int w = FirstAdjVex(G, node_idx); w >= 0; w = NextAdjVex(G, node_idx, w)){
    //     if (!visited[w]) DFS(G, w);
    // }
}

void DFSTraverse(ALGraph *G, void (*visit)(VNode*))
{
    if (!G) return;
   
    for (int i = 0; i < G->vexnum; i++){
        visited[i] = 0;
    }
    
    visitFunc = visit;

    for (int i = 0; i < G->vexnum; i++){
        if (!visited[i]) DFS(G, i);
    }
}

void BFSTraverse(ALGraph *G, void (*visit)(VNode *))
{
    if (!G) return;

    for (int i = 0; i < G->vexnum; i++){
        visited[i] = 0;
    }
    queue *Q = queue_new(G->vexnum);
    if (!Q) return;

    visitFunc = visit;
    
    for (int v = 0; v < G->vexnum; v++){
        if (visited[v]) continue;
    
        visited[v] = 1;
        visitFunc(&G->vertices[v]);
        queue_enqueue(Q, v);

        while (!queue_isEmpty(Q)){
            int v;
            queue_dequeue(Q, &v);
            for (ArcNode *tmp = G->vertices[v].firstarc; tmp; tmp = tmp->nextarc){
                if (visited[tmp->adjvex]) continue;
                visited[tmp->adjvex] = 1;
                visitFunc(&G->vertices[tmp->adjvex]);
                queue_enqueue(Q, tmp->adjvex);
            }//for
        }//while
    }//for
    queue_destroy(&Q);
}
void print(VNode* node)
{
    printf("%d ", node->data);
    fflush(stdout);
}
int main()
{
    ALGraph *G = malloc(sizeof(ALGraph));
    init_ALGraph(G);

    AddVertex(G, 1);
    AddVertex(G, 2);
    AddVertex(G, 3);
    AddVertex(G, 4);
    AddVertex(G, 5);
    AddVertex(G, 6);
    AddVertex(G, 7);
    AddVertex(G, 8);

    AddEdge(G, 1, 2, NULL);
    AddEdge(G, 1, 3, NULL);
    AddEdge(G, 2, 4, NULL);
    AddEdge(G, 2, 5, NULL);
    AddEdge(G, 4, 8, NULL);
    AddEdge(G, 5, 8, NULL);
    AddEdge(G, 3, 6, NULL);
    AddEdge(G, 3, 7, NULL);
    AddEdge(G, 6, 7, NULL);

    BFSTraverse(G, print);
    printf("\n");
    DFSTraverse(G, print);

    printf("\n");
}
