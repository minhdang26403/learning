package main

import "fmt"

func square(n int) int { return n * n }
func negative(n int) int { return -n }
func product(m, n int) int { return m * n}

var g func(int) int

func main() {
	f := square
	fmt.Println(f(3))

	f = negative
	fmt.Println(f(3))

	// compile error: can't assign f(int, int) int to f(int) int
	// f = product

	// no compile time error, but panic due to calling nil function
	// g(3)
}
