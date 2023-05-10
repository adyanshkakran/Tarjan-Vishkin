import networkx as nx
import matplotlib.pyplot as plt
import random

def generate_random_graph(num_vertices, num_edges):
    G = nx.Graph()
    # Add vertices
    for i in range(num_vertices):
        G.add_node(i)
    # Add edges
    while G.number_of_edges() < num_edges:
        u = random.randint(0, num_vertices - 1)
        v = random.randint(0, num_vertices - 1)
        if u != v and not G.has_edge(u, v):
            G.add_edge(u, v)
    # Display the graph
    # print(G.number_of_nodes(), G.number_of_edges())
    
    with open("graph.in", "w") as f:
        # Write number of vertices and edges
        f.write(str(G.number_of_nodes()) + " " + str(G.number_of_edges()) + "\n")

        for edge in G.edges():
            f.write(str(edge[0]) + " " + str(edge[1]) + "\n")
    # nx.draw(G, with_labels=True)
    # plt.show()

generate_random_graph(10, 30)