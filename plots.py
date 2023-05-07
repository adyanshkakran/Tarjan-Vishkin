import csv
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np
import plotly.express as px

# csv structure:
# num_vertices, num_edges, avg_degree, tarjan_vishkin_time, tarjan_vishkin_union_find_time, tarjan_vishkin_parallel_time, tarjan_vishkin_parallel_union_find_time

def printList(list):
    for data in list:
        print(data)

def degree_vs_time():
    with open("results.csv") as f:
        # read csv file
        csv_reader = csv.reader(f, delimiter=',')
        # skip the header
        headers = next(csv_reader)
        
        # strip the headers of whitespaces
        headers = [header.strip() for header in headers]
        
        datalist = []
        
        for row in csv_reader:
            num_vertices = int(row[0])
            num_edges = int(row[1])
            avg_degree = float(row[2])
            
            # create a dictionary to store the data in the format we want
            # for each type of time, we have a dictionary
            for i in range(4):
                data = {}
                
                data[headers[0]] = num_vertices
                data[headers[1]] = num_edges
                data[headers[2]] = avg_degree
                data["Algorithm"] = headers[i + 3]
                data["Time (s)"] = float(row[i + 3])
                
                datalist.append(data)
                
        # printList(datalist)

        # convert to pandas dataframe
        df = pd.DataFrame(datalist)
        
        print(df)
        
        # sns.catplot(data=df, x="Edges", y="Time", hue="Algorithm", kind="bar", ci=None)
        
        # add tooltips
        fig = px.bar(df, x="Average Degree", y="Time (s)", color="Algorithm", barmode="group", hover_data=["Vertices", "Edges", "Average Degree", "Time (s)"])
        fig.update_layout(title="Time taken by Tarjan-Vishkin and its variants")
        fig.show()
        
        # plt.show()
        
def vertices_vs_time():
    with open("plot_datas/data2.csv") as f:
        # read csv file
        csv_reader = csv.reader(f, delimiter=',')
        # skip the header
        headers = next(csv_reader)
        
        # strip the headers of whitespaces
        headers = [header.strip() for header in headers]
        
        datalist = []
        
        for row in csv_reader:
            num_vertices = int(row[0])
            num_edges = int(row[1])
            avg_degree = float(row[2])
            
            # create a dictionary to store the data in the format we want
            # for each type of time, we have a dictionary
            for i in range(4):
                data = {}
                
                data[headers[0]] = num_vertices
                data[headers[1]] = num_edges
                data[headers[2]] = avg_degree
                data["Algorithm"] = headers[i + 3]
                data["Time (s)"] = float(row[i + 3])
                
                datalist.append(data)
                
        # printList(datalist)

        # convert to pandas dataframe
        df = pd.DataFrame(datalist)
        
        print(df)
        
        # sns.catplot(data=df, x="Edges", y="Time", hue="Algorithm", kind="bar", ci=None)
        
        # add tooltips
        fig = px.bar(df, x="Vertices", y="Time (s)", color="Algorithm", barmode="group", hover_data=["Vertices", "Edges", "Average Degree", "Time (s)"])
        fig.update_layout(title="Time taken by Tarjan-Vishkin and its variants")
        fig.show()
        
        # plt.show()

def threads_vs_time():
    with open("plot_datas/threads_300k.csv") as f: # also try threads_50k.csv
        # read csv file
        csv_reader = csv.reader(f, delimiter=',')
        # skip the header
        headers = next(csv_reader)
        
        # strip the headers of whitespaces
        headers = [header.strip() for header in headers]
        
        datalist = []
        
        for row in csv_reader:
            num_vertices = int(row[0])
            num_edges = int(row[1])
            avg_degree = float(row[2])
            thread_count = row[3]
            
            # create a dictionary to store the data in the format we want
            # for each type of time, we have a dictionary
            for i in range(4):
                data = {}
                
                data[headers[0]] = num_vertices
                data[headers[1]] = num_edges
                data[headers[2]] = avg_degree
                data["Thread Count"] = thread_count
                data["Algorithm"] = headers[i + 4]
                data["Time (s)"] = float(row[i + 4])
                
                datalist.append(data)
                
        # printList(datalist)

        # convert to pandas dataframe
        df = pd.DataFrame(datalist)
        
        print(df)
        
        # sns.catplot(data=df, x="Edges", y="Time", hue="Algorithm", kind="bar", ci=None)
        
        # add tooltips
        fig = px.bar(df, x="Thread Count", y="Time (s)", color="Algorithm", barmode="group", hover_data=["Vertices", "Edges", "Average Degree", "Time (s)"])
        fig.update_layout(title="Time taken by Tarjan-Vishkin and its variants")
        fig.show()
        
        # plt.show()

# vertices_vs_time()
threads_vs_time()