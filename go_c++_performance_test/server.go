package main

// Source: https://hyr.mn/golang-tcp-echo/

import (
	"bufio"
	"flag"
	"log"
	"net"
	"os"
)

var port string

func init() {
	flag.StringVar(&port, "p", os.Getenv("PORT"), "Default listen port")
	flag.Parse()

	if port == "" {
		port = "4242"
	}
}

func handleConn(conn net.Conn) {
	defer conn.Close()
	// // _, err := io.Copy(conn, conn)
	// var b []byte
	// _, err := conn.Read(b)

	// if err == io.EOF {
	// 	log.Printf("received EOF. Client disconnected")
	// 	return
	// } else if err != nil {
	// 	log.Fatalf("error, err=%s", err)
	// } else {
	// 	fmt.Println("client> ", string(b[:]))
	// 	conn.Write(b)
	// }
	b := bufio.NewReader(conn)
	for {
		line, err := b.ReadBytes('\n')
		if err != nil { // EOF, or worse
			break
		}
		conn.Write(line)
	}
}

func main() {
	log.Printf("listening on port %v...\n", port)

	listener, err := net.Listen("tcp", ":"+port)

	if err != nil {
		log.Fatalf("listen error, err=%s", err)
		return
	}

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Fatalf("accept error, err=%s", err)
			return
		}

		go handleConn(conn)
		log.Printf("connection accepted %v", conn.RemoteAddr())
	}
}
