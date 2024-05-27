#rungo.py

import subprocess
import math
import os


if os.path.exists("DNS_100_go.csv"):
    os.remove("DNS_100_go.csv")

file1=open("DNS_100_go.csv", "a")
mean=0
sd=0
error=0
dataset=[]

files= ['../input/100_DNS1.txt', '../input/100_DNS2.txt', '../input/100_DNS3.txt', '../input/100_DNS4.txt', '../input/100_DNS5.txt']


#process = subprocess.Popen(['go', 'run', 'multi_lookup.go', '../input/500_DNS.txt', 'results.txt'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

i=0

for i in range(0, 2):
    print("test")
    process = subprocess.Popen(['go', 'run', 'multi-lookup.go', files[i], 'results.txt'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err=process.communicate()
    err=err.decode("utf-8")
    out=out.decode("utf-8")
    line=out.split("\n")
    dataset.append(float(line[-2]))
    file1.write(line[-2]+"\n")
    print("test1")



while(error>=.1*mean):
    process = subprocess.Popen(['go', 'run', 'multi-lookup.go',  files[i], 'results.txt'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err=process.communicate()
    err=err.decode("utf-8")
    out=out.decode("utf-8")
    line=out.split("\n")
    dataset.append(float(line[-2]))
    file1.write(line[-2]+"\n")
    print("test2")

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

    i+=i

file1.write("\n")
file1.write("Mean: ")
file1.write(str(mean))

