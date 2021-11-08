import os
import sys
import asyncio
import socket
import time

async def ping_server(port: int, i: int):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("127.0.0.1", port))
    echo_msg = "Echo request {}\n".format(i)
    s.send(echo_msg.encode())
    buf = s.recv(1024).decode()
    if buf != echo_msg:
        print("Some error occured in iteration {}".format(i))
    s.close()


async def test_server(lang: str, port: int, num_req: int): 
    start = time.time_ns()
    await asyncio.gather(*[ping_server(port, i) for i in range(num_req)])
    end = time.time_ns()
    time_taken = end-start
    print("Time taken(ns) for {}: {}".format(lang, time_taken))


if __name__ == "__main__":
    if len(sys.argv) < 4:
        print('Format: python3 <script_name> <server_language> <port> <num_requests>')
        exit(1)
    
    asyncio.run(test_server(str(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3])))