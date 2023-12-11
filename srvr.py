import socket
from pathlib import Path
import os
import time

port = 60001
s = socket.socket()
host = ""

s.bind((host, port))
s.listen(5)

while True:
    print("Hello!! Server is up and running...")
    conn, addr = s.accept()
    no_files = conn.recv(1024)
    time.sleep(0.01)
    no_files = int(no_files)
    file_names = []
    file_sizes = []

    for i in range(no_files):
        file_name = conn.recv(1024)
        time.sleep(0.01)
        file_names.append(file_name)
        my_file = Path(file_name)
        if my_file.is_file():
            size = os.path.getsize(file_name)
            file_sizes.append(size)
        else:
            file_sizes.append(-1)

    for i in range(no_files):
        conn.send(str(file_sizes[i]))
        time.sleep(0.01)

    for i in range(no_files):
        filename = file_names[i]
        if file_sizes[i] == -1:
            print("Sorry! The file was not found!")
            continue

        time.sleep(0.5)
        print 'Got connection from', addr
        f = open(filename,'rb')
        l = f.read(1024)
        while (l):
           conn.send(l)
        #   print('Sent ',repr(l))
           l = f.read(1024)
        f.close()
        print("The file has been successfully downloaded");
    conn.close()
