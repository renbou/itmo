from os import remove


NUM_VERTS = 1000

# инициализирует граф с NUM_VERTS вершинами, изначально у которых мн-во связных с ними вершин пусто
def init_graph():
    return dict(zip(
        [i for i in range(NUM_VERTS)],
        [set() for i in range(NUM_VERTS)]
    ))

# функция которая просто считает кол-во ребер у каждой вершины
def count_edges(graph):
    cnt = 0
    for _, value in graph.items():
        cnt += len(value)
    return cnt // 2 # т.к. каждое ребро посчитали дважды

# функция которая просто считает кол-во вершин у которых кол-во ребер равно 0
def count_isolated(graph):
    cnt = 0
    for _, value in graph.items():
        if len(value) == 0:
            cnt += 1
    return cnt

# функция которая просто исчет вершину макс. степени т.е. с макс. кол-вом ребер
def find_vertex_of_max_degree(graph):
    vertex = 0
    max_degree = 0
    for key, value in graph.items():
        if len(value) > max_degree:
            vertex = key
            max_degree = len(value)
    return vertex

# обход DFS для определения вершин комп. связности
def dfs_mark_component(graph, visited, v, component):
    component.add(v)
    if v in visited:
        return
    visited[v] = True
    for to in graph[v]:
        dfs_mark_component(graph, visited, to, component)

# функция которая ищет макс. связный подграф (компоненту связности)
# возвращает граф, отображающий компоненту
def get_max_component(graph):
    components = list()
    visited = dict()
    for v in graph:
        if v not in visited:
            c = set()
            dfs_mark_component(graph, visited, v, c)
            components.append(c)
    max_component = None
    max_component_len = 0
    for c in components:
        if len(c) > max_component_len:
            max_component_len = len(c)
            max_component = c
    
    component_graph = dict()
    for v in max_component:
        component_graph[v] = graph[v]
    return component_graph


# обход DFS для получения самой отдаленной вершины (от v)
def dfs_find_farthest(graph, visited, v):
    if v in visited:
        return 0, None
    visited[v] = True
    farthest_length = 0
    farthest = v
    for to in graph[v]:
        to_farthest_length, to_farthest = dfs_find_farthest(graph, visited, to)
        to_farthest_length += 1
        if to_farthest != None and to_farthest_length > farthest_length:
            farthest_length = to_farthest_length
            farthest = to_farthest
    return farthest_length, farthest

# алгоритм нахождения диаметра
def graph_get_diameter(graph):
    # возьмем любую вершину как начало
    start = list(graph.keys())[0]
    farthest_length, farthest = dfs_find_farthest(graph, dict(), start)
    diameter_size, _ = dfs_find_farthest(graph, dict(), farthest)
    return diameter_size

# алгоритм нахождения диаметра - найдем расст. между всеми парами вершин через флоайда-уоршелла и возьмём макс.
# def graph_get_diameter(graph):
#     distances = [[int(1e9)] * NUM_VERTS for i in range(NUM_VERTS)]
#     for i in graph:
#         distances[i][i] = 0
#         for j in graph[i]:
#             distances[i][j] = 1
#     for k in range(NUM_VERTS):
#         for i in range(NUM_VERTS):
#             for j in range(NUM_VERTS):
#                 distances[i][j] = min(distances[i][j], distances[i][k]+distances[k][j])
#     diameter_size = 0
#     for i in range(NUM_VERTS):
#         for j in range(NUM_VERTS):
#             if distances[i][j] != int(1e9) and distances[i][j] > diameter_size:
#                 diameter_size = distances[i][j]
#     return diameter_size

# алгоритм нахождения кратчайшего пути в графе и сохранении пути
def dijkstra(graph, start): 
        row = len(graph) 
        col = len(graph[0]) 
        
        dist = [int(1e9)] * NUM_VERTS
        dist[start] = 0
        
        parent = [-1] * NUM_VERTS

        queue = set()
        queue.add((0, start))
        while queue: 
            v = next(iter(queue))
            queue.remove(v) 
            v = v[1]
            for to in graph[v]:  
                if dist[v] + 1 < dist[to]: 
                    if (dist[to], to) in queue:
                        queue.remove((dist[to], to))
                    dist[to] = dist[v] + 1 
                    parent[to] = v
                    queue.add((dist[to], to))
        return dist, parent

# нахождение кратчайшего расстояния и вывод пути от одной до другой вершины
def find_min_path(graph, a, b):
    dist, parent = dijkstra(graph, a)
    if dist[b] != int(1e9):
        print('Минимальный путь между точками имеет длину', dist[b])
        print('Путь от первой вершины ко второй:')
        cur = b
        path = []
        while cur != a:
            path.append(cur)
            cur = parent[cur]
        path.append(a)
        print(path[::-1])
    else:
        print('Между точками нет пути!')

# функция, удобно объединяющая все прошлые функции для простого вывода ответа по каждому из двух "подзаданий"
# (до и после удаления некоторых вершин) 
def answer(graph, A, B, C, D, E, F):
    print(f'1. В графе {count_edges(graph)} ребер.')
    print(f'2. В графе {count_isolated(graph)} изолятов.')
    max_degree_vertex = find_vertex_of_max_degree(graph)
    print(f'3. Вершина {max_degree_vertex} имеет наибольшую степень - {len(graph[max_degree_vertex])}. Связанные с ней вершины:')
    s = ''
    for v in set(graph[max_degree_vertex]):
        s += f'{v}, '
    s = s[:-2]
    print(s)
    print(f'4. Диаметр компоненты связности (макс. размера) = {graph_get_diameter(get_max_component(graph))}')
    print(f'5. Кратчайший путь между A={A} и B={B}:')
    find_min_path(graph, A, B)
    print(f'6. Кратчайший путь между C={C} и D={D}:')
    find_min_path(graph, C, D)
    print(f'7. Кратчайший путь между E={E} и F={F}:')
    find_min_path(graph, E, F)

# полностью удалим вершину и все ребра с ней из графа
def remove_vert(graph, v):
    for to in graph[v]:
        graph[to].remove(v)
    graph[v] = set()
 
def main():
    # вершины из задания
    A = 658
    B = 23
    C = 378
    D = 811
    E = 541
    F = 988
    # вершины которые удалить + кратные 17
    to_delete = [434, 180, 342, 311, 604, 349]

    # будем хранить граф как ассоциативный массив у которого ключи - номера вершин,
    # а значения - мн-ва вершин, к которым есть ребро из вершины-ключа
    graph = init_graph()
    try:
        convert_to_int_tuple = lambda x: (int(x[0]), int(x[1]))
        with open('graphedges17.txt', 'r') as file:
            edge_list = list(
                map(convert_to_int_tuple, 
                    map(str.split,
                        file.readlines()
                        )
                    )
                )
    except ValueError as error:
        raise ValueError(f"Неправилный файл с ребрами: {error}")
    except FileNotFoundError as error:
        raise FileNotFoundError(f"Невозможно работать без файла graphedges17.txt: {error}")

    for edge in edge_list:
        graph[edge[0]].add(edge[1])
        graph[edge[1]].add(edge[0])
    
    print('7 заданий до удаления вершин:')
    answer(graph, A, B, C, D, E, F)
    
    # удалим по заданию а также кратные 17ти
    for v_del in to_delete:
        remove_vert(graph, v_del)
    for v_del in range(0, NUM_VERTS, 17):
        remove_vert(graph, v_del)

    print('\n\n7 заданий после удаления вершин:')
    answer(graph, A, B, C, D, E, F)


if __name__ == '__main__':
    main()