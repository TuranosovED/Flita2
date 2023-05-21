#include <stdio.h>
#include <stdlib.h>

#define swap(a, b, type) \
    type c = a;          \
    a = b;               \
    b = c;

typedef struct Points
{
    int point;
    int degree;
} Points;

typedef struct Edge
{
    int bottom;
    int top; // bottom -> top
} Edge;

typedef struct Graph
{
    Edge *edges;       // ссылка на массив ребер
    int countOfpoints; // число вершин
    int countOfEdges;  // число ребер
    int directed;      // направленный ли граф
    Points *pointsArray;
    int CountOfPersonalPoints;
} Graph;

typedef struct Array
{
    int *mas;
    int size;
} Array;

Points *MakePointsMass(Graph *);
int CountOfPersonalPoints(Graph *);
void UpdatePointsArray(Graph *);

void PrintGraph(Graph *graph)
{
    printf("Count of edges: %d\nCount of points: %d\n", graph->countOfEdges, graph->countOfpoints);
    if (graph->directed)
        puts("Is directed: true");
    else
        puts("Is directed: false");
    puts("List of edges:");
    for (int i = 0; i < graph->countOfEdges; i++)
    {
        if (i / 10 == 0)
            putchar(' ');
        if (graph->directed)
        {
            if (graph->edges[i].top != -1)
            {
                printf("%d: %d -> %d\n", i, graph->edges[i].bottom, graph->edges[i].top);
            }
            else
            {
                printf("%d: %d\n", i, graph->edges[i].bottom);
            }
        }
        else
        {
            if (graph->edges[i].top != -1)
            {
                printf("%d: %d -- %d\n", i, graph->edges[i].bottom, graph->edges[i].top);
            }
            else
            {
                printf("%d: %d\n", i, graph->edges[i].bottom);
            }
        }
    }
    putchar('\n');
}

void clearBuffer()
{
    char c;
    while ((c = getchar()) != '\n')
        ;
}

int EdgePresence(Graph *graph, Edge edge)
{
    for (int i = 0; i < graph->countOfEdges; i++)
    {
        if ((graph->edges[i].bottom == edge.bottom) && (graph->edges[i].top == edge.top))
            return 1;
    }
    return -1;
}

void DeleteEdge(Graph *graph, Edge edge, int mode) // 1 - удалить копии ребра(оставляет 1 экземпляр), 2 - удалить ребро и его копии
{
    int shift = 0;
    int countOfSameEdges = 0;
    Edge *buffer;
    for (int i = 0; i < graph->countOfEdges; i++)
    {
        if ((graph->edges[i].bottom == edge.bottom) && (graph->edges[i].top == edge.top))
            countOfSameEdges++;
    }
    // printf("countOfSameEdges %d\n",countOfSameEdges);
    switch (mode)
    {
    case 1:
        buffer = (Edge *)malloc(sizeof(Edge) * ((graph->countOfEdges) - countOfSameEdges + 1));
        break;
    case 2:
        buffer = (Edge *)malloc(sizeof(Edge) * (graph->countOfEdges - countOfSameEdges));
        break;
    }

    int trigger = 0;

    for (int i = 0; i < graph->countOfEdges; i++)
    {
        if ((graph->edges[i].bottom != edge.bottom) || (graph->edges[i].top != edge.top))
        {
            buffer[i - shift] = graph->edges[i];
        }
        else
        {
            if ((mode == 1) && (trigger != 1))
            {
                buffer[i - shift] = graph->edges[i];
                trigger = 1;
                printf("%d %d\n", graph->edges[i].bottom, graph->edges[i].top);
            }
            else
                shift++;
        }
    }
    free(graph->edges);

    graph->edges = buffer;
    if(edge.top != -1)
        graph->countOfpoints -= 2;
    else 
        graph->countOfpoints--;

    switch (mode)
    {
    case 1:
        graph->countOfEdges -= (countOfSameEdges - 1);
        break;
    case 2:
        graph->countOfEdges -= countOfSameEdges;
        break;
    }
    graph->CountOfPersonalPoints = CountOfPersonalPoints(graph);
    UpdatePointsArray(graph);
}

void DeleteSameEdges(Graph *graph)
{
    for (int i = 0; i < graph->countOfEdges; i++)
    {
        for (int j = i + 1; j < graph->countOfEdges; j++)
        {
            if ((graph->edges[i].bottom == graph->edges[j].bottom) && (graph->edges[i].top == graph->edges[j].top))
            {
                DeleteEdge(graph, graph->edges[i], 1);
            }
        }
    }
}

Graph *MakeGraph(char *fileName)
{
    char value;
    char Commads = '\0';
    FILE *f = fopen(fileName, "r");
    if (f == NULL)
    {
        puts("File not founded");
        return NULL;
    }

    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->countOfEdges = 0;

    for (graph->countOfpoints = 0; (value = fgetc(f)) != EOF;)
    {

        if (value != '\n')
        {
            if (value == ' ')
                graph->countOfpoints++;
        }
        else
        {
            graph->countOfpoints++;
            graph->countOfEdges++;
        }
    }
    graph->countOfEdges++;
    graph->countOfpoints++;
    fclose(f);

    // for(graph->countOfpoints = 0;(fscanf(f,"%d",&value1)!=EOF);graph->countOfpoints++); //считаем кол-во вершин
    // fclose(f);

    // fopen(fileName,"r");
    // for(int i = 0;(value = fgetc(f)) != EOF;)
    //     if(value == '\n')graph->countOfEdges++;
    // fclose(f);
    // graph->countOfEdges++;

    graph->edges = (Edge *)malloc(sizeof(Edge) * graph->countOfEdges);
    fopen(fileName, "r");

    for (int i = 0; (value = fgetc(f)) != EOF;)
    {
        if (value != '\n')
        {
            graph->edges[i].bottom = value - '0';
            value = fgetc(f);
            if (value != '\n')
            {
                value = fgetc(f);
                if (value >= '0' && value <= '9')
                {
                    graph->edges[i].top = value - '0';
                }
                else
                {
                    graph->edges[i].top = -1;
                }
                if (value != '\n')
                    value = fgetc(f);
                i++;
            }
            else
            {
                graph->edges[i].top = -1;
                i++;
            }
        }
        else
            i++;
    }

    fclose(f);

    while (Commads == '\0')
    {
        puts("Is graph directed? (Y/N)");
        scanf("%c", &Commads);

        switch (Commads)
        {
        case 'Y':
            graph->directed = 1;
            break;
        case 'y':
            graph->directed = 1;
            break;
        case 'N':
            graph->directed = 0;
            break;
        case 'n':
            graph->directed = 0;
            break;
        default:
            Commads = '\0';
            break;
        }
        clearBuffer();
    }
    graph->pointsArray = MakePointsMass(graph);

    graph->CountOfPersonalPoints = CountOfPersonalPoints(graph);

    return graph;
}

void MakeDotFile(Graph *graph)
{
    FILE *f = fopen("graph.dot", "w");
    if (graph->directed)
        fprintf(f, "digraph mygraph {\n");
    else
        fprintf(f, "graph mygraph {\n");
    for (int i = graph->countOfEdges - 1; i >= 0; i--)
    {
        if (graph->directed)
        {
            if (graph->edges[i].top != -1)
            {
                fprintf(f, "%d -> %d\n", graph->edges[i].bottom, graph->edges[i].top);
            }
            else
            {
                fprintf(f, "%d\n", graph->edges[i].bottom);
            }
        }
        else
        {
            if (graph->edges[i].top != -1)
            {
                fprintf(f, "%d -- %d\n", graph->edges[i].bottom, graph->edges[i].top);
            }
            else
            {
                fprintf(f, "%d\n", graph->edges[i].bottom);
            }
        }
    }
    fputc('}', f);
    fclose(f);
}

void MakeGraphPNG(Graph *graph)
{
    MakeDotFile(graph);
    system("move graph.dot %cd%\\graphviz\\bin & cd %cd%\\graphviz\\bin & dot -Tpng graph.dot -o graph.png & move graph.dot ..\\..\\ & move graph.png ..\\..\\");
}

void DeepFirstSeatch(Graph *graph, int point, Array *VisitedPoints)
{
    int trigger = 0;
    for (int i = 0; i < graph->countOfEdges; i++)
    {
        trigger = 0;
        if (graph->edges[i].bottom == point)
        {
            for (int j = 0; j < (*VisitedPoints).size; j++)
            {
                if (graph->edges[i].top == (*VisitedPoints).mas[j])
                    trigger = 1;
            }
            if (trigger == 0)
            {
                for (int j = 0; j < (*VisitedPoints).size; j++)
                {
                    if ((*VisitedPoints).mas[j] == point)
                        break;
                    if ((*VisitedPoints).mas[j] == -1)
                    {
                        (*VisitedPoints).mas[j] = point;
                        break;
                    }
                }
                DeepFirstSeatch(graph, graph->edges[i].top, VisitedPoints);
            }
        }
        else if (graph->edges[i].top == point)
        {
            for (int j = 0; j < (*VisitedPoints).size; j++)
            {
                if (graph->edges[i].bottom == (*VisitedPoints).mas[j])
                    trigger = 1;
            }
            if (trigger == 0)
            {
                for (int j = 0; j < (*VisitedPoints).size; j++)
                {
                    if ((*VisitedPoints).mas[j] == point)
                        break;
                    if ((*VisitedPoints).mas[j] == -1)
                    {
                        (*VisitedPoints).mas[j] = point;
                        break;
                    }
                }
                DeepFirstSeatch(graph, graph->edges[i].bottom, VisitedPoints);
            }
        }
        else
        {
            if (trigger == 0)
            {
                for (int j = 0; j < (*VisitedPoints).size; j++)
                {
                    if ((*VisitedPoints).mas[j] == point)
                        break;
                    if ((*VisitedPoints).mas[j] == -1)
                    {
                        (*VisitedPoints).mas[j] = point;
                        break;
                    }
                }
            }
        }
    }
    // for(int i =0;i<(*VisitedPoints).size;i++)
    //     printf("%d ",(*VisitedPoints).mas[i]);
    // putchar('\n');
}

int CountOfPersonalPoints(Graph *graph)
{
    int trigger;
    int *mas = (int *)malloc(sizeof(int) * graph->countOfpoints);
    for (int i = 0; i < graph->countOfpoints; i++)
        mas[i] = -1;
    for (int i = 0; i < graph->countOfEdges; i++)
    {
        trigger = 0;
        for (int j = 0; j < graph->countOfpoints; j++)
        {
            if (graph->edges[i].bottom == mas[j])
                trigger = 1;
        }
        if (trigger == 0)
        {
            for (int j = 0; j < graph->countOfpoints; j++)
            {
                if (mas[j] == -1)
                {
                    mas[j] = graph->edges[i].bottom;
                    break;
                }
            }
        }
        trigger = 0;
        for (int j = 0; j < graph->countOfpoints; j++)
        {
            if (graph->edges[i].top == mas[j])
                trigger = 1;
        }
        if (trigger == 0)
        {
            for (int j = 0; j < graph->countOfpoints; j++)
            {
                if (mas[j] == -1)
                {
                    mas[j] = graph->edges[i].top;
                    break;
                }
            }
        }
    }
    int count = 0;
    for (int i = 0; mas[i] != -1; i++)
        count++;
    return count;
}

int CheakGraphConnectivity(Graph *graph)
{
    Array VisitedPoints;
    VisitedPoints.size = graph->CountOfPersonalPoints;
    VisitedPoints.mas = (int *)malloc(sizeof(int) * VisitedPoints.size);
    for (int i = 0; i < VisitedPoints.size; i++)
        VisitedPoints.mas[i] = -1;
    DeepFirstSeatch(graph, graph->edges[0].bottom, &VisitedPoints);
    putchar('\n');
    for (int i = 0; i < VisitedPoints.size; i++)
        if (VisitedPoints.mas[i] == -1)
            return 0;
    return 1;
}

Points *MakePointsMass(Graph *graph)
{
    int count;
    int trigger;
    int len = graph->CountOfPersonalPoints;
    Points *pointsArray = malloc(sizeof(Points) * len);
    for (int i = 0; i < len; i++)
    {
        pointsArray[i].point = -2;
        pointsArray[i].degree = 0;
    }

    for (int i = 0; i < graph->countOfEdges; i++) // 0 - встреч не было 1 - была
    {
        trigger = 0;
        for (int j = 0; j < len; j++)
        {
            if (pointsArray[j].point == graph->edges[i].bottom)
            {
                trigger = 1;
            }
        }
        if (trigger == 0 && graph->edges[i].bottom != -1)
        {
            for (int j = 0; j < len; j++)
            {
                if (pointsArray[j].point == -2)
                {
                    pointsArray[j].point = graph->edges[i].bottom;
                    break;
                }
            }
        }
        trigger = 0;
        for (int j = 0; j < len; j++)
        {
            if (pointsArray[j].point == graph->edges[i].top)
            {
                trigger = 1;
            }
        }
        if (trigger == 0 && graph->edges[i].top != -1)
        {
            for (int j = 0; j < len; j++)
            {
                if (pointsArray[j].point == -2)
                {
                    pointsArray[j].point = graph->edges[i].top;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < graph->countOfEdges; j++)
        {
            if ((pointsArray[i].point == graph->edges[j].bottom) && graph->edges[j].top != -1)
                pointsArray[i].degree++;
            if (pointsArray[i].point == graph->edges[j].top)
                pointsArray[i].degree++;
        }
    }
    return pointsArray;
}

void UpdatePointsArray(Graph *graph)
{
    free(graph->pointsArray);
    graph->pointsArray = MakePointsMass(graph);
}

void PrintPointsArray(Graph *graph)
{
    int len = graph->CountOfPersonalPoints;
    for (int j = 0; j < len; j++)
    {
        printf("point: %d degree - %d\n", graph->pointsArray[j].point, graph->pointsArray[j].degree);
    }
    putchar('\n');
}

void SortPointsMass(Graph *graph)
{
    int left = 0;
    int right = graph->CountOfPersonalPoints - 1;
    int buffer; // так как после первого прохода наиб элемент будет справа, а наим слева, то сужаем границы

    while (left < right)
    {
        for (int i = left; i < right; i++)
        {
            if (graph->pointsArray[i].degree < graph->pointsArray[i + 1].degree)
            {
                swap(graph->pointsArray[i], graph->pointsArray[i + 1], Points);
                buffer = i;
            }
        }
        right = buffer;
        for (int i = right; i > left; i--)
        {
            if (graph->pointsArray[i].degree > graph->pointsArray[i - 1].degree)
            {
                swap(graph->pointsArray[i], graph->pointsArray[i - 1], Points);
                buffer = i;
            }
        }
        left = buffer;
    }
}

void SortGraphByBottom(Graph *graph)
{
    Edge *buffer = (Edge*)malloc(sizeof(Edge) * graph->countOfEdges);
    for (int i = 0; i < graph->CountOfPersonalPoints; i++)
    {
        for(int j =0;j<graph->countOfEdges;j++)
        {
            if(graph->edges[j].bottom == graph->pointsArray[i].point)
            {
                buffer[j] = graph->edges[j];
            }
        }
    }
    free(graph->edges);
    graph->edges = buffer;
}

int main(int argc, char *argv[])
{
    Graph *graph = MakeGraph(argv[1]);

    PrintPointsArray(graph);
    PrintGraph(graph);
    // DeleteSameEdges(graph); удаление одинковых ребер
    // PrintGraph(graph);

    MakeGraphPNG(graph);
    if (CheakGraphConnectivity(graph))
        puts("Graph is connected");
    else
        puts("Graph is not connected");

    int bottom, top;
    puts("enter bottom");
    scanf("%d", &bottom);
    puts("enter top");
    scanf("%d", &top);
    //printf("count of personal points until %d\n", graph->CountOfPersonalPoints);
    // Удаление и ререндер png

    if (EdgePresence(graph, (Edge){bottom, top}))
        DeleteEdge(graph, (Edge){bottom, top}, 2);
    else
        puts("Edge not founded!");

    PrintPointsArray(graph);
    PrintGraph(graph);
    MakeGraphPNG(graph);
    //printf("count of personal points after %d\n", graph->CountOfPersonalPoints);
    SortPointsMass(graph);
    PrintPointsArray(graph);
    SortGraphByBottom(graph);
    PrintGraph(graph);
    return 0;
}