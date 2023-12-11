import socket
import sys
import time

no_files = len(sys.argv)-1
s = socket.socket()
host = ""
port = 60001

s.connect((host, port))
s.send(str(no_files))
time.sleep(0.01)
file_names = []
for i in range(no_files):
    file_name = './Data/' + sys.argv[i+1]
    file_names.append(file_name)
    s.send(file_name)
    time.sleep(0.01)

file_sizes = []

for i in range(no_files):
    size = s.recv(1024)
    file_sizes.append(int(size))
    time.sleep(0.01)

for i in range(no_files):
    #status = s.recv(1024)
    if file_sizes[i] == -1:
        print("Sorry! The file was not found!")
        continue
    time.sleep(0.5)
    size = 0
    with open(sys.argv[i+1], 'wb') as f:
        while True:
            print('Downloading data...')
            data = s.recv(1024)
            size = size + len(data)
            time.sleep(0.1)
            if size == file_sizes[i]:
                f.write(data)
                print("File has been downloaded successfully")
                break
            f.write(data)


#print("Yeay! File has been downloaded...")
#f.close()
s.close()

print('connection closed')
