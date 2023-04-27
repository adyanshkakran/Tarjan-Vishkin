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
    
    with open("datasets/graph1.txt", "w") as f:
        f.write(str(num_vertices) + " " + str(num_edges) + "\n")
        # print the edges
        
        for(u, v) in G.edges():
            f.write(str(u) + " " + str(v) + "\n")
        
    # nx.draw(G, with_labels=True)
    # plt.show()

generate_random_graph(1000, 20000)