#rungo.py

import subprocess
import math
import os


if os.path.exists("MCP10million_go.csv"):
    os.remove("MCP10million_go.csv")

file1=open("MCP10million_go.csv", "a")
mean=0
sd=0
error=0
dataset=[]


for i in range(0, 2):
    process = subprocess.Popen(['go', 'run', 'MCP.go', '10000000'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err=process.communicate()
    err=err.decode("utf-8")
    out=out.decode("utf-8")
    line=out.split("\n")
    dataset.append(float(line[0]))
    file1.write(line[0]+"\n")



while(error>=.1*mean):
    process = subprocess.Popen(['go', 'run', 'MCP.go', '10000000'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err=process.communicate()
    err=err.decode("utf-8")
    out=out.decode("utf-8")
    line=out.split("\n")
    dataset.append(float(line[0]))
    file1.write(line[0]+"\n")

    sm=0
    for i in range(len(dataset)):
        data=dataset[i]
        sm+=data
        mean=sm/len(dataset)

    deviation_sum=0
    for i in range(len(dataset)):
        deviation_sum+=(dataset[i]-mean)**2.0
    sd=math.sqrt((deviation_sum)/len(dataset))

    error=(1.96*sd)/math.sqrt(len(dataset))

file1.write("\n")
file1.write("Mean: ")
file1.write(str(mean))

