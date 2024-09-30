package main

import "fmt"

func main() {
	var p *[]int = new([]int)
	var q = make([]int, 10)
	p = &q
	fmt.Printf("%v", (*p)[0])
}