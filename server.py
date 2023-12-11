import socket
from pathlib import Path

port = 60001
s = socket.socket()
host = ""

s.bind((host, port))
s.listen(5)

while True:
    print("Hello!! Server is up and running...")
    conn, addr = s.accept()
    filename = conn.recv(1024)
    filename = './Data/' + filename
    print filename
    my_file = Path(filename)
    if not my_file.is_file():
        conn.send("Sorry! The file was not found!")
        print("Sorry! The file was not found!")
        continue

    conn.send("The file is being downloaded")
    print 'Got connection from', addr
    f = open(filename,'rb')
    l = f.read(1024)
    while (l):
       conn.send(l)
       print('Sent ',repr(l))
       l = f.read(1024)
    f.close()

    print('Done sending')
    conn.close()
