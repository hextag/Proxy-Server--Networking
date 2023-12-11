import socket
import sys

no_files = len(sys.argv)-1

for i in range(no_files):
    filename = sys.argv[i+1]
    s = socket.socket()
    host = ""
    port = 60001

    s.connect((host, port))
    s.send(filename)
    status = s.recv(1024)
    print status
    if status == "Sorry! The file was not found!":
        continue
    with open(filename, 'wb') as f:
        while True:
            print('Downloading data...')
            data = s.recv(1024)
            if not data:
                break
            f.write(data)
    print("Yeay! File has been downloaded...")
    f.close()
    s.close()

print('connection closed')
