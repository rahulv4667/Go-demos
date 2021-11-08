package main

import (
	"fmt"
	"time"
)

func goroutine1(ping <-chan int, pong chan<- int) {
	for {
		ball := <-ping
		fmt.Println("Ping", ball)
		time.Sleep(time.Second)
		pong <- ball + 1
	}
}

func goroutine2(pong <-chan int, ping chan<- int) {
	for {
		ball := <-pong
		fmt.Println("Pong", ball)
		time.Sleep(time.Second)
		ping <- ball + 1
	}
}

func main() {
	pingChan := make(chan int, 1)
	pongChan := make(chan int, 1)

	go goroutine1(pingChan, pongChan)
	go goroutine2(pongChan, pingChan)

	pingChan <- 1
	select {}
}
