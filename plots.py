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
    with open("plot_datas/avg_degree.csv") as f:
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
            avg_degree = row[2]
            
            # create a dictionary to store the data in the format we want
            # for each type of time, we have a dictionary
            for i in range(4):
                data = {}
                
                data[headers[0]] = num_vertices
                data[headers[1]] = num_edges
                data[headers[2]] = avg_degree
                data["Algorithm"] = headers[i + 3]
                data["Time (s)"] = float(row[i + 3])
                # normalize the time
                data["Time (%)"] = float(row[i + 3]) / max(float(row[3]), float(row[4]), float(row[5]), float(row[6]))
                
                datalist.append(data)
                
        # printList(datalist)

        # convert to pandas dataframe
        df = pd.DataFrame(datalist)
        
        print(df)
        
        # sns.catplot(data=df, x="Edges", y="Time", hue="Algorithm", kind="bar", ci=None)
        
        # add tooltips
        fig = px.bar(df, x="Average Degree", y="Time (%)", color="Algorithm", barmode="group", hover_data=["Vertices", "Edges", "Average Degree", "Time (s)", "Time (%)"])
        fig.update_layout(title="Time taken as percentage by Tarjan-Vishkin and its variants")
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

def aux_edges():
    with open("plot_datas/aux_edges.csv") as f:
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
            avg_degree = row[2]
            
            # create a dictionary to store the data in the format we want
            # for each type of time, we have a dictionary
            for i in range(2):
                data = {}
                
                data[headers[0]] = num_vertices
                data[headers[1]] = num_edges
                data[headers[2]] = avg_degree
                data["Algorithm"] = headers[i + 3]
                
                if (float(row[i+3]) == 0):
                    continue
                
                data["Edges"] = float(row[i + 3])
                data["Edges (%)"] = float(row[i + 3])/float(row[3])
            
                datalist.append(data)
                
        # printList(datalist)

        # convert to pandas dataframe
        df = pd.DataFrame(datalist)
        
        print(df)
        
        # sns.catplot(data=df, x="Edges", y="Time", hue="Algorithm", kind="bar", ci=None)
        
        # add tooltips
        fig = px.bar(df, x="Average Degree", y="Edges (%)", color="Algorithm", barmode="group", hover_data=["Vertices", "Edges", "Average Degree", "Edges (%)"])
        fig.update_layout(title="Edges in the auxiliary graph as a percentage by Tarjan-Vishkin and its variants")
        fig.show()

def aux_times():
    with open("plot_datas/aux_times.csv") as f:
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
            avg_degree = row[2]
            
            # create a dictionary to store the data in the format we want
            # for each type of time, we have a dictionary
            for i in range(2):
                data = {}
                
                data[headers[0]] = num_vertices
                data[headers[1]] = num_edges
                data[headers[2]] = avg_degree
                data["Algorithm"] = headers[i + 3]
                
                if (float(row[i+3]) == 0):
                    continue
                
                data["Time (s)"] = float(row[i + 3]) / 10
                data["Time (%)"] = float(row[i + 3])/float(row[3])
            
                datalist.append(data)
                
        # printList(datalist)

        # convert to pandas dataframe
        df = pd.DataFrame(datalist)
        
        print(df)
        
        # sns.catplot(data=df, x="Edges", y="Time", hue="Algorithm", kind="bar", ci=None)
        
        # add tooltips
        fig = px.bar(df, x="Average Degree", y="Time (%)", color="Algorithm", barmode="group", hover_data=["Vertices", "Edges", "Average Degree", "Time (s)"])
        fig.update_layout(title="Time taken as a percentage by Tarjan-Vishkin and its variants")
        fig.show()

def combine_time_aux():
    
    edge_df = None
    time_df = None
    
    with open("plot_datas/aux_named.csv") as f:
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
            avg_degree = row[2]
            
            # create a dictionary to store the data in the format we want
            # for each type of time, we have a dictionary
            for i in range(2):
                data = {}
                
                data[headers[0]] = num_vertices
                data[headers[1]] = num_edges
                data[headers[2]] = avg_degree
                data["Graph Name"] = row[3]
                data["Algorithm"] = headers[i + 4]
                
                if (float(row[i+4]) == 0):
                    continue
                
                data["Edges"] = float(row[i + 4])
                data["Edges (%)"] = float(row[i + 4])/float(row[4])
            
                datalist.append(data)
                
        # printList(datalist)

        # convert to pandas dataframe
        df = pd.DataFrame(datalist)
        
        print(df.head())
        
        edge_df = df
        
    with open("plot_datas/named_graphs.csv") as f:
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
            avg_degree = row[2]
            
            # create a dictionary to store the data in the format we want
            # for each type of time, we have a dictionary
            for i in range(2):
                data = {}
                
                data[headers[0]] = num_vertices
                data[headers[1]] = num_edges
                data[headers[2]] = avg_degree
                data["Graph Name"] = row[3]
                data["Algorithm"] = headers[i + 4]
                
                if (float(row[i+4]) == 0):
                    continue
                
                data["Time (s)"] = float(row[i + 4])
                data["Time (%)"] = float(row[i + 4])/float(row[4])
            
                datalist.append(data)
                
        # printList(datalist)

        # convert to pandas dataframe
        df = pd.DataFrame(datalist)
        
        print(df.head())
    

# degree_vs_time()
# threads_vs_time()
# degree_vs_time()
# aux_edges()
# aux_times()
combine_time_aux()