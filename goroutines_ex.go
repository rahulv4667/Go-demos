package main

import (
	"fmt"
	"sync"
	"time"
)

var mu sync.Mutex
var num int

func goroutine1() {
	for {
		mu.Lock()
		num++
		fmt.Println("Goroutine1: ", num)
		time.Sleep(time.Second)
		mu.Unlock()
	}
}

func goroutine2() {
	mu.Lock()
	num++
	fmt.Println("Goroutine2: ", num)
	time.Sleep(time.Second)
	mu.Unlock()
}

func main() {
	num = 0
	go goroutine1()
	go goroutine2()
	select {}
}
