#run.py

import subprocess
import math
import os


if os.path.exists("DNS_100.csv"):
    os.remove("DNS_100.csv")

file1=open("DNS_100.csv", "a")
mean=0
sd=0
error=0
dataset=[]

files=['../input/100_DNS1.txt', '../input/100_DNS2.txt', '../input/100_DNS3.txt', '../input/100_DNS4.txt', '../input/100_DNS5.txt']

#cmd="./MM"

for i in range(0, 2):
    process = subprocess.Popen(['./multi-lookup', files[i], 'results.txt'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err=process.communicate()
    err=err.decode("utf-8")
    out=out.decode("utf-8")
    line=out.split("\n")
    dataset.append(float(line[-1]))
    file1.write(line[-1]+"\n")
    print("test")



while(error>=.1*mean):
    process = subprocess.Popen(['./multi-lookup', files[i], 'results.txt'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err=process.communicate()
    err=err.decode("utf-8")
    out=out.decode("utf-8")
    line=out.split("\n")
    dataset.append(float(line[-1]))
    file1.write(line[-1]+"\n")

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
    print("error: ", error)
    print("mean: ", mean)

    i+=i


file1.write("\n")
file1.write("Mean: ")
file1.write(str(mean))

