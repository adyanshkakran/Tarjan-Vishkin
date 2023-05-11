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

def get_edge_data():
    with open("plot_datas/aux_named.csv") as f:
        # read csv file
        csv_reader = csv.reader(f, delimiter=',')
        # skip the header
        headers = next(csv_reader)
        
        # strip the headers of whitespaces
        headers = [header.strip() for header in headers]
        
        datalist = []
        
        aggDegrees = {}
        
        for row in csv_reader:
            if (float(row[4]) == 0):
                continue
            
            num_vertices = int(row[0])
            num_edges = int(row[1])
            avg_degree = row[2]
            
            data = {}
            
            data[headers[0]] = num_vertices
            data[headers[1]] = num_edges
            data[headers[2]] = avg_degree
            data["Graph Name"] = row[3]
            data[headers[4]] = float(row[4])
            data[headers[5]] = float(row[5])
            
            data["Decrease using Union Find (%)"] = (1-(data[headers[5]]/data[headers[4]])) * 100
            # round to 2 decimal places
            data["Decrease using Union Find (%)"] = round(data["Decrease using Union Find (%)"], 2)
            datalist.append(data)
            
            # add to aggregate degrees dictionary
            avg_degree = float(avg_degree)
            if round((avg_degree)) not in aggDegrees:
                aggDegrees[round(avg_degree)] = {
                    "count": 1,
                    "decrease": 1-(data[headers[5]]/data[headers[4]])
                }
            else:
                aggDegrees[round(avg_degree)]["count"] += 1
                aggDegrees[round(avg_degree)]["decrease"] += 1-(data[headers[5]]/data[headers[4]])
        # printList(datalist)
        
        # get average decrease for each degree
        for degree in aggDegrees:
            aggDegrees[degree]["decrease"] /= aggDegrees[degree]["count"]
            # multiply by 100 to get percentage
            aggDegrees[degree]["decrease"] *= 100
            
            # round to 2 decimal places
            aggDegrees[degree]["decrease"] = round(aggDegrees[degree]["decrease"], 2)
            
        print(aggDegrees)
        

        # convert to pandas dataframe
        df = pd.DataFrame(datalist)
        
        print(df.head())
        
        # write the df to a csv file
        df.to_csv("plot_datas/edge_table.csv", index=False)
        
        # create a df from the dictionary
        df = pd.DataFrame.from_dict(aggDegrees, orient="index")
        df = df.reset_index()

        # write the df to a csv file
        df.to_csv("plot_datas/edge_agg.csv", index=False)

def get_time_data():
    with open("plot_datas/named_graphs.csv") as f:
        # read csv file
        csv_reader = csv.reader(f, delimiter=',')
        # skip the header
        headers = next(csv_reader)
        
        # strip the headers of whitespaces
        headers = [header.strip() for header in headers]
        
        datalist = []
        
        aggNames = ["as-caida","delaunay","Erdos", "fe_","cca_G", "ccc_G", "bfly"]

        aggDict = {}
        
        for row in csv_reader:
            if (float(row[4]) == 0):
                continue
            
            num_vertices = int(row[0])
            num_edges = int(row[1])
            avg_degree = row[2]
            
            data = {}
            
            data[headers[0]] = num_vertices
            data[headers[1]] = num_edges
            data[headers[2]] = avg_degree
            data["Graph Name"] = row[3]
            data[headers[4]] = float(row[4])
            data[headers[5]] = float(row[5])
            data[headers[6]] = float(row[6])
            data[headers[7]] = float(row[7])
            
            # round the times to 2 decimal places
            data[headers[4]] = round(data[headers[4]], 2)
            data[headers[5]] = round(data[headers[5]], 2)
            data[headers[6]] = round(data[headers[6]], 2)
            data[headers[7]] = round(data[headers[7]], 2)
            
            graphAgg = ""
            
            # find aggName in graph name
            for name in aggNames:
                if name in row[3]:
                    graphAgg = name
                    break
            
            if graphAgg == "":
                # not a common graph
                splitList = row[3].split("/")
                graphAgg = splitList[len(splitList)-1]
                
            if graphAgg not in aggDict:
                aggDict[graphAgg] = {
                    "count": 1,
                    "Tarjan-Vishkin Time": data[headers[4]],
                    "Tarjan-Vishkin (UF) Time": data[headers[5]],
                    "Tarjan-Vishkin (Parallel) Time": data[headers[6]],
                    "Tarjan-Vishkin (Parallel UF) Time": data[headers[7]],
                    "Decrease using parallel UF (%)": (1-(data[headers[7]]/data[headers[4]])) * 100
                }
            else:
                aggDict[graphAgg]["count"] += 1
                aggDict[graphAgg]["Tarjan-Vishkin Time"] += data[headers[4]]
                aggDict[graphAgg]["Tarjan-Vishkin (UF) Time"] += data[headers[5]]
                aggDict[graphAgg]["Tarjan-Vishkin (Parallel) Time"] += data[headers[6]]
                aggDict[graphAgg]["Tarjan-Vishkin (Parallel UF) Time"] += data[headers[7]]
                aggDict[graphAgg]["Decrease using parallel UF (%)"] += (1-(data[headers[7]]/data[headers[4]])) * 100
            
            data["Decrease using Parallel Union Find (%)"] = (1-(data[headers[7]]/data[headers[4]])) * 100
            datalist.append(data)
        # printList(datalist)
        
        # get average times for each graph type in aggDict
        for graph in aggDict:
            aggDict[graph]["Tarjan-Vishkin Time"] /= aggDict[graph]["count"]
            aggDict[graph]["Tarjan-Vishkin (UF) Time"] /= aggDict[graph]["count"]
            aggDict[graph]["Tarjan-Vishkin (Parallel) Time"] /= aggDict[graph]["count"]
            aggDict[graph]["Tarjan-Vishkin (Parallel UF) Time"] /= aggDict[graph]["count"]
            aggDict[graph]["Decrease using parallel UF (%)"] /= aggDict[graph]["count"]
            
            # truncate to 2 decimal places
            aggDict[graph]["Tarjan-Vishkin Time"] = round(aggDict[graph]["Tarjan-Vishkin Time"], 2)
            aggDict[graph]["Tarjan-Vishkin (UF) Time"] = round(aggDict[graph]["Tarjan-Vishkin (UF) Time"], 2)
            aggDict[graph]["Tarjan-Vishkin (Parallel) Time"] = round(aggDict[graph]["Tarjan-Vishkin (Parallel) Time"], 2)
            aggDict[graph]["Tarjan-Vishkin (Parallel UF) Time"] = round(aggDict[graph]["Tarjan-Vishkin (Parallel UF) Time"], 2)
            aggDict[graph]["Decrease using parallel UF (%)"] = round(aggDict[graph]["Decrease using parallel UF (%)"], 2)
        

        # convert to pandas dataframe
        df = pd.DataFrame(datalist)
        
        print(df.head())
        
        # write the df to a csv file
        df.to_csv("plot_datas/time_table.csv", index=False)
        
        # create a df from the dictionary
        df = pd.DataFrame.from_dict(aggDict, orient="index")
        df = df.reset_index()
    
        # write the df to a csv file
        df.to_csv("plot_datas/time_agg.csv", index=False)
        
def get_time_data_tarjan():
    with open("plot_datas/named_graphs_test.csv") as f:
        # read csv file
        csv_reader = csv.reader(f, delimiter=',')
        # skip the header
        headers = next(csv_reader)
        
        # strip the headers of whitespaces
        headers = [header.strip() for header in headers]
        
        datalist = []
        
        aggNames = ["as-caida","delaunay","Erdos", "fe_","cca_G", "ccc_G", "bfly"]

        aggDict = {}
        
        for row in csv_reader:
            if (float(row[4]) == 0):
                continue
            
            num_vertices = int(row[0])
            num_edges = int(row[1])
            avg_degree = row[2]
            
            data = {}
            
            data[headers[0]] = num_vertices
            data[headers[1]] = num_edges
            data[headers[2]] = avg_degree
            data["Graph Name"] = row[3]
            data[headers[4]] = float(row[4])
            data[headers[5]] = float(row[5])
            
            # round the times to 2 decimal places
            data[headers[4]] = round(data[headers[4]], 2)
            data[headers[5]] = round(data[headers[5]], 2)
            
            graphAgg = ""
            
            # find aggName in graph name
            for name in aggNames:
                if name in row[3]:
                    graphAgg = name
                    break
            
            if graphAgg == "":
                # not a common graph
                splitList = row[3].split("/")
                graphAgg = splitList[len(splitList)-1]
                
            if graphAgg not in aggDict:
                aggDict[graphAgg] = {
                    "count": 1,
                    "Tarjan-Vishkin (Parallel UF) Time": data[headers[4]],
                    "Tarjan Time": data[headers[5]],
                    # "Decrease using parallel UF (%)": (1-(data[headers[4]]/data[headers[5]])) * 100
                }
            else:
                aggDict[graphAgg]["count"] += 1
                aggDict[graphAgg]["Tarjan-Vishkin (Parallel UF) Time"] += data[headers[4]]
                aggDict[graphAgg]["Tarjan Time"] += data[headers[5]]
                # aggDict[graphAgg]["Decrease using parallel UF (%)"] += (1-(data[headers[7]]/data[headers[4]])) * 100
            
            # data["Decrease using Parallel Union Find (%)"] = (1-(data[headers[7]]/data[headers[4]])) * 100
            datalist.append(data)
        # printList(datalist)
        
        # get average times for each graph type in aggDict
        for graph in aggDict:
            aggDict[graph]["Tarjan Time"] /= aggDict[graph]["count"]
            aggDict[graph]["Tarjan-Vishkin (Parallel UF) Time"] /= aggDict[graph]["count"]
            
            # truncate to 2 decimal places
            aggDict[graph]["Tarjan Time"] = float("{:.2f}".format(aggDict[graph]["Tarjan Time"]))
            aggDict[graph]["Tarjan-Vishkin (Parallel UF) Time"] = float("{:.2f}".format(aggDict[graph]["Tarjan-Vishkin (Parallel UF) Time"]))
            
            # aggDict[graph]["Decrease using parallel UF (%)"] /= aggDict[graph]["count"]
        

        # convert to pandas dataframe
        df = pd.DataFrame(datalist)
        
        print(df.head())
        
        # write the df to a csv file
        df.to_csv("plot_datas/time_table_test.csv", index=False)
        
        # create a df from the dictionary
        df = pd.DataFrame.from_dict(aggDict, orient="index")
        df = df.reset_index()
    
        # write the df to a csv file
        df.to_csv("plot_datas/time_agg_test.csv", index=False)

# degree_vs_time()
# threads_vs_time()
# degree_vs_time()
# aux_edges()
# aux_times()
# get_edge_data()
# get_time_data()
get_time_data_tarjan()