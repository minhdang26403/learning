package main

import "fmt"

func main() {
	a, b := 1, 2

	// there must be a new variable on the left side of `:=`
	// Otherwise, use `=`
	_, b = 3, 4

	fmt.Println(a, b)
	// fmt.Println(c)

	// print pointer
	var x int
  ptr := &x
  fmt.Printf("Pointer value: %p\n", ptr)
}
